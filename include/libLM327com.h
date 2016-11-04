/* 
 * File:   libLM327com.h
 * Author: Fernando Alcala <asecking@gmail.com>
 *
 * Created on 28 de abril de 2016, 16:28
 * 
 * This library implements basic functions to dialog with the LM327 with the
 * required protocol.
 * 
 */
#ifndef LM327COMLIB_H
#define LM327COMLIB_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
    //define something for Windows (32-bit and 64-bit, this part is common)
#define OBD_PORT "COM8"
#ifdef _WIN64
    //define something for Windows (64-bit only)
#endif
#elif __APPLE__
#include "TargetConditionals.h"
#if TARGET_IPHONE_SIMULATOR
    // iOS Simulator
#elif TARGET_OS_IPHONE
    // iOS device
#elif TARGET_OS_MAC
//#define OBD_PORT "/dev/cu.OBDII-Port"
#define OBD_PORT "/dev/cu.OBDLinkLX-STN-SPP"
    // Other kinds of Mac OS
#else
#error "Unknown Apple platform"
#endif
#elif __linux__
    // linux
    //#define OBD_PORT "/dev/ttyS0"
#define OBD_PORT "/dev/rfcomm0"
#elif __unix__ // all unices not caught above
    // Unix
#elif defined(_POSIX_VERSION)
    // POSIX
#else
#error "Unknown compiler"
#endif

#include "OBDparameters.h"
#define HEXLENGTH 5
#define ATMSGLEN 500
#define OBDMSGLEN 500
#define MAX_ANSWER 500
    /* Terminator character of LM327 answers */
#define LM327_EOL '\r'
    /* Terminator to end LM327 command strings */
#define LM327_EOLSTRING "\r"
    /* Initialization command for LM327 */
#define OBD_INITSTRING "e0"
#define OBDV_MAXSTR 40

    /*Errors definition*/
    typedef enum {       
        OBD_END = -10,
        OBD_ERRPORT = -9,
        OBD_SYNC = -8,
        OBD_WRITE = -7,
        OBD_FIELDS = -6,
        OBD_COMMAND = -5,
        OBD_EMPTY = -4,
        OBD_ERROR = -3,
        OBD_TIMEOUT = -2,
        OBD_CLOSED = -1,
        OBD_OK = 0
    } OBD_ERRORCODE;

    typedef struct obdvalue_str {
        struct obdvalue_str *next;
        time_t obdv_ts;
        int obdv_parameter;

        union {
            int i;
            float w;
            unsigned long flags;
            double d;
            char str[OBDV_MAXSTR];
        } obdv_value;
    } OBD_value;

    typedef struct {
        OBD_value *first;
        OBD_value *last;
    } OBD_vallist;


    char *cmd_description(t_obdcmdcode cmd);
    void obdparameter(FILE *fp);
    int model(FILE *fp, int fd, char *MVIN);
    int person(FILE *fp);
    void vehicle(FILE *fp, char *VIN);
    void drives(FILE *fp, char *VIN, int PId);
    void track(FILE *fp, char *VIN, int PId);
    int gps_data(FILE *fp);
    int cmd_fields(t_obdcmdcode cmd);
    void read_timeout(int sig);
    void sample(char *buffer, FILE *fp, int parameter, OBD_value *value);
    int openOBD_port();
    int write_port(int fd, char *buffer, int l);
    int write_atmsg(int fd, char *msg);
    int write_obdmsg(int fd, char *msg);
    int read_port(int fd, char *buffer, int l, int timeout);
    int read_msg(int fd, char *buffer, int l, int timeout);
    int read_VINmsg(int fd, char *vinstring, int l, int timeout);
    int sync_protocol(int fd);
    OBD_value *obd_newvalue();
    void obd_appendvalue(OBD_vallist *list, OBD_value *value);
    int hex2int(char *answer);
    int separate_string(char *answer, unsigned int *Cack, unsigned int *A, unsigned int *B, unsigned int *C, unsigned int *D);
    int separate_VINstring(char *answer, unsigned int *Cack, char *vinstring);
    int read_parameter(int fd, int hexadecimal, char *answer, OBD_value *value);

#ifdef __cplusplus
}
#endif

#endif /* LM327COMLIB_H */