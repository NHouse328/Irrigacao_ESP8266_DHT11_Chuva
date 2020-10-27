//Progeto Irrigacao automatica com controle de chuva e humidade

//Sensor de temperatura e umdade DHT11
#include "DHT.h" // Inclui a biblioteca DHT

#define DHTPIN D3 // Define que OUT será conectado em D3
#define DHTTYPE DHT11 // Define o modelo de sensor como DHT 11
DHT dht(DHTPIN, DHTTYPE);


//Relogio

#include <Wire.h> //INCLUSÃO DA BIBLIOTECA
#include "RTClib.h" //INCLUSÃO DA BIBLIOTECA

RTC_DS3231 rtc; //OBJETO DO TIPO RTC_DS3231

//DECLARAÇÃO DOS DIAS DA SEMANA
char daysOfTheWeek[7][12] = {"Domingo", "Segunda", "Terça", "Quarta", "Quinta", "Sexta", "Sábado"};


//Chuva
int pino_d = D0; // Pino ligado ao D0 do sensor
int pino_a = A0; // Pino ligado ao A0 do sensor
int val_d = 0;   // Armazena o valor lido do pino digital
int val_a = 0;   // Armazena o valor lido do pino analógico

//Reles
int R1 = D5;
int R2 = D6;
int R3 = D7;
int R4 = D8;

//Botoes
int Btn1 = D4;

//Tempos de acionamento
int tempo1 = 900000;
int tempo2 = 900000;
int tempo3 = 900000;
int tempo4 = 900000;

//Hora de Acionamento

const int rows  = 1;;
int horaDeLigar[rows][2] = {{10, 30}};

//Outros

//int timeAcionamento = 10;

int tempoJaMolho = 30;//900;
int tempoDeChuva = 0;
boolean chuva = false;
boolean choveu = false;
int diaAnterior = 0;

boolean Rele1 = false;
boolean Rele2 = false;
boolean Rele3 = false;
boolean Rele4 = false;

boolean Auto = true;

void setup() {
  Serial.begin(9600);

  //Relogio

  if(! rtc.begin()) { // SE O RTC NÃO FOR INICIALIZADO, FAZ
    Serial.println("DS3231 não encontrado"); //IMPRIME O TEXTO NO MONITOR SERIAL
    while(1); //SEMPRE ENTRE NO LOOP
  }
  if(rtc.lostPower()){ //SE RTC FOI LIGADO PELA PRIMEIRA VEZ / FICOU SEM ENERGIA / ESGOTOU A BATERIA, FAZ
    Serial.println("DS3231 OK!"); //IMPRIME O TEXTO NO MONITOR SERIAL
    //REMOVA O COMENTÁRIO DE UMA DAS LINHAS ABAIXO PARA INSERIR AS INFORMAÇÕES ATUALIZADAS EM SEU RTC
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); //CAPTURA A DATA E HORA EM QUE O SKETCH É COMPILADO
    //rtc.adjust(DateTime(2018, 9, 29, 15, 00, 45)); //(ANO), (MÊS), (DIA), (HORA), (MINUTOS), (SEGUNDOS)
  }
  
  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); //CAPTURA A DATA E HORA EM QUE O SKETCH É 
  

  //Em teste sempre arruma a Hora
  
  // Sensor de Chuva INPUT
  pinMode(pino_d, INPUT);
  pinMode(pino_a, INPUT);

  //Sensor DHT11
  dht.begin();

  //Reles
  pinMode(R1, OUTPUT);
  pinMode(R2, OUTPUT);
  pinMode(R3, OUTPUT);
  pinMode(R4, OUTPUT);
  digitalWrite(R1, HIGH);
  digitalWrite(R2, HIGH);
  digitalWrite(R3, HIGH);
  digitalWrite(R4, HIGH);

  //Entradas
  pinMode(Btn1, INPUT);
}

void loop() {
  //Serial.println("-----------------------------------------------");
  DateTime now = rtc.now();
  printRelogio(now);
  sensorDht11();
  sensorChuva();
  
  if(chuva){
    Auto = false;
    tempoDeChuva++;
    //Serial.print(" Ta Chovendo ");
    if(tempoDeChuva >= tempoJaMolho){
      //Serial.println("Não vai mais irrigar hoje");
      tempoDeChuva  = 0; 
      choveu        = true;
      diaAnterior   = now.day();
      digitalWrite(R1, HIGH);
      digitalWrite(R2, HIGH);
      digitalWrite(R3, HIGH);
      digitalWrite(R4, HIGH);
    }
    
  }else{
    //Acionamento Manual
    if(digitalRead(Btn1) == 0){
      //Serial.println("Inicio Irrigação Manual");
      digitalWrite(R1, LOW);
      digitalWrite(R2, LOW);
      digitalWrite(R3, LOW);
      digitalWrite(R4, LOW);
      Rele1 = true;
      Rele2 = true;
      Rele3 = true;
      Rele4 = true;
    }else{
      digitalWrite(R1, HIGH);
      digitalWrite(R2, HIGH);
      digitalWrite(R3, HIGH);
      digitalWrite(R4, HIGH);
      Rele1 = false;
      Rele2 = false;
      Rele3 = false;
      Rele4 = false;
      
      if(!choveu){
        irrigacaoAuto();
      }else{
        Auto = false;
        if  ((diaAnterior != now.day() and now.hour() == 12) or 
            (dht.readHumidity() < 50.00 and dht.readTemperature() > 35.00 )){
          //Serial.print(" choveu Desligado ");
          choveu = false;
          tempoDeChuva = 0;
        }
      }
    }
  }
  Serial.print(" ");
  Serial.print(Rele1);
  Serial.print(" ");
  Serial.print(Rele2);
  Serial.print(" ");
  Serial.print(Rele3);
  Serial.print(" ");
  Serial.print(Rele4);
  Serial.print("  ");
  Serial.print(chuva);
  Serial.print(" ");
  Serial.print(tempoDeChuva);
  Serial.print(" ");
  Serial.print(choveu);
  Serial.print(" ");
  Serial.print(Auto);
  Serial.print(" ");
  Serial.print(diaAnterior);
  Serial.print(" ");
  
  Serial.println();
  delay(1000);
}

int irrigacaoAuto(){
  DateTime now = rtc.now();
  Auto = true;

  //Para cada item na lista testar se é igual a agora

  for ( int i = 0; i < rows; ++i ){
    //Serial.print(horaDeLigar[i][0]);
    //Serial.print(":");
    //Serial.println(horaDeLigar[i][1]);
  
    if(now.hour() == horaDeLigar[i][0] and now.minute() == horaDeLigar[i][1]){
      digitalWrite(R1, LOW);
      Rele1 = true;
      //Serial.println("R1 Ligado");
      delay(tempo1); 
      Rele1 = false;                   
      digitalWrite(R1, HIGH); 
      //Serial.println("R1 Desligado"); 
      delay(1000); 
      digitalWrite(R2, LOW);
      Rele2 = true;
      //Serial.println("R2 Ligado");
      delay(tempo2);                      
      digitalWrite(R2, HIGH); 
      Rele2 = false;
      //Serial.println("R2 Desligado");  
      delay(1000);
      digitalWrite(R3, LOW);
      Rele3 = true;
      //Serial.println("R3 Ligado");
      delay(tempo3);                      
      digitalWrite(R3, HIGH);
      Rele3 = false;  
      //Serial.println("R3 Desligado"); 
      delay(1000);
      digitalWrite(R4, LOW);
      Rele4 = true;
      //Serial.println("R4 Ligado");
      delay(tempo4);                      
      digitalWrite(R4, HIGH);
      Rele4 = false; 
      //Serial.println("R4 Desligado");  
    }
  }
}

int sensorChuva(){
  // Armazena os valores de leitura dos sensores nas variáveis
  val_d = digitalRead(pino_d);
  val_a = analogRead(pino_a);
  // Se a leitura analógica for menor que 300 
  if ( val_a < 300) {       // Chuva intensa
    chuva = true;
    //Serial.println("Chuva forte");
  }
  // Se a leitura analógica for menor que 500 e maior que 300 
  if (val_a <= 500 && val_a >= 300) { // Chuva moderada
    chuva = true;
    //Serial.println("Chuva moderada");
  }
  // Se a leitura analógica for maior que 500 
  if ( val_a > 500) {        // Sem previsão de Chuva 
    chuva = false;
    //Serial.println("Sem Chuva");
  }
}

int sensorDht11(){
  float h = dht.readHumidity();     //Le Umidade
  // Indica a temperatura em Celsius (°C)
  float t = dht.readTemperature();  //Le Temperatura
  
//   Escreve os valores de leitura no monitor serial
  Serial.print(" ");
  Serial.print(h);
  Serial.print(" ");
  //Serial.print(" Temperature: ");
  Serial.print(t);
  Serial.print(" "); 
}

int printRelogio(DateTime now){
    //Serial.print("Data: "); //IMPRIME O TEXTO NO MONITOR SERIAL
    Serial.print(now.day(), DEC); //IMPRIME NO MONITOR SERIAL O DIA
    Serial.print('/'); //IMPRIME O CARACTERE NO MONITOR SERIAL
    Serial.print(now.month(), DEC); //IMPRIME NO MONITOR SERIAL O MÊS
    Serial.print('/'); //IMPRIME O CARACTERE NO MONITOR SERIAL
    Serial.print(now.year(), DEC); //IMPRIME NO MONITOR SERIAL O ANO
    Serial.print(" "); //IMPRIME O TEXTO NA SERIAL
    //Serial.print(daysOfTheWeek[now.dayOfTheWeek()]); //IMPRIME NO MONITOR SERIAL O DIA
    //Serial.print("Horas: "); //IMPRIME O TEXTO NA SERIAL
    Serial.print(now.hour(), DEC); //IMPRIME NO MONITOR SERIAL A HORA
    Serial.print(':'); //IMPRIME O CARACTERE NO MONITOR SERIAL
    Serial.print(now.minute(), DEC); //IMPRIME NO MONITOR SERIAL OS MINUTOS
    Serial.print(':'); //IMPRIME O CARACTERE NO MONITOR SERIAL
    Serial.print(now.second(), DEC); //IMPRIME NO MONITOR SERIAL OS SEGUNDOS
    //Serial.print(" "); //QUEBRA DE LINHA NA SERIAL

    //Serial.print("Temperature: ");
    //Serial.print(rtc.getTemperature());
    //Serial.println(" C");
}
