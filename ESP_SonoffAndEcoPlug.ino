
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!PICK ONE TO CHOOSE THE NODE !!!!!!!!!!!!!!!!!!!!!!!!
//*Sonoff Singles
//#include "ESP_SonoffSingleNodes_Node_25-wemostest.h"                //OTA deployed date:
#include "ESP_SonoffSingleNodes_Node_46-FrontCornerMotionLights.h"  //OTA deployed date:
//#include "ESP_SonoffSingleNodes_Node_51-GarageLightsLF.h"           //OTA deployed date:  2017-07-25
//#include "ESP_SonoffSingleNodes_Node_52-GarageLightsRR.h"           //OTA deployed date:  2017-07-25
//#include "ESP_SonoffSingleNodes_Node_47-RearDeckFloodLights.h"      //OTA deployed date:
//#include "ESP_SonoffSingleNodes_Node_90-LRChristmastree.h"          //OTA deployed date:  2017-07-25

//**EcoPlugs
//#include "Node_60-ESP_ECOPlug1.h"      //OTA deployed date:
//#include "Node_61-ESP_ECOPlug2.h"     //OTA deployed date:
//#include "Node_62-ESP_ECOPlug3PM.h"   //OTA deployed date:
//#include "Node_63-ESP_ECOPlug4PM.h"   //OTA deployed date:
//#include "Node_64-ESP_ECOPlug5PM.h"   //OTA deployed date:
//#include "Node_65-ESP_ECOPlugOD1.h"   //OTA deployed date:
//#include "Node_66-ESP_ECOPlugOD2.h"   //OTA deployed date:
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


//////////////Jim's Customizations Comments/////////////////////////////////////////////////////////////////////////////
//2017/04   jim integrated enableable/disableable OTA system
//****ALERT***  KEEP SPIFFS AT 128, NO NEED FOR HIGHER AND IT MUST HAVE ENOUGH ROOM IN THE 
//              **PROGRAM AREA** NOT SPIFFS AREA FOR THE NEW FILE.
//              **For Sonoff and ECOPLUGS, the proper setting for Flash Size is 1M (128K SPIFFS) 
//seperated the header files from the code
//added on at startup option
//added send version at start
//added a toggle for motion floodlights...see options in header file and see dev 41 and setswitchlock function
//customizable OTA name
//made the option for persistant messages or not.
//2017/07  Merged the ECOPlugs(Walmart) into this

//Todo:  Sonoff Dual,4ch,POW
//        add the JSON messaging system like the WindowShade node on device 72 so I dont have to add IF blocks for specialty functions 
//        power monitoring circuitry reporting for ecoplug node 62-64
//////////////End Jim's Customizations Comments/////////////////////////////////////////////////////////////////////////


//////////////Begin Computourist Comments///////////////////////////////////////////////////////////////////////////////

//	ESP_SONOFF_V1.2
//
//	This MQTT client will is designed to run on a SONOFF device.
//	https://www.itead.cc/smart-home/sonoff-wifi-wireless-switch.html
//
//	It will connect over Wifi to the MQTT broker and controls a digital output (LED, relay):
//	- toggle output and send status message on local button press
//	- receive messages from the MQTT broker to control output, change settings and query state
//	- periodically send status messages
//
//	Several nodes can operate within the same network; each node has a unique node ID.
//	On startup the node operates with default values, as set during compilation.
//	Hardware used is a ESP8266 WiFi module that connects directly to the MQTT broker.
//
//	Message structure is equal to the RFM69-based gateway/node sytem by the same author.
//	This means both type of gateway/nodes can be used in a single Openhab system.
//
//	The MQTT topic is /home/esp_gw/direction/nodeid/devid
//	where direction is "sb" towards the node and "nb" towards the MQTT broker.
//
//	Defined devices are:
//	0	uptime:		read uptime in minutes
//	1	interval:	read/set transmission interval for push messages
//	3	version:	read software version
//	2	RSSI:		read radio signal strength
//	5	ACK:		read/set acknowledge message after a 'set' request
//	6	toggle:		read/set select toggle / timer function
//	7	timer:		read/set timer interval in seconds
//	10	IP:			Read IP address
//	16	actuator:	read/set relay output
//	40	button		tx only: button pressed
//  41  Flash Switch: RX Only -  This does an ON - wait - OFF/ON to keep motion type floodlights on
//	92	error:		tx only: device not supported
//	91	error:		tx only: syntax error
//	99	wakeup:		tx only: first message sent on node startup
//
//	Hardware connections as in Sonoff device:
//	-	pin 0 is connected to a button that switches to GND, pullup to VCC, also used to enter memory flash mode.
//	-	pin 13 is connected to LED and current limiting resistor to VCC, used to indicate MQTT link status
//	-	pin 12 is connected to the relais output
//
//	version 1.0 by computourist@gmail.com June 2016
//	version 1.1 by computourist@gmail.com July 2016
//		specific client name to avoid conflicts when using multiple devices.
//		added mqtt will to indicate disconnection of link
//	version 1.2 by computourist@gmail.com July 2016
//		fallback SSI and autorecovery for wifi connection. 
//		a connection attempt to each SSI is made during 20 seconds .
//

#include <ESP8266WiFi.h>
#include <PubSubClient.h>


#ifdef OTAenable
  #include <ESP8266mDNS.h>
  #include <WiFiUdp.h>
  #include <ArduinoOTA.h>
#endif


	//	sensor setting

#define SERIAL_BAUD 115200
#define HOLDOFF 1000							// blocking period between button messages

	//	STARTUP DEFAULTS

 
	long 	TXinterval = 120;						// periodic transmission interval in seconds
	long	TIMinterval = 20;						// timer interval in seconds
	bool	ackButton = false;						// flag for message on button press
	bool	toggleOnButton = true;					// toggle output on button press
	bool	fallBackSsi = false;					// toggle access point

	//	VARIABLES

	int		DID;									// Device ID
	int		error;									// Syntax error code
	long	lastPeriod = -1;						// timestamp last transmission
	long 	lastBtnPress = -1;						// timestamp last buttonpress
	long	lastMinute = -1;						// timestamp last minute
	long	upTime = 0;								// uptime in minutes
	int		ACT1State;								// status ACT1 output
	bool	mqttNotCon = true;						// MQTT broker not connected flag
	int		signalStrength;							// radio signal strength
	bool	wakeUp = true;							// wakeup indicator
	bool	setAck = false; 						// acknowledge receipt of actions
	bool	curState = true;						// current button state 
	bool	lastState = true;						// last button state
	bool	timerOnButton = false;					// timer output on button press
	bool	msgBlock = false;						// flag to hold button message
	bool	readAction;								// indicates read / set a value
	bool	send0, send1, send2, send3, send5, send6, send7;
	bool	send10, send16, send40, send41, send99;			// message triggers
	String	IP;										// IPaddress of ESP
	char	buff_topic[30];							// mqtt topic
	char	buff_msg[32];							// mqtt message
	char	clientName[10];							// Mqtt client name
	char	wifi_ssid[20];								// Wifi SSI name
	char	wifi_password[20];							// Wifi password


////////////////////////////////////////////////////Jim Start/////////////////////////////////////////////////////////

void errorblink (int blinks){
  errorblink(blinks,200);
}

void errorblink(int blinks, int blinkdelay)
  {
  int prevstat = digitalRead(MQCON);
  int ebdelay=blinkdelay*1;
  digitalWrite(MQCON,!prevstat);//it may be on...turn it off and wait a sec.  remember the led is reversed of the state, 1=off
  for (int i= 0; i <  blinks; i++){  
    delay(ebdelay);
    digitalWrite(MQCON,!prevstat);
    delay(ebdelay);
    digitalWrite(MQCON,prevstat);
    
  }
  
  }

void SetSwitchLock(int lockState)
  {
    //***************Logic:******************************************
    //CMD Issued      |Switch Currently ON  |Switch Currently OFF   |
    //----------      |-------------------- |-----------------------|
    //LOCK ON         | OFF 1 sec/ON        |ON 5sec/OFF 1sec/ON    |
    //UNLOCK          | OFF 5 sec/ON        |OFF                    |
    //***************************************************************
    //The seconds are set in the defines at the top-adjust there    |
    //***************************************************************
  #ifdef DEBUG
  Serial.print("Setting Hold [");
  Serial.print(lockState);
  Serial.println("] ");
#endif
#ifdef EnToggle
  if (lockState==0)
    {//Take them out of Locked On
          if (ACT1State == 1)
            {//Switch is ON and wish to cancel the HOLD
              digitalWrite(ACT1,0);
              delay(SWtoggleDelay);
              digitalWrite(ACT1,0);
            }
          if (ACT1State == 0)
            {//Switch is OFF and wish to cancel the HOLD
              digitalWrite(ACT1,0);
              delay(SWtoggleDelay);  //leaving this here just in case a stray message comes behind or a user "mis-click"
            }
    }
  if (lockState==1)
    {
          if (ACT1State == 1)
            {//Switch is ON and wish to initiate the HOLD
              digitalWrite(ACT1,0);
              delay(SWpulseDelay);
              digitalWrite(ACT1,1);
            }
          if (ACT1State == 0)
            {//Switch is OFF and wish to cancel the HOLD
              digitalWrite(ACT1,1);
              delay(SWtoggleDelay);  //leaving this here just in case a stray message comes behind or a user "mis-click"
              digitalWrite(ACT1,0);
              delay(SWpulseDelay);
              digitalWrite(ACT1,1);
            }
    
    }
 #endif
  }

////////////////////////////////////////////////////Jim End/////////////////////////////////////////////////////////


void mqttSubs(char* topic, byte* payload, unsigned int length);

	WiFiClient espClient;
	PubSubClient client(mqtt_server, 1883, mqttSubs, espClient); // instantiate MQTT client
	
	//	FUNCTIONS

//===============================================================================================

void pubMQTT(String topic, String topic_val){ // publish MQTT message to broker
#ifdef DEBUG
	Serial.print("topic " + topic + " value:");
	Serial.println(String(topic_val).c_str());
#endif

	client.publish(topic.c_str(), String(topic_val).c_str(), RETAINMSG);  //use this line for retained messages
	}

void mqttSubs(char* topic, byte* payload, unsigned int length) {	// receive and handle MQTT messages
	int i;
	error = 4; 										// assume invalid device until proven otherwise
#ifdef DEBUG
	Serial.print("Message arrived [");
	Serial.print(topic);
	Serial.print("] ");
	for (int i = 0; i < length; i++) {
		Serial.print((char)payload[i]);
		}
	Serial.println();
#endif
	if (strlen(topic) == 27) {						// correct topic length ?
		DID = (topic[25]-'0')*10 + topic[26]-'0';	// extract device ID from MQTT topic
		payload[length] = '\0';						// terminate string with '0'
		String strPayload = String((char*)payload);	// convert to string
	readAction = (strPayload == "READ");			// 'READ' or 'SET' value
	if (length == 0) {error = 2;}					// no payload sent
	else {
		if (DID ==0) {								// uptime 
			if (readAction) {
				send0 = true;
				error = 0;
			} else error = 3;						// invalid payload; do not process
		}
		if (DID==1) {								// transmission interval
			error = 0;
			if (readAction) {
				send1 = true;
			} else {								// set transmission interval
				TXinterval = strPayload.toInt();
				if (TXinterval <10 && TXinterval !=0) TXinterval = 10;	// minimum interval is 10 seconds
			}
		}
		if (DID ==2) {								// RSSI
			if (readAction) {
				send2 = true;
				error = 0;
			} else error = 3;						// invalid payload; do not process
		}
		if (DID==3) {								// version
			if (readAction) {
				send3 = true;
				error = 0;
			} else error = 3;						// invalid payload; do not process
		}
		if (DID==5) {								// ACK
			if (readAction) {
				send5 = true;
				error = 0;
			} else if (strPayload == "ON") {
					setAck = true;
					if (setAck) send5 = true;
					error = 0;
			}	else if (strPayload == "OFF") {
					setAck = false;
					if (setAck) send5 = true;
					error = 0;
			}	else error = 3;
		}
		
		if (DID==6) {								// toggle / timer mode selection
			if (readAction) {
				send6 = true;
				error = 0;
			} else if (strPayload == "ON") {		// select toggle mode
					toggleOnButton = true;
					if (setAck) send6 = true;
					error = 0;
			}	else if (strPayload == "OFF") {		// select timer mode
					toggleOnButton = false;
					if (setAck) send6 = true;
					error = 0;
			}	else error = 3;
		}
		
		if (DID==7) {								// Timer interval
			error = 0;
			if (readAction) {
				send7 = true;
			} else {								// set timer interval
				TIMinterval = strPayload.toInt();
				if (TIMinterval <5 && TIMinterval !=0) TIMinterval = 5;	// minimum interval is 5 seconds
			}
		}
		if (DID ==10) {								// IP address 
			if (readAction) {
				send10 = true;
				error = 0;
			} else error = 3;						// invalid payload; do not process
		}
		if (DID==16) {								// state of actuator
			if (readAction) {
				send16 = true;
				error = 0;
			} else if (strPayload == "ON") {
					ACT1State = 1;
					digitalWrite(ACT1,ACT1State);
					if (setAck) {
					  send16 = true;
            send3 = true;
					}
					error = 0;
			}	else if (strPayload == "OFF") {
					ACT1State = 0;
					digitalWrite(ACT1,ACT1State);
					if (setAck) send16 = true;
					error = 0;
			}	else error = 3;
		}
      if (DID==41) {                // toggle the actuator
      if (readAction) {
        //no action - there is not much a point trying to read this state - 
        //            the floodlights will come out of sync with the actual state anyway once daylight breaks.
        error = 0;
      } else if (strPayload == "1") {
          SetSwitchLock(1);
          if (setAck) send41 = true;
          error = 0;
      } else if (strPayload == "0") {
          //the best way to handle this is turn off for 10 seconds, then if they are on "ON/AUTO", turn it back on, if that is the state.
          SetSwitchLock(0);
          if (setAck) send41 = true;
          error = 0;
      } else error = 3;
    }
	}
		} else error =1;
		if (error !=0) {							// send error message
				sprintf(buff_topic, "home/esp_gw/nb/node%02d/dev91", nodeId);
				sprintf(buff_msg, "syntax error %d", error);
				pubMQTT(buff_topic, buff_msg);
			}
		}

	void connectMqtt() {								// reconnect to mqtt broker
		sprintf(buff_topic, "home/esp_gw/sb/node%02d/#", nodeId);
		sprintf(clientName, "ESP_%02d", nodeId);
		if (!client.loop()) {
			mqttNotCon = true;							// LED off means high voltage
#ifdef DEBUG
			Serial.print("Connect to MQTT broker...");
#endif
			if (client.connect(clientName,"home/esp_gw/disconnected",0,true,clientName)) { 
				mqttNotCon = false;						// LED on means low voltage
				client.subscribe(buff_topic);
	
#ifdef DEBUG
				Serial.println("connected");
#endif
			} else {
#ifdef DEBUG
				Serial.println("Failed, try again in 5 seconds");
#endif
				delay(5000);
				}
			digitalWrite(MQCON, mqttNotCon);			// adjust MQTT link indicator
			}
		}

		
	void connectWifi() {								// reconnect to Wifi
	while (WiFi.status() != WL_CONNECTED ) {
	int i=0;
	digitalWrite(MQCON,HIGH);							// turn off MQTT link indicator 
	if (fallBackSsi) 									// select main or fallback access point
		{strcpy(wifi_ssid, wifi_ssid_B);
		strcpy(wifi_password, wifi_password_B);}
	else
		{strcpy(wifi_ssid, wifi_ssid_A);
		strcpy(wifi_password, wifi_password_A);}
#ifdef DEBUG
		Serial.println();
		Serial.print("Connecting to ");
		Serial.println(wifi_ssid);
#endif
	WiFi.begin(wifi_ssid, wifi_password);
	while ((WiFi.status() != WL_CONNECTED) && (i<20)) {
			delay(1000);
			i++;
#ifdef DEBUG
			Serial.print(".");
#endif
	}
	fallBackSsi =!fallBackSsi;							// toggle access point
	}	
	digitalWrite(MQCON,LOW);							// turn on MQTT link indicator 
	delay(1000);										// for 1 second to indicate WIFI connection
	digitalWrite(MQCON,HIGH);							// turn off MQTT link indicator 
	delay(1000);
	IP = WiFi.localIP().toString();
#ifdef STAONLY
  WiFi.enableAP(0);  //Jim Added to stop all the devices showing up on the Wifi network 3/2017
#endif
#ifdef DEBUG
	Serial.print("\nWiFi successfully connected to AP:  ");
  Serial.println(wifi_ssid);
	Serial.println("IP address: ");
	Serial.println(IP);
  wifiOTAcheck();
#endif
	}

void wifiOTAcheck(){
#ifdef OTAenable
  Serial.println("WIFI OTA Checks beginning.");

  ArduinoOTA.setHostname(OTAname);
  
  ArduinoOTA.onStart([]() {
    Serial.println("Start .onStart");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    errorblink(10);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  Serial.println("Ready - OTA.begin");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("My Port Name: ");
  Serial.println(OTAname);

    
#endif
}

	void sendMsg() {								// send any outstanding messages
	int i;
	if (wakeUp) {									// send wakeup message
		wakeUp = false;
		sprintf(buff_topic, "home/esp_gw/nb/node%02d/dev99", nodeId);
		sprintf(buff_msg, "NODE %d WAKEUP: %s",  nodeId, clientName);
		send99 = false;
		pubMQTT(buff_topic, buff_msg);
		}

	if (send0) {									// send uptime
		sprintf(buff_topic, "home/esp_gw/nb/node%02d/dev00", nodeId);
		sprintf(buff_msg, "%d", upTime);
		send0 = false;
		pubMQTT(buff_topic, buff_msg);
		}

	if (send1) {									// send transmission interval
		sprintf(buff_topic, "home/esp_gw/nb/node%02d/dev01", nodeId);
		sprintf(buff_msg, "%d", TXinterval);
		send1 = false;
		pubMQTT(buff_topic, buff_msg);
		}

	if (send2) {									// send transmission interval
		sprintf(buff_topic, "home/esp_gw/nb/node%02d/dev02", nodeId);
		signalStrength = WiFi.RSSI();
		sprintf(buff_msg, "%d", signalStrength);
		send2 = false;
		pubMQTT(buff_topic, buff_msg);
		}

	if (send3) {									// send software version
		sprintf(buff_topic, "home/esp_gw/nb/node%02d/dev03", nodeId);
		for (i=0; i<sizeof(VERSION); i++) {
			buff_msg[i] = VERSION[i];}
		buff_msg[i] = '\0';
		send3 = false;
		pubMQTT(buff_topic, buff_msg);
		}

	if (send5) {									// send ACK state
		sprintf(buff_topic, "home/esp_gw/nb/node%02d/dev05", nodeId);
		if (!setAck) sprintf(buff_msg, "OFF");
		else sprintf(buff_msg, "ON");
		pubMQTT(buff_topic, buff_msg);
		send5 = false;
		}

	if (send6) {									// send toggleOnButton state
		sprintf(buff_topic, "home/esp_gw/nb/node%02d/dev06", nodeId);
		if (!toggleOnButton) sprintf(buff_msg, "OFF");
		else sprintf(buff_msg, "ON");
		pubMQTT(buff_topic, buff_msg);
		send6 = false;
		}

	if (send7) {									// send timer value
		sprintf(buff_topic, "home/esp_gw/nb/node%02d/dev07", nodeId);
		sprintf(buff_msg, "%d", TIMinterval);
		pubMQTT(buff_topic, buff_msg);
		send7 = false;
		}

	if (send10) {								// send IP address
		sprintf(buff_topic, "home/esp_gw/nb/node%02d/dev10", nodeId);
		for (i=0; i<16; i++) {
			buff_msg[i] = IP[i];}
		buff_msg[i] = '\0';
		pubMQTT(buff_topic, buff_msg);
		send10 = false;
		}

	if (send16) {									// send actuator state
		sprintf(buff_topic, "home/esp_gw/nb/node%02d/dev16", nodeId);
		if (ACT1State ==0) sprintf(buff_msg, "OFF");
		if (ACT1State ==1) sprintf(buff_msg, "ON");
		pubMQTT(buff_topic, buff_msg);
		send16 = false;
		}

	if (send40) {									// send button pressed message
		sprintf(buff_topic, "home/esp_gw/nb/node%02d/dev40", nodeId);
		if (ACT1State ==0) sprintf(buff_msg, "OFF");
		if (ACT1State ==1) sprintf(buff_msg, "ON");
		pubMQTT(buff_topic, buff_msg);
		send40 = false;
		}

}

//	SETUP

//===============================================================================================
	void setup() {									// set up serial, output and wifi connection

	pinMode(ACT1,OUTPUT);							// configure relay output
  digitalWrite(ACT1,0);         // switch off relay
 errorblink(5,200);
  
	ACT1State = OnAtPowerup;
	digitalWrite(ACT1,ACT1State);					// switch off relay
	pinMode(MQCON,OUTPUT);							// configure MQTT connection indicator

	digitalWrite(MQCON,HIGH);  						// switch off MQTT connection indicator
	send0 = false;
	send1 = false;
	send3 = true;                     //send version on startup
	send5 = false;
	send7 = false;
	send10 = true;									// send IP on startup
	send16 = false;
	send40 = false;

#ifdef DEBUG
	Serial.begin(SERIAL_BAUD);
#endif
	}

//	LOOP

//===============================================================================================
	void loop() {												// Main program loop

	if (WiFi.status() != WL_CONNECTED) {						// Wifi connected ?
		connectWifi();
		}

#ifdef OTAenable
  ArduinoOTA.handle();
#endif
  
	if (!client.connected()) {									// MQTT connected ?
		connectMqtt();
		}
	client.loop();
	
	


// DETECT INPUT CHANGE

	curState = digitalRead(BTN);								// Read button
	msgBlock = ((millis() - lastBtnPress) < HOLDOFF);			// hold-off time for additional button messages
	if (!msgBlock &&  (curState != lastState)) {				// input changed ?
		delay(5);
		lastBtnPress = millis();								// take timestamp
		if (setAck) send40 = true;								// send button message
		if (curState == LOW) {
		if (toggleOnButton) {									// button in toggle state ?
			ACT1State = !ACT1State; 							// toggle output
			digitalWrite(ACT1, ACT1State);
			send16 = true;										// send message on status change
		} else
		if (TIMinterval > 0 && !timerOnButton) {				// button in timer state ?
			timerOnButton = true;								// start timer interval
			ACT1State = HIGH;									// switch on ACT1
			digitalWrite(ACT1, ACT1State);
			send16 = true;
		}}
		lastState = curState;
		}

// TIMER CHECK

	if (TIMinterval > 0 && timerOnButton) {						// =0 means no timer
		if ( millis() - lastBtnPress > TIMinterval*1000) {		// timer expired ?
			timerOnButton = false;								// then end timer interval 
			ACT1State = LOW;									// and switch off Actuator
			digitalWrite(ACT1,ACT1State);
			send16 = true;
			}
		}

// INCREASE UPTIME 

	if (lastMinute != (millis()/60000)) {						// another minute passed ?
		lastMinute = millis()/60000;
		upTime++;
		}

// PERIODIC TRANSMISSION


	if (TXinterval > 0){
		int currPeriod = millis()/(TXinterval*1000);
		if (currPeriod != lastPeriod) {							// interval elapsed ?
			lastPeriod = currPeriod;

			// list of sensordata to be sent periodically..
			// remove comment to include parameter in transmission
 
//			send10 = true;										// send IP address
			send2 = true;										// send RSSI
//			send3 = true;										// send version
			send16 = true;										// output state
			}
		}

	sendMsg();													// send any mqtt messages

}		// end loop
    

