#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

#include <arpa/inet.h>
#include <assert.h>
#include <ctype.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define BUFF_SIZE        2048
#define PAYLOAD_SIZE     1024
#define LENGTH_SIZE      2
#define TIME_OUT         1200
#define COMMAND_USER     "USER"
#define COMMAND_PASSWORD "PASS"
#define COMMAND_LOGOUT   "LOGOUT"
#define COMMAND_REGISTER "REGISTER"

typedef enum {
    TYPE_AUTHENTICATE,
    TYPE_REQUEST_FILE,
    TYPE_REQUEST_DOWNLOAD,
    TYPE_REQUEST_DIRECTORY,
    TYPE_UPLOAD_FILE,
    TYPE_ERROR,
    TYPE_OK,
    TYPE_CANCEL,
    TYPE_CREATE_FOLDER,
    TYPE_DELETE_FOLDER,
    TYPE_DELETE_FILE
} MessageType;

typedef struct Message {
    MessageType type;
    int requestId;
    int length;
    char payload[PAYLOAD_SIZE];
} Message;

typedef struct Client {
    int requestId;
    char username[30];
    int connSock;
    int uploadSuccess;
} Client;

/*
 * clone Message mess from temp
 * @param Message* mess, Message temp
 * @return 1 if clone success
 */
int copyMess(Message* mess, Message temp);

int sendMessage(int socket, Message msg);

int receiveMessage(int socket, Message* msg);

#endif
