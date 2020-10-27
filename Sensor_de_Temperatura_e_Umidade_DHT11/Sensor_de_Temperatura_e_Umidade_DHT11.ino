// Projeto Curto Circuito - Estação Meteorológica - DHT22

#include "DHT.h" // Inclui a biblioteca DHT

#define DHTPIN D3 // Define que OUT será conectado em D3

#define DHTTYPE DHT11 // Define o modelo de sensor como DHT 11 
// DHT irá interpretar os valores de leitura do pino de acordo com o tipo de sensor
DHT dht(DHTPIN, DHTTYPE); 
void setup() {
  Serial.begin(9600);    // Taxa de transmissão 9600 dBs 
  Serial.println("DHTxx test!");
  dht.begin();           // Taxa de transmissão do sensor 
}

void loop() {
   delay(2000);          // Intervalo de 2 segundos entre as leituras 
  // Indica a Umidade (%)
  float h = dht.readHumidity();
  // Indica a temperatura em Celsius (°C)
  float t = dht.readTemperature();
 // Escreve os valores de leitura no monitor serial
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.println(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.println(" *C ");  
}
