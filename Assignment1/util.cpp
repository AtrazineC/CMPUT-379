#include <iostream>
#include <string>
#include <vector>
#include <cstring>

#include "util.h"

using namespace std;

string rtrim(const string &str) {
    size_t end = str.find_last_not_of(" \n\r\t\f\v");
    return (end == string::npos) ? "" : str.substr(0, end + 1);
}

vector<string> split(const string &str, char delim) {
    vector<string> result;
    string temp;

    for (char i : str) {
        if (i == delim) {
            result.push_back(temp);
            temp = "";
        } else {
            temp += i;
        }
    }

    if (!temp.empty()) {
        result.push_back(temp);
    }

    return result;
}

char *convert(const string &s) {
    char *pc = new char[s.size()+1];
    strcpy(pc, s.c_str());

    return pc;
}

vector<char *> string_vector_to_char_vector(vector<string> original) {
    vector<char *> conversion;
    transform(original.begin(), original.end(), back_inserter(conversion), convert);

    return conversion;
}

bool tokens_contain_pid(vector<char *> tokens) {
    if (tokens.size() < 2) {
        return false;
    }

    return true;
}

pid_t get_pid_from_tokens(vector<char *> tokens) {
    char *pid_token = tokens[1];
    pid_t pid = atoi(pid_token);

    return pid;
}
