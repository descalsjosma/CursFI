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

//Crear el objeto lcd  dirección  0x3F y 16 columnas x 2 filas
//LiquidCrystal_I2C lcd(0x3F,16,2);  //
LiquidCrystal_I2C lcd(0x27,16,2);  //

// Update these with values suitable for your network.
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192,168,8,30);
IPAddress server(192,168,8,6);

//Sensor
const int sensorPin= A0;
char lm35dz;
long now;
long lastMsg =0;
int value = analogRead(sensorPin);
float millivolts = (value / 1023.0) * 5000;
float celsius = millivolts / 10; 
  
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }
  if((String)topic=="juanjo_rele"){
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
      client.publish("juanjo_temp","Proba arduino esta publicant");
      // ... and resubscribe
      client.subscribe("juanjo_rele");
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

  client.setServer(server, 1883);
  client.setCallback(callback);

  Ethernet.begin(mac, ip);
  // Allow the hardware to sort itself out
  delay(1500);
}

void loop()
{
  int Value;         // variable que almacena el valor raw (0 a 1023)
  float milivolts, celsius;
  char charTemperature[] = "00.0";
  
 
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  
  now=millis();
  if ((now-lastMsg)>2000) {
    lastMsg = now;
    
    Value = analogRead(sensorPin);          // realizar la lectura
    milivolts = (Value / 1023.0) * 5000;
    celsius = milivolts / 10;
    //Serial.print(celsius);
    //Serial.println(" C");
    if (client.connect("arduinoClient")) {
      //Serial.println("connected");
     
      dtostrf(celsius, 4, 1, charTemperature);
      client.publish("juanjo_temp", charTemperature);
      //client.publish(String(RAIZ)+"/"+String(DISPOSITIVO)+"/", charTemperature);
    } 
  }

  lcd.setCursor(0, 0);
  lcd.print("CEFI:");
  lcd.print(" t:");
  // Escribimos el número de segundos trascurridos
  lcd.print(millis()/1000);
  lcd.print(" s");
  // Ubicamos el cursor en la primera posición(columna:0) de la segunda línea(fila:1)
  lcd.setCursor(0, 1);
  lcd.print("A0: ");
  lcd.print(celsius);
  lcd.print(" ºC");
  
}
