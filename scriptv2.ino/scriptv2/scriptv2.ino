/*********
  Rui Santos
  Complete project details at http://randomnerdtutorials.com  
*********/

// Load Wi-Fi library
#include <ESP8266WiFi.h>
#include <Adafruit_NeoPixel.h>

#define PIN 2
#define NUMPIXELS 30
#define DELAYVAL 500

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// Replace with your network credentials
const String ssid = "SSID";
const String password = "PWD";

// Set web server port number to 80
WiFiServer server(80);

// Decode HTTP GET value
String redString = "0";
String greenString = "0";
String blueString = "0";
int pos1 = 0;
int pos2 = 0;
int pos3 = 0;
int pos4 = 0;

// Variable to store the HTTP req  uest
String header;

// Red, green, and blue pins for PWM control
const int RED = 2;     // 13 corresponds to GPIO13
const int GREEN = 4;   // 12 corresponds to GPIO12
const int BLUE = 5;    // 14 corresponds to GPIO14


// Setting PWM frequency, channels and bit resolution
const int freq = 5000;
const int redChannel = 0;
const int greenChannel = 1;
const int blueChannel = 2;
// Bit resolution 2^8 = 256
const int resolution = 8;


// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

void setup() {
  // initialisation de la communication série
  Serial.begin(115200);
  delay(100);

  pixels.begin();
  
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
 
  // Connexion wifi
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);

  // connection  en cours ...
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  // Wifi connecter
  Serial.println("WiFi connecter");
 
  // Démmarrage du serveur.
  server.begin();
  Serial.println("Serveur demarrer !");
 
  // Affichage de l'adresse IP
  Serial.print("Utiliser cette adresse URL pour la connexion :");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
}

void display_color(int r, int g, int b) {
  Serial.println("display_color triggered");
  pixels.clear();

  for(int i=0; i<NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(r, g, b));
    pixels.show();
    delay(DELAYVAL);
  }
}

void loop(){
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected() && currentTime - previousTime <= timeoutTime) {            // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
                   
            // Display the HTML web page
            client.println("<!DOCTYPE html>");
            client.println("<html lang='en'>");
            client.println("<head>");
            client.println("<meta charset='UTF-8'>");
            client.println("<meta http-equiv='X-UA-Compatible' content='IE=edge'>");
            client.println("<meta name='viewport' content='width=device-width, initial-scale=1.0'>");
            client.println("<style>");
            client.println("body, input {");
            client.println("background-color: rgb(107, 169, 180);");
            client.println("}");
            client.println("p {");
            client.println("margin-left: 15px;");
            client.println("text-decoration: underline;");
            client.println("font-style: italic;");
            client.println("font-weight: bold;");
            client.println("color: rgba(0, 0, 0, 0.993);");
            client.println("font-size : 36px;");
            client.println("}");
            client.println("button{");
            client.println("background-color: rgb(240, 182, 234);");
            client.println("padding: 10px;");
            client.println("margin: 20px;");
            client.println("border-radius: 30%;");
            client.println("border: 3px solid rgb(102, 38, 88);");
            client.println("height: 70px;");
            client.println("width: 120px;");
            client.println("}");
            client.println("button:hover {");
            client.println("cursor: pointer;");
            client.println("color: rgb(214, 81, 176);");
            client.println("background-color: rgb(255, 0, 0);");
            client.println("}");
            client.println("a {");
            client.println("color: inherit;");
            client.println("text-decoration: none;");
            client.println("}");
            client.println("input#color1 {");
            client.println("-webkit-appearance: none;");
            client.println("-moz-appearence: none;");
            client.println("appearance: none;");
            client.println("width: 100px;");
            client.println("height: 100px;");
            client.println("background-color: transparent;");
            client.println("border: none;");
            client.println("cursor: pointer;");
            client.println("margin-left: 30px;");
            client.println("}");
            client.println("input#color1::-webkit-color-swatch {");
            client.println("border-radius: 50%;");
            client.println("border: none;");
            client.println("}");
            client.println("</style>");
            client.println("<title>Document</title>");
            client.println("</head>");
            client.println("<body>");
            client.println("<p> Leds Strip </p>");
            client.println("<button><a href='/LED=ON'>ON</a></button><br>");
            client.println("<input id='color1' type='color' value='#FFFFFF' onChange='printColor(event)'><br>");
            client.println("<button><a href='/LED=OFF'>OFF</a></button>");
            client.println("<script> //JavaScript");
            client.println("function printColor(ev) {");
            client.println("const color = ev.target.value");
            client.println("const r = parseInt(color.substr(1,2), 16)");
            client.println("const g = parseInt(color.substr(3,2), 16)");
            client.println("const b = parseInt(color.substr(5,2), 16)");
            client.println("window.location = `?r${r}g${g}b${b}`");
            client.println("}");
            client.println("</script>");
            client.println("<br>");
            client.println("</body>");
            client.println("</html>");

            // The HTTP response ends with another blank line
            client.println();

            // Request sample: /?r201g32b255&
            // Red = 201 | Green = 32 | Blue = 255
            if(header.indexOf("GET /?r") >= 0) {
              pos1 = header.indexOf('r');
              pos2 = header.indexOf('g');
              pos3 = header.indexOf('b');
              pos4 = header.indexOf('&');
              redString = header.substring(pos1+1, pos2);
              greenString = header.substring(pos2+1, pos3);
              blueString = header.substring(pos3+1, pos4);
//              Serial.println(redString.toInt());
//              Serial.println(greenString.toInt());
//              Serial.println(blueString.toInt());
             display_color(
               redString.toInt(),
               greenString.toInt(),
               blueString.toInt()
             );
            }
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
