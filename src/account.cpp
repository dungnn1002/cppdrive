#include "account.h"
#include <iostream>
#include <fstream>
#include <sstream>

bool process_login(const std::vector<Account>& accounts,const std::string &username, const std::string &password){
    for (const Account& account : accounts) {
        if (account.username == username && account.password == password) {
            if (account.is_active == "active") {
                return true;  // Đăng nhập thành công
            } else {
                return false;  // Tài khoản bị khóa
            }
        }
    }
    return false;  // Không tìm thấy tài khoản
}
std::vector<Account> readFile(const std::string& filename) {
    std::vector<Account> accounts;
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string username, password, status;
        if (iss >> username >> password >> status) {
            accounts.push_back({username, password, status});
        }
    }

    file.close();
    return accounts;
}
