/******************************************************************************
 * Name             logger_client.h
 *
 * Programmer:      Philip Muskovac
 *
 * Description:     logger client interface
 *
 *****************************************************************************/

#ifndef __logger_client_h__
#define __logger_client_h__

#include "logger.h"

int iLoggerClientSocketFd;

int sanitizeLogMessage(char *szLogMessage);
void log_to_string(struct log_record *pstLog, char *szMessage);

int init_logger_client(char *server_address);

int log_message(char *message);

int log_access(int iId, int iTerminal);

void close_logger();

#endif /* __logger_client_h__ */
