//////////Include file for Node 66-ECOPlugOD1////////////////////////////
////Jim 4/2016
//Outdoor Model

#define VERSION "EPSGW V1.2 - EcoPlugOD2"          // this value can be queried as device 3

#define wifi_ssid_A "YOURFIRSTACCESSPOINTNAME"         // wifi station name
#define wifi_password_A "YOURwifiPASSWORD"      // wifi password
#define wifi_ssid_B "YOURsecondACCESSPOINTNAME"          // fallback wifi station name; if not present use same as A
#define wifi_password_B "YOURwifiPASSWORD"      // fallback wifi password; if not present use same as A
#define mqtt_server "10.1.1.15"      // mqtt server IP

#define nodeId 66               // node ID
#define DEBUG                 // uncomment for debugging
#define OnAtPowerup 0             //jim - 0=off, 1=on
#define RETAINMSG 0                //jim  comment to make non retained.  1=retained, 0=no
#define STAONLY                   //to turn off the built in AP

#define ACT1 15                 // Actuator pin (LED or relay to ground)
#define MQCON 2                // MQTT link indicator
#define BTN 13                 // Button pin (also used for Flash setting)

#define OTAenable                        //enable OTA code upgrades
#define OTAname "ESP8266-EcoPlugOD1-N66"       //enable OTA code upgrades  --Max Chars about 20 & no _ or the port wont show!
