#ifndef GENERAL_H
#define GENERAL_H

#include <iostream>
#include <vector>

using namespace std;

string ToLower(const string& str);
string GetTextInput();
string GetPasswordInput(const string& prompt);
vector<string> SplitUtf8String(const string& str);

#endif