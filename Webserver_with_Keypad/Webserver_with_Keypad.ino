#include <Keypad.h>
/*
  Web Server

 A simple web server that shows the value of the analog input pins.
 using an Arduino Wiznet Ethernet shield.

 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 * Analog inputs attached to pins A0 through A5 (optional)

 created 18 Dec 2009
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe

 */

#include <SPI.h>
#include <Ethernet.h>

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0x90, 0xA2, 0xDA, 0x0D, 0x6F, 0xEF
};
IPAddress ip(192, 168, 1, 50);

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(80);

/*  Keypadtest.pde
 *
 *  Demonstrate the simplest use of the  keypad library.
 *
 *  The first step is to connect your keypad to the
 *  Arduino  using the pin numbers listed below in
 *  rowPins[] and colPins[]. If you want to use different
 *  pins then  you  can  change  the  numbers below to
 *  match your setup.
 *
 */
#include <Keypad.h>
char tweet [140]; // array of the tweet
char empty_tweet [140]; // Used to flash the tweet array

int count=0; // key counter


const byte ROWS = 4; // Four rows
const byte COLS = 4; // Four columns
// Define the Keymap
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
// Connect keypad ROW0, ROW1, ROW2 and ROW3 to these Arduino pins.
byte rowPins[ROWS] = { 0, 1, 2 , 3 };
// Connect keypad COL0, COL1 and COL2 to these Arduino pins.
byte colPins[COLS] = { 4 , 5 , 6 , 7}; 

// Create the Keypad
Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

#define ledpin 13 //out put led :D دي يا معلم ال بنطلع عليها الخارجي

void setup()
{
    // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ;}
    
  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}

void loop()
{
    // listen for incoming clients
  EthernetClient client = server.available();

  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          // output the value of each analog input pin
          for (int analogChannel = 0; analogChannel < 6; analogChannel++) {
              char key = kpd.getKey();
  if(key)  // Check for a valid key.
  {
    if (key=='*') // As if we are pressing enter to submit the message on the server
    { 
      client.println(tweet); 
      Serial.println(tweet);// replace with message on our webserver 
   memcpy(tweet,empty_tweet,140); 
   }
   else if (key=='0') // Replacing Zero with space 
   {
     tweet[count]=' ';
     count++;
   }
   else 
   {
     tweet[count]=key;
     count++;
   }
  }
            int sensorReading = analogRead(analogChannel);
            client.print("Voltage input ");
            client.print(analogChannel);
            client.print(" is maximum 5V reading with scale 0 to 1024 ");
            client.print(sensorReading);
            client.println("<br />");
          }
          client.println("</html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        }
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
}
