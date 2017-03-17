#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <sys/time.h>

using namespace std;

void GenerateRandomSPDIReachTask(const string& filename, const size_t startEdgesCount, const size_t finalEdgesCount) {
    vector<string> edges;

    string s;
    char state = 'n';

    ifstream spdiFile(filename);

    while (!spdiFile.eof()) {
        getline(spdiFile, s);

        if (s.length() == 0 || s[0] == '*') {
            continue;
        } else if (s == "Regions:") {
            state = 'r';
            continue;
        }

        if (state == 'r') {
            vector<string> vertices;
            while (s.find(':') != string::npos) {
                vertices.push_back(s.substr(0, s.find(':') - 1));
                s = s.substr(s.find(':') + 2);
            }
            vertices.push_back(s.substr(0, s.find(',')));

            for (size_t i = 0; i + 1 < vertices.size(); ++i) {
                edges.push_back(vertices[i] + " " + vertices[i + 1]);
            }
        }
    }

    spdiFile.close();

    cout << "Start:" << endl;

    for (size_t i = 0; i < startEdgesCount; ++i) {
        double s1 = rand() * 1.0 / RAND_MAX;
        double s2 = rand() * 1.0 / RAND_MAX;
        size_t edgeIndex1 = rand() % edges.size();

        if (max(s1, s2) - min(s1, s2) < 0.01) {
            if (max(s1, s2) < 0.9) {
                s1 += 0.1;
            } else {
                s2 -= 0.1;
            }
        }

        cout << edges[edgeIndex1] << " " << min(s1, s2) << " " << max(s1, s2) << endl;
    }

    cout << "Final:" << endl;

    for (size_t i = 0; i < finalEdgesCount; ++i) {
        double s1 = rand() * 1.0 / RAND_MAX;
        double s2 = rand() * 1.0 / RAND_MAX;
        size_t edgeIndex1 = rand() % edges.size();

        if (max(s1, s2) - min(s1, s2) < 0.01) {
            if (max(s1, s2) < 0.9) {
                s1 += 0.1;
            } else {
                s2 -= 0.1;
            }
        }

        cout << edges[edgeIndex1] << " " << min(s1, s2) << " " << max(s1, s2) << endl;
    }
}

int main(int argc, char** argv) {
    if (argc < 4) {
        cerr << "Arguments needed: .spdi file, start edges count, final edges count, seed (optional, default = milliseconds from midnight)" << endl;
        return 1;
    }

    if (argc == 4) {
        struct timeval time;
        gettimeofday(&time, NULL);
        srand((time.tv_sec * 1000) + (time.tv_usec / 1000));
    } else {
        srand(atoi(argv[2]));
    }

    GenerateRandomSPDIReachTask(argv[1], atoi(argv[2]), atoi(argv[3]));

    return 0;
}
