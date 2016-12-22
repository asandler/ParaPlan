#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <sys/time.h>

using namespace std;

void GenerateRandomSPDIReachTask(const string& filename, int seed) {
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
                edges.push_back(vertices[i] + "-" + vertices[i + 1]);
            }
        }
    }

    spdiFile.close();

    //struct timeval time;
    //gettimeofday(&time, NULL);
    //srand((time.tv_sec * 1000) + (time.tv_usec / 1000));
    srand(seed);

    double s1 = rand() * 1.0 / RAND_MAX;
    double s2 = rand() * 1.0 / RAND_MAX;
    double s3 = rand() * 1.0 / RAND_MAX;
    double s4 = rand() * 1.0 / RAND_MAX;
    size_t edgeIndex1 = rand() % edges.size();
    size_t edgeIndex2 = rand() % edges.size();

    if (max(s1, s2) - min(s1, s2) < 0.01) {
        if (max(s1, s2) < 0.9) {
            s1 += 0.1;
        } else {
            s2 -= 0.1;
        }
    }

    if (max(s3, s4) - min(s3, s4) < 0.01) {
        if (max(s3, s4) < 0.9) {
            s3 += 0.1;
        } else {
            s4 -= 0.1;
        }
    }

    cout << "[" << min(s1, s2) << "," << max(s1, s2) << "] ";
    cout << "[" << min(s3, s4) << "," << max(s3, s4) << "] ";
    cout << edges[edgeIndex1] << " " << edges[edgeIndex2] << endl;
}

int main(int argc, char** argv) {
    if (argc < 3) {
        cerr << "Arguments needed: .spdi file, seed" << endl;
        return 1;
    }

    GenerateRandomSPDIReachTask(argv[1], atoi(argv[2]));

    return 0;
}
