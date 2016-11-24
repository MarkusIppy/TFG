/* 
 * File:   OBDparameters.h
 * Author: Fernando Alcala <asecking@gmail.com>
 *
 * Created on 28 de abril de 2016, 16:28
 * 
 * This file contains PIDs descriptions and codes which can be read through OBD.
 * It also contains t_obdparameter structure.
 */
#ifndef OBDPARAMETERS_H
#define OBDPARAMETERS_H

#ifdef __cplusplus
extern "C" {
#endif

    /* Details from http://en.wikipedia.org/wiki/OBD-II_PIDs */

    typedef enum {
        OBDCMDCODE_ENDOFLIST = -1,
        ENGINE_LOAD = 0x04,
        ENGINE_COOLANT_TEMP = 0x05,
        INTAKE_PRESSURE = 0x0B,
        ENGINE_RPM = 0x0C,
        VEHICLE_SPEED = 0x0D,
        INTAKE_TEMP = 0x0F,
        MAF_SENSOR = 0x10,
        THROTTLE = 0x11,
        ENGINE_RUNTIME = 0x1F,
        FUEL_RAIL_PRES_ALT = 0x23,
        BAROMETRIC_PRESSURE = 0x33,
        O2S1_WR_LAMBDA_I = 0x34,
        REL_THROTTLE_POS = 0x45,
        AMB_AIR_TEMP = 0x46,
        ACCEL_POS_D = 0x49,
        ACCEL_POS_E = 0x4A,
        COMMANDED_THROTTLE = 0x4C,
        VIN = 0x0902
    } t_obdcmdcode;

    typedef struct {
        /* Hex code in OBD protocol */
        t_obdcmdcode obdp_code;
        /* Number of bytes in answer string including command ACK */
        int obdp_fields;
        /* Name for parameter */
        char *obdp_parname;
        /* String describing parameter */
        char *obdp_desc;
        /* Data type of answer */
        int obdp_data;
    } t_obdparameter;

    /* Supported PIDs descripitions
     */
#define PID_0_20_DESC               "PID 0100 - 0120 Supported"
#define ENGINE_LOAD_DESC            "Calculated Engine Load"
#define ENGINE_COOLANT_TEMP_DESC    "Engine Coolant Temperature"
#define INTAKE_PRESSURE_DESC        "Intake Manifold Absolute Pressure"
#define ENGINE_RPM_DESC             "Engine RPM"
#define VEHICLE_SPEED_DESC          "Vehicle Speed"
#define INTAKE_TEMP_DESC            "Intake Air Temperature"
#define MAF_SENSOR_DESC             "MAF Sensor Air Flow Rate"
#define THROTTLE_DESC               "Throttle Position"
#define ENGINE_RUNTIME_DESC         "Run Time Since Engine Started"
#define FUEL_RAIL_PRES_ALT_DESC     "MPI/Diesel Fuel Rail Pressure"
#define BAROMETRIC_PRESSURE_DESC    "Barometric Pressure"
#define O2S1_WR_LAMBDA_I_DESC       "O2 Sensor 1 Equivalence Ratio Current"
#define REL_THROTTLE_POS_DESC       "Relative Throttle Position"
#define AMB_AIR_TEMP_DESC           "Ambient Air Temperature"
#define ACCEL_POS_D_DESC            "Accelerator Pedal Position D"
#define ACCEL_POS_E_DESC            "Accelerator Pedal Position E"
#define COMMANDED_THROTTLE_DESC     "Commanded Throttle Actuator"

#ifdef __cplusplus
}
#endif

#endif /* OBDPARAMETERS_H */
