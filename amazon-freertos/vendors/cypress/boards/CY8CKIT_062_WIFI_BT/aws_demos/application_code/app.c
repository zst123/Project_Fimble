#include "app.h"

/* Rfid Application */
int Rfid_direction = 0;
bool Rfid_alreadySent = false;

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

	/* Start RFID reader as a separate task */
	xTaskCreate(Rfid_Task, "RFID Task",
			Rfid_TASK_TOUCH_STACK_SIZE, NULL,
			Rfid_TASK_TOUCH_PRIORITY, NULL);

    /* Start main loop in this task */
	APP_MainLoop();

	return EXIT_SUCCESS;
}

void APP_MainLoop() {
    // Initialise TFT display
    GUI_Init();
    GUI_SetColor(APP_COLOR_FG);
    GUI_SetBkColor(APP_COLOR_BG);
    GUI_Clear();

    // Retrieve IP address
    uint8_t ucTempIp[4] = { 0 };
    WIFI_GetIP( ucTempIp );

    // Retrieve MAC address
    uint8_t ucTempMac[6] = { 0 };
    WIFI_GetMAC( ucTempMac );
    char MacAddress[22];
    snprintf(MacAddress, 22, "%d:%d:%d:%d:%d:%d",
             ucTempMac[0], ucTempMac[1], ucTempMac[2],
             ucTempMac[3], ucTempMac[4], ucTempMac[5]);

    // Screen switcher values
    const uint8_t noOfScreens = 3;
    uint8_t screenIndex = 0;
    bool screenChanged = true;

    while (1) {
        // Use slider to change screens
        if (CapSense_TouchData == SLIDER_TOUCHED) {
            // Calculate screen number from touch slider position
            uint8_t newScreenIndex = noOfScreens * CapSense_TouchSlider / CapSense_TOUCH_SLIDER_MAX;

            // Correct the index if out of bounds
            if (newScreenIndex < 0) {
                newScreenIndex = 0;
            } else if (newScreenIndex >= noOfScreens) {
                newScreenIndex = noOfScreens - 1;
            }

            // Indicate we need to change screens
            if (newScreenIndex != screenIndex) {
                screenIndex = newScreenIndex;
                screenChanged = true;
            }
        }

        // Change screen if necessary (static screens)
        if (screenChanged) {
            screenChanged = false;

            if (screenIndex == 0) {
                // Splash Screen
                GUI_Clear();
                GUI_DrawBitmap(&PSoC_Splash_Screen, 0, 0);
            } else if (screenIndex == 1) {
                // Rfid Screen

                // Make the state of the RFID empty when first entering the screen
                Rfid_direction = 0;
                Rfid_alreadySent = false;
                strcpy(Rfid_CardUid, "");

				// Draw Idle Logo
				GUI_Clear();
				GUI_SetFont(GUI_FONT_32B_1);
				GUI_DispStringAt("Inventory Scan", 8, 8);
				GUI_DrawBitmap(&RfidIdle, 0, 240/2 - 88/2);

            } else if (screenIndex == 2) {
                // Debug Screen
                GUI_Clear();

                GUI_SetFont(GUI_FONT_32B_1);
                GUI_DispStringAt("Hello World", 8, 8);

                // Local IP Address
                char buffer[50];
                snprintf(buffer, 50, "IP Address: %d.%d.%d.%d",
                         ucTempIp[0], ucTempIp[1], ucTempIp[2], ucTempIp[3]);
                GUI_SetFont(GUI_FONT_20B_1);
                GUI_DispStringAt(buffer,  8, 44);

                // Device MAC address
                snprintf(buffer, 50, "MAC: %s", MacAddress);
                GUI_SetFont(GUI_FONT_20B_1);
                GUI_DispStringAt(buffer,  8, 72);
            }

        }

        // Update screen if necessary (continuously updating screens)

        if (screenIndex == 0) {
            // Splash Screen
        } else if (screenIndex == 1) {
            // Rfid Screen

            // Rfid Debug
            char buffer[50];
			snprintf(buffer, 50, "(%c) %s [%c]                                    ",
					Rfid_direction ? 'I' : 'O',
					Rfid_CardUid,
					Rfid_CardPresent ? 'Y' : 'N'
			);
			GUI_SetFont(GUI_FONT_13B_1);
			GUI_DispStringAt(buffer,  8, 240 - 20);

            // When touched button, empty the UID
            if (CapSense_TouchData == BUTTON0_TOUCHED) {
                Rfid_direction = 0;
                Rfid_alreadySent = false;
                strcpy(Rfid_CardUid, "");
            } else if (CapSense_TouchData == BUTTON1_TOUCHED) {
                Rfid_direction = 1;
                Rfid_alreadySent = false;
                strcpy(Rfid_CardUid, "");
            }

        	// Wait until a card is detected, and the Uid is valid,
        	// Then send a MQTT request
        	if (strlen(Rfid_CardUid) > 1 && !Rfid_alreadySent) {
        		Rfid_alreadySent = true;

        		char * _data = Rfid_CardUid;
				char * _device = MacAddress;
				int _dir = Rfid_direction;

				char _payload[MQTT_PAYLOAD_MAX_LENGTH];
				snprintf(_payload, MQTT_PAYLOAD_MAX_LENGTH,
						"{"
						"\"data\":\"%s\","
						"\"device\":\"%s\","
						"\"direction\":\"%d\""
						"}",
						_data, _device, _dir);


				GUI_SetFont(GUI_FONT_20B_1);
				GUI_DispStringAt("Uploading ",  8, 200);

				// Show uploading logo
				if (Rfid_direction == true) {
					GUI_DrawBitmap(&RfidUpdateIn, 0, 240/2 - 88/2); // IN
				} else {
					GUI_DrawBitmap(&RfidUpdateOut, 0, 240/2 - 88/2); // OUT
				}

				// Send payload and wait until it is done by monitoring the activity lights
				Mqtt_SendPayload(_payload);
				while (0 != Cy_GPIO_ReadOut(CYHAL_GET_PORTADDR(CYBSP_USER_LED4), CYHAL_GET_PIN(CYBSP_USER_LED4))); // Wait until it lights up
				while (0 == Cy_GPIO_ReadOut(CYHAL_GET_PORTADDR(CYBSP_USER_LED4), CYHAL_GET_PIN(CYBSP_USER_LED4))); // Wait until it turns off

				GUI_SetFont(GUI_FONT_20B_1);
				GUI_DispStringAt("Idle              ",  8, 200);

				// Show success logo for 3 sec
				GUI_DrawBitmap(&RfidSuccess, 0, 240/2 - 88/2);
				vTaskDelay(3000);
				GUI_DrawBitmap(&RfidIdle, 0, 240/2 - 88/2);
        	}

        } else if (screenIndex == 2) {
            // Debug Screen

            // CapSense debugging values
            char buffer[50];
            if (CapSense_TouchData == SLIDER_TOUCHED) {
                snprintf(buffer, 50, "Touch: Slider %03d    ", CapSense_TouchSlider);
            } else if (CapSense_TouchData == BUTTON0_TOUCHED) {
                snprintf(buffer, 50, "Touch: Button 0       ");
            } else if (CapSense_TouchData == BUTTON1_TOUCHED) {
                snprintf(buffer, 50, "Touch: Button 1       ");
            } else {
                snprintf(buffer, 50, "Touch: None           ");
            }
            GUI_DispStringAt(buffer,  8, 100);

            // Rfid Debug
            snprintf(buffer, 50, "RFID Version: 0x%02X", Rfid_VersionReg);
            GUI_SetFont(GUI_FONT_20B_1);
            GUI_DispStringAt(buffer,  8, 128);

            snprintf(buffer, 50, "RFID Card: %s [%s]", Rfid_CardUid, Rfid_CardPresent ? "Y" : "N");
            GUI_SetFont(GUI_FONT_20B_1);
            GUI_DispStringAt(buffer,  8, 156);
        }

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

/* RFID Reader */
uint8_t Rfid_VersionReg;
char Rfid_CardUid[20];
bool Rfid_CardPresent;

void Rfid_Task(void *pvParameters) {
	/* Remove warning for unused parameter */
	(void)pvParameters ;

	vLoggingPrintf("Rfid_Task MFRC522_Init \r\n");
	MFRC522_Init();

	PCD_DumpVersionToSerial();
	Rfid_VersionReg = PCD_ReadRegister(VersionReg);

	while (1) {
		vTaskDelay(500);

		// Update card info
		if (PICC_IsNewCardPresent() && PICC_ReadCardSerial()) {
			//if PICC_ReadCardSerial returns 1, the "uid" struct (see h lines 238-45)) contains the ID of the read card.
			// Among other things, the PICC_ReadCardSerial() method reads the UID and the SAK (Select acknowledge) into the uid struct, which is also instantiated
			// during this process.
			// The UID is needed during the authentication process
			//The Uid struct:
			//typedef struct {
			//byte		size;			// Number of bytes in the UID. 4, 7 or 10.
			//byte		uidByte[10];            //the user ID in 10 bytes.
			//byte		sak;			// The SAK (Select acknowledge) byte returned from the PICC after successful selection.
			//} Uid;

			// Parse UID into string
			snprintf(Rfid_CardUid, 20, "%02X-%02X-%02X-%02X",
					uid.uidByte[0], uid.uidByte[1], uid.uidByte[2], uid.uidByte[3]);

			// Debug information of tag
			if (false) {
				char buffer[40];
				snprintf(buffer, 40, "Found tag: %s \r\n", Rfid_CardUid);
				vLoggingPrintf(buffer);
			}
			Rfid_CardPresent = true;
		} else {
			Rfid_CardPresent = false;
		}

	}
}
