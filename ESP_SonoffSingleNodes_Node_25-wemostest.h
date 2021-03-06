//////////Include file for Node 45 - Rear Deck Lights////////////////////////////
////Jim 12/2016

//Commands for this node:
//Lock on:  home/esp_gw/sb/node51/dev41   1
//Cancel lock:  home/esp_gw/sb/node51/dev41  0
//ON:  home/esp_gw/sb/node51/dev16   ON
//OFF:  home/esp_gw/sb/node51/dev16  OFF

//Different Motion floodlights need different timings...this works for the Utilitech set on the front corner
#define EnToggle                //enable the functions of the motion light toggle...if not defined, like in ECOplugs header,
                                //the below defines are not needed
#define SWtoggleDelay 5000      //the amount of time the switch needs to be off to reset the motion detection
#define SWpulseDelay 350      //the amount of time the switch needs to be off to trip the lock-on
#define SWpulseCycles 1       //some need the switch cycled more than once

//Limit 28 Chrs:|----------------------------|
#define VERSION "Sonoff V1.2.1-N25-WemosRelay"          // this value can be queried as device 3

#define wifi_ssid_B "YOURsecondACCESSPOINTNAME"					// wifi station name
#define wifi_password_B "YOURwifiPASSWORD"			// wifi password

#define wifi_ssid_A "YOURFIRSTACCESSPOINTNAME"					// fallback wifi station name; if not present use same as A
#define wifi_password_A "YOURwifiPASSWORD"			// fallback wifi password; if not present use same as A
#define mqtt_server "10.1.1.15"      // mqtt server IP


#define nodeId 85								// node ID
#define DEBUG									// uncomment for debugging
#define OnAtPowerup 1             //jim - 0=off, 1=on
#define RETAINMSG 0                //jim  comment to make non retained.  1=retained, 0=no
//#define STAONLY                   //to turn off the built in AP

#define MQCON D3                // MQTT link indicator
#define ACT1 D1                  // Actuator pin (LED or relay to ground)   D1 on Wemos Mini
#define BTN 0                 // Button pin (also used for Flash setting)

#define OTAenable                        //enable OTA code upgrades
#define OTAname "ESP8266-WemosRly-N25"       //enable OTA code upgrades  --Max Chars about 20 & no _ or the port wont show!
//Limit 20 Chrs:|--------------------|

