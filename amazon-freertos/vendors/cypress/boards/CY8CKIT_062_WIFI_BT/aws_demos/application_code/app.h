#pragma once

/* The config header is always included first. */
#include "iot_config.h"

/* Standard includes. */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Set up logging for this demo. */
#include "iot_demo_logging.h"

/* Platform layer includes. */
#include "platform/iot_clock.h"
#include "platform/iot_threads.h"

/* MQTT include. */
#include "iot_mqtt.h"

/* AWS library includes. */
#include "iot_system_init.h"
#include "iot_logging_task.h"
#include "iot_wifi.h"
#include "aws_clientcredential.h"
#include "aws_application_version.h"
#include "aws_dev_mode_key_provisioning.h"

/* Cypress Board Support Package */
#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"
#include "cycfg.h"
#include "task.h"

/* CY8CKIT-028-TFT Library */
#include "GUI.h"

/* CapSense Library */
#include "cy_capsense.h"
#include "cycfg_capsense.h"

/********************* MyApplication ******************************/

/* GUI colors and logos */
#define APP_HEX_BG      (0x00cccccc)
#define APP_HEX_FG      (GUI_BLACK)
#define APP_HEX_LOGO    (0x00141A46)

#define APP_COLOR_BG    GUI_MAKE_COLOR(APP_HEX_BG)
#define APP_COLOR_FG    GUI_MAKE_COLOR(APP_HEX_FG)
#include "cypress_logo.h"

/* Declaration of demo function. */
int RunMyApplication( bool awsIotMqttMode,
                 const char * pIdentifier,
                 void * pNetworkServerInfo,
                 void * pNetworkCredentialInfo,
                 const IotNetworkInterface_t * pNetworkInterface );


/********************* CapSense ******************************/

/* Scanning interval of 10ms is used to get 100 scans per second */
#define CapSense_TOUCH_SCAN_INTERVAL  (pdMS_TO_TICKS(10u))
/* Touch slider maximum value is 300 */
#define CapSense_TOUCH_SLIDER_MAX (300)
/* Touch Task settings */
#define CapSense_TASK_TOUCH_STACK_SIZE       (configMINIMAL_STACK_SIZE)
#define CapSense_TASK_TOUCH_PRIORITY         (10u)

/* Data-type for touch data */
typedef enum
{
    BUTTON0_TOUCHED     = 0x00u,
    BUTTON1_TOUCHED     = 0x01u,
    SLIDER_TOUCHED      = 0x02u,
    NO_TOUCH            = 0x03u
} CapSense_TouchData_t;

extern CapSense_TouchData_t CapSense_TouchData;
extern uint16_t CapSense_TouchSlider;

/* CapSense functions */
static void CapSense_Interrupt(void);
void CapSense_Task(void *pvParameters);
