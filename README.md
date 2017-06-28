# ESP_SonoffAndEcoPlug
OTA enabled MQTT firmware for the Sonoff, ECOplug, Wemos, and similar

For up to date content and support, please visit this thread:

http://homeautomation.proboards.com/thread/254/esp8266-update-sonoff-ecoplugs-similar

on this site:

http://homeautomation.proboards.com/board/2/openhab-rfm69-based-arduino

for more great resources, help, and support on Arduino, ESP8266, and Home Automation



ESP8266 OTA Update code for Sonoff, Ecoplugs, Wemos +similar

Since Computourist did such a great job with the ESP8266 code, and specifically targeted a version toward Sonoffs, I decided this was in desperate need of OTA updating due to the fact that Sonoffs can me installed in locations that are not so easy to get to once installed.  Secondly, this device is a bit dangerous if proper care is not taken with it, so before I post the code, please heed these warnings!  I've seen so much bad advice on the internet and youtube where people are so concerned with getting this device working they overlook these three most important safety steps:

-At no time should you EVER hook your FTDI programmer to the board with the mains live!!!  Let me repeat, NEVER EVER, not even to debug it, and I dont care if you left the 3.3v line disconnected or not, this thing is referenced to the mains, and your device will be then also and possibly you!  Therefore, it's particlary tricky to debug since you shouldnt be hooking up this FTDI adapter with it running!  Additionally, most FTDI adapters cannot supply enough power to run it, much less activate the relay.  I have found that some will, and some will not, and I'll put a document in the code with complete instructions.  See this video starting at 9:30:

http://www.superhouse.tv/21-six-sonoff-secrets/


-It is never OK to leave the ground floating!!!  If you are outputting a ground, you need to CARRY IT AROUND the device.  Just because there is no terminal for it doesnt mean you can skip it.  Strip the ground long, and shorten the power connector, then use a WAGO connector to connect them in the box, enter my next rant:

-These devices have precious little IP!!!  (Ingress Protection)  It has to be a 42 at best.  IF you are going to put it anywhere that can be accessed easy, put it in an enclosure.  A link to some nice boxes:
https://www.aliexpress.com/item/Waterproof-Clear-Cover-Plastic-Electronic-Project-Box-115x90x55mm/32758724664.html?spm=2114.13010608.0.0.P3V9IH
..for a double.  They have a single 100x55mm also.

-It has a rating of 90-250VAC.  If you are going to put a dimmer, put it AFTER and not before.   See attachement for a good way to close it, with a triac after it to dim my christmas tre  See attachement for a good way to close it, with a triac after it to dim my christmas tree lights.

Additionally, watch the solder joints on the mains connector.  I had one that wiggled loose that would have caused a loose connection and probably heat up.  If it seems loose at all, reflow it with some lead based solder.  Stupid counterproductive ROHS...

Next post, programming it and troubleshooting uploads:







Programming it properly can be tricky. Best is to make up a premade cord for your programmer so you can eliminate errors. I attached a 2 pictures of my setup. From the broken switch (fragile things, sometimes and external one is better anyway) left to right, the pins are connected as follows:
3V3
TX
RX
GND
N/C

There is no need to solder a header. You can place the programmer in the holes, and hold some sideways pressure to make a good contact.

Make the proper settings in the Arduino environment AFTER installing support for the ESP8266 boards. Instructions here:

https://learn.sparkfun.com/tutorials/esp8266-thing-hookup-guide/installing-the-esp8266-arduino-addon

...but stop at the heading "Selecting the ESP8266 Thing Board", you dont need to go any further

Open Arduino(I use 1.6.8, some older versions dont have proper features, so use at least this version).
-In Tools menu, under boards, scroll down to the ESP8266 boards section, and select "GEneric ESP8266 Board". This will give you all options!
-Don't touch Flash Mode, Frequency, and CPU frequency, leave those default.
-All Single Sonoffs have 8megabit flash chips, which is plenty for OTA of this code. You can upgrade them if you want to run Tasmota firmware, see the link in the above post to Superhouse. 8megabits/8 = 1 Megabyte flash. However, we need space in the FLASH area not the SPIFFS (SPI Flash File System) area for the upgrade code. Keep in mind when programming or adding new features, you cannot go over 50% of the program space and keep OTA working. In my next post I'll show you hot to tell if you went over and troubleshoot uploading OTA. So we need to have as much program space and very little SPIFFS as possible. (I dont use it for anything yet, but it's a built in SSD drive for storing files, settings etc...) See the attached image to explain how it must fit. 
So, if we choose 1megabyte with 128kb spiffs, 1024-128=896.  896/2=448.  Currently, here is where my attached code is at:
Sketch uses 279,689 bytes (31%) of program storage space.
...so I can grow it 169 bytes more to 50%.
Now you understand the memory and SPIFFS, select 1M(128K SPIFFS) from the flash size.  If you want to verify your flash size, upload the sketch EXAMPLES/ESP8266/CHECKFLASHCONFIG then open serial monitor.  It will tell you the size and if you selected the correct flash.
-NEXT,  leave Debug Port and Debug Level to disabled and none.  Next post I'll tell you how to use them to troubleshoot upload OTA issues if you have them.
-Leave reset method on CK
-Speed up the upload speed to 230,400.   Wemos Mini's can take 921,600!
-for Port select the port your USB FTDI will be on when you plug it in in the next step.


Now place the programmer in the holes, and hold some sideways pressure to make a good contact. While doing this, HOLD DOWN the switch, and connect the USB of your FTDI to the computer. To Reiterate:

Connect FTDI, holding sideways pressure with one hand
Hold down the Sonoff's switch with the same hand
With your free hand, plug in the FTDI.
Check/Change the Port

Then upload.

After it uploads, you should be in business!
Close the arduino interface, and open it again.  Then under Tools/Ports your OTA ports should be alive!  For me, I had to restart Arduino for them to show.
See image

If they dont show, make sure you followed my advice for naming this port(if you did it manually like I did and not let the sketch assign)!!  Limit about 20 characters, no underscores.
Sometimes, NONE showed up if even one broke the rules!

Tomorrow I explain troubleshooting, and I have to clean my personal info from the code and upload it.
Stay tuned...


Troubleshooting Preperation.

If the ports do not show up:
-Limit about 20 characters, no underscores.
-Use a very simple name to troubleshoot, and expand if need be.
-Make sure to use Arduino 1.6.8 at least with the extensions for ESP8266


If the port shows up, and you cannot upload:
Easiest thing to do is to recompule and reupload with the debug options on.
From the Tools Menu, under the boards section, change the "Debug Port" to Serial, and change the "Debug Level" to "OTA + Uploader", and reupload via serial like you did in a pervious step.
At this point, you will need an alternate serial Terminal Emulator program like screen for Linux or Putty for Windoze.
Start your terminal program and watch the board via this terminal.  Easy way on linux is "screen /dev/ttyS0 115200".  Change of course the port (/dev/ttyS0" to your FTDI adapter and the port speed to match the serial baud IN THE SKETCH, not the upload speed.  
I quickly unplug my adapter and replug and quickly execute this command.  Then in screen or Putty you can see your sketch boot.
At this point, I may mention you may get stack traces if your FTDI adapter does not have enough power!  So you may have to provide the sonoff 3.3 volts seperately.  The one I have is this guy as you see in my other pictures and he does work:
https://www.aliexpress.com/item/Free-Shipping-FT232RL-FTDI-USB-3-3V-5-5V-to-TTL-Serial-Adapter-Module-forArduin-Mini/32519490747.html

This is also why I recomment you buy a Wemos D1 Mini and a relay shield to start off with.  Its easy for a first troubleshooting with it's built in CH340 chip and no power problems.  After you have that mastered, better then to move on to a Sonoff.  But I digress...

At this point you should have live serial data on screen or Putty.  Switch back to the Arduino environment, and switch the port to the OTA port of this device.  Then retry your upload.  When it fails, look at the output on screen or Putty and it will tell you the REAL REASON it failed, not the nonsense error from the Arduino Interface.

Next up the code:




Onto the code:

Ive separated it into code and header (.h) files.  I has little point to have seperate projects where you target the same hardware and end up with different versions of the code base.  Therefore, if we upgrade the code, we can just drop the code file in and upload without messing around with personalized settings.  All that changes is the header includes at the top.

.ino file:
in the first 18 lines, you will notice there are several includes, each to go with the corresponding header file that will open up in the tabs of the IDE.  Uncomment the one you wish to use, and then choose the corresponding tab to customize your settings.  Ive given several examples there, some for sonoff, some for Ecoplugs indoor, Ecoplugs with Power monitoring (to come, I havent finished...but the code will be a drop in replacement.), and EcoPlugs Outdoor and a test version to run on the Wemos D1 Mini with relay shield.

.header files:
-the version I customize and put that in openhab on a version pages.  This way when I push an update I can see it's version and the change took effect.  Customize to your liking if you use this feature, or generalize it...
-Wifi access point A and B...see Computourist notes...
-node ID - obvious - must be unique
-debug - keep it on, pointless to turn it off it wont save any power and only 224 bytes.  You have 169kb or 169,000bytes free.  I point this out, please forgive my erroneous math in the previous post.
Three new functions I added:
#define OnAtPowerup 1             //jim - 0=off, 1=on
#define RETAINMSG 0                //jim  comment to make non retained.  1=retained, 0=no
#define STAONLY                   //to turn off the built in AP

-On at power up will make the sonoff do just that.  I like to still use my switch if I put them near the light.  This way, I can turn the switch on/off and have it work normal, and then the HA can take over if it's on.  So you can interpret your light switch as OFF/ON AUTO.  If you dont want that, ovbiously put it at 0

-Retainmsg - persistant messages annoy me.  I like them to go away.  Thanks Computourist for the help with getting rid of them, I put it as an option.  Again, if you like this, put it back on to 1.

-STA only.  As you deploy tons of these ESP things, they will host in AP mode as well.  It annoys me to see them all in my Wifi networks, so I leave them off.  You may want that if you implement a special function.  (perhaps a fallback web page OTA update if you replace your AP and it cannot connect?)  Anyway, looked like a bit of a security risk to me.  I use the ESP8266 on my window shades and I dont want someone connecting to them and rolling them up at night :)  (I wont go there....LOL)

#define ACT1 12                  // Actuator pin (LED or relay to ground)
#define MQCON 13                // MQTT link indicator
#define BTN 0                 // Button pin (also used for Flash setting)

-Act 1 is the activator pin the relay is on.  12 for Sonoff, D1 for Weemos, 15 for Ecoplugs indoor + outdoor.
-MQcon is the LED  I think more research may be needed on this, I just do not care about it.  I have some off the old version sonoffs where the green light is tied to the relay.  There is a hack to enable the red LED(see the support files directory).  I think in the black label Sonoff this may be a bit different, maybe someone can look into it.
-BTN is obviously the pin the button is wired too.  0 for sonoff, 13 for ECOplug, and the Wemos is mostly on D2 I believe although it depends on the shield.  Some are configurable...  I put you a spreadsheet in the support directory.


more...

...code continued.

//#define PMenable //enables the power management feature
//#define PMpin 5 //the PM read pin

These two are for the Power monitoring of the ECOplugs and the Sonoff POW I havent finished yet.  If someone has some research on that, pass it on, but right now I dont have the time or a direct need so it will be a while until I finish it.  Under images-ecoplug is a discussion, there is a head start in there about it if anyone is energetic.  Its as far as I got.

And the grand finale...the good stuff:

#define OTAenable //enable OTA code upgrades
#define OTAname "ESP8266-Sonoff-N46" //enable OTA code upgrades- --Max Chars about 20 & no _ or the port wont show!
//Limit 20 Chrs:|--------------------|

You can turn on and off OTA here. With it on and 1M/128kb SPIFFS:
Sketch uses 231,029 bytes (24%) of program storage space. Maximum is 958,448 bytes.
With it off:
Sketch uses 236,522 bytes (24%) of program storage space. Maximum is 958,448 bytes.
Both are without debug on as well  (again, pointless to disable)
...not much savings.

With full debugging (OTA+Updater, and serial debugging enabled) and OTA enabled the max I came up with is:
Sketch uses 247,630 bytes (25%) of program storage space. Maximum is 958,448 bytes.

Last is the port name.  Keep it simple, avoid special characters except - and keep it under 20, or the port wont show up (unless this is fixed in later versions of arduino)

I wish you all good luck with it, and hope it makes your life as easy as it does mine.  Opening up the ECOplugs to reflash is a pain!!!
Special thanks to Computourist for this great work.  So much better and cheaper than those pesky RFM's when you need something simple.



