/* 
 * File:   OBDparameters.h
 * Author: Fernando Alcala <asecking@gmail.com>
 *
 * Created on 28 de abril de 2016, 16:28
 * 
 * This file contains definitions of engine parameters which can be read
 * through OBD.
 */
#ifndef OBDPARAMETERS_H
#define	OBDPARAMETERS_H

#ifdef	__cplusplus
extern "C" {
#endif


    /* Details from http://en.wikipedia.org/wiki/OBD-II_PIDs */

#define PID_0_20            0x0100    //PID 0 - 20 supported
#define PID_0_20_DESC               "PID 0100 - 0120 Supported"
#define STATUS_DTC          0x0101    ///
#define STATUS_DTC_DESC             "Status since DTC Cleared"
#define FREEZE_DTC          0x0102    ///
#define FREEZE_DTC_DESC             "Freeze Diagnostic Trouble Code"
#define FUEL_SYS_STATUS     0x0103    ///
#define FUEL_SYS_STATUS_DESC        "Fuel System Status"
#define ENGINE_LOAD         0x0104    //
#define ENGINE_LOAD_DESC            "Calculated Engine Load"
#define ENGINE_COOLANT_TEMP 0x0105
#define ENGINE_COOLANT_TEMP_DESC    "Engine Coolant Temperature"
#define ST_FUEL_TRIM_1      0x0106    ///
#define ST_FUEL_TRIM_1_DESC         "Short Term Fuel % Trim - Bank 1"
#define LT_FUEL_TRIM_1      0x0107    ///
#define LT_FUEL_TRIM_1_DESC         "Long Term Fuel % Trim - Bank 1"
#define ST_FUEL_TRIM_2      0x0108    ///
#define ST_FUEL_TRIM_2_DESC         "Short Term Fuel % Trim - Bank 2"
#define LT_FUEL_TRIM_2      0x0109    ///
#define LT_FUEL_TRIM_2_DESC         "Long Term Fuel % Trim - Bank 2"
#define FUEL_PRESSURE       0x010A    //
#define FUEL_PRESSURE_DESC          "Fuel Pressure"
#define INTAKE_PRESSURE     0x010B    //
#define INTAKE_PRESSURE_DESC        "Intake Manifold Absolute Pressure"
#define ENGINE_RPM          0x010C
#define ENGINE_RPM_DESC             "Engine RPM"
#define VEHICLE_SPEED       0x010D
#define VEHICLE_SPEED_DESC          "Vehicle Speed"
#define TIMING_ADVANCE      0x010E    //
#define TIMING_ADVANCE_DESC         "Timing Advance"
#define INTAKE_TEMP         0x010F    //
#define INTAKE_TEMP_DESC            "Intake Air Temperature"
#define MAF_SENSOR          0x0110
#define MAF_SENSOR_DESC             "MAF Sensor Air Flow Rate"
#define THROTTLE            0x0111
#define THROTTLE_DESC               "Throttle Position"
#define COMMANDED_SEC_AIR   0x0112    ///
#define COMMANDED_SEC_AIR_DESC      "Commanded Secondary Air Status"
#define O2_SENS_PRES        0x0113    ///
#define O2_SENS_PRES_DESC           "Detected O2 Sensors"
#define O2_B1S1_VOLTAGE     0x0114    ///
#define O2_B1S1_VOLTAGE_DESC        "O2 Sensor Voltage - Bank 1 Sensor 1"
#define O2_B1S2_VOLTAGE     0x0115    ///
#define O2_B1S2_VOLTAGE_DESC        "O2 Sensor Voltage - Bank 1 Sensor 2"
#define O2_B1S3_VOLTAGE     0x0116    ///
#define O2_B1S3_VOLTAGE_DESC        "O2 Sensor Voltage - Bank 1 Sensor 3"
#define O2_B1S4_VOLTAGE     0x0117    ///
#define O2_B1S4_VOLTAGE_DESC        "O2 Sensor Voltage - Bank 1 Sensor 4"
#define O2_B2S1_VOLTAGE     0x0118    ///
#define O2_B2S1_VOLTAGE_DESC        "O2 Sensor Voltage - Bank 2 Sensor 1"
#define O2_B2S2_VOLTAGE     0x0119    ///
#define O2_B2S2_VOLTAGE_DESC        "O2 Sensor Voltage - Bank 2 Sensor 2"
#define O2_B2S3_VOLTAGE     0x011A    ///
#define O2_B2S3_VOLTAGE_DESC        "O2 Sensor Voltage - Bank 2 Sensor 3"
#define O2_B2S4_VOLTAGE     0x011B    ///
#define O2_B2S4_VOLTAGE_DESC        "O2 Sensor Voltage - Bank 2 Sensor 4"
#define OBDII_STANDARDS     0x011C    //List of OBDII Standars the car conforms to
#define OBDII_STANDARDS_DESC        "Supported OBDII Standards"
#define O2_SENS_PRES_ALT    0x011D    ///
#define O2_SENS_PRES_ALT_DESC       "Detected O2 Sensors - Alternate Grouping"
#define AUX_IN_STATUS       0x011E    ///
#define AUX_IN_STATUS_DESC          "Auxiliary Input Status"
#define ENGINE_RUNTIME      0x011F    //
#define ENGINE_RUNTIME_DESC         "Run Time Since Engine Started"
#define PID_21_40           0x0120    //PID 21-40 supported
#define PID_21_40_DESC              "PID 0121 - 0140 Supported"
#define DIST_TRAVELED_MIL   0x0121    ///
#define DIST_TRAVELED_MIL_DESC      "Distance Traveled with MIL On"
#define FUEL_RAIL_PRESSURE  0x0122    //
#define FUEL_RAIL_PRESSURE_DESC     "Fuel Rail Pressure Relative to Manifold"
#define FUEL_RAIL_PRES_ALT  0x0123    ///
#define FUEL_RAIL_PRES_ALT_DESC     "MPI/Diesel Fuel Rail Pressure"
#define O2S1_WR_LAMBDA_V    0x0124    ///
#define O2S1_WR_LAMBDA_V_DESC       "O2 Sensor 1 Equivalence Ratio Voltage"
#define O2S2_WR_LAMBDA_V    0x0125    ///
#define O2S2_WR_LAMBDA_V_DESC       "O2 Sensor 2 Equivalence Ratio Voltage"
#define O2S3_WR_LAMBDA_V    0x0126    ///
#define O2S3_WR_LAMBDA_V_DESC       "O2 Sensor 3 Equivalence Ratio Voltage"
#define O2S4_WR_LAMBDA_V    0x0127    ///
#define O2S4_WR_LAMBDA_V_DESC       "O2 Sensor 4 Equivalence Ratio Voltage"
#define O2S5_WR_LAMBDA_V    0x0128    ///
#define O2S5_WR_LAMBDA_V_DESC       "O2 Sensor 5 Equivalence Ratio Voltage"
#define O2S6_WR_LAMBDA_V    0x0129    ///
#define O2S6_WR_LAMBDA_V_DESC       "O2 Sensor 6 Equivalence Ratio Voltage"
#define O2S7_WR_LAMBDA_V    0x012A    ///
#define O2S7_WR_LAMBDA_V_DESC       "O2 Sensor 7 Equivalence Ratio Voltage"
#define O2S8_WR_LAMBDA_V    0x012B    ///
#define O2S8_WR_LAMBDA_V_DESC       "O2 Sensor 8 Equivalence Ratio Voltage"
#define COMMANDED_EGR       0x012C    //
#define COMMANDED_EGR_DESC          "Commanded EGR"
#define EGR_ERROR           0x012D    //
#define EGR_ERROR_DESC              "EGR Error"
#define COMMANDED_EVAP_P    0x012E    ///
#define COMMANDED_EVAP_P_DESC       "Commanded Evaporative Purge"
#define FUEL_LEVEL          0x012F    //
#define FUEL_LEVEL_DESC             "Fuel Level Input"
#define WARMUPS_SINCE_CLR   0x0130    ///
#define WARMUPS_SINCE_CLR_DESC      "Number of Warmups since DTC Cleared"
#define DIST_SINCE_CLR      0x0131    ///
#define DIST_SINCE_CLR_DESC         "Distance Traveled Since DTC Cleared"
#define EVAP_PRESSURE       0x0132    //
#define EVAP_PRESSURE_DESC          "Evap. System Vapor Pressure"
#define BAROMETRIC_PRESSURE 0x0133    //
#define BAROMETRIC_PRESSURE_DESC    "Barometric Pressure"
#define O2S1_WR_LAMBDA_I    0x0134    ///
#define O2S1_WR_LAMBDA_I_DESC       "O2 Sensor 1 Equivalence Ratio Current"
#define O2S2_WR_LAMBDA_I    0x0135    ///
#define O2S2_WR_LAMBDA_I_DESC       "O2 Sensor 2 Equivalence Ratio Current"
#define O2S3_WR_LAMBDA_I    0x0136    ///
#define O2S3_WR_LAMBDA_I_DESC       "O2 Sensor 3 Equivalence Ratio Current"
#define O2S4_WR_LAMBDA_I    0x0137    ///
#define O2S4_WR_LAMBDA_I_DESC       "O2 Sensor 4 Equivalence Ratio Current"
#define O2S5_WR_LAMBDA_I    0x0138    ///
#define O2S5_WR_LAMBDA_I_DESC       "O2 Sensor 5 Equivalence Ratio Current"
#define O2S6_WR_LAMBDA_I    0x0139    ///
#define O2S6_WR_LAMBDA_I_DESC       "O2 Sensor 6 Equivalence Ratio Current"
#define O2S7_WR_LAMBDA_I    0x013A    ///
#define O2S7_WR_LAMBDA_I_DESC       "O2 Sensor 7 Equivalence Ratio Current"
#define O2S8_WR_LAMBDA_I    0x013B    ///
#define O2S8_WR_LAMBDA_I_DESC       "O2 Sensor 8 Equivalence Ratio Current"
#define CAT_TEMP_B1S1       0x013C    ///
#define CAT_TEMP_B1S1_DESC          "Catalyst Temperature Bank 1 Sensor 1"
#define CAT_TEMP_B1S2       0x013E    ///
#define CAT_TEMP_B1S2_DESC          "Catalyst Temperature Bank 1 Sensor 2"
#define CAT_TEMP_B2S1       0x013D    ///
#define CAT_TEMP_B2S1_DESC          "Catalyst Temperature Bank 2 Sensor 1"
#define CAT_TEMP_B2S2       0x013F    ///
#define CAT_TEMP_B2S2_DESC          "Catalyst Temperature Bank 2 Sensor 2"
#define PID_41_60           0x0140    //PID 41-60 supported
#define PID_41_60_DESC              "PID 0141 - 0160 Supported"
#define MONITOR_STATUS      0x0141    ///
#define MONITOR_STATUS_DESC         "Monitor Status This Drive Cycle"
#define ECU_VOLTAGE         0x0142    //
#define ECU_VOLTAGE_DESC            "Control Module Voltage"
#define ABSOLUTE_LOAD       0x0143    //
#define ABSOLUTE_LOAD_DESC          "Absolute Load Value"
#define COMMANDED_EQUIV_R   0x0144    ///
#define COMMANDED_EQUIV_R_DESC      "Commanded Equivalence Ratio"
#define REL_THROTTLE_POS    0x0145    ///
#define REL_THROTTLE_POS_DESC       "Relative Throttle Position"
#define AMB_AIR_TEMP        0x0146    ///
#define AMB_AIR_TEMP_DESC           "Ambient Air Temperature"
#define ABS_THROTTLE_POS_B  0x0147    ///
#define ABS_THROTTLE_POS_B_DESC     "Absolute Throttle Position B"
#define ABS_THROTTLE_POS_C  0x0148    ///
#define ABS_THROTTLE_POS_C_DESC     "Absolute Throttle Position C"
#define ACCEL_POS_D         0x0149    ///
#define ACCEL_POS_D_DESC            "Accelerator Pedal Position D"
#define ACCEL_POS_E         0x014A    ///
#define ACCEL_POS_E_DESC            "Accelerator Pedal Position E"
#define ACCEL_POS_F         0x014B    ///
#define ACCEL_POS_F_DESC            "Accelerator Pedal Position F"
#define COMMANDED_THROTTLE  0x014C    ///
#define COMMANDED_THROTTLE_DESC     "Commanded Throttle Actuator"
#define TIME_RUN_WITH_MIL   0x014D    ///
#define TIME_RUN_WITH_MIL_DESC      "Time Run with MIL on"
#define TIME_SINCE_CLR      0x014E    ///
#define TIME_SINCE_CLR_DESC         "Time Since DTC Cleared"
#define MAX_R_O2_VI_PRES    0x014F    ///
#define MAX_R_O2_VI_PRES_DESC       "Maximum Value - Equivalence ratio, O2 Voltage, O2 Current, Intake Manifold Pressure"
#define MAX_AIRFLOW_MAF     0x0150    ///
#define MAX_AIRFLOW_MAF_DESC        "Maximum MAF Airflow Value"
#define FUEL_TYPE           0x0151    //
#define FUEL_TYPE_DESC              "Fuel Type"
#define ETHANOL_PERCENT     0x0152    //
#define ETHANOL_PERCENT_DESC        "Ethanol fuel %"
#define ABS_EVAP_SYS_PRES   0x0153    ///
#define ABS_EVAP_SYS_PRES_DESC      "absolute Evap. System Vapor Pressure"
#define EVAP_SYS_PRES       0x0154    ///
#define EVAP_SYS_PRES_DESC          "Evap. System Vapor Pressure"
#define ST_O2_TRIM_B1B3     0x0155    ///
#define ST_O2_TRIM_B1B3_DESC        "Short Term Secondary O2 Sensor Trim - Bank 1 and 3"
#define LT_O2_TRIM_B1B3     0x0156    ///
#define LT_O2_TRIM_B1B3_DESC        "Long Term Secondary O2 Sensor Trim - Bank 1 and 3"
#define ST_02_TRIM_B2B4     0x0157    ///
#define ST_O2_TRIM_B2B4_DESC        "Short Term Secondary O2 Sensor Trim - Bank 2 and 4"
#define LT_O2_TRIM_B2B4     0x0158    ///
#define LT_O2_TRIM_B2B4_DESC        "Long Term Secondary O2 Sensor Trim - Bank 2 and 4"
#define ABS_FUEL_RAIL_PRES  0x0159    ///
#define ABS_FUEL_RAIL_PRES_DESC     "Absolute Fuel Rail Pressure"
#define REL_ACCEL_POS       0x015A    ///
#define REL_ACCEL_POS_DESC          "Relative Accelerator Pedal Position"
#define HYBRID_BATT_PCT     0x015B    ///
#define HYBRID_BATT_PCT_DESC        "Hybrid Battery Pack Charge Percent"
#define ENGINE_OIL_TEMP     0x015C    ///
#define ENGINE_OIL_TEMP_DESC        "Engine Oil Temperature"
#define FUEL_TIMING         0x015D    //
#define FUEL_TIMING_DESC            "Fuel Injection Timing"
#define FUEL_RATE           0x015E    //
#define FUEL_RATE_DESC              "Engine Fuel Rate"
#define EMISSIONS_STANDARD  0x015F    ///
#define EMISSIONS_STANDARD_DESC     "Emmissions Requirements"
#define DEMANDED_TORQUE     0x0161    ///
#define DEMANDED_TORQUE_DESC        "Driver's Demanded Torque - Percent"
#define ACTUAL_TORQUE       0x0162    ///
#define ACTUAL_TORQUE_DESC          "Actual Engine Torque - Percent"
#define REFERENCE_TORQUE    0x0163    //
#define REFERENCE_TORQUE_DESC       "Engine Reference Torque"
#define ENGINE_PCT_TORQUE   0x0164    ///
#define ENGINE_PCT_TORQUE_DESC      "Engine Percent Torque"
#define AUX_IO_SUPPORTED    0x0165    ///
#define AUX_IO_SUPPORTED_DESC       "Auxiliary Input/Output Supported"
#define P_MAF_SENSOR        0x0166    ///
#define P_ENGINE_COOLANT_T  0x0167    ///
#define P_INTAKE_TEMP       0x0168    ///
#define P_COMMANDED_EGR     0x0169    ///
#define P_COMMANDED_INTAKE  0x016A    ///
#define P_EGR_TEMP          0x016B    ///
#define P_COMMANDED_THROT   0x016C    ///
#define P_FUEL_PRESSURE     0x016D    ///
#define P_FUEL_INJ_PRES     0x016E    ///
#define P_TURBO_PRESSURE    0x016F    ///
#define P_BOOST_PRES_CONT   0x0170    ///
#define P_VGT_CONTROL       0x0171    ///
#define P_WASTEGATE_CONT    0x0172    ///
#define P_EXHAUST_PRESSURE  0x0173    ///
#define P_TURBO_RPM         0x0174    ///
#define P_TURBO_TEMP1       0x0175    ///
#define P_TURBO_TEMP2       0x0176    ///
#define P_CACT              0x0177    ///
#define P_EGT_B1            0x0178    ///
#define P_EGT_B2            0x0179    ///
#define P_DPF1              0x017A    ///
#define P_DPF2              0x017B    ///
#define P_DPF_TEMP          0x017C    ///
#define P_NOX_NTE_STATUS    0x017D    ///
#define P_PM_NTE_STATUS     0x017E    ///
#define P_ENGINE_RUNTUME    0x017F    ///
#define P_ENGINE_AECD_1     0x0181    ///
#define P_ENGINE_AECD_2     0x0182    ///
#define P_NOX_SENSOR        0x0183    ///
#define P_MANIFOLD_TEMP     0x0184    ///
#define P_NOX_SYSTEM        0x0185    ///
#define P_PM_SENSOR         0x0186    ///
#define P_IN_MANIF_TEMP     0x0187    ///


#ifdef	__cplusplus
}
#endif

#endif	/* OBDPARAMETERS_H */
