int analog_pin = 0;

float temperatura;
float celsius;

void setup () {
  Serial.begin(9600);
}

void loop() {
  temperatura = analogRead(analog_pin);
  temperatura= (temperatura/1024.0) * 5000;
  celsius= (temperatura/10)- 273.15;
  Serial.print(celsius);
  Serial.println(" degrees Celsius");
  delay(1000);
}
