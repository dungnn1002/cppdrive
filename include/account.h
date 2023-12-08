#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>
#include <vector>


struct Account
{
    std::string username;
    std::string password;
    std::string is_active;
};

std::vector<Account> readFile(const std::string &filename);
bool process_login(const std::vector<Account> &accounts, const std::string &username, const std::string &password);
#endif // ACCOUNT_H
