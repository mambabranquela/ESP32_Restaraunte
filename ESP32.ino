#include <WiFi.h>
#include <PubSubClient.h>
#include <SPI.h>
#include <SD.h>
#include <HardwareSerial.h>

#define SD_CS 5

File myFile;

const int pin_motor = 33;
// Configurações Wi-Fi
const char* ssid = "aaaaaa";
const char* password = "aaaabbbb";

int x = 0;
// Configurações MQTT
const char* mqtt_server = "broker.emqx.io";
const int mqtt_port = 1883;
const char* mqtt_user = "";
const char* mqtt_password = "";

// Tópicos MQTT
const char* subscribe_topic = "/esteiraapi";
const char* publish_topic = "/esteiraapi";

WiFiClient espClient;
PubSubClient client(espClient);

unsigned long lastReconnectAttempt = 0;

void setup_wifi() {
  Serial.println();
  Serial.print("Conectando ao Wi-Fi: ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi conectado!");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Mensagem recebida no tópico: ");
  Serial.println(topic);

  String message = "";

  for (unsigned int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Serial.println("Mensagem: " + message);


  if (message == "ligar") {
    myFile = SD.open("dados.txt", FILE_WRITE);
  // if the file opened okay, write to it:
    
    if (myFile) {
      Serial.println("Writing to file");
      myFile.println("ligado");
      myFile.close(); // close the file:
      Serial.println("completed.");
    } else {
      Serial.println("Erro ao acessar o arquivo");
    }

    digitalWrite(pin_motor, HIGH);
    delay(5000);
    digitalWrite(pin_motor, LOW);
  }

  client.publish(publish_topic, "Mensagem recebida!");
}

bool reconnect() {
  if (!client.connected()) {
    Serial.print("Tentando conectar ao MQTT...");
    if (client.connect("ESP32Client", mqtt_user, mqtt_password)) {
      Serial.println("Conectado!");
      client.subscribe(subscribe_topic);
      Serial.print("Inscrito no tópico: ");
      Serial.println(subscribe_topic);
      return true;
    } else {
      Serial.print("Falha, rc=");
      Serial.print(client.state());
      Serial.println(" Tentando novamente em 5 segundos...");
      return false;
    }
  }
  return true;
}

void setup() {
  Serial.begin(9600);
  pinMode(pin_motor, OUTPUT);
  setup_wifi();

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  // Configura o keepalive para 60 segundos
  client.setKeepAlive(60);

}

void loop() {
   // Verificar e ler dados do Serial2
  if(x == 0)
  {
  if (!SD.begin(SD_CS)) {
    Serial.println("Falha ao inicializar o cartão SD!");
    return;
  } else{
    Serial.println("Cartão SD inicializado com sucesso!");
    x = 1;
  }
  }

  if (!client.connected()) {
    unsigned long now = millis();
    if (now - lastReconnectAttempt > 5000) {
      lastReconnectAttempt = now;
      if (reconnect()) {
        lastReconnectAttempt = 0;
      }
    }
  } else {
    client.loop();
  }
  

  static unsigned long lastMsg = 0;
  unsigned long now = millis();
  if (now - lastMsg > 10000) {
    lastMsg = now;
    const char* msg = "Mensagem do ESP32";
    Serial.print("Publicando: ");
    Serial.println(msg);
    client.publish(publish_topic, msg);
  }
 
}
