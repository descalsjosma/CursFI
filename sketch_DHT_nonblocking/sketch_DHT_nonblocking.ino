/*
 * Example of a non-blocking read of DHTxx sensor values.
 *
 * The sensor is connected to the Arduino with a 10 kOhm pull-up
 * resistor connected to the data pin.
 *
 * (C) 2015 Ole Wolf <wolf@blazingangles.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <Ethernet.h>
#include <SPI.h>
#include <dht_nonblocking.h>


// Configuracion del Ethernet Shield
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[] = { 192,168,1,111 }; // Direccion IP del Arduino
byte server[] = { 192,168,1,173 }; // Direccion IP del servidor
EthernetClient client; 
float temperatura;
int analog_pin = 0;

/* Uncomment according to your sensortype. */
#define DHT_SENSOR_TYPE DHT_TYPE_11
//#define DHT_SENSOR_TYPE DHT_TYPE_21
//#define DHT_SENSOR_TYPE DHT_TYPE_22

static const int DHT_SENSOR_PIN = 2;
DHT_nonblocking dht_sensor( DHT_SENSOR_PIN, DHT_SENSOR_TYPE );



/*
 * Initialize the serial port.
 */
void setup( )
{
  Serial.begin( 115200 );
  Ethernet.begin(mac, ip); // Inicializamos el Ethernet Shield
  delay(1000); // Esperamos 1 segundo de cortesia
}



/*
 * Poll for a measurement, keeping the state machine alive.  Returns
 * true if a measurement is available.
 */
static bool measure_environment( float *temperature, float *humidity )
{
  static unsigned long measurement_timestamp = millis( );

  /* Measure once every four seconds. */
  if( millis( ) - measurement_timestamp > 4000ul )
  {
    if( dht_sensor.measure( temperature, humidity ) == true )
    {
      measurement_timestamp = millis( );
      return( true );
    }
  }

  return( false );
}


/*
 * Main program loop.
 */
void loop( )
{
  float temperature;
  float humidity;

  /* Measure temperature and humidity.  If the functions returns
     true, then a measurement is available. */
  //if( measure_environment( &temperature, &humidity ) == true )
  //{
    Serial.print( "T = " );
    Serial.print( temperature, 1 );
    Serial.print( " deg. C, H = " );
    Serial.print( humidity, 1 );
    Serial.println( "%" );
    // Proceso de envio de muestras al servidor
    Serial.println("Connecting...");
    if (client.connect(server, 80)>0) {  // Conexion con el servidor
      client.print("GET /sensores/iot.php?valor="); // Enviamos los datos por GET
      client.print(temperature);
      client.println(" HTTP/1.0");
      client.println("User-Agent: Arduino 1.0");
      client.println();
      Serial.println("Conectado");
    } else {
      Serial.println("Fallo en la conexion");
    }
    if (!client.connected()) {
      Serial.println("Disconnected!");
    }
    client.stop();
    client.flush();
    delay(60000); // Espero un minuto antes de tomar otra muestra
  //}
}
