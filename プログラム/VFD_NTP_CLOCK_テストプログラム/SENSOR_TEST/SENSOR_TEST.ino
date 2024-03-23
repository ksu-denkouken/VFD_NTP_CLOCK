#include <DHT20.h>

DHT20 dht = DHT20();

void setup(){
  dht.begin(); //default: 21,22
  Serial.begin(115200);
  while (!Serial); //wait for serial connected.
  delay(1000);
}

void loop(){
  if(millis() - dht.lastRead() >= 1000) {
    if(DHT20_OK == dht.read()){
      Serial.print("Humidity: ");
      Serial.print(dht.getHumidity(), 1);
      Serial.print("  Temerature: ");
      Serial.println(dht.getTemperature(), 1);
    }
  }
}