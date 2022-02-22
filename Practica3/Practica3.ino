/*
 Basic MQTT example

 This sketch demonstrates the basic capabilities of the library.
 It connects to an MQTT server then:
  - publishes "hello world" to the topic "outTopic"
  - subscribes to the topic "inTopic", printing out any messages
    it receives. NB - it assumes the received payloads are strings not binary

 It will reconnect to the server if the connection is lost using a blocking
 reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
 achieve the same result without blocking the main loop.
 
*/

#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>                
#include <DallasTemperature.h>

#define DISPOSITIVO "tossal" //Dispositivo que identifica al publicar en MQTT
#define RAIZ "cefi/proj1"  //raiz de la ruta donde va a publicar
 
OneWire ourWire1(2);                //Se establece el pin 2  como bus OneWire
OneWire ourWire2(3);                //Se establece el pin 2  como bus OneWire
OneWire ourWire3(9);                //Se establece el pin 2  como bus OneWire
DallasTemperature sensors1(&ourWire1); //Se declara una variable u objeto para nuestro sensor
DallasTemperature sensors2(&ourWire2); //Se declara una variable u objeto para nuestro sensor
DallasTemperature sensors3(&ourWire3); //Se declara una variable u objeto para nuestro sensor

//Crear el objeto lcd  dirección  0x3F y 16 columnas x 2 filas
//LiquidCrystal_I2C lcd(0x3F,16,2);  //
LiquidCrystal_I2C lcd(0x27,16,2);  //

// Update these with values suitable for your network.
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192,168,8,30);
IPAddress server(192,168,8,6);

//Sensor
long now;
long lastMsg =0;

//Topics
String topic_root =  String(RAIZ) + "/" + String(DISPOSITIVO);
String publish_temperatura1_string = topic_root + "/temperatura1";
const char* publish_temperatura1 = publish_temperatura1_string.c_str();
String publish_temperatura2_string = topic_root + "/temperatura2";
const char* publish_temperatura2 = publish_temperatura2_string.c_str();
String publish_temperatura3_string = topic_root + "/temperatura3";
const char* publish_temperatura3 = publish_temperatura3_string.c_str();
String subs_rele_string = topic_root + "/led";
const char* subs_rele = subs_rele_string.c_str();
  
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }
  if((String)topic==subs_rele){
    //if(payload[0]==1){
    //  digitalWrite(4, HIGH);
    //}else{
      digitalWrite(4, LOW);
    //}
  }
}

EthernetClient ethClient;
PubSubClient client(ethClient);

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("arduinoClient")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(publish_temperatura1,"Proba arduino esta publicant");
      client.publish(publish_temperatura2,"Proba arduino esta publicant");
      client.publish(publish_temperatura3,"Proba arduino esta publicant");
      // ... and resubscribe
      client.subscribe(subs_rele);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}





void setup()
{
  Serial.begin(57600);

  // Inicializar el LCD
  lcd.init();
  
  //Encender la luz de fondo.
  lcd.backlight();
  
  // Escribimos el Mensaje en el LCD.
  lcd.print("HOLA MUNDO");

  sensors1.begin();   //Se inicia el sensor 1
  sensors2.begin();   //Se inicia el sensor 2
  sensors3.begin();   //Se inicia el sensor 2

  client.setServer(server, 1883);
  client.setCallback(callback);

  Ethernet.begin(mac, ip);
  // Allow the hardware to sort itself out
  delay(1500);
}

void loop()
{
  float temp1,temp2,temp3;
  char charTemperature1[] = "00.0";
  char charTemperature2[] = "00.0";
  char charTemperature3[] = "00.0";
  

  now=millis();
  if ((now-lastMsg)>2000) {
    lastMsg = now;

    sensors1.requestTemperatures();   //Se envía el comando para leer la temperatura
    temp1= sensors1.getTempCByIndex(0); //Se obtiene la temperatura en ºC
    sensors2.requestTemperatures();   //Se envía el comando para leer la temperatura
    temp2= sensors1.getTempCByIndex(0); //Se obtiene la temperatura en ºC
    sensors3.requestTemperatures();   //Se envía el comando para leer la temperatura
    temp3= sensors1.getTempCByIndex(0); //Se obtiene la temperatura en ºC

    if (client.connect("arduinoClient")) {
      Serial.println("connected");
      dtostrf(temp1, 4, 1, charTemperature1);
      client.publish(publish_temperatura1, charTemperature1);
      client.publish(publish_temperatura2, charTemperature2);
      client.publish(publish_temperatura3, charTemperature3);
    } 
    // Ubicamos el cursor en la primera posición(columna:0) de la segunda línea(fila:1)
    lcd.setCursor(0, 1);
    lcd.print("T");
    lcd.print(temp1);
    lcd.print(" ");
    lcd.print(temp2);
    lcd.print(" ");
    lcd.print(temp3);
    lcd.print(" ");
  }

  lcd.setCursor(0, 0);
  lcd.print("CEFI:");
  lcd.print(" t:");
  // Escribimos el número de segundos trascurridos
  lcd.print(millis()/1000);
  lcd.print(" s");

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  
}
