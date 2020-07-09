#include "app.h"

/* CapSense touch */
CapSense_TouchData_t CapSense_TouchData;
uint16_t CapSense_TouchSlider;

void APP_MainLoop(void);


/**
 * @brief My application entry function, called by the demo runner.
 *
 * @param[in] awsIotMqttMode Specify if this demo is running with the AWS IoT
 * MQTT server. Set this to `false` if using another MQTT server.
 * @param[in] pIdentifier NULL-terminated MQTT client identifier.
 * @param[in] pNetworkServerInfo Passed to the MQTT connect function when
 * establishing the MQTT connection.
 * @param[in] pNetworkCredentialInfo Passed to the MQTT connect function when
 * establishing the MQTT connection.
 * @param[in] pNetworkInterface The network interface to use for this demo.
 *
 * @return `EXIT_SUCCESS` if the demo completes successfully; `EXIT_FAILURE` otherwise.
 */
int RunMyApplication( bool awsIotMqttMode,
                 const char * pIdentifier,
                 void * pNetworkServerInfo,
                 void * pNetworkCredentialInfo,
                 const IotNetworkInterface_t * pNetworkInterface )
{
	vLoggingPrintf("Started RunMyApplication();\r\n");

	/* Initialize LEDs */
	cyhal_gpio_init(CYBSP_USER_LED1, CYHAL_GPIO_DIR_OUTPUT,
			CYHAL_GPIO_DRIVE_STRONG, CYBSP_LED_STATE_ON);
	cyhal_gpio_init(CYBSP_USER_LED2, CYHAL_GPIO_DIR_OUTPUT,
			CYHAL_GPIO_DRIVE_STRONG, CYBSP_LED_STATE_ON);
	cyhal_gpio_init(CYBSP_LED_RGB_RED, CYHAL_GPIO_DIR_OUTPUT,
			CYHAL_GPIO_DRIVE_STRONG, CYBSP_LED_STATE_OFF);
	cyhal_gpio_init(CYBSP_LED_RGB_GREEN, CYHAL_GPIO_DIR_OUTPUT,
			CYHAL_GPIO_DRIVE_STRONG, CYBSP_LED_STATE_OFF);
	cyhal_gpio_init(CYBSP_LED_RGB_BLUE, CYHAL_GPIO_DIR_OUTPUT,
			CYHAL_GPIO_DRIVE_STRONG, CYBSP_LED_STATE_OFF);

	/* Initialize push buttons */
	cyhal_gpio_init(CYBSP_USER_BTN, CYHAL_GPIO_DIR_INPUT,
			CYHAL_GPIO_DRIVE_PULLUP, CYBSP_BTN_OFF);

	/* Start CapSense touch as a separate task */
    xTaskCreate(CapSense_Task, "CapSense Task",
    		CapSense_TASK_TOUCH_STACK_SIZE, NULL,
			CapSense_TASK_TOUCH_PRIORITY, NULL);

	/* Start APP_TryMqtt touch as a separate task */
	xTaskCreate(Mqtt_Task, "MQTT Task",
			MQTT_TASK_TOUCH_STACK_SIZE, NULL,
			MQTT_TASK_TOUCH_PRIORITY, NULL);

    /* Start main loop in this task */
	APP_MainLoop();

	return EXIT_SUCCESS;
}

void APP_MainLoop() {

	GUI_Init();
    GUI_SetColor(APP_COLOR_FG);
    GUI_SetBkColor(APP_COLOR_BG);
    GUI_Clear();

    GUI_DrawBitmap(&bmCypress_Logo_1_BPP_Inv, 8, 8);


    GUI_SetFont(GUI_FONT_32B_1);
    GUI_DispStringAt("Hello World", 8, 52);


    char buffer[100];
    uint8_t ucTempIp[4] = { 0 };
    WIFI_GetIP( ucTempIp );
    snprintf(buffer, 100, "IP Address: %d.%d.%d.%d",
    		ucTempIp[0], ucTempIp[1], ucTempIp[2], ucTempIp[3]);

    GUI_SetFont(GUI_FONT_20B_1);
    GUI_DispStringAt(buffer,  8, 88);

    uint8_t ucTempMac[6] = { 0 };
    WIFI_GetMAC( ucTempMac );
    char MacAddress[22];
    snprintf(MacAddress, 22, "%d:%d:%d:%d:%d:%d",
    		ucTempMac[0], ucTempMac[1], ucTempMac[2],
			ucTempMac[3], ucTempMac[4], ucTempMac[5]);

    snprintf(buffer, 100, "MAC: %s", MacAddress);

    GUI_SetFont(GUI_FONT_20B_1);
    GUI_DispStringAt(buffer,  8, 116);


    while (1) {
    	if (CapSense_TouchData == SLIDER_TOUCHED) {
    		snprintf(buffer, 100, "Touch: Slider %03d    ", CapSense_TouchSlider);
    	} else if (CapSense_TouchData == BUTTON0_TOUCHED) {
    		snprintf(buffer, 100, "Touch: Button 0       ");
    	} else if (CapSense_TouchData == BUTTON1_TOUCHED) {
    		snprintf(buffer, 100, "Touch: Button 1       ");
    	} else {
    		snprintf(buffer, 100, "Touch: None           ");
    	}
    	GUI_DispStringAt(buffer,  8, 144);

    	vTaskDelay(100);
    	cyhal_gpio_toggle(CYBSP_USER_LED5);

    	// Test MQTT using push button
    	if (cyhal_gpio_read(CYBSP_USER_BTN) == CYBSP_BTN_PRESSED) {
    		while (cyhal_gpio_read(CYBSP_USER_BTN) == CYBSP_BTN_PRESSED);

    		char * _data = MacAddress;
    		char * _device = MacAddress;
    		char * _dir = "1";
    		char _payload[MQTT_PAYLOAD_MAX_LENGTH];
    		snprintf(_payload, MQTT_PAYLOAD_MAX_LENGTH,
    				"{"
    				"\"data\":\"%s\","
    				"\"device\":\"%s\","
    				"\"direction\":\"%s\""
    				"}",
					_data, _device, _dir);

    		Mqtt_SendPayload(_payload);
    	}
    }
}

/* CapSense ISR */
static void CapSense_Interrupt(void)
{
    Cy_CapSense_InterruptHandler(CSD0, &cy_capsense_context);
}

/* CapSense Init and Loop */
void CapSense_Task(void *pvParameters)
{
	/* Remove warning for unused parameter */
	(void)pvParameters ;

	/*Initialize CapSense Data structures */
	Cy_CapSense_Init(&cy_capsense_context);

	/* CapSense ISR configuration parameters */
	const cy_stc_sysint_t CapSense_ISR_cfg = {
			.intrSrc = csd_interrupt_IRQn,
			.intrPriority = 7u,
	};
	Cy_SysInt_Init(&CapSense_ISR_cfg, &CapSense_Interrupt);
	NVIC_ClearPendingIRQ(CapSense_ISR_cfg.intrSrc);
	NVIC_EnableIRQ(CapSense_ISR_cfg.intrSrc);

	/* Start CapSense block and perform first scan to set up sensor baselines */
	Cy_CapSense_Enable(&cy_capsense_context);

	/* Repeatedly running part of the task */
	// Task that reads touch data from CapSense button and slider widgets
	for(;;)
	{
		/* Check if CapSense is busy with a previous scan */
		if(CY_CAPSENSE_NOT_BUSY == Cy_CapSense_IsBusy(&cy_capsense_context))
		{
			/* Process all widgets and read touch information */
			Cy_CapSense_ProcessAllWidgets(&cy_capsense_context);

			/* Get slider status */
			cy_stc_capsense_touch_t * slider_touch_info =
					Cy_CapSense_GetTouchInfo(CY_CAPSENSE_LINEARSLIDER0_WDGT_ID, &cy_capsense_context);
			uint8_t slider_touch_status = slider_touch_info->numPosition;
			uint16_t slider_pos = slider_touch_info->ptrPosition->x;

			/* Button0 is active */
			if(Cy_CapSense_IsWidgetActive(CY_CAPSENSE_BUTTON0_WDGT_ID, &cy_capsense_context))
			{
				CapSense_TouchData = BUTTON0_TOUCHED;
			}
			/* Button1 is active */
			else if (Cy_CapSense_IsWidgetActive (CY_CAPSENSE_BUTTON1_WDGT_ID, &cy_capsense_context))
			{
				CapSense_TouchData = BUTTON1_TOUCHED;
			}

			/* Button Slider is active */
			else if (slider_touch_status != 0 &&
					Cy_CapSense_IsWidgetActive (CY_CAPSENSE_LINEARSLIDER0_WDGT_ID, &cy_capsense_context))
			{
				CapSense_TouchData = SLIDER_TOUCHED;
				CapSense_TouchSlider = slider_pos;
			}
			else
			{
				CapSense_TouchData = NO_TOUCH;
			}

			/* Start the next CapSense scan */
			Cy_CapSense_ScanAllWidgets(&cy_capsense_context);
		}

		/* Block until next scan interval */
		vTaskDelay(CapSense_TOUCH_SCAN_INTERVAL);
	}
}

