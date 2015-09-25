/*********
 *  =======  ho0 temperature reader =========
 *  using DHT11 & DS18B20
 * :date: 2015-09-25
 * :author: ele1000
 */
#include <OneWire.h>
#include <DallasTemperature.h>
#include <DHT.h>

// Data wire is plugged into pin 10 on the Arduino
const byte pin_DS18B20 =  4;

const byte pin_DHT11 = 2   ;  // what pin we're connected to

#define DHTTYPE DHT11   // DHT 11

DHT my_dht11(pin_DHT11, DHTTYPE);

// Setup a oneWire instance to communicate with any OneWire devices
// (not just Maxim/Dallas temperature ICs)
OneWire ds18b20(pin_DS18B20);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature my_18b20(&ds18b20);

float dht11_hum;
float dht11_tempc;
float dht11_tempf;

float db18b20_temp;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("ho0 temperature reader");
  my_dht11.begin();
  my_18b20.begin();
}

void loop() {

  // Wait a few seconds between measurements.
  delay(2500);

  show_dht11();
  show_18b20();
  report();

}

void show_18b20() {
  Serial.println("__DS18B20::");
  // call my18b20.requestTemperatures() to issue a global temperature
  // request to all devices on the bus
  my_18b20.requestTemperatures(); // Send the command to get temperatures

  db18b20_temp = my_18b20.getTempCByIndex(0);

  Serial.print("Temperature for DS18B20 is: ");
  Serial.print(db18b20_temp); // Why "byIndex"?
  // You can have more than one IC on the same bus.
  // 0 refers to the first IC on the wire
  Serial.println();
}

void show_dht11() {

  Serial.println("__dht11::");
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  dht11_hum = my_dht11.readHumidity();
  // Read temperature as Celsius (the default)
  dht11_tempc = my_dht11.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  dht11_tempf = my_dht11.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(dht11_hum) || isnan(dht11_tempc) || isnan(dht11_tempf)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = my_dht11.computeHeatIndex(dht11_tempf, dht11_hum);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = my_dht11.computeHeatIndex(dht11_tempc, dht11_hum, false);

  Serial.print("Humidity: ");
  Serial.print(dht11_hum);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(dht11_tempc);
  Serial.print(" *C ");
  Serial.print(dht11_tempf);
  Serial.print(" *F\t");
  Serial.print("Heat index: ");
  Serial.print(hic);
  Serial.print(" *C ");
  Serial.print(hif);
  Serial.println(" *F");

}

void report() {
  Serial.println();
  Serial.println("report::");

  // save program storage space
//  String __ = ",";
//  String  report_str = String("$TEMPER")  + __ + db18b20_temp + __ + String(dht11_hum, 2)  + __
//                       + dht11_tempc + __ + dht11_tempf;
//  Serial.println( report_str );

  Serial.print("$TEMPER");
  Serial.print(",");
  Serial.print(db18b20_temp);
  Serial.print(",");
  Serial.print(dht11_hum);
  Serial.print(",");
  Serial.print(dht11_tempc);
  Serial.print(",");
  Serial.print(dht11_tempf);

  Serial.println();

}
