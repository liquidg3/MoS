
//Lets see if this works
#include <Adafruit_CC3000.h>
#include <SPI.h>
#include <Wire.h>
//#include "utility/debug.h"
//#include "utility/socket.h"
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"

// These are the interrupt and control pins
#define ADAFRUIT_CC3000_IRQ   3  // MUST be an interrupt pin!
// These can be any two pins
#define ADAFRUIT_CC3000_VBAT  5
#define ADAFRUIT_CC3000_CS    10
// Use hardware SPI for the remaining pins
// On an UNO, SCK = 13, MISO = 12, and MOSI = 11
Adafruit_CC3000 cc3000 = Adafruit_CC3000(ADAFRUIT_CC3000_CS, ADAFRUIT_CC3000_IRQ, ADAFRUIT_CC3000_VBAT,
                                         SPI_CLOCK_DIVIDER); // you can change this clock speed

//#define WLAN_SSID       "sprucemen"           // cannot be longer than 32 characters!
//#define WLAN_PASS       "wifimealready"
#define WLAN_SECURITY   WLAN_SEC_WPA2
#define LISTEN_PORT     7777    // What TCP port to listen on for connections.  The echo protocol uses port 7.

Adafruit_CC3000_Server server(LISTEN_PORT);


int totalLeds = 8;

//
//Adafruit_7segment leds[8] = {
//  Adafruit_7segment(), Adafruit_7segment(), Adafruit_7segment(), Adafruit_7segment(), Adafruit_7segment(), Adafruit_7segment(), Adafruit_7segment(), Adafruit_7segment()
//  };

void setup(void)
{
  Serial.begin(115200);
  
  /* Initialise the module */
//  Serial.println(F("\nInitializing..."));
  if (!cc3000.begin())
  {
//    Serial.println(F("Couldn't begin()! Check your wiring?"));
    while(1);
  }

//Serial.println(F("Enabling LEDS"));
//  for (int c = 0; c < totalLeds; c++) {
//    leds[c].begin(0x70+c);
//  }

//  led(1111);
 

  if (!cc3000.connectToAP("sprucemen", "wifimealready", WLAN_SECURITY)) {
//    Serial.println(F("Failed!"));
    while(1);
  }

   
   
//  Serial.println(F("Connected!"));
  
//  Serial.println(F("Request DHCP"));
  while (!cc3000.checkDHCP())
  {
    delay(100); // ToDo: Insert a DHCP timeout!
  }  

//led(2222);

  /* Display the IP address DNS, Gateway, etc. */  
//  while (! displayConnectionDetails()) {
//    delay(1000);
//  }

  
  // Start listening for connections
  server.begin();

//  led(3333);
//  Serial.println();
//  Serial.println(F("Sending IP to mospi"));

  uint32_t ip = cc3000.IP2U32(192, 168, 1, 12);
  Adafruit_CC3000_Client www;

  int attempt = 3333;
  
  do {

    attempt ++;
//    led(attempt);
    
    www = cc3000.connectTCP(ip, 80);
    if (www.connected()) {
      delay(1000);
      www.fastrprint(F("arduino1"));
      www.println();
//      Serial.println(F("Listening for connections..."));
    } else {
//      Serial.println(F("Failed to connect to mospi!")); 
      delay(1000);
    }
  
    
  }  while (!www.connected());

//  led(4444);
}

void loop(void)
{
  // Try to get a client which is connected.
  Adafruit_CC3000_ClientRef client = server.available();
  if (client) {


    
     // Check if there is data available to read.
    int ledIndex = 0;
   char minute[5];
    int minuteIndex = 0;
       
     while (client.available() > 0 || ledIndex < totalLeds) {
      
      // Read a byte and write it to all clients.
      if (client.available() > 0) {
        minute[minuteIndex] = (char) client.read();
       Serial.print ("Setting char at index ");
       Serial.print (minuteIndex);
       Serial.print (" to ");
       Serial.println (minute[minuteIndex]);
        minuteIndex++;
        if (minuteIndex > 3) {
           minuteIndex = 0;
           ledIndex ++;
           minute[4] = '\0';
           int m = String(minute).toInt();
           Adafruit_7segment led = Adafruit_7segment();
           led.begin(0x70+ledIndex);
           led.println(m);
           led.drawColon(true);
           led.writeDisplay();
           Serial.print ("setting Led ");
           Serial.print (ledIndex);
           Serial.print (" to ");
           Serial.println (minute); 
        }
      }
      
    }

//    Serial.println((char *) buffer);
   
   
//    for (int index = 0; index < bufferIndex - 1; index++) {
//      
//      if ((char) buffer[index] != '|') {
//          minute[minuteIndex] = (char) buffer[index];
//          minuteIndex++;
//      } else {
////          Serial.print("LED INDEX ");
////          Serial.print(ledIndex);
////          Serial.print(": ");
//          
//          minute[minuteIndex] = '\0';
//          int m = String(minute).toInt();
//          
////          Serial.println(m);
//Adafruit_7segment led = Adafruit_7segment();
//led.begin(0x70+ledIndex);
//led.println(m);
//led.drawColon(true);
//led.writeDisplay();
//
////          leds[ledIndex].println(m);
////          leds[ledIndex].drawColon(true);
////          leds[ledIndex].writeDisplay();
//
//          //setup for next minute chunk
//          memset(&minute, 0, sizeof(minute));
//          minuteIndex = 0;
//          ledIndex ++;
//          
//      }
      
//    }
    
  }
}

//void led (int msg) {
//  for (int c = 0; c < totalLeds; c++) {
//     leds[c].println(msg);
//     leds[c].writeDisplay();
//  }
//}

/**************************************************************************/
/*!
    @brief  Tries to read the IP address and other connection details
*/
/**************************************************************************/
//bool displayConnectionDetails(void)
//{
//  uint32_t ipAddress, netmask, gateway, dhcpserv, dnsserv;
//  
//  if(!cc3000.getIPAddress(&ipAddress, &netmask, &gateway, &dhcpserv, &dnsserv))
//  {
////    Serial.println(F("Unable to retrieve the IP Address!\r\n"));
//    return false;
//  }
//  else
//  {
////    Serial.print(F("\nIP Addr: ")); cc3000.printIPdotsRev(ipAddress);
////    Serial.print(F("\nNetmask: ")); cc3000.printIPdotsRev(netmask);
////    Serial.print(F("\nGateway: ")); cc3000.printIPdotsRev(gateway);
////    Serial.print(F("\nDHCPsrv: ")); cc3000.printIPdotsRev(dhcpserv);
////    Serial.print(F("\nDNSserv: ")); cc3000.printIPdotsRev(dnsserv);
////    Serial.println();
//    return true;
//  }
//}
