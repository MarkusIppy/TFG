#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "libLM327com.h"
#include "OBDparameters.h"

#define MAX_ANSWER 500

int main() {
    int fd, i, n, status;
    char answer[MAX_ANSWER];
    FILE * fp;
    fp = fopen("file.txt", "w+");
    OBD_value values[10000];
    OBD_vallist lista_val;
    OBD_value *value;

    printf("Vamos a proceder a abrir el puerto..\n");
    fd = openOBD_port();
    if (fd < 0) {
        fprintf(stderr, "Error opening port\n");
        exit(1);
    }

    printf("Vamos a proceder a escribir en el puerto..\n");
    n = sync_protocol(fd);
    //    while ((status = read_msg(fd, answer, MAX_ANSWER, -1)) > 0);
    //    if (status != OBD_EMPTY)
    //    {
    //        perror("Error emptying buffer");
    //    }
    if (n == OBD_OK) {
        for (i = 0; i <= 4; i++) {
            //            value = obd_newvalue();

            //            if (value == NULL) {
            n = read_parameter(fd, ENGINE_RPM, answer);
            //                obd_appendvalue(&lista_val, value);
            timestamp(answer, fp);
            sleep(1);
        }
    } else {
        fprintf(stderr, "Error syncing protocol\n");
    }
    close(fd);
    return 0;
}
