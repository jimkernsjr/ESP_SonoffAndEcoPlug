//////////Include file for Node 51 - Garage Lights////////////////////////////
////Jim 12/2016

//Commands for this node:
//Lock on:  home/esp_gw/sb/node51/dev41   1
//Cancel lock:  home/esp_gw/sb/node51/dev41  0
//ON:  home/esp_gw/sb/node51/dev16   ON
//OFF:  home/esp_gw/sb/node51/dev16  OFF

//Different Motion floodlights need different timings...this works for the Utilitech Pro sku0330004 on the garage
#define EnToggle                //enable the functions of the motion light toggle...if not defined, like in ECOplugs header,
                                //the below defines are not needed
#define SWtoggleDelay 5000      //the amount of time the switch needs to be off to reset the motion detection
#define SWpulseDelay 350      //the amount of time the switch needs to be off to trip the lock-on


#define VERSION "Sonoff V1.2-GarLFFlood"          // this value can be queried as device 3 --Limit 28 chars

#define wifi_ssid_A "YOURFIRSTACCESSPOINTNAME"					// wifi station name
#define wifi_password_A "YOURwifiPASSWORD"			// wifi password

#define wifi_ssid_B "YOURFIRSTACCESSPOINTNAME"					// fallback wifi station name; if not present use same as A
#define wifi_password_B "YOURwifiPASSWORD"			// fallback wifi password; if not present use same as A

#define mqtt_server "10.1.1.15"			// mqtt server IP

#define nodeId 51								// node ID
#define DEBUG									// uncomment for debugging
#define OnAtPowerup 1             //jim - 0=off, 1=on
#define RETAINMSG 0                //jim  comment to make non retained.  1=retained, 0=no
#define STAONLY                   //to turn off the built in AP

#define ACT1 12                  // Actuator pin (LED or relay to ground)
#define MQCON 13                // MQTT link indicator
#define BTN 0                 // Button pin (also used for Flash setting)

#define OTAenable                        //enable OTA code upgrades
#define OTAname "ESP8266-Sonoff-N51"       //enable OTA code upgrades --Max Chars about 20 & no _ or the port wont show!

