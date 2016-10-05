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
#include "OBDparameters.h"
#include "libLM327com.h"

#ifdef __cplusplus
extern "C" {
#endif

    /* Constants definition.
     */
#define DEBUGLEVEL 1

    static t_obdparameter obd_parameters[] = {
        {ENGINE_LOAD, 1, "ENGLOAD", ENGINE_LOAD_DESC},
        {ENGINE_COOLANT_TEMP, 1, "COOLTEMP", ENGINE_COOLANT_TEMP_DESC},
        {INTAKE_PRESSURE, 1, "INPRESS", INTAKE_PRESSURE_DESC},
        {ENGINE_RPM, 2, "ENGRPM", ENGINE_RPM_DESC},
        {VEHICLE_SPEED, 1, "SPEED", VEHICLE_SPEED_DESC},
        {INTAKE_TEMP, 1, "AIRTEMP", INTAKE_TEMP_DESC},
        {MAF_SENSOR, 2, "MAFRATE", MAF_SENSOR_DESC},
        {THROTTLE, 1, "THROTTLE", THROTTLE_DESC},
        {ENGINE_RUNTIME, 2, "RUNTIME", ENGINE_RUNTIME_DESC},
        {FUEL_RAIL_PRES_ALT, 2, "FURAPRESS", FUEL_RAIL_PRES_ALT_DESC},
        {BAROMETRIC_PRESSURE, 1, "ABSBARPRESS", BAROMETRIC_PRESSURE_DESC},
        {O2S1_WR_LAMBDA_I, 2, "AIRFUELRAT", O2S1_WR_LAMBDA_I_DESC},
        {REL_THROTTLE_POS, 1, "RELTHROPOS", REL_THROTTLE_POS_DESC},
        {AMB_AIR_TEMP, 1, "AMBAIRTEMP", AMB_AIR_TEMP_DESC},
        {ACCEL_POS_D, 1, "ACCELPOSD", ACCEL_POS_D_DESC},
        {ACCEL_POS_E, 1, "ACCELPOSE", ACCEL_POS_E_DESC},
        {COMMANDED_THROTTLE, 1, "THROACT", COMMANDED_THROTTLE_DESC},
        {VIN, 17, "VIN", "Vehicle Identification Number"},
        {OBDCMDCODE_ENDOFLIST, -1, ""}
    };

    /* Translates command code to a description text.
     * Arguments:
     *              cmd: code for OBD command.
     * Return value:
     *              obdp_desc: description for a specified command code.
     *              NULL:  Command code not found.
     */
    char *cmd_description(t_obdcmdcode cmd) {
        int i;

        for (i = 0; obd_parameters[i].obdp_code != OBDCMDCODE_ENDOFLIST; i++) {
            if (obd_parameters[i].obdp_code == cmd) return obd_parameters[i].obdp_desc;
        }
        return NULL;
    }
    
        /* Prints text code of OBD PARAMETER table for the database.
     * Arguments:
     *              fp: This is the pointer to a FILE object that identifies the
     *                  stream.
     */
    void obdparameter(FILE *fp) {
        int i;

        for (i = 0; obd_parameters[i].obdp_code != OBDCMDCODE_ENDOFLIST; i++) {
            fprintf(fp, "INSERT INTO OBD_PARAMETER (OBDHexCode, OBDLabel) VALUES (%d, %s);", obd_parameters[i].obdp_code, obd_parameters[i].obdp_desc);
        }
    }
    
        /* Prints text code of the car MODEL table for the database.
     * Arguments:
     *              fp: This is the pointer to a FILE object that identifies the
     *                  stream.
     *              fd: file descriptor to write in.
     * Return value:
     *              0 : Everything went as desired.
     *              -1 : Something went wrong. 
     */
    int model(FILE *fp, int fd, char *MVIN) {
        int n, MCC, MWeight, MHP;
        char MFuel, MEngine[20];
        OBD_value * values;
        values = obd_newvalue();
        if (values->next == NULL) {
            n = read_parameter(fd, 17, MVIN, values);
            sleep(1); //TODO ver tiempo
            if (n < 0) { //If something went wrong or finished reading, reset OBD and close the file
                write_atmsg(fd, "Z");
                sleep(1);
                close(fd);
                return -1;
            } else {
                fprintf(fp, "INSERT INTO MODEL (MVIN, MCC, MFuel, MEngine, MHP, MWeight) VALUES (%s, %d, %c, %s, %d, %d);", values->obdv_value.str, MCC, MFuel, MEngine, MHP, MWeight);
            }
        }
        return 0;
    }

       /* Prints text code of PERSON table for the database.
     * Arguments:
     *              fp: This is the pointer to a FILE object that identifies the
     *                  stream.
     * Return value:
     *              PId: Identificator of the person.
     */
    int person(FILE *fp) {
        int PId;
        char PName[20];

        fprintf(fp, "INSERT INTO PERSON (PId, PName) VALUES (%d, %s);", PId, PName);
        return PId;
    }
    
       /* Prints text code of VEHICLE table for the database.
     * Arguments:
     *              fp: This is the pointer to a FILE object that identifies the
     *                  stream.
     *              fd: file descriptor to write in.
     * Return value:
     *              0 : Everything went as desired.
     *              -1 : Something went wrong.
     */
    int vehicle(FILE *fp, int fd) {
        char VIN[30];
        int n = 0;
        
        n = model(fp, fd, VIN);
        fprintf(fp, "INSERT INTO VEHICLE (VVIN, MVIN) VALUES (%s, %s);", VIN, VIN);
        return n;
    }
    
        /* Prints text code of Drives table for the database.
     * Arguments:
     *              fp: This is the pointer to a FILE object that identifies the
     *                  stream.
     *              fd: file descriptor to write in.
     * Return value:
     *              0 : Everything went as desired.
     *              -1 : Something went wrong.
     */
    int drives(FILE *fp, int fd) {
        char VIN[30];
        int n = 0, PId;
        
        PId = person(fp);
        n = model(fp, fd, VIN);
        fprintf(fp, "INSERT INTO Drives (PId, VVIN) VALUES (%d, %s);", PId, VIN);
        return n;
    }
    
    
    
    
    /* Searchs command code to get the number of bytes of its answer.
     * Arguments: 
     *              cmd: code for OBD command.
     * Return value:
     *              obdp_fields: number of bytes in answer including command
     *                           code ACK.
     *              -1: command code not found.
     */
    int cmd_fields(t_obdcmdcode cmd) {
        int i;

        for (i = 0; obd_parameters[i].obdp_code != OBDCMDCODE_ENDOFLIST; i++) {
            if (obd_parameters[i].obdp_code == cmd) return obd_parameters[i].obdp_fields;
        }
        return -1;
    }

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
     *              parameter: number of the desired parameter.
     * No return value.
     */
    void timestamp(char *buffer, FILE *fp, int parameter, OBD_value *value) {
        time_t ltime;
        char SName [5] = "Hola";
        int id = 5;
        
        //        struct tm *Tm;

        ltime = time(NULL);
        //        Tm = localtime(&ltime);
        //        fprintf(fp, "%s;%ld\n", buffer, ltime);
        fprintf(fp, "INSERT INTO SAMPLE (STs, OBDHexCode, SName, SFloatValue, SIntValue, TId) VALUES (%ld, %d, %s, %f, %d, %d);", value->obdv_ts, value->obdv_parameter, SName,value->obdv_value.w, value->obdv_value.i, id);
        //fprintf(fp, "{\"Pname\":\"%s\",\"INTvalue\":%d,\"FLOATvalue\":%f,\"CHARvalue\":\"%s\",\"Ts\":%ld},\n", obd_parameters[parameter].obdp_parname, value->obdv_value.i, value->obdv_value.w, value->obdv_value.str, value->obdv_ts);
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
            perror("openOBD_port: Unable to open communication port\n");
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
     *              n: number of bytes written. If -1 indicates an error.
     */
    int write_port(int fd, char *buffer, int l) {
        int n;

        n = write(fd, buffer, l);
        if (n < 0)
            perror("write_port: error sending bytes to ELM port");
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
     *              n: number of bytes written. If -1 indicates an error.
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
     *          1: read one character.
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
            return OBD_CLOSED; //TODO No estoy muy seguro de que implique que se ha cerrado
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
     *          > 0: read n characters not including EOL nor final zero
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
            if (((buffer[i] == LM327_EOL || buffer[i] == '>') && i == 0) || buffer[i] == 0) { //TODO creo que sobra la segunda condición
                buffer[i] = 0;
                continue;
            }
            /* A NL character marks the end of the message. Overwrite it with a zero
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

    /* 
     * Reads input from OBD until first NL when VIN is asked, removing it from 
     * the string and adding a zero at the end.
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
     *          > 0: read n characters not including EOL nor final zero
     *          OBD_EMPTY: non block input returned inmediately when using non 
     *                     blocking input.
     *          OBD_CLOSED: fd is closed.
     *          OBD_TIMEOUT: blocking input didn't answer before specified timeout.
     *          OBD_ERROR: other errors.
     */
    int read_VINmsg(int fd, char *vinstring, int l, int timeout) {
        int r;
        unsigned int Cack;
        char buffer[OBDV_MAXSTR];

        read_msg(fd, buffer, l, timeout);
        sleep(1);
        read_msg(fd, buffer, l, timeout);
        sleep(1);
        strcat(vinstring, buffer);
        read_msg(fd, buffer, l, timeout);
        sleep(1);
        strcat(vinstring, buffer);
        read_msg(fd, buffer, l, timeout);
        strcat(vinstring, buffer);
        sleep(1);
        return r;
    }

    /* This function makes sure the buffer is empty and OBD is ready to work 
     * correctly by writing a neutral command and checking the expected answer. 
     * Arguments:
     *              fd: file descriptor.
     * Return value:
     *              -1: Not synced properly.
     *               0: Synced properly.
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
            read_msg(fd, answer, MAX_ANSWER, 0); //TODO por que timeout= 0 y no guardamos su salida
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

    /* Allocates memory for new OBD_value struct.
     */
    OBD_value *obd_newvalue() {
        return (OBD_value *) malloc(sizeof (OBD_value));
    }

    /* Shifts to next struct of a list.
     */
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
     *              Cack: command value for checking.
     *              A, B, C, D: the letter in which a hexadecimal string is
     *                          going to be stored.
     * Return value:
     *              fields: number of fields read in the string.
     */
    int separate_string(char *answer, unsigned int *Cack, unsigned int *A, unsigned int *B, unsigned int *C, unsigned int *D) {
        int fields;
        unsigned int C1, C2;

        fields = sscanf(answer, "%2x %2x %2x %2x %2x %2x", &C1, &C2, A, B, C, D);
        if (fields >= 2) *Cack = (C1 - 64) * 256 + C2; /* C = (C1 << 8) | C2 */
        return fields;
    }

    /* This function separates the hexadecimal string given by the OBD when VIN
     * is asked modified by us into 2 digits elements and storing them into their
     * corresponding letters for calculating afterwards the real value.
     * Arguments:
     *              answer: the hexadecimal string.
     *              Cack: command value for checking.
     * No return value.
     */
    int separate_VINstring(char *vinstring, unsigned int *Cack, char *VIN) {
        int fields, A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q;
        unsigned int C1, C2;

        fields = sscanf(vinstring, "0: %2x %2x 01 %2x %2x %2x 1: %2x %2x %2x %2x %2x %2x %2x 2: %2x %2x %2x %2x %2x %2x %2x ", &C1, &C2, &A, &B, &C, &D, &E, &F, &G, &H, &I, &J, &K, &L, &M, &N, &O, &P, &Q);
        if (fields >= 2) {
            *Cack = (C1 - 64) * 256 + C2; /* C = (C1 << 8) | C2 */
            sprintf(VIN, "%C%C%C%C%C%C%C%C%C%C%C%C%C%C%C%C%C", A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q);
        }

        return fields;
    }

    /* This function takes the command which you want to know the information
     * from, writes it into the OBD, reads the answer, translates it into human
     * readable information.
     * Arguments:
     *              fd: file descriptor.
     *              i: number of the desired parameter.
     *              answer: where the final value is going to be stored.
     *              value: an OBD_value struct.
     * Return value:
     *              r: read r characters not including EOL nor final zero.
     *              n: number of bytes written. 
     *              OBD_ERROR: Error when getting the answer.
     *              OBD_COMMAND: Error matching answer with command.
     */
    int read_parameter(int fd, int i, char *answer, OBD_value *value) {
        unsigned int n, r, A, B, C, D, commandACK;
        char buffer[HEXLENGTH], vinstring[OBDV_MAXSTR];
        int fields;

        sprintf(buffer, "%04X", obd_parameters[i].obdp_code);
        //        itoa(hexadecimal, buffer, 16);
        n = write_obdmsg(fd, buffer);
        if (obd_parameters[i].obdp_code != 0x0902) {
            r = read_msg(fd, answer, MAX_ANSWER, 0);
            fields = separate_string(answer, &commandACK, &A, &B, &C, &D);
            if (fields < 2) {
                perror("read_parameter: unexpected error when getting answer\n");
                return OBD_ERROR;
            } else {
                if (obd_parameters[i].obdp_code != commandACK) {
                    perror("read_parameter: error matching command with answer\n");
                    return OBD_COMMAND;
                }
                value->obdv_parameter = obd_parameters[i].obdp_code;
                value->obdv_ts = time(NULL);
                strncpy(value->obdv_value.str, "", OBDV_MAXSTR);
                switch (obd_parameters[i].obdp_code) {
                    case 0x0104: //Calculated engine load
                        value->obdv_value.w = 100 * A / 255;
                        sprintf(answer, "%f", value->obdv_value.w);
                        break;
                    case 0x0105: //Engine coolant temperature
                        value->obdv_value.i = A - 40;
                        sprintf(answer, "%d", value->obdv_value.i);
                        break;
                    case 0x010B: //Intake manifold absolute pressure
                        value->obdv_value.i = A;
                        sprintf(answer, "%d", value->obdv_value.i);
                        break;
                    case 0x010C: //Engine RPM
                        value->obdv_value.w = (256 * A + B) / 4;
                        sprintf(answer, "%f", value->obdv_value.w);
                        break;
                    case 0x010D: //Vehicle speed
                        value->obdv_value.i = A;
                        sprintf(answer, "%d", value->obdv_value.i);
                        break;
                    case 0x010F: //Intake air temperature
                        value->obdv_value.i = A - 40;
                        sprintf(answer, "%d", value->obdv_value.i);
                        break;
                    case 0x0110: //MAF air flow rate
                        value->obdv_value.w = (256 * A + B) / 100;
                        sprintf(answer, "%f", value->obdv_value.w);
                        break;
                    case 0x0111: //Throttle position
                        value->obdv_value.w = 100 * A / 255;
                        sprintf(answer, "%f", value->obdv_value.w);
                        break;
                    case 0x011F: //Run time since engine start
                        value->obdv_value.i = 256 * A + B;
                        sprintf(answer, "%d", value->obdv_value.i);
                        break;
                    case 0x0123: //Fuel Rail Gauge Pressure
                        value->obdv_value.i = 10 * (256 * A + B);
                        sprintf(answer, "%d", value->obdv_value.i);
                        break;
                    case 0x0133: //Absolute Barometric Pressure
                        value->obdv_value.i = A;
                        sprintf(answer, "%d", value->obdv_value.i);
                        break;
                    case 0x0134: //Oxygen Sensor 1 Fuel–Air Equivalence Ratio
                        value->obdv_value.w = 2 * (256 * A + B) / 65536;
                        sprintf(answer, "%f", value->obdv_value.w);
                        break;
                    case 0x0145: //Relative throttle position
                        value->obdv_value.w = (100 * A) / 255;
                        sprintf(answer, "%f", value->obdv_value.w);
                        break;
                    case 0x0146: //Ambient air temperature
                        value->obdv_value.i = A - 40;
                        sprintf(answer, "%d", value->obdv_value.i);
                        break;
                    case 0x0149: //Accelerator pedal position D
                        value->obdv_value.w = (100 * A) / 255;
                        sprintf(answer, "%f", value->obdv_value.w);
                        break;
                    case 0x014A: //Accelerator pedal position E
                        value->obdv_value.i = (100 * A) / 255;
                        sprintf(answer, "%d", value->obdv_value.i);
                        break;
                    case 0x014C: //Commanded throttle actuator
                        value->obdv_value.i = (100 * A) / 255;
                        sprintf(answer, "%d", value->obdv_value.i);
                        break;
                    case -1:
                        return OBD_END;

                }
            }
        } else {
            r = read_VINmsg(fd, answer, MAX_ANSWER, 0);
            printf("Hola el answer es : %s\n", answer); //TODO eliminar es solo chequeo
            fields = separate_VINstring(answer, &commandACK, vinstring); //TODO defininar lo del 2 de fields
            puts(vinstring);
            printf("\n Command: %d", commandACK);
            if (fields < 2) {
                perror("read_parameter: unexpected error when getting answer\n");
                return OBD_ERROR;
            } else {
                if (obd_parameters[i].obdp_code != commandACK) {
                    perror("read_parameter: error matching command with answer\n");
                    return OBD_COMMAND;
                }
                value->obdv_parameter = obd_parameters[i].obdp_code;
                value->obdv_ts = time(NULL);
                strncpy(value->obdv_value.str, vinstring, OBDV_MAXSTR);
            }
        }
        return r;

    }


#ifdef __cplusplus
}
#endif
