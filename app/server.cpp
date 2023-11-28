#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <filesystem>
#include <string>

#include "account.h"
#include "file_tree.h"
#include "protocol.h"
#include "status.h"
#include "utils.h"

#define BACKLOG  20 /* Number of allowed connections */
#define FILENAME "../account.txt"

Account account_list = NULL;

/* Receive and echo message to client */
void echo(int);

/**
 * @brief handle request message, extract operation from payload and call handle function
 * @param Message mess, int connSock
 * @return void
 */
void handleAuthenticateRequest(Message mess, int connSock);

/**
 * @brief handle login request
 * @param Message mess, int connSock
 * @return void
 */
void handleLogin(Message mess, int connSock);

void handleRequesetDirectory(Message msg, int connSock);

/*
 * count number element in array with unknown size
 * @param temp[][]
 * @return size of array
 */
int numberElementsInArray(char **temp) {
    int i;
    for (i = 0; *(temp + i); i++) {
        // count number elements in array
    }
    return i;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: ./server <port_number>\n");
        return 0;
    }
    char *endstr;
    // check if the port number is valid
    errno = 0;
    uint16_t PORT = strtoul(argv[1], &endstr, 10);

    if (errno != 0 || *endstr != '\0') {
        printf("Error: %s\n", errno == EINVAL ? "invalid base" : "invalid input");
        return 0;
    }
    account_list = read_account(FILENAME);

    int listenfd, connfd;
    struct sockaddr_in server; /* server's address information */
    struct sockaddr_in client; /* client's address information */
    socklen_t sin_size;

    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) { /* calls socket() */
        perror("\nError: ");
        return 0;
    }

    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = htonl(INADDR_ANY); /* INADDR_ANY puts your IP address automatically */

    if (bind(listenfd, (struct sockaddr *) &server, sizeof(server)) == -1) {
        perror("\nError: ");
        return 0;
    }

    if (listen(listenfd, BACKLOG) == -1) {
        perror("\nError: ");
        return 0;
    }

    sin_size = sizeof(struct sockaddr_in);

    while (true) {
        if ((connfd = accept(listenfd, (struct sockaddr *) &client, &sin_size)) == -1) {
            perror("\nError: ");
        }

        printf("You got a connection from %s\n",
               inet_ntoa(client.sin_addr)); /* prints client's IP */

        echo(connfd);

        close(connfd);
    }

    close(listenfd);
    return 0;
}

void echo(int connfd) {
    Message msg;
    while (true) {
        int byte_received;
        byte_received = receiveMessage(connfd, &msg);
        if (byte_received <= 0) {
            printf("Connection closed.\n");
            break;
        }
        switch (msg.type) {
            case TYPE_AUTHENTICATE:
                handleAuthenticateRequest(msg, connfd);
                break;
            case TYPE_REQUEST_DIRECTORY:
                handleRequesetDirectory(msg, connfd);
                break;
            default:
                break;
        }
    }
    close(connfd);
}

void handleAuthenticateRequest(Message mess, int connSock) {
    char *operation;
    char temp[PAYLOAD_SIZE];
    strcpy(temp, mess.payload);
    operation = str_split(temp, '\n')[0];
    if (!strcmp(operation, "LOGIN")) {
        handleLogin(mess, connSock);
    } else if (!strcmp(operation, "LOGOUT")) {
    } else if (!strcmp(operation, "REGISTER")) {
    }
}

void handleRequesetDirectory(Message msg, int connSock) {
    std::string username(msg.payload);
    std::string path = DATA_PATH + username + "/";
    FileTree root(".");
    root.populateFromDirectory(path);
    std::string treeString = root.toString();
    printf("%s\n", treeString.c_str());
    Message *mess = (Message *) malloc(sizeof(Message));
    mess->type = TYPE_OK;
    mess->requestId = msg.requestId;
    strcpy(mess->payload, treeString.c_str());
    mess->length = strlen(mess->payload);
    sendMessage(connSock, *mess);
}

void handleLogin(Message mess, int connSock) {
    StatusCode status;
    char **temp = str_split(mess.payload,
                            '\n');   // handle payload, divide payload to array string split by '\n'
    if (numberElementsInArray(temp) == 3) {
        char **userStr = str_split(temp[1], ' ');   // get username
        char **passStr = str_split(temp[2], ' ');   // get password
        if ((numberElementsInArray(userStr) == 2) &&
            (numberElementsInArray(passStr) ==
             2)) {   // check payload structure valid with two parameters
            if (!(strcmp(userStr[0], COMMAND_USER) ||
                  strcmp(
                      passStr[0],
                      COMMAND_PASSWORD))) {   // check payload structure valid with two parameters
                char username[30];
                char password[20];
                strcpy(username, userStr[1]);
                strcpy(password, passStr[1]);
                int fl = process_login(account_list, username, password);
                if (fl != VALID_CREDENTIALS) {
                    mess.type = TYPE_ERROR;
                    status = USERNAME_OR_PASSWORD_INVALID;
                } else {
                    mess.type = TYPE_OK;
                    createFolder(username);
                }

            } else {
                status = COMMAND_INVALID;
                mess.type = TYPE_ERROR;
            }
        } else {
            status = COMMAND_INVALID;
            mess.type = TYPE_ERROR;
        }
    } else {
        mess.type = TYPE_ERROR;
        status = COMMAND_INVALID;
        printf("Fails on handle Login!!");
    }
    Message *msg = (Message *) malloc(sizeof(Message));
    msg->type = mess.type;
    msg->requestId = mess.requestId;
    sprintf(msg->payload, "%d: %s", status, statusToString(status).c_str());
    printf("%s\n", msg->payload);
    msg->length = strlen(msg->payload);
    sendMessage(connSock, *msg);
}
