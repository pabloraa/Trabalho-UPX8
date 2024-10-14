#include <ESP8266WiFi.h> 
#include <PubSubClient.h>
#include <WiFiManager.h>

#define pinRele 0  //GP0 ESP-01

//WiFi
WiFiClient wifiClient;                        
 
//MQTT Server
//const char* BROKER_MQTT = "mqtt.eclipseprojects.io"; //URL do broker MQTT que se deseja utilizar
const char* BROKER_MQTT = "test.mosquitto.org";
int BROKER_PORT = 1883;                      // Porta do Broker MQTT

#define ID_MQTT  "ProjEsp01UPX7"             //Informe um ID unico e seu. Caso sejam usados IDs repetidos a ultima conexão irá sobrepor a anterior. 
#define TOPIC_SUBSCRIBE "topico_sensor_umidade"   //Informe um Tópico único. Caso sejam usados tópicos em duplicidade, o último irá eliminar o anterior.
PubSubClient MQTT(wifiClient);        // Instancia o Cliente MQTT passando o objeto espClient

//Declaração das Funções
void mantemConexoes();  //Garante que as conexoes com WiFi e MQTT Broker se mantenham ativas
void conectaWiFi();     //Faz conexão com WiFi
void conectaMQTT();     //Faz conexão com Broker MQTT
void recebePacote(char* topic, byte* payload, unsigned int length);

void setup() {
  pinMode(pinRele, OUTPUT);         

  Serial.begin(115200);

  conectaWiFi();
  MQTT.setServer(BROKER_MQTT, BROKER_PORT);  
  MQTT.setCallback(recebePacote); 
}

void loop() {
  mantemConexoes();
  MQTT.loop();
}

void mantemConexoes() {
    if (!MQTT.connected()) {
       conectaMQTT(); 
    }
    
    conectaWiFi(); //se não há conexão com o WiFI, a conexão é refeita
}

void conectaWiFi() {

  Serial.begin(115200);
    
    
    WiFiManager wm;

    wm.setConfigPortalTimeout(60);
    bool res;

    res = wm.autoConnect("ESP01","12345678");

    if(!res) {
        Serial.println("Failed to connect");
        //ESP.restart();
    } 
    else {
  
        Serial.println("connected...yeey :)");
    }
}

void conectaMQTT() { 
    while (!MQTT.connected()) {
        Serial.print("Conectando ao Broker MQTT: ");
        Serial.println(BROKER_MQTT);
        if (MQTT.connect(ID_MQTT)) {
            Serial.println("Conectado ao Broker com sucesso!");
            MQTT.subscribe(TOPIC_SUBSCRIBE);
        } 
        else {
            Serial.println("Noo foi possivel se conectar ao broker.");
            Serial.println("Nova tentatica de conexao em 10s");
            delay(10000);
        }
    }
}

void recebePacote(char* topic, byte* payload, unsigned int length) 
{
    String msg;

    //obtem a string do payload recebido
    for(int i = 0; i < length; i++) 
    {
       char c = (char)payload[i];
       msg += c;
    }

    if (msg == "0") {
       digitalWrite(pinRele, LOW);
    }

    if (msg == "1") {
       digitalWrite(pinRele, HIGH);
    }
}
