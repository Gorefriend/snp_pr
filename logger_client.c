/******************************************************************************
 * Name             logger_client.c
 *
 * Programmer:      Philip Muskovac
 *
 * Description:     logger client implementation
 *
 *****************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "logger.h"
#include "logger_client.h"

int sanitizeLogMessage(char *szLogMessage)
{
    char szTmp[LOG_MESSAGE_SIZE];

    sprintf(szTmp, "%-78s\n", szLogMessage);

    strncpy(szLogMessage, szTmp, LOG_MESSAGE_SIZE);

    return 0;
}

void log_to_string(struct log_record *pstLog, char *szMessage)
{
    sprintf(szMessage, "%-12s%-12s%-12s%-43s\n", pstLog->msgtype, pstLog->id, pstLog->terminal, pstLog->date);
}

int init_logger_client(char *server_address)
{
    struct sockaddr_in server;

    if ((iLoggerClientSocketFd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        return -1;
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(LOG_LISTEN_PORT);

    server.sin_addr.s_addr = inet_addr(server_address);

    if (connect(iLoggerClientSocketFd, (struct sockaddr *) &server, sizeof(server))) {
        return -1;
    }

    return 0;
}

int log_message(char *message)
{
    char szBuffer[LOG_MESSAGE_SIZE];

    strncpy(szBuffer, message, sizeof(szBuffer));

    sanitizeLogMessage(szBuffer);

    if (write(iLoggerClientSocketFd, szBuffer, LOG_MESSAGE_SIZE) != LOG_MESSAGE_SIZE) {
        return -1;
    }

    return 0;
}

int log_access(int iId, int iTerminal)
{
    return 0;
}

void close_logger()
{
    close(iLoggerClientSocketFd);
}
