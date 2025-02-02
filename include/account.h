#ifndef ACCOUNT_H
#define ACCOUNT_H

#define MAX_CHARS          1024
#define MAX_ATTEMPTS       3
#define USERNAME_REQUIRED  1
#define PASSWORD_REQUIRED  2
#define VALID_CREDENTIALS  3
#define WRONG_PASSWORD     4
#define ACCOUNT_NOT_ACTIVE 5

struct account {
    char *username;
    char *password;
    char *is_active;
    struct account *next;
};

typedef struct account *Account;

Account add_account(Account account_list, char *username, char *password, char *is_active);

Account read_account(const char *filename);

int process_login(Account account_list, char *username, char *password);

void save_to_file(Account account_list, const char *filename);

#endif   // ACCOUNT_H
