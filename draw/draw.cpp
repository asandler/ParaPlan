#include <SFML/Graphics.hpp>

#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 750

using namespace std;
using namespace sf;

unordered_map<string, pair<double, double> > Vertices;
unordered_map<string, pair<double, double> > Vectors;

VertexArray ConstructVertexArrayFromSPDI(const string& filename) {
    VertexArray ans(Lines);

    string s;
    char state = 'n';

    double maxX = -1e9;
    double maxY = -1e9;
    double minX = 1e9;
    double minY = 1e9;

    ifstream spdiFile(filename);
    while (!spdiFile.eof()) {
        getline(spdiFile, s);

        if (s.length() == 0 || s[0] == '*') {
            continue;
        } else if (s == "Points:") {
            state = 'p';
            continue;
        } else if (s == "Vectors:") {
            state = 'v';

            //normalization to screen size
            for (auto it = Vertices.begin(); it != Vertices.end(); ++it) {
                double aw = (SCREEN_WIDTH * 0.9) / (maxX - minX);
                double bw = 0.05 * SCREEN_WIDTH - minX * aw;

                double ah = (SCREEN_HEIGHT * 0.9) / (maxY - minY);
                double bh = 0.05 * SCREEN_HEIGHT - minY * ah;

                it->second.first = aw * it->second.first + bw;
                it->second.second = ah * it->second.second + bh;
            }
            continue;
        } else if (s == "Regions:") {
            state = 'r';
            continue;
        }

        switch (state) {
            case 'p': {
                string id = s.substr(0, s.find('.'));
                string coords = s.substr(id.length() + 2);
                double coord1 = stod(coords.substr(0, coords.find(',')));
                double coord2 = stod(coords.substr(coords.find(',') + 2));
                Vertices[id] = make_pair(coord1, -coord2);

                maxX = max(maxX, coord1);
                minX = min(minX, coord1);
                maxY = max(maxY, -coord2);
                minY = min(minY, -coord2);

                break;
            }
            case 'v': {
                string id = s.substr(0, s.find('.'));
                string coords = s.substr(id.length() + 2);
                double coord1 = stod(coords.substr(0, coords.find(',')));
                double coord2 = stod(coords.substr(coords.find(',') + 2));
                Vectors[id] = make_pair(coord1, coord2);

                break;
            }
            case 'r': {
                vector<string> regVert;
                while (s.find(':') != string::npos) {
                    regVert.push_back(s.substr(0, s.find(':') - 1));
                    s = s.substr(s.find(':') + 2);
                }
                regVert.push_back(s.substr(0, s.find(',')));

                s = s.substr(s.find(',') + 2);
                string v1 = s.substr(0, s.find(','));
                string v2 = s.substr(v1.length() + 2);

                double localMaxX = -1e9;
                double localMaxY = -1e9;
                double localMinX = 1e9;
                double localMinY = 1e9;

                double xSum = 0;
                double ySum = 0;
                for (size_t i = 0; i < regVert.size() - 1; ++i) {
                    double coord1 = Vertices[regVert[i]].first;
                    double coord2 = Vertices[regVert[i]].second;
                    double coord3 = Vertices[regVert[i + 1]].first;
                    double coord4 = Vertices[regVert[i + 1]].second;

                    ans.append(Vertex(Vector2f(coord1, coord2), sf::Color::Black));
                    ans.append(Vertex(Vector2f(coord3, coord4), sf::Color::Black));

                    localMaxX = max(localMaxX, coord1);
                    localMinX = min(localMinX, coord1);
                    localMaxY = max(localMaxY, coord2);
                    localMinY = min(localMinY, coord2);

                    xSum += coord1;
                    ySum += coord2;
                }

                ans.append(Vertex(Vector2f(Vertices[regVert.back()].first, Vertices[regVert.back()].second), sf::Color::Black));
                ans.append(Vertex(Vector2f(Vertices[regVert[0]].first, Vertices[regVert[0]].second), sf::Color::Black));

                localMaxX = max(localMaxX, Vertices[regVert.back()].first);
                localMinX = min(localMinX, Vertices[regVert.back()].first);
                localMaxY = max(localMaxY, Vertices[regVert.back()].second);
                localMinY = min(localMinY, Vertices[regVert.back()].second);

                Vector2f center(xSum / (regVert.size() - 1), ySum / (regVert.size() - 1));

                double lenCoef1 = ((localMaxX - localMinX) + (localMaxY - localMinY)) / 16;
                double lenCoef2 = lenCoef1;
                lenCoef1 /= sqrt(Vectors[v1].first * Vectors[v1].first + Vectors[v1].second * Vectors[v1].second);
                lenCoef2 /= sqrt(Vectors[v2].first * Vectors[v2].first + Vectors[v2].second * Vectors[v2].second);

                Vector2f vec1(Vectors[v1].first * lenCoef1, -Vectors[v1].second * lenCoef1);
                Vector2f vec2(Vectors[v2].first * lenCoef2, -Vectors[v2].second * lenCoef2);

                ans.append(Vertex(center, sf::Color::Blue));
                ans.append(Vertex(center + vec1, sf::Color::Blue));
                ans.append(Vertex(center, sf::Color::Red));
                ans.append(Vertex(center + vec2, sf::Color::Red));

                break;
            }
            default: {
                break;
            }
        }
    }
    spdiFile.close();

    return ans;
}

int main(int argc, char** argv) {
    RenderWindow window(VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "SPDI");

    if (!argv[1]) {
        cerr << "argument missing: SPDI file" << endl;
        return 1;
    }
    VertexArray lines = ConstructVertexArrayFromSPDI(argv[1]);

    Font arialFont;
    if (!arialFont.loadFromFile("arial.ttf")) {
        cerr << "Couldn't load font from file arial.ttf" << endl;
        return 1;
    }
    
    vector<Text> verticesNumbers;
    for (const auto& v : Vertices) {
        Text numberText;
        numberText.setFont(arialFont);
        numberText.setString(v.first);
        numberText.setFillColor(sf::Color::Black);
        numberText.setCharacterSize(12);
        numberText.setPosition(v.second.first, v.second.second);
        verticesNumbers.push_back(numberText);
    }

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
        }

        window.clear(Color::White);
        window.draw(lines);

        for (const auto& text : verticesNumbers) {
            window.draw(text);
        }

        window.display();
    }

    return 0;
}
