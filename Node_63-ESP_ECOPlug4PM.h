//////////Include file for Node 63 - EcoPlug4////////////////////////////
////////////////POWER MONITOR CAPABLE CIRCUITRY/////////////////////////
////Jim 12/2016


#define VERSION "Sonoff V1.2 - EcoPlug4-PM"          // this value can be queried as device 3 
// This plug has power monitoring circuitry not implimented yet

#define wifi_ssid_A "YOURFIRSTACCESSPOINTNAME"         // wifi station name
#define wifi_password_A "YOURwifiPASSWORD"      // wifi password
#define wifi_ssid_B "YOURsecondACCESSPOINTNAME"          // fallback wifi station name; if not present use same as A
#define wifi_password_B "YOURwifiPASSWORD"      // fallback wifi password; if not present use same as A
#define mqtt_server "10.1.1.15"      // mqtt server IP

#define nodeId 63               // node ID
#define DEBUG                 // uncomment for debugging
#define OnAtPowerup 1             //jim - 0=off, 1=on
#define RETAINMSG 0                //jim  comment to make non retained.  1=retained, 0=no
#define STAONLY                   //to turn off the built in AP

#define ACT1 15                 // Actuator pin (LED or relay to ground)
#define MQCON 2                // MQTT link indicator
#define BTN 13                 // Button pin (also used for Flash setting)

//#define PMenable                      //enables the power management feature
//#define PMpin  5                      //the PM read pin

#define OTAenable                        //enable OTA code upgrades
#define OTAname "ESP8266-EcoPlug4PM-N63"       //enable OTA code upgrades  --Max Chars about 20 & no _ or the port wont show!
