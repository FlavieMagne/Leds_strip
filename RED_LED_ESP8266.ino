
#include <ESP8266WiFi.h>
 
const char* ssid = "Bbox-3B2C6180";
const char* password = "JaX2hLyt";
 
int ledPin = 16;
WiFiServer server(80);
 
void setup() 
{
  // initialisation de la communication série
  Serial.begin(115200);
  
  delay(100);

  // initialisation de la sortie pour la led 
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
 
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
 
void loop() 
{
WiFiClient client;

  
  // Vérification si le client est connecter.
  client = server.available();
  if (!client)
  {
    return;
  }
 
  // Attendre si le client envoie des données ...
  Serial.println("nouveau client");
  while(!client.available()){
    delay(1);
  }
 
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  int value = LOW;
  if (request.indexOf("/LED=ON") != -1)  {
    digitalWrite(ledPin, HIGH); // allumer la led
    value = HIGH;
  }
  if (request.indexOf("/LED=OFF") != -1)  {
    digitalWrite(ledPin, LOW); // éteindre la led
    value = LOW;
  }
 
  // Réponse
  client.println("<!DOCTYPE html>");
  client.println("<html lang='en'>");
  client.println("<head>");
  client.println("<meta charset='UTF-8'>");
  client.println("<meta http-equiv='X-UA-Compatible' content='IE=edge'>");
  client.println("<meta name='viewport' content='width=device-width, initial-scale=1.0'>");
  client.println("<style>");
  client.println("* {");
  client.println("background-color: rgb(255,207,241);");
  client.println("}");
  client.println("p{");
  client.println("text-decoration: underline;");
  client.println("font-style: italic;");
  client.println("font-weight: bold;");
  client.println("color: rgba(182, 11, 11, 0.993);");
  client.println("font-size : 36px;");
  client.println("}");
  client.println("button{");
  client.println("background-color: rgb(60, 145, 160);");
  client.println("padding: 10px;");
  client.println("margin: 10px;");
  client.println("border-radius: 30%;");
  client.println("border: 3px solid rgb(0, 0, 0);");
  client.println("height: 70px;");
  client.println("width: 120px;");
  client.println("}");
  client.println("button:hover {");
  client.println("cursor: pointer;");
  client.println("color: rgb(214, 81, 176);");
  client.println("background-color: rgb(251, 255, 0);");
  client.println("}");
  client.println("</style>");
  client.println("<title>Document</title>");
  client.println("</head>");
  client.println("<body>");
  if(value==HIGH){
    client.println("<p> État de la led : On<br> </p>");
  } else {
    client.println("<p> État de la led : Off<br> </p>");
  }
  client.println("<button>Allumer<a href='/LED=ON''></a></button>");
  client.println("<button>Éteindre<a href='/LED=OFF''></a></button>");
  client.println("<!-- <a href='/LED=ON''><button>Allumer </button></a>");
  client.println("<a href='/LED=OFF''><button>Eteindre </button></a><br /> -->");
  client.println("</body>");
  client.println("</html>");
 
 
  delay(1);
  Serial.println("Client deconnecter");
  Serial.println("");
 
}
