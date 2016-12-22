#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <unordered_map>

using namespace std;

int main(int argc, char** argv) {
    vector<string> buffer;
    unordered_map<string, bool> visitedTests;

    string s;
    size_t fileNo = 0;
    string dirName(argv[1]);

    while (getline(cin, s)) {
        if (s == "Points:") {
            if (buffer.size() > 0) {
                if (!visitedTests[buffer[1]]) {
                    visitedTests[buffer[1]] = true;

                    ofstream outputStream;
                    outputStream.open(dirName + "/" + to_string(fileNo) + ".spdi");

                    for (size_t i = 0; i < buffer.size(); ++i) {
                        outputStream << buffer[i] << endl;
                    }

                    outputStream.close();
                }
            }
            buffer.clear();
            fileNo++;
        }

        buffer.push_back(s);
    }
    return 0;
}
