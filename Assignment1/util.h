#ifndef ASSIGNMENT1_UTIL_H
#define ASSIGNMENT1_UTIL_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

/**
 * Trim trailing whitespace from string
 * @param str String to trim
 */
string rtrim(const string &str);

/**
 * Split string
 * @param str String to split
 * @param delim Deliminator to split by
 * @return Vector containing split strings
 */
vector<string> split(const string &str, char delim = ' ');

vector<char *> string_vector_to_char_vector(vector<string> original);

bool tokens_contain_pid(vector<char *> tokens);

pid_t get_pid_from_tokens(vector<char *> tokens);

#endif //ASSIGNMENT1_UTIL_H
