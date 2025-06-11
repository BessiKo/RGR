#ifndef PERMUTATION_H
#define PERMUTATION_H

#include <iostream>
#include <vector>

using namespace std;

extern "C" {
    vector<int> GetInversePermutation(const vector<int>& permutation);
    vector<string> PermutationEncrypt(const vector<string>& symbols);
    vector<string> PermutationDecrypt(const vector<string>& symbols);
}
#endif