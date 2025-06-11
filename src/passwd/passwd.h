#ifndef PASSWD_H
#define PASSWD_H

#include <string>

using namespace std;

const int MIN_PASSWORD_LENGTH = 6;
const int MAX_PASSWORD_ATTEMPTS = 3;
const string PASSWORD_FILE = "/etc/password_ciphers";

string GetHiddenInput(const string& prompt);
size_t CalculatePasswordHash(const string& password);
bool IsPasswordSet();
void SetNewPassword();
void ChangePassword();
bool VerifyPassword(const string& password);
bool HasWriteAccessToPasswordFile();

#endif