#include "protocol.h"

#include "status.h"

int copyMess(Message* mess, Message temp) {
    mess->type = temp.type;
    mess->requestId = temp.requestId;
    mess->length = temp.length;
    memcpy(mess->payload, temp.payload, temp.length + 1);
    return 1;
}

int sendMessage(int socket, Message msg) {
    int dataLength, nLeft, idx;
    nLeft = sizeof(Message);
    idx = 0;
    while (nLeft > 0) {
        dataLength = send(socket, (char*) &msg + idx, nLeft, 0);
        if (dataLength <= 0) {
            perror("\nError: ");
            close(socket);
            return -1;
        }
        nLeft -= dataLength;
        idx += dataLength;
    }
    return 1;
}

int receiveMessage(int socket, Message* msg) {
    char recvBuff[BUFF_SIZE];
    int ret, nLeft, idx, bytes_recv;
    Message recvMessage;
    ret = 0;
    idx = 0;
    nLeft = sizeof(Message);
    while (nLeft > 0) {
        bytes_recv = nLeft > BUFF_SIZE ? BUFF_SIZE : nLeft;
        ret = recv(socket, recvBuff, bytes_recv, 0);
        if (ret <= 0) {
            perror("\nError: ");
            close(socket);
            return -1;
        }
        memcpy(&(((char*) &recvMessage)[idx]), recvBuff, ret);
        idx += ret;
        nLeft -= ret;
    }
    copyMess(&(*msg), recvMessage);
    return 1;
}