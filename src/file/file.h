#ifndef FILE_H
#define FILE_H

#include <iostream>
#include <sys/stat.h> 
#include <fstream>
#include <vector>

using namespace std;

bool FileExists(const string& filename);
string ReadFile(const string& filename);
void WriteFile(const string& filename, const string& content);
void WriteFileVector(const string& filename, const vector<string>& content);

#endif