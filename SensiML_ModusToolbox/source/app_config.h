
/******************************************************************************
* File Name:   app_config.h
*
* Description: This header file defines the app configuration for the SensiML
*              DataCapture and Knowledge Pack app
*
* Related Document: See README.md
*
*
*******************************************************************************
* Copyright 2021, Cypress Semiconductor Corporation (an Infineon company) or
* an affiliate of Cypress Semiconductor Corporation.  All rights reserved.
*
* This software, including source code, documentation and related
* materials ("Software") is owned by Cypress Semiconductor Corporation
* or one of its affiliates ("Cypress") and is protected by and subject to
* worldwide patent protection (United States and foreign),
* United States copyright laws and international treaty provisions.
* Therefore, you may use this Software only as provided in the license
* agreement accompanying the software package from which you
* obtained this Software ("EULA").
* If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
* non-transferable license to copy, modify, and compile the Software
* source code solely for use in connection with Cypress's
* integrated circuit products.  Any reproduction, modification, translation,
* compilation, or representation of this Software except as specified
* above is prohibited without the express written permission of Cypress.
*
* Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
* reserves the right to make changes to the Software without notice. Cypress
* does not assume any liability arising out of the application or use of the
* Software or any product or circuit described in the Software. Cypress does
* not authorize its products for use in any products where a malfunction or
* failure of the Cypress product may reasonably be expected to result in
* significant property damage, injury or death ("High Risk Product"). By
* including Cypress's product in a High Risk Product, the manufacturer
* of such system or application assumes all risk of such use and in doing
* so agrees to indemnify Cypress against all liability.
*******************************************************************************/

/******************************************************************************
 * Constants
 *****************************************************************************/

// Running Modes
// 1 = DATA CAPTURE => Use this mode for collecting data and use the Data Capture Lab
// 2 = RECOGNITION => Use this mode for running a Knowledge pack from the sensor
#define DATA_CAPTURE_RUNNING_MODE 1
#define RECOGNITION_RUNNING_MODE 2

// Change the below to either DATA_CAPTURE_RUNNING_MODE (or) RECOGNITION_RUNNING_MODE
#define APPLICATION_RUNNING_MODE RECOGNITION_RUNNING_MODE

// Type of Datacapture -
// 1 = SENSOR_MOTION
// 2 = SENSOR_AUDIO
#define SENSOR_MOTION    1
#define SENSOR_AUDIO    2

// Change the below to either SENSOR_MOTION (or) SENSOR_AUDIO
#define SENSOR_SELECT_MODE SENSOR_AUDIO


// Motion sensor valid sample rates
#define MOTION_SAMPLE_RATE_400HZ    400
#define MOTION_SAMPLE_RATE_200HZ    200
#define MOTION_SAMPLE_RATE_100HZ    100
#define MOTION_SAMPLE_RATE_50HZ     50
#define MOTION_SAMPLE_RATE_10HZ     10

#define SENSOR_SCAN_RATE       MOTION_SAMPLE_RATE_200HZ
