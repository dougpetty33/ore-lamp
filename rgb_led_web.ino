//Disclaimer:
//I did not write nor do I own the libraries this code consumes.
//They each have their own licenses which are binding.
//The following code is loosely based on several sketches included in the arduino ide

#include <Adafruit_NeoPixel.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

//Redacted wifi credentials
const char *ssid = "";
const char *password = "";

ESP8266WebServer server ( 80 );
//Constants obatined from Adafruit neo_pixel library
Adafruit_NeoPixel strip = Adafruit_NeoPixel(10, 4, NEO_GRB + NEO_KHZ800);

//todo: handle post data rather than get
void handleSetColor()
{
  byte red = 0;
  byte green = 0;
  byte blue = 0;

  if(server.hasArg("red") && server.hasArg("blue") && server.hasArg("green"))
  {
      red = (byte)server.arg("red").toInt();
      blue = (byte)server.arg("blue").toInt();
      green = (byte)server.arg("green").toInt();
      colorWipe(strip.Color(red, green, blue), 10); // Red
      server.send ( 200, "text/html", "set color");
  }
  else
  {
    server.send ( 400, "text/html", "Need a red, green, and blue value" );
  }
}

void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void handleRoot() {
  server.send ( 200, "text/html", "All is OK" );
}

void handleNotFound() {
	server.send ( 404, "text/plain" );
}

/*
 * Arduino standard methods 
 */
void setup ( void ) {
  strip.begin();
  strip.show();
	WiFi.begin ( ssid, password );

	// Wait for connection
	while ( WiFi.status() != WL_CONNECTED ) {
		delay ( 500 );
	}

	server.on ( "/", handleRoot );
  server.on("/ore-lamp", HTTP_GET, handleSetColor);
	server.onNotFound ( handleNotFound );
	server.begin();
}

void loop ( void ) {
	server.handleClient();
}

