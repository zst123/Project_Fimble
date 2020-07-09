#include "app_mqtt.h"

char mqttPayload[MQTT_PAYLOAD_MAX_LENGTH];
bool mqttReadyToSend = false;

/*-----------------------------------------------------------*/

/**
 * @brief Establish a new connection to the MQTT server.
 *
 * @param[in] awsIotMqttMode Specify if this demo is running with the AWS IoT
 * MQTT server. Set this to `false` if using another MQTT server.
 * @param[in] pIdentifier NULL-terminated MQTT client identifier.
 * @param[in] pNetworkServerInfo Passed to the MQTT connect function when
 * establishing the MQTT connection.
 * @param[in] pNetworkCredentialInfo Passed to the MQTT connect function when
 * establishing the MQTT connection.
 * @param[in] pNetworkInterface The network interface to use for this demo.
 * @param[out] pMqttConnection Set to the handle to the new MQTT connection.
 *
 * @return `EXIT_SUCCESS` if the connection is successfully established; `EXIT_FAILURE`
 * otherwise.
 */
static int _establishMqttConnection( bool awsIotMqttMode,
                                     const char * pIdentifier,
                                     void * pNetworkServerInfo,
                                     void * pNetworkCredentialInfo,
                                     const IotNetworkInterface_t * pNetworkInterface,
                                     IotMqttConnection_t * pMqttConnection )
{
    int status = EXIT_SUCCESS;
    IotMqttError_t connectStatus = IOT_MQTT_STATUS_PENDING;
    IotMqttNetworkInfo_t networkInfo = IOT_MQTT_NETWORK_INFO_INITIALIZER;
    IotMqttConnectInfo_t connectInfo = IOT_MQTT_CONNECT_INFO_INITIALIZER;
    IotMqttPublishInfo_t willInfo = IOT_MQTT_PUBLISH_INFO_INITIALIZER;
    char pClientIdentifierBuffer[ CLIENT_IDENTIFIER_MAX_LENGTH ] = { 0 };

    /* Set the members of the network info not set by the initializer. This
     * struct provided information on the transport layer to the MQTT connection. */
    networkInfo.createNetworkConnection = true;
    networkInfo.u.setup.pNetworkServerInfo = pNetworkServerInfo;
    networkInfo.u.setup.pNetworkCredentialInfo = pNetworkCredentialInfo;
    networkInfo.pNetworkInterface = pNetworkInterface;

    #if ( IOT_MQTT_ENABLE_SERIALIZER_OVERRIDES == 1 ) && defined( IOT_DEMO_MQTT_SERIALIZER )
        //networkInfo.pMqttSerializer = IOT_DEMO_MQTT_SERIALIZER;
    #endif

    /* Set the members of the connection info not set by the initializer. */
    connectInfo.awsIotMqttMode = awsIotMqttMode;
    connectInfo.cleanSession = true;
    connectInfo.keepAliveSeconds = KEEP_ALIVE_SECONDS;
    connectInfo.pWillInfo = &willInfo;

    /* Set the members of the Last Will and Testament (LWT) message info. The
     * MQTT server will publish the LWT message if this client disconnects
     * unexpectedly. */
    willInfo.pTopicName = WILL_TOPIC_NAME;
    willInfo.topicNameLength = WILL_TOPIC_NAME_LENGTH;
    willInfo.pPayload = WILL_MESSAGE;
    willInfo.payloadLength = WILL_MESSAGE_LENGTH;

    /* Use the parameter client identifier if provided. Otherwise, generate a
     * unique client identifier. */
    if( ( pIdentifier != NULL ) && ( pIdentifier[ 0 ] != '\0' ) )
    {
        connectInfo.pClientIdentifier = pIdentifier;
        connectInfo.clientIdentifierLength = ( uint16_t ) strlen( pIdentifier );
    }
    else
    {
        /* Every active MQTT connection must have a unique client identifier. The demos
         * generate this unique client identifier by appending a timestamp to a common
         * prefix. */
        status = snprintf( pClientIdentifierBuffer,
                           CLIENT_IDENTIFIER_MAX_LENGTH,
                           CLIENT_IDENTIFIER_PREFIX "%lu",
                           ( long unsigned int ) IotClock_GetTimeMs() );

        /* Check for errors from snprintf. */
        if( status < 0 )
        {
            IotLogError( "Failed to generate unique client identifier for demo." );
            status = EXIT_FAILURE;
        }
        else
        {
            /* Set the client identifier buffer and length. */
            connectInfo.pClientIdentifier = pClientIdentifierBuffer;
            connectInfo.clientIdentifierLength = ( uint16_t ) status;

            status = EXIT_SUCCESS;
        }
    }

    /* Establish the MQTT connection. */
    if( status == EXIT_SUCCESS )
    {
        IotLogInfo( "MQTT demo client identifier is %.*s (length %hu).",
                    connectInfo.clientIdentifierLength,
                    connectInfo.pClientIdentifier,
                    connectInfo.clientIdentifierLength );

        connectStatus = IotMqtt_Connect( &networkInfo,
                                         &connectInfo,
                                         MQTT_TIMEOUT_MS,
                                         pMqttConnection );

        if( connectStatus != IOT_MQTT_SUCCESS )
        {
            IotLogError( "MQTT CONNECT returned error %s.",
                         IotMqtt_strerror( connectStatus ) );

            status = EXIT_FAILURE;
        }
    }

    return status;
}

/*-----------------------------------------------------------*/

/**
 * @brief Transmit all messages and wait for them to be received on topic filters.
 *
 * @param[in] mqttConnection The MQTT connection to use for publishing.
 * @param[in] pTopicNames Array of topic names for publishing. These were previously
 * subscribed to as topic filters.
 * @param[in] pPublishReceivedCounter Counts the number of messages received on
 * topic filters.
 *
 * @return `EXIT_SUCCESS` if all messages are published and received; `EXIT_FAILURE`
 * otherwise.
 */
static int _publishMessage( IotMqttConnection_t mqttConnection,
                                const char * pTopicName,
								const char * pPublishPayload)
{
    int status = EXIT_SUCCESS;
    IotMqttError_t publishStatus = IOT_MQTT_STATUS_PENDING;
    IotMqttPublishInfo_t publishInfo = IOT_MQTT_PUBLISH_INFO_INITIALIZER;
    IotMqttCallbackInfo_t publishComplete = IOT_MQTT_CALLBACK_INFO_INITIALIZER;

    /* The MQTT library should invoke this callback when a PUBLISH message
     * is successfully transmitted. */
    //publishComplete.function = _operationCompleteCallback;

    /* Set the common members of the publish info. */
    publishInfo.qos = IOT_MQTT_QOS_1;
    publishInfo.topicNameLength = TOPIC_FILTER_LENGTH;
    publishInfo.retryMs = PUBLISH_RETRY_MS;
    publishInfo.retryLimit = PUBLISH_RETRY_LIMIT;

	/* Pass the PUBLISH number to the operation complete callback. */
	//publishComplete.pCallbackContext = ( void * ) publishCount;

	/* Choose a topic name (round-robin through the array of topic names). */
	publishInfo.pTopicName = pTopicName;

	/* Generate the payload for the PUBLISH. */
    publishInfo.pPayload = pPublishPayload;
	publishInfo.payloadLength = ( size_t ) strlen( pPublishPayload );

	/* PUBLISH a message. This is an asynchronous function that notifies of
	 * completion through a callback. */
	publishStatus = IotMqtt_Publish( mqttConnection,
									 &publishInfo,
									 0,
									 &publishComplete,
									 NULL );

	if( publishStatus != IOT_MQTT_STATUS_PENDING )
	{
		IotLogError( "MQTT PUBLISH %d returned error %s.",
					 ( int ) 8,
					 IotMqtt_strerror( publishStatus ) );
		status = EXIT_FAILURE;
	}

	/* Stop publishing if there was an error. */
	if( status == EXIT_FAILURE )
	{
		return status;
	}

    return status;
}

/*-----------------------------------------------------------*/

/**
 * @brief The function that runs the MQTT demo, called by the demo runner.
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
static int _sendPayload( bool awsIotMqttMode,
                 const char * pIdentifier,
                 void * pNetworkServerInfo,
                 void * pNetworkCredentialInfo,
                 const IotNetworkInterface_t * pNetworkInterface,
				 const char * pPublishPayload )
{
    /* Return value of this function and the exit status of this program. */
    int status = EXIT_SUCCESS;

    vLoggingPrintf("Preparing to send MQTT payload \r\n");

    /* Handle of the MQTT connection used in this demo. */
    IotMqttConnection_t mqttConnection = IOT_MQTT_CONNECTION_INITIALIZER;

    /* Topics used as both topic filters and topic names in this demo. */
    const char * pTopic = IOT_DEMO_MQTT_TOPIC_PREFIX "/topic/1";

    /* Flags for tracking which cleanup functions must be called. */
    bool librariesInitialized = false;
    bool connectionEstablished = false;

    /* Initialize the libraries required for this demo. */
    IotMqttError_t mqttInitStatus = IotMqtt_Init();
	if (mqttInitStatus != IOT_MQTT_SUCCESS) {
		status = EXIT_FAILURE;
	}

	/* Establish the MQTT connection  */
    if (status == EXIT_SUCCESS) {
        vLoggingPrintf("Establishing MQTT connection \r\n");

        /* Mark the libraries as initialized. */
        librariesInitialized = true;

        /* Establish a new MQTT connection. */
        status = _establishMqttConnection( awsIotMqttMode,
                                           pIdentifier,
                                           pNetworkServerInfo,
                                           pNetworkCredentialInfo,
                                           pNetworkInterface,
                                           &mqttConnection );
    }

    /* Publish message */
    if (status == EXIT_SUCCESS) {
    	vLoggingPrintf("Publishing MQTT message \r\n");
    	connectionEstablished = true;

    	status = _publishMessage( mqttConnection, pTopic, pPublishPayload );
    }


    /* Disconnect the MQTT connection if it was established. */
    if( connectionEstablished == true ) {
        vLoggingPrintf("Disconnect MQTT connection \r\n");
        IotMqtt_Disconnect( mqttConnection, 0 );
    }


    /* Clean up libraries if they were initialized. */
    if( librariesInitialized == true ) {
        vLoggingPrintf("Clean up MQTT connection \r\n");
    	IotMqtt_Cleanup();
    }

    return status;
}


void Mqtt_Task(void *pvParameters) {
	/* Remove warning for unused parameter */
	(void)pvParameters ;

	/* Initialize buffer */
	memset(mqttPayload, 0x00, MQTT_PAYLOAD_MAX_LENGTH);
	mqttReadyToSend = false;

	/* Wait until ready to send*/
	while (1) {
		vTaskDelay(500);
		if (mqttReadyToSend) {
			mqttReadyToSend = false;
			vLoggingPrintf("Ready for Mqtt_Task %s \r\n", mqttPayload);

			/* Activity light */
			cyhal_gpio_toggle(CYBSP_USER_LED4);

			/* Retrieve network details */
		    const IotNetworkInterface_t * pNetworkInterface = NULL;
		    void * pConnectionParams = NULL, * pCredentials = NULL;
		    const uint32_t networkType = AWSIOT_NETWORK_TYPE_WIFI;
			pNetworkInterface = AwsIotNetworkManager_GetNetworkInterface( networkType );
			pConnectionParams = AwsIotNetworkManager_GetConnectionParams( networkType );
			pCredentials = AwsIotNetworkManager_GetCredentials( networkType );

	        /* Run the demo. */
			_sendPayload(
					true, /* awsIotMqttMode */
					clientcredentialIOT_THING_NAME, /* pIdentifier */
					pConnectionParams, /* pNetworkServerInfo */
					pCredentials, /* pNetworkCredentialInfo */
					pNetworkInterface, /* pNetworkInterface */
					mqttPayload /* pPublishPayload */
			);

			/* Activity light */
			cyhal_gpio_toggle(CYBSP_USER_LED4);
		}
	}
}

void Mqtt_SendPayload(const char * payload) {
	strcpy(mqttPayload, payload);
	mqttReadyToSend = true;
}

/*-----------------------------------------------------------*/
