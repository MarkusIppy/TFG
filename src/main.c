#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include "libLM327com.h"
#include "OBDparameters.h"
#include "gps.h"

#define MAX_ANSWER 500
#define OBDV_MAXSTR 40

int main(int argc, char** argv) {
    int fd, i = 1, j = 0, n = 0, status, parameters[6] = {3, 4, 5, 6, 7, 8};
    char answer[MAX_ANSWER];
    char name[MAX_ANSWER];
    char MVIN[MAX_ANSWER];
    int N, ID;
    sprintf(name, "PEPE_%ld.txt", time(NULL));
    FILE * fp;
    fp = fopen(name, "w+");
    OBD_value * obdvalues[10000]; //TODO change constant
    GPS_value * gpsvalues[10000];
    struct gps_data_t gpsdata;
    //Opening OBD port and checking if an error ocurred
    char *portname = NULL;
    /*
    if (argc > 1) portname = argv[1];
    
    printf("Portname = %s\n", portname);
    fd = openOBD_port(portname);
    if (fd == OBD_ERRPORT) {
        fprintf(stderr, "Error while opening port\n");
        exit(1);
    }
    */
    //Opening GPS and checking if an error ocurred
    if (gps_open(GPSD_SHARED_MEMORY, "", &gpsdata) != 0) {
        printf("Syncing port..\n");
        (void) fprintf(stderr,
                "%s: no gpsd running or network error: %d, %s\n",
                argv[0], errno, gps_errstr(errno));
        exit(1);
    }
    
    //Syncing protocol
    printf("Syncing port..\n");
    //n = sync_protocol(fd);

    //while ((status = read_msg(fd, answer, MAX_ANSWER, -1)) > 0);
    
    if (n == OBD_OK) { //Syncing protocol succeeded
        //TXT DATA
        sleep(2);
        obdparameter(fp);
        ID = person(fp);
        //N = model(fp, fd, MVIN);
        //vehicle(fp, MVIN);
        //drives(fp, MVIN, ID);
        //track(fp, MVIN, ID);
        n = 10;
        while (n >= 0) {
            //for (i = 1; i<=10; i++) {
            /*values[i] = obd_newvalue();
            if (values[i]->next == NULL) {
                n = read_parameter(fd, parameters, answer, values[i]);
                strncpy(values[i]->obdv_value.str, answer, OBDV_MAXSTR);
                obd_appendvalue(&lista_val, values[i]);
                values[i]->obdv_ts = time(NULL);
                values[i]->obdv_parameter = ENGINE_RPM;
                sleep(10); //TODO ver tiempo*/
            //n = read_MULTparameter(fd, parameters, answer, obdvalues, i);
            if (n < 0) { //If something went wrong or finished reading, reset OBD and close the file
                (void) gps_close(&gpsdata);
                //write_atmsg(fd, "Z");
                sleep(1);
                //close(fd);
                return n;
            } else {
                for (j = 0; j <= 5; j++) {
                    gps_collect(gpsdata, gpsvalues, i);
                    gps_sample(fp, gpsvalues[i]);
                    //sample(answer, fp, parameters[j], obdvalues[i]);
                    i++;
                }
            }
            //sleep(5); //TODO ver tiempo
            //while ((status = read_msg(fd, answer, MAX_ANSWER, -1)) > 0);
            n--;
            //sleep(5);
        }
    } else {
        fprintf(stderr, "Error syncing protocol\n"); //Syncing protocol did not succeed
        (void) gps_close(&gpsdata);
        //write_atmsg(fd, "Z");
        sleep(1);
        //close(fd);
        return OBD_SYNC;
    }

    //Reset OBD and close the file
    (void) gps_close(&gpsdata);
    //write_atmsg(fd, "Z");
    sleep(1);
    //close(fd);
    return 0;
}