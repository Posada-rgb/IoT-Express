#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include "DHT.h"
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN,DHTTYPE);


int contconexion = 0;
int sensor = analogRead(A0);



const char *ssid = "Yanos Connect_2842";
const char *password = "81641062";

unsigned long previousMillis = 0;

char host[48];
String strhost = "localhost:3000"; //IP del Server
String strurl = "/add"; //URL
String chipid = "";


String enviardatos(String datos) {
  Serial.print(datos);
  String linea = "error";
  WiFiClient client;
  strhost.toCharArray(host, 49);
  if (!client.connect(host, 3000)) {
    Serial.println("Fallo de conexion al servidor");
    return linea;
  }

  client.print(String("POST ") + strurl + " HTTP/1.1" + "\r\n" +
               "Host: " + strhost + "\r\n" +
               "Accept: */*" + "*\r\n" +
               "Content-Length: " + datos.length() + "\r\n" +
               "Content-Type: application/x-www-form-urlencoded" + "\r\n" +
               "\r\n" + datos);
  delay(10);

  Serial.print("Enviando datos a MongoDB...");

  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println("No conectado con el cliente");
      client.stop();
      return linea;
    }
  }
  // imprime las lineas que recibe del servidor
  while (client.available()) {
    linea = client.readStringUntil('\r');
  }
  Serial.println(linea);
  return linea;
}


void setup() {
  Serial.begin(115200);//Inicia la comunicación serial
  Serial.println("");
  dht.begin();
  delay(500);

  //Serial.println("valor del sensor de humedad");
  //delay(1000);
  

  Serial.print("chipId: ");
  chipid = String(ESP.getChipId());
  Serial.println(chipid);

  // Conexión WIFI
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED and contconexion < 50) { //50 intentos para conectar  si no lo cancela
    ++contconexion;
    delay(500);
    Serial.print(".");
  }
  if (contconexion < 50) {
    //para usar con ip fija
    // IPAddress ip(192, 168, 43, 5);
    // IPAddress gateway(192, 168, 43, 1);
    // IPAddress subnet(255, 255, 255, 0);
    // WiFi.config(ip, gateway, subnet);

    Serial.println("");
    Serial.println("WiFi conectado con la IP ");
    Serial.println(WiFi.localIP());

  }
  else {
    Serial.println("");
    Serial.println("Error de conexion al wifi");
  }
}

void loop() {

  // Iniciamos el envio de ondas
  float Humedad = dht.readHumidity();
  float Temperatura = dht.readTemperature();
  float f = dht.readTemperature(true);
  delay(1000);

  Serial.print("valor humedad");
  Serial.println(Suelo);
  delay(1000);
  

  //Serial.print("Lectura: ");
 // Serial.println(Suelo);
  
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= 10000) { //envia la temperatura cada 10 segundos
    previousMillis = currentMillis;


    enviardatos("&Temperatura=" + String(Temperatura) + "&Humedad=" + String(Humedad + "&Luz="));
  }  
}
