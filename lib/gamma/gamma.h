#ifndef GAMMA_H
#define GAMMA_H

#include <iostream>

using namespace std;

extern "C" {
    string GammaEncrypt(const string& text, string& key);
    string GammaDecrypt(const string& text, const string& key);
}

#endif