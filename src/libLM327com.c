/* 
 * File:   libLM327com.c
 * Author: Fernando Alcala <asecking@gmail.com>
 *
 * Created on 28 de abril de 2016, 15:45
 * 
 * This library implements basic functions to dialog with the LM327 with the
 * required protocol.
 * 
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>
#include "libLM327com.h"
#include "OBDparameters.h"

#ifdef __cplusplus
extern "C" {
#endif

    /* Constants definition.
     */
#define DEBUGLEVEL 1

    /* This function is a handler of the alarm if timeout happens.
     */
    void read_timeout(int sig) {
    }

    /* This function prints the information with a timestamp of the current time
     * and date in a text file.
     * Arguments: 
     *              buffer: pointer to the buffer where the information is.
     *              fp: This is the pointer to a FILE object that identifies the
     *                  stream.
     * No return value.
     */
    void timestamp(char *buffer, FILE *fp) {
        time_t ltime;
        //        struct tm *Tm;

        ltime = time(NULL);
        //        Tm = localtime(&ltime);
        fprintf(fp, "%s;%ld\n", buffer, ltime);
    }

    /* This function opens the OBD BT port of the car. It has no arguments. 
     * Return value:
     *                  fd: file descriptor.
     */
    int openOBD_port() {
        char *portname = OBD_PORT;
        int fd;

        fd = open(portname, O_RDWR | O_NOCTTY);
        if (fd == -1)
            perror("open_port: Unable to open communication port\n");
        else
            fcntl(fd, F_SETFL, 0);
        return (fd);
    }

    /* This function writes into the OBD port.
     * Arguments:
     *              fd: file descriptor to write in.
     *              buffer: pointer to the buffer where the command is.
     *              l: maximum length of the buffer.
     * Return value:
     *              n: integer that indicates wether the function has succeeded.
     */
    int write_port(int fd, char *buffer, int l) {
        int n;
        n = write(fd, buffer, l);
        if (n < 0)
            perror("write_port:: error sending bytes to ELM port");
        return n;
    }

    /* This function creates an AT commad legible for the OBD and writes it into it.
     * Arguments:
     *              fd: file descriptor to write in.
     *              buffer: pointer to the buffer where the command is.
     * Return value:
     *              n: integer that indicates wether the function has succeeded.
     */
    int write_atmsg(int fd, char *msg) {
        char buffer[ATMSGLEN];
        strncpy(buffer, "AT", ATMSGLEN);
        strncat(buffer, msg, ATMSGLEN);
        strncat(buffer, LM327_EOLSTRING, ATMSGLEN);
        return write_port(fd, buffer, strlen(buffer));
    }

    /*This function creates an OBD commad legible for the OBD and writes it into it.
     * * Arguments:
     *              fd: file descriptor to write in.
     *              buffer: pointer to the buffer where the command is.
     * Return value:
     *              n: integer that indicates wether the function has succeeded.
     */
    int write_obdmsg(int fd, char *msg) {
        char buffer[OBDMSGLEN] = "";
        strncat(buffer, msg, OBDMSGLEN);
        strncat(buffer, LM327_EOLSTRING, OBDMSGLEN);
        return write_port(fd, buffer, strlen(buffer));
    }

    /* This function reads a single character from OBD.
     * Arguments:
     *              fd: Open file descriptor to read from.
     *              buffer: pointer to the buffer to store the character.
     *              l: maximum length of the buffer.
     *              timeout : 
     *                          > 0 time to wait for input before aborting read.
     *                          0 > wait input forever.
     *                          -1  uses non blocking input and returns inmediately 
     *                              if input stream is empty in this moment.
     * Return value:
     *          1: read 1 character
     *          OBD_EMPTY: non block input returned inmediately when using non 
     *                     blocking input.
     *          OBD_CLOSED: fd is closed.
     *          OBD_TIMEOUT: blocking input didn't answer before specified timeout.
     *          OBD_ERROR: other errors.
     */

    int read_port(int fd, char *buffer, int l, int timeout) {
        struct sigaction act, oact;

        int prev_flags, n;
        if (timeout == -1) {
            prev_flags = fcntl(fd, F_GETFL, 0);
            fcntl(fd, F_SETFL, prev_flags | O_NONBLOCK);
#if DEBUGLEVEL
            fprintf(stderr, "DEBUG::read_port, set non block\n");
#endif
        } else if (timeout > 0) {
#if DEBUGLEVEL
            fprintf(stderr, "DEBUG::read_port, set timeout\n");
#endif
            act.sa_handler = read_timeout;
            act.sa_flags = 0;
            sigaction(SIGALRM, &act, &oact);
            alarm(timeout);
        }
        n = read(fd, buffer, 1);
        if (timeout == -1)
            fcntl(fd, F_SETFL, prev_flags);
        else if (timeout > 0) {
            alarm(0);
            sigaction(SIGALRM, &oact, NULL);
        }

        if (n == 0) {
            perror("read_port:: closed ELM port while reading");
            return OBD_CLOSED;
        }
        if (n < 0) {
            switch (errno) {
                case EAGAIN:
                    perror("read_port:: empty ELM port");
                    return OBD_EMPTY;
                case EINTR:
                    perror("read_port:: timeout reading from ELM port");
                    return OBD_TIMEOUT;
                default:
                    perror("read_port:: error reading bytes from ELM port");
                    return OBD_ERROR;
            }
        } else buffer[n] = 0;
#if DEBUGLEVEL
        fprintf(stderr, "DEBUG::read_port, read %d bytes, {%c}\n", n, buffer[0]);
#endif
        return n;
    }

    /* 
     * Reads input from OBD until first NL, removing it from the string and adding
     * a zero at the end.
     * This function goes byte by byte cleaning the output information for a better
     * output. 
     * Arguments: 
     *              fd: Open file descriptor to read from.
     *              buffer: pointer the string in which to store the information.
     *              l: maximum length of the buffer.
     *              timeout : 
     *                          > 0 time to wait for input before aborting read.
     *                          0 > wait input forever.
     *                          -1  uses non blocking input and returns inmediately 
     *                              if input stream is empty in this moment.
     * The out parameter is an integer showing the error or the length of what is
     * being read.
     * Return value:
     *          >0: read n characters not including EOL nor final zero
     *          OBD_EMPTY: non block input returned inmediately when using non 
     *                     blocking input.
     *          OBD_CLOSED: fd is closed.
     *          OBD_TIMEOUT: blocking input didn't answer before specified timeout.
     *          OBD_ERROR: other errors.
     */

    int read_msg(int fd, char *buffer, int l, int timeout) {
        int i = 0, r;
        while (1) {
            r = read_port(fd, &(buffer[i]), 1, timeout);
            /* Check if read returned an error after reading some characters and 
             * return the partial line */
            if (r < 0 && i > 0) {
                buffer[i] = 0;
                break;
            }
            /* Check if read returned an error with an empty buffer */
            if (r < 0) {
#if DEBUGLEVEL
                fprintf(stderr, "DEBUG::read_msg, PORTRESULT = %d\n", r);
#endif
                return r;
            }
            /* We read one new character */
            /* If we receive EOL or > at the beginning of a line or a NULL character
             * then ignore it and go on reading characters */
            if (((buffer[i] == LM327_EOL || buffer[i] == '>') && i == 0) || buffer[i] == 0) {
                buffer[i] = 0;
                continue;
            }
            /* A NL character marks the end of the message. Overwrite it witha zero
             * and return the message */
            if (buffer[i] == LM327_EOL) {
                buffer[i] = 0;
                break;
            }
            i++;
        }
#if DEBUGLEVEL
        fprintf(stderr, "DEBUG::read_msg, read %d bytes, PORTRESULT = %d, {%s}\n", i, r, buffer);
#endif
        return i;
    }

    /* This function makes sure the buffer is empty and OBD is ready to work 
     * correctly by writing a neutral command and checking the expected answer. 
     * Arguments:
     *              fd: file descriptor.
     * Return value:
     *              n: integer that indicates wether the function has succeeded.
     */

    int sync_protocol(int fd) {
        char answer[MAX_ANSWER];
        int n = -1;
        /* Empty input channel */
#if DEBUGLEVEL
        fprintf(stderr, "DEBUG::sync_protocol, starting to clean input\n");
#endif
        while (1) {
            int status;
            while ((status = read_msg(fd, answer, MAX_ANSWER, -1)) > 0);
            if (status != OBD_EMPTY) {
                perror("error emptying buffer");
                break;
            }
#if DEBUGLEVEL
            fprintf(stderr, "DEBUG::sync_protocol, input empty\n");
#endif
            write_atmsg(fd, OBD_INITSTRING);
#if DEBUGLEVEL
            fprintf(stderr, "DEBUG::sync_protocol, sent command echo off (AT%s)\n", OBD_INITSTRING);
#endif
            read_msg(fd, answer, MAX_ANSWER, 0);
#if DEBUGLEVEL
            fprintf(stderr, "DEBUG::sync_protocol, read answer=%s\n", answer);
#endif
            if (strncmp(answer, "OK", strlen("OK")) == 0) {
                n = 0;
                break;
            }
        }
        return n;
    }

    //    OBD_vallist *obd_createlist() {
    //        return (OBD_vallist*) malloc(sizeof (OBD_vallist));
    //    }

    OBD_value *obd_newvalue() {
        return (OBD_value *) malloc(sizeof (OBD_value));
    }

    void obd_appendvalue(OBD_vallist *list, OBD_value *value) {
        list->last = value;
        list->last->next = value;
        value->next = NULL;
    }

    /* This function converts a hexadecimal string not starting with 0x into an
     * integer in decimal value.
     * Arguments:
     *              answer: the hexadecimal string.
     * No return value.
     */
    int hex2int(char *answer) {
        return ((int) strtol(answer, NULL, 16));
    }

    /* This function separates the hexadecimal string given by the OBD and
     * modified by us into 2 digits elements and storing them into their
     * corresponding letters for calculating afterwards the real value.
     * Arguments:
     *              answer: the hexadecimal string.
     *              letters: data bytes given by the OBD.
     *              A, B, C, D: the letter in which a hexadecimal string is
     *                          going to be stored.
     * No return value.
     */
    void separate_string(char *answer, int databyte, int *A, int *B, int *C, int *D) {
        char hexadecimal[databyte + 2][3];
        switch (databyte) {
            case 1:
                sscanf(answer, "%2s %2s %2s ", (char *) &hexadecimal[0], (char *) &hexadecimal[1], (char *) &hexadecimal[2]);
                *A = hex2int(hexadecimal[2]);
                *B = 0;
                *C = 0;
                *D = 0;
                break;
            case 2:
                sscanf(answer, "%2s %2s %2s %2s ", (char *) &hexadecimal[0], (char *) &hexadecimal[1], (char *) &hexadecimal[2], (char *) &hexadecimal[3]);
                *A = hex2int(hexadecimal[2]);
                *B = hex2int(hexadecimal[3]);
                *C = 0;
                *D = 0;
                break;
            case 3:
                sscanf(answer, "%2s %2s %2s %2s %2s ", (char *) &hexadecimal[0], (char *) &hexadecimal[1], (char *) &hexadecimal[2], (char *) &hexadecimal[3], (char *) &hexadecimal[4]);
                *A = hex2int(hexadecimal[2]);
                *B = hex2int(hexadecimal[3]);
                *C = hex2int(hexadecimal[4]);
                *D = 0;
                break;
            case 4:
                sscanf(answer, "%2s %2s %2s %2s %2s %2s ", (char *) &hexadecimal[0], (char *) &hexadecimal[1], (char *) &hexadecimal[2], (char *) &hexadecimal[3], (char *) &hexadecimal[4], (char *) &hexadecimal[5]);
                *A = hex2int(hexadecimal[2]);
                *B = hex2int(hexadecimal[3]);
                *C = hex2int(hexadecimal[4]);
                *D = hex2int(hexadecimal[5]);
                break;
        }
    }

    /* This function takes the command which you want to know the information
     * from, writes it into the OBD, reads the answer, translates it into human
     * readable information.
     * Arguments:
     *              fd: file descriptor.
     *              hexadecimal: hexadecimal identification number of the 
     *                           command.
     *              answer: where the final value is going to be stored.
     * Return value:
     *              n: integer that indicates whether an error ocurred.  
     */
    int read_parameter(int fd, int hexadecimal, char *answer, OBD_value *value) {
        int n, A, B, C, D, finalvalue;
        char buffer[HEXLENGTH];
        sprintf(buffer, "%04X", hexadecimal);
        //        itoa(hexadecimal, buffer, 16);
        switch (hexadecimal) {
            case 0x0104:
                n = write_obdmsg(fd, buffer);
                n = read_msg(fd, answer, MAX_ANSWER, 0);
                separate_string(answer, 1, &A, &B, &C, &D);
                value->obdv_parameter = hexadecimal;
                value->obdv_ts = time(NULL);
                value->obdv_value.w = 100 * A / 255;
                sprintf(answer, "%f", value->obdv_value.w);
                strncpy(value->obdv_value.str, answer, OBDV_MAXSTR);
                break;
            case 0x0105:
                n = write_obdmsg(fd, buffer);
                n = read_msg(fd, answer, MAX_ANSWER, 0);
                separate_string(answer, 1, &A, &B, &C, &D);
                value->obdv_parameter = hexadecimal;
                value->obdv_ts = time(NULL);
                value->obdv_value.i = A - 40;
                sprintf(answer, "%d", value->obdv_value.i);
                strncpy(value->obdv_value.str, answer, OBDV_MAXSTR);
                break;
            case 0x010B:
                n = write_obdmsg(fd, buffer);
                n = read_msg(fd, answer, MAX_ANSWER, 0);
                separate_string(answer, 1, &A, &B, &C, &D);
                value->obdv_parameter = hexadecimal;
                value->obdv_ts = time(NULL);
                value->obdv_value.i = A;
                sprintf(answer, "%d", value->obdv_value.i);
                strncpy(value->obdv_value.str, answer, OBDV_MAXSTR);
                break;
            case 0x010C:
                n = write_obdmsg(fd, buffer);
                n = read_msg(fd, answer, MAX_ANSWER, 0);
                separate_string(answer, 2, &A, &B, &C, &D);
                value->obdv_parameter = hexadecimal;
                value->obdv_ts = time(NULL);
                value->obdv_value.w = (256 * A + B) / 4;
                sprintf(answer, "%f", value->obdv_value.w);
                strncpy(value->obdv_value.str, answer, OBDV_MAXSTR);
                break;
            case 0x010D:
                n = write_obdmsg(fd, buffer);
                n = read_msg(fd, answer, MAX_ANSWER, 0);
                separate_string(answer, 1, &A, &B, &C, &D);
                value->obdv_parameter = hexadecimal;
                value->obdv_ts = time(NULL);
                value->obdv_value.i = A;
                sprintf(answer, "%d", value->obdv_value.i);
                strncpy(value->obdv_value.str, answer, OBDV_MAXSTR);
                break;
        }
        return n;

    }


#ifdef __cplusplus
}
#endif
