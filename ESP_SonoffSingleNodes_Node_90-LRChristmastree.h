//////////Include file for Node 90 - LR Christmas Tree////////////////////////////
////Jim 3/2017

//Commands for this node:
//Lock on:  home/esp_gw/sb/node90/dev41   1
//Cancel lock:  home/esp_gw/sb/node90/dev41  0
//ON:  home/esp_gw/sb/node90/dev16   ON
//OFF:  home/esp_gw/sb/node90/dev16  OFF


#define VERSION "Sonoff V1.2-LRChristmastree"          // this value can be queried as device 3--Limit 28 chars

#define wifi_ssid_A "YOURfirstACCESSPOINTNAME"					// wifi station name
#define wifi_password_A "YOURwifiPASSWORD"			// wifi password

#define wifi_ssid_B "YOURsecondACCESSPOINTNAME"					// fallback wifi station name; if not present use same as A
#define wifi_password_B "YOURwifiPASSWORD"			// fallback wifi password; if not present use same as A

#define mqtt_server "10.1.1.15"			// mqtt server IP

#define nodeId 90								// node ID
#define DEBUG									// uncomment for debugging
#define OnAtPowerup 1             //jim - 0=off, 1=on
#define RETAINMSG 0                //jim  comment to make non retained.  1=retained, 0=no
#define STAONLY                   //to turn off the built in AP

#define ACT1 12                  // Actuator pin (LED or relay to ground)
#define MQCON 13                // MQTT link indicator
#define BTN 0                 // Button pin (also used for Flash setting)

#define OTAenable                        //enable OTA code upgrades
#define OTAname "ESP8266-Sonoff-N90"       //enable OTA code upgrades  --Max Chars about 20 & no _ or the port wont show!

