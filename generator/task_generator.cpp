#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>

using namespace std;

void GenerateRandomSPDIReachTask(const string& filename) {
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

    srand(time(NULL));
    double s1 = rand() * 1.0 / RAND_MAX;
    double s2 = rand() * 1.0 / RAND_MAX;
    double s3 = rand() * 1.0 / RAND_MAX;
    double s4 = rand() * 1.0 / RAND_MAX;
    size_t edgeIndex1 = rand() % edges.size();
    size_t edgeIndex2 = rand() % edges.size();

    cout << "Start:" << endl << edges[edgeIndex1] << " " << min(s1, s2) << " " << max(s1, s2) << endl;
    cout << "Finish:" << endl << edges[edgeIndex2] << " " << min(s3, s4) << " " << max(s3, s4) << endl;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        cerr << "Argument needed: .spdi file" << endl;
        return 1;
    }

    GenerateRandomSPDIReachTask(argv[1]);

    return 0;
}
