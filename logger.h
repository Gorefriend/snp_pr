/******************************************************************************
 * Name             logger.h
 *
 * Programmer:      Philip Muskovac
 *
 * Defines:         LOG_LISTEN_PORT: socket port the logger is listening on
 *
 * Description:     logger interface
 *
 *****************************************************************************/

#ifndef __logger_h__
#define __logger_h__

#define LOG_LISTEN_PORT 1199
#define DEFAULT_LOGFILE "access.log"

struct log_record {

    /* message type: access, error, ... */
    char msgtype[10];

    /* access id */
    char id[10];

    /* termial */
    char terminal[10];

    /* date and time in RFC 2822 format */
    char date[40];

};

#endif /* __logger_h__ */
