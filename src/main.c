#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "libLM327com.h"
#include "OBDparameters.h"

#define MAX_ANSWER 500
#define OBDV_MAXSTR 40

int main() {
    int fd, i = 1, j = 0, n, status, parameters[6] = {3, 4, 5, 6, 7, 8};
    char answer[MAX_ANSWER];
    char name[MAX_ANSWER];
    char MVIN[MAX_ANSWER];
    int N, ID;
    sprintf(name, "%ld.txt", time(NULL));
    FILE * fp;
    fp = fopen(name, "w+");
    OBD_value * values[10000]; //TODO change constant

    //Opening OBD port and checking if an error ocurred
    fd = openOBD_port();
    if (fd == OBD_ERRPORT) {
        fprintf(stderr, "Error while opening port\n");
        exit(1);
    }

    //Syncing protocol
    printf("Syncing port..\n");
    n = sync_protocol(fd);

    while ((status = read_msg(fd, answer, MAX_ANSWER, -1)) > 0); //TODO no se la validez de esto
    //    if (status != OBD_EMPTY)
    //    {
    //        perror("Error emptying buffer");
    //    }
    //    if (n == OBD_OK) {
    //        for (i = 0; i <= 4; i++) {
    //            value = obd_newvalue();
    //            if (value->next == NULL) {
    //                n = read_parameter(fd, ENGINE_RPM, answer);
    //                strncpy(value->obdv_value.str, answer, OBDV_MAXSTR);
    //                //              value->obdv_value.str = answer;
    //                obd_appendvalue(&lista_val, value);
    //                values[i].obdv_ts = time(NULL);
    //                value->obdv_ts = time(NULL);
    //                timestamp(answer, fp);
    //                sleep(1);
    //            }
    //            values[i] = *value;
    //        }

    if (n == OBD_OK) { //Syncing protocol succeeded
        //TXT DATA
        sleep(2);
        obdparameter(fp);
        ID = person(fp);
        //N = model(fp, fd, MVIN);
        //vehicle(fp, MVIN);
        //drives(fp, MVIN, ID);
        //track(fp, MVIN, ID);
        while (n >= 0) {
            //for (i = 1; i<=10; i++) {
            /*values[i] = obd_newvalue();
            if (values[i]->next == NULL) {
                n = read_parameter(fd, parameters, answer, values[i]);*/
            n = read_MULTparameter(fd, parameters, answer, values, i);
            //                strncpy(values[i]->obdv_value.str, answer, OBDV_MAXSTR);
            //                obd_appendvalue(&lista_val, values[i]);
            //                values[i]->obdv_ts = time(NULL);
            //                values[i]->obdv_parameter = ENGINE_RPM;
            //sleep(10); //TODO ver tiempo
            if (n < 0) { //If something went wrong or finished reading, reset OBD and close the file
                write_atmsg(fd, "Z");
                sleep(1);
                close(fd);
                return n;
            } else {
                for (j = 0; j <= 5; j++) {
                    sample(answer, fp, parameters[j], values[i]);
                    i++;
                }
            }
            sleep(5); //TODO ver tiempo
            while ((status = read_msg(fd, answer, MAX_ANSWER, -1)) > 0);
            sleep(5);
        }
    } else {
        fprintf(stderr, "Error syncing protocol\n"); //Syncing protocol did not succeed
        return OBD_SYNC;
    }

    //Reset OBD and close the file
    write_atmsg(fd, "Z");
    sleep(1);
    close(fd);
    return 0;
}