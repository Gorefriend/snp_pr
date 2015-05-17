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
#define LOG_MESSAGE_SIZE 80

struct log_record {

    // message type: access, error, ...
    char msgtype[12];

    // access id
    char id[12];

    // termial
    char terminal[12];

    // date and time in RFC 2822 format
    char date[43];

    char eol[1];

};

#endif /* __logger_h__ */
