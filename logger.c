/******************************************************************************
 * Name             logger.h
 *
 * Programmer:      Philip Muskovac
 *
 * Description:     logger process implementation
 *
 *****************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>

#include "logger.h"
#include "logger_client.h"

int iLoggerSocketFd;
FILE *fdLogFile;

void vClose(void)
{
    close(iLoggerSocketFd);

    // close logfile if it's open
    if (!fdLogFile) {
        fflush(fdLogFile);
        fclose(fdLogFile);
    }
}


int main(int argc, char **argv)
{
    int iConnectionFd;
    int iOptVal;
    int iRead;
    socklen_t AddressLength;
    struct sockaddr_in server, client;

    char szOpt;
    char quiet = 0;

    int iRc;
    pid_t pid;

    char *szFileName;
    struct flock stFLock;

    char szLogMessageBuffer[LOG_MESSAGE_SIZE];

    atexit(vClose);

    // command line option parsing
    while ((szOpt = getopt(argc, argv, "q")) >= 0) {
        switch (szOpt) {
            case 'q':
                // -q: quiet
                quiet = 1;
                break;
        }
    }

    // open a socket
    if ((iLoggerSocketFd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        fprintf(stderr, "Error during socket creation: %s\n", strerror(errno));
        exit(1);
    }

    iOptVal = 1;
    setsockopt(iLoggerSocketFd, SOL_SOCKET, SO_REUSEADDR, &iOptVal, sizeof(iOptVal));

    // set the socket settings
    server.sin_family = AF_INET;
    server.sin_port = htons(LOG_LISTEN_PORT);
    server.sin_addr.s_addr = INADDR_ANY;

    if (bind(iLoggerSocketFd, (struct sockaddr *) &server, sizeof(server)) < 0) {
        fprintf(stderr, "Error during socket bind: %s\n", strerror(errno));
        exit(1);
    }

    // open the socket for incoming connections
    if (listen(iLoggerSocketFd, 16) < 0) {
        fprintf(stderr, "Error during socket listen: %s\n", strerror(errno));
        exit(1);
    }

    if (!quiet) {
        printf("Logger started, waiting for connections...\n");
    }

    // wait for remote connections and fork a transmission handler
    while ((iConnectionFd = accept(iLoggerSocketFd, (struct sockaddr *) &client, &AddressLength)) > 0) {
        if ((pid = fork()) < 0) {
            fprintf(stderr, "Error during fork: %s\n", strerror(errno));
            exit(1);
        } else if (pid == 0) {
            pid = fork();

            if (pid == 0) {

                // get the logfile name from the environment if one is set
                if (!(szFileName = getenv("LOGFILE"))) {
                    szFileName = DEFAULT_LOGFILE;
                }

                if (!(fdLogFile = fopen(szFileName, "a"))) {
                    fprintf(stderr, "Error opening logfile: %s\n", strerror(errno));
                    exit(2);
                }

                stFLock.l_whence = SEEK_END;
                stFLock.l_start = 0;
                stFLock.l_len = sizeof(szLogMessageBuffer) + 1;

                // read a full log message from the socket
                while ((iRead = read(iConnectionFd, szLogMessageBuffer, sizeof(szLogMessageBuffer))) > 0) {

                    // check whether the recieved data is a sane log message
                    if (sanitizeLogMessage(szLogMessageBuffer) < 0) {
                        fprintf(stderr, "Invalid log Message recieved, skipping: %s\n", szLogMessageBuffer);
                        continue;
                    }

                    if (!quiet) {
                        printf("Recieved Message:\n%s\n", szLogMessageBuffer);
                        printf("Length: %d\n", iRead);
                    }

                    // lock the file at the end for a log message
                    stFLock.l_type = F_WRLCK;
                    if (fcntl(fileno(fdLogFile), F_SETLKW, &stFLock) < 0) {
                        fprintf(stderr, "Error locking <%s>: %s\n", szFileName, strerror(errno));
                        continue;
                    }

                    // write to the end of the file
                    fseek(fdLogFile, 0L, SEEK_END);
                    fwrite(szLogMessageBuffer, sizeof(szLogMessageBuffer), 1, fdLogFile);
                    fflush(fdLogFile);

                    // release the file lock
                    stFLock.l_type = F_UNLCK;
                    fcntl(fileno(fdLogFile), F_SETLK, &stFLock);
                }

            } else {
                exit(0);
            }
        } else {
            waitpid(pid, &iRc, 0);
        }
    }

    return 0;
}
