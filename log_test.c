#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "logger_client.h"
#include "logger.h"

#define LOCALHOST "127.0.0.1"

int main(int argc, char **argv)
{
    int iRet;

    struct log_record record;

    strcpy(record.msgtype, "access");
    strcpy(record.id, "039430");
    strcpy(record.terminal, "02");
    strcpy(record.date, "foo");

    init_logger_client(LOCALHOST);

    char szMessage[LOG_MESSAGE_SIZE];

    log_to_string(&record, szMessage);

    iRet = log_message(szMessage);

    printf("log_message: %d\n", iRet);

    iRet = log_access(0, 1);

    printf("log_access. %d\n", iRet);

    close_logger();

    return 0;
}
