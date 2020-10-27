// Projeto Curto Circuito - Estação Meteorológica - Sensor de Chuva
//Chuva
int pino_d = D0; // Pino ligado ao D0 do sensor
int pino_a = A0; // Pino ligado ao A0 do sensor
int val_d = 0;   // Armazena o valor lido do pino digital
int val_a = 0;   // Armazena o valor lido do pino analógico
// LED

void setup() {
  Serial.begin(9600);
  // Sensores INPUT
  pinMode(pino_d, INPUT);
  pinMode(pino_a, INPUT);
}

void loop() {
  // Armazena os valores de leitura dos sensores nas variáveis
  val_d = digitalRead(pino_d);
  val_a = analogRead(pino_a);
  Serial.print("Pino A ");
  Serial.println(pino_a);
  Serial.print("Pino D ");
  Serial.println(pino_d);
  // Se a leitura analógica for menor que 300 
  if ( val_a < 300) {       // Chuva intensa
    Serial.println("Chuva forte");
  }
  // Se a leitura analógica for menor que 500 e maior que 300 
  if (val_a <= 500 && val_a >= 300) { // Chuva moderada
    Serial.println("Chuva moderada");
  }
  // Se a leitura analógica for maior que 500 
  if ( val_a > 500) {        // Sem previsão de Chuva 
    Serial.println("Sem Chuva");
  }
}
