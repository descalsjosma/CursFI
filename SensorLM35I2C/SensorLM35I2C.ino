#include <LiquidCrystal_I2C.h>

//Crear el objeto lcd  dirección  0x3F y 16 columnas x 2 filas
//LiquidCrystal_I2C lcd(0x3F,16,2);  //
LiquidCrystal_I2C lcd(0x27,16,2);  //

const int sensorPin = A0;   // seleccionar la entrada para el sensor
int Value;         // variable que almacena el valor raw (0 a 1023)
float milivolts, celsius;

void setup() {  
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(13, OUTPUT);
  digitalWrite(3,HIGH);
  digitalWrite(4,HIGH);
  digitalWrite(5,HIGH);
  digitalWrite(6,HIGH);
  digitalWrite(7,HIGH);
  digitalWrite(8,HIGH);
  digitalWrite(9,HIGH);
  digitalWrite(13,HIGH);
  
  // Inicializar el LCD
  lcd.init();
  
  //Encender la luz de fondo.
  lcd.backlight();
  
  // Escribimos el Mensaje en el LCD.
  lcd.print("HOLA MUNDO");
  Serial.println("HOLA MUNDO");
  delay(1000);
}

void loop() {
  
  Value = analogRead(sensorPin);          // realizar la lectura
  milivolts = (Value / 1023.0) * 5000;
  celsius = milivolts / 10; 

  lcd.setCursor(0, 0);
  lcd.print("CFGS: 3R");
  lcd.print(" t:");
  // Escribimos el número de segundos trascurridos
  lcd.print(millis()/1000);
  lcd.print(" s");
  // Ubicamos el cursor en la primera posición(columna:0) de la segunda línea(fila:1)
  lcd.setCursor(0, 1);
  lcd.print("A0: ");
  lcd.print(celsius);
  lcd.print(" ºC");
  
  Serial.println(celsius);                     // mostrar el valor por serial
  delay(1000);
    
}
