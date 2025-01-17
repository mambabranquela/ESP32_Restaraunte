#include <Wire.h> //protocolo de cominicação para disposistivos de baixa velocidade 
#include <NewPing.h> //sensor ultrasonico 
#include <LowPower.h> //gotoslepp - "desligar"
#include <MFRC522.h>

#define trigger_pin 7
#define echo_pin 8 //ultra sonico 

#define max_dist 20
#define distance_good 7 //uso para o sensor 

#define pin_motor 6
#define pwm_value 200 // em RPM 

#define button_ligar_pin A0 

#define SS_PIN 10 //PINO SDA
#define RST_PIN 9 //PINO DE RESET

#define pino_ESP_Esteira 5
#define pino_ESP_Sinal 2
#define pino_ESP_Pedido 3

MFRC522 rfid(SS_PIN, RST_PIN);

NewPing sonar(trigger_pin, echo_pin, max_dist); // configura o sensor ultrassônico


// inicialização de variáveis
bool flag = 0;
volatile bool sleepFlag = false;
int estadoBotaoLigado = 0;
int ESP_Esteira = 0;
int pinoEsp = 0;
int esteraCount = 0;
int LigarEsteira = 0;
int ESP_Pedido = 0;
int quantidadePedido = 0;


void setup() {
  Serial.begin(9600);
  espSerial.begin(9600);    // Comunicação com o ESP32

  SPI.begin(); //INICIALIZA O BARRAMENTO SPI
  rfid.PCD_Init(); //INICIALIZA MFRC522

  // Configuração dos pinos
  pinMode(trigger_pin, OUTPUT);
  pinMode(echo_pin, INPUT);
  pinMode(pin_motor, OUTPUT);
  pinMode(button_ligar_pin, INPUT_PULLUP);
  pinMode(pino_ESP_Esteira, INPUT);
  pinMode(pino_ESP_Sinal, OUTPUT);
  pinMode(pino_ESP_Pedido, INPUT);

  SPI.begin(); //INICIALIZA O BARRAMENTO SPI
  rfid.PCD_Init(); //INICIALIZA MFRC522
}

void loop() {

  while(LigarEsteira == 0){
    estadoBotaoLigado = digitalRead(button_ligar_pin);
    ESP_Esteira = digitalRead(pino_ESP_Esteira);

    Serial.println("Esteira Desligada");
    delay(500);
    
    if(estadoBotaoLigado == LOW  ||  ESP_Esteira == HIGH){
      LigarEsteira = 1;
  }
  }

  digitalWrite(pin_motor, HIGH); 
  

  Serial.println("Iniciando o Sitema");
  int flag = 0;

  int sleepFlag = 0;

  ESP_Esteira = digitalRead(pino_ESP_Pedido);
  if(ESP_Pedido == HIGH){

    quantidadePedido = 1;

  }

  /*estadoBotaoLigado = digitalRead(button_ligar_pin);
    if(estadoBotaoLigado == LOW){
      Serial.println("Botao Precionado 1");
      sleepFlag = 1;
      digitalWrite(pin_motor, LOW);
      delay(2000);
      while (sleepFlag == 1)
      {
        estadoBotaoLigado = digitalRead(button_ligar_pin);
        if(estadoBotaoLigado == LOW)
        {
          Serial.println("Botao Precionado 2");
          sleepFlag = 0;
          delay(2000);
        }
      }
      
    }*/



    
    digitalWrite(pin_motor, HIGH); 
    estadoBotaoLigado = digitalRead(button_ligar_pin);
    if(estadoBotaoLigado == LOW)
    {
      quantidadePedido = 1;
      delay(500);
    }

    

 
  if (esteraCount == 1){
    
    while(esteraCount == 1){
    int distancia = sonar.convert_cm(sonar.ping_median(10));
    Serial.println(distancia);
    if (distancia <= distance_good && distancia != 7) { 
        digitalWrite(pin_motor, LOW);
        // desliga o motor
        flag = 1;

        while (flag == 1)
      {

        distancia = sonar.convert_cm(sonar.ping_median(10));
        if (distancia == 7) {
          flag = 0;
          esteraCount = 0;
          quantidadePedido = 0;
          delay(3000);
        }
        Serial.println(distancia);
      }
    }
    }

      
  }

    estadoBotaoLigado = digitalRead(button_ligar_pin);
    if(estadoBotaoLigado == LOW){
      Serial.println("Botao Precionado 1");
      sleepFlag = 1;
      digitalWrite(pin_motor, LOW);
      delay(2000);
      while (sleepFlag == 1)
      {
        estadoBotaoLigado = digitalRead(button_ligar_pin);
        if(estadoBotaoLigado == LOW)
        {
          Serial.println("Botao Precionado 2");
          sleepFlag = 0;
          delay(2000);
        }
      }
      
    }



    
    digitalWrite(pin_motor, HIGH); 


  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()){
  Serial.println("Teste 1");
  return;}

  Serial.println("Teste 2");

    String strID = "";
  for (byte i = 0; i < 4; i++) {
    strID +=
    (rfid.uid.uidByte[i] < 0x10 ? "0" : "") +
    String(rfid.uid.uidByte[i], HEX) +
    (i!=3 ? ":" : "");
  }
  strID.toUpperCase();
  /FIM DO BLOCO DE CÓDIGO RESPONSÁVEL POR GERAR A TAG RFID LIDA/
 
  Serial.print("Identificador (UID) da tag: "); //IMPRIME O TEXTO NA SERIAL
  Serial.println(strID); //IMPRIME NA SERIAL O UID DA TAG RFID

  rfid.PICC_HaltA(); //PARADA DA LEITURA DO CARTÃO
  rfid.PCD_StopCrypto1();
  esteraCount = 1;
  

  
  
}
