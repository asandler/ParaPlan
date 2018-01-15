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

unordered_map<string, pair<float, float> > Vertices;
unordered_map<string, pair<float, float> > Vectors;

float GetMagnitude(const Vector2f& v) {
    return sqrt(v.x * v.x + v.y * v.y);
}

float Dot(const Vector2f& u, const Vector2f& v) {
    return u.x * u.x + v.y * v.y;
}

Vector2f GetNormal(Vector2f v) {
    return Vector2f(-v.y, v.x);
}

Vector2f Normalized(Vector2f v) {
    return v / GetMagnitude(v);
}

VertexArray RegionVA(const vector<Vector2f>& points, Color color, float thickness) {
    VertexArray ary(TrianglesStrip);

    for (size_t i = 1; i < points.size() + 2; i++) {
        Vector2f v0 = points[(i - 1) % points.size()];
        Vector2f v1 = points[i % points.size()];
        Vector2f v2 = points[(i + 1) % points.size()];

        Vector2f v01 = Normalized(v1 - v0);
        Vector2f v12 = Normalized(v2 - v1);

        Vector2f d = GetNormal(v01 + v12);
        float dot = Dot(d, d);

        if (std::abs(dot) > 0.001) {
            d *= thickness / 2.f / dot;
        } else {
            d = Normalized(GetNormal(v01));
            d *= thickness / 2.f;
        }

        ary.append(Vertex(v1 + d, color));
        ary.append(Vertex(v1 - d, color));
    }

    return ary;
}

VertexArray ArrowVA(const Vector2f& a, const Vector2f& b, Color color, float thickness) {
    VertexArray ary(TrianglesStrip);

    Vector2f d = Normalized(GetNormal(b - a));
    d *= thickness / 2.f;

    ary.append(Vertex(a + d, color));
    ary.append(Vertex(a - d, color));
    ary.append(Vertex(a + (b - a) * 0.7f + d, color));
    ary.append(Vertex(a + (b - a) * 0.7f - d, color));

    ary.append(Vertex(a + (b - a) * 1.f, color));
    ary.append(Vertex(a + (b - a) * 0.7f + d * 3.f, color));
    ary.append(Vertex(a + (b - a) * 0.7f - d * 3.f, color));

    return ary;
}

vector<VertexArray> ConstructVertexArrayFromSPDI(const string& filename) {
    vector<VertexArray> ans;

    string s;
    char state = 'n';

    float maxX = -1e9;
    float maxY = -1e9;
    float minX = 1e9;
    float minY = 1e9;

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
                float aw = (SCREEN_WIDTH * 0.9) / (maxX - minX);
                float bw = 0.05 * SCREEN_WIDTH - minX * aw;

                float ah = (SCREEN_HEIGHT * 0.9) / (maxY - minY);
                float bh = 0.05 * SCREEN_HEIGHT - minY * ah;

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
                float coord1 = stod(coords.substr(0, coords.find(',')));
                float coord2 = stod(coords.substr(coords.find(',') + 2));
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
                float coord1 = stod(coords.substr(0, coords.find(',')));
                float coord2 = stod(coords.substr(coords.find(',') + 2));
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

                float localMaxX = -1e9;
                float localMaxY = -1e9;
                float localMinX = 1e9;
                float localMinY = 1e9;

                float xSum = 0;
                float ySum = 0;

                vector<Vector2f> regionVertices;
                for (size_t i = 0; i < regVert.size() - 1; ++i) {
                    float coord1 = Vertices[regVert[i]].first;
                    float coord2 = Vertices[regVert[i]].second;
                    regionVertices.push_back(Vector2f(coord1, coord2));

                    localMaxX = max(localMaxX, coord1);
                    localMaxY = max(localMaxY, coord2);
                    localMinX = min(localMinX, coord1);
                    localMinY = min(localMinY, coord2);

                    xSum += coord1;
                    ySum += coord2;
                }

                ans.push_back(RegionVA(regionVertices, sf::Color::Black, 5));

                Vector2f center(xSum / (regVert.size() - 1), ySum / (regVert.size() - 1));

                float lenCoef1 = ((localMaxX - localMinX) + (localMaxY - localMinY)) / 16;
                float lenCoef2 = lenCoef1;
                lenCoef1 /= sqrt(Vectors[v1].first * Vectors[v1].first + Vectors[v1].second * Vectors[v1].second);
                lenCoef2 /= sqrt(Vectors[v2].first * Vectors[v2].first + Vectors[v2].second * Vectors[v2].second);

                Vector2f vec1(Vectors[v1].first * lenCoef1, -Vectors[v1].second * lenCoef1);
                Vector2f vec2(Vectors[v2].first * lenCoef2, -Vectors[v2].second * lenCoef2);

                ans.push_back(ArrowVA(center - ((vec1 + vec2) / 2.f), center + vec1, sf::Color::Red, 3));
                ans.push_back(ArrowVA(center - ((vec1 + vec2) / 2.f), center + vec2, sf::Color::Blue, 3));

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
    vector<VertexArray> arrays = ConstructVertexArrayFromSPDI(argv[1]);

    Font arialFont;
    if (!arialFont.loadFromFile("/usr/share/fonts/truetype/msttcorefonts/arial.ttf")) {
        if (!arialFont.loadFromFile("arial.ttf")) {
            cerr << "Couldn't load font file arial.ttf" << endl;
            return 1;
        }
    }

    vector<Text> verticesNumbers;

    for (const auto& v : Vertices) {
        Text numberText;
        numberText.setFont(arialFont);
        numberText.setString(v.first);
        numberText.setFillColor(sf::Color::Black);
        numberText.setCharacterSize(12);
        numberText.setPosition(v.second.first + 2, v.second.second + 2);
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
        for (size_t i = 0; i < arrays.size(); ++i) {
            window.draw(arrays[i]);
        }

        if ((argc == 2) || (argc > 2 && string(argv[2]) != "nonumbers")) {
            for (const auto& text : verticesNumbers) {
                window.draw(text);
            }
        }

        window.display();
    }

    return 0;
}
