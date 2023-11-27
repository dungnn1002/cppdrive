#ifndef __UTILS_H__
#define __UTILS_H__

#define DATA_PATH "/tmp/cppdrive/"

char** str_split(char* a_str, const char a_delim);

/**
 * @brief create folder for user
 *
 * path: /tmp/cppdrive/{username}/
 * @return void
 */
void createFolder(char* username);
#endif
