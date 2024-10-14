

#include "dht.h" //INCLUSÃO DE BIBLIOTECA



const int pinoDHT11 = 15; //PINO ANALÓGICO UTILIZADO PELO DHT11


#define pinSensorTrepidacao T8
#define pinSensorChuva 4
#define pinoGasDigital 14

const int MQPIN = 12 ;//sensor MQ-2
const int MQPIN1 = 27;

dht DHT; //VARIÁVEL DO TIPO DHT
#include <arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

#define pinoDigitalUmidadeSolo 5


#define PIN_SENU T9

#define LED_BUILTIN 15
#define PIN_LED 15
#define PIN_LED1 2
#define TOPICO_SUBSCRIBE_LED         "topico_liga_desliga_led"
#define TOPICO_PUBLISH_TEMPERATURA   "topico_sensor_temperatura"
#define TOPICO_PUBLISH_UMIDADE       "topico_sensor_umidade"
#define TOPICO_PUBLISH_UMIDADE_SOLO  "topico_sensor_umidade_solo"
#define TOPICO_PUBLISH_TREPIDACAO    "topico_trepidacao" 
#define TOPICO_PUBLISH_CHUVA         "topico_chuva"
#define TOPICO_PUBLISH_OLA           "Ola_mundo"
#define TOPICO_PUBLICH_GAS           "topico_gas"
#define TOPICO_PUBLISH_GAS_DIGITAL   "topico_gas_digital"

#define ID_MQTT "ProconcepSistemas"



const char* SSID     = "AndroidAP"; 
const char* PASSWORD = "txgu8338";
const char* BROKER_MQTT = "test.mosquitto.org";

int BROKER_PORT = 1883; // Porta do Broker MQTT


WiFiClient espClient; 
PubSubClient MQTT(espClient); 
long numAleatorio;


void initWiFi(void);
void initMQTT(void);
void mqtt_callback(char* topic, byte* payload, unsigned int length);
void reconnectMQTT(void);
void reconnectWiFi(void);
void VerificaConexoesWiFIEMQTT(void);



//double porcentualGas;

//void Gas(){

      //double valorLido = analogRead(MQPIN);

      //int x;

      //x = 100 * (4095-(valorLido) / 4095);

      //porcentualGas = map(valorLido, 0, 1023, 0, 100);

      //porcentualGas = map(valorLido, 0, 4095, 0, 100);
  
      //porcentualGas = (int)x;

      //const int gasValue = analogRead(MQPIN);

      //float porcentualGas;

      //porcentualGas = 100*((4095-(float)gasValue) / 4095);

      //gasValue = (int)porcentualGas;

//}

void initWiFi(void)
{
  delay(10);
  Serial.println("------Conexao WI-FI------");
  Serial.print("Conectando-se na rede: ");
  Serial.println(SSID);
  Serial.println("Aguarde");
  reconnectWiFi();
}

void initMQTT(void)
{
  MQTT.setServer(BROKER_MQTT, BROKER_PORT);   
  MQTT.setCallback(mqtt_callback);            

}

void mqtt_callback(char* topic, byte* payload, unsigned int length)
{
  String msg;
  /* obtem a string do payload recebido */
  for (int i = 0; i < length; i++)
  {
    char c = (char)payload[i];
    msg += c;
  }
  Serial.print("Chegou a seguinte string via MQTT: ");
  Serial.println(msg);
  /* toma ação dependendo da string recebida */
  if (msg.equals("L"))
  {
    digitalWrite(PIN_LED, HIGH);
    Serial.println("LED aceso mediante comando MQTT");
  }
  if (msg.equals("D"))
  {
    digitalWrite(PIN_LED, LOW);
    Serial.println("LED apagado mediante comando MQTT");
  }

  if (msg.equals("A"))
  {
    digitalWrite(PIN_LED1, HIGH);
    Serial.println("LED apagado mediante comando MQTT");
  }
  if (msg.equals("B"))
  {
    digitalWrite(PIN_LED1, LOW);
    Serial.println("LED apagado mediante comando MQTT");
  }
  if (msg.equals("C"))
  {
//    digitalWrite(PIN_LED2, HIGH);
    Serial.println("LED apagado mediante comando MQTT");
  }
  if (msg.equals("E"))
  {
    //digitalWrite(PIN_LED2, LOW);
    Serial.println("LED apagado mediante comando MQTT");
  }
}


void reconnectMQTT(void)
{
  while (!MQTT.connected())
  {
    Serial.print("* Tentando se conectar ao Broker MQTT: ");
    Serial.println(BROKER_MQTT);
    if (MQTT.connect(ID_MQTT))
    {
      Serial.println("Conectado com sucesso ao broker MQTT!");
      MQTT.subscribe(TOPICO_SUBSCRIBE_LED);
    }
    else
    {
      Serial.println("Falha ao reconectar no broker.");
      Serial.println("Havera nova tentatica de conexao em 2s");
      delay(2000);
    }
  }  
}

void VerificaConexoesWiFIEMQTT(void)
{
  if (!MQTT.connected())
    reconnectMQTT(); //se não há conexão com o Broker, a conexão é refeita
  reconnectWiFi(); //se não há conexão com o WiFI, a conexão é refeita
}


void reconnectWiFi(void)
{
  
  if (WiFi.status() == WL_CONNECTED)
    return;
  WiFi.begin(SSID, PASSWORD); 
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(100);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Conectado com sucesso na rede ");
  Serial.print(SSID);
  Serial.println("\nIP obtido: ");
  Serial.println(WiFi.localIP());
}


void setup() {
  Serial.begin(921600); 
  delay(1000);
  Serial.println("ProconceptSistemas");
  delay(1000);
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, LOW); 
  pinMode(PIN_LED1,OUTPUT);
  pinMode(pinoGasDigital, INPUT);
  digitalWrite(PIN_LED1,LOW);
  pinMode(pinoDigitalUmidadeSolo, INPUT);
  pinMode(pinSensorTrepidacao,INPUT);
  pinMode(MQPIN,INPUT);
  analogSetAttenuation(ADC_11db);
  delay(20000);
  
  initWiFi();
  
  initMQTT();
}

void Gas(){
    
    float valor;
    valor = analogRead(MQPIN);
    float porcentualGas = 100*((4095-(float)valor) / 4095);
    //int porcentualGas = map(valor, 0, 4095, 0, 100);
    MQTT.publish("DadosGasMQ_02",String(porcentualGas).c_str());
    delay(1000);

}

void Gas_MQ135(){
    
    float valor;
    valor = analogRead(MQPIN1);
    float porcentualGas = 100*((4095-(float)valor) / 4095);
    //int porcentualGas = map(valor, 0, 4095, 0, 100);
    MQTT.publish("DadosGasMQ_135",String(porcentualGas).c_str());
    delay(1000);

}

void Temperatura(){
  int valor;
  valor = DHT.temperature;
  MQTT.publish("DadosTemperatura", String(valor).c_str());
  delay(2000);
}
     
void loop(){

  char temperatura_str[10] = {0};
  char umidade_str[10] = {0};
  char umidadeSolo_str[10] = {0};
  char umidadeSoloAnalogico_str[10] = {0};
  char trepidacao_str[10] = {0};
  char chuva_str[10] = {0};
  char ola_str[10] = {0};
  char texto[25] = "olá mundo";
  char gas_str[10] = {0};
  char gas_digital_str[10] = {0};

  Gas();
  //Temperatura();
  Gas_MQ135();
  
  DHT.read11(pinoDHT11); //LÊ AS INFORMAÇÕES DO SENSOR
  Serial.println("Umidade: "); //IMPRIME O TEXTO NA SERIAL
  Serial.println(DHT.humidity); //IMPRIME NA SERIAL O VALOR DE UMIDADE MEDIDO
  Serial.println("-----------------------------------------------------------");
  Serial.println("Temperatura: "); //IMPRIME O TEXTO NA SERIAL
  Serial.println(DHT.temperature); //IMPRIME NA SERIAL O VALOR DE UMIDADE MEDIDO E REMOVE A PARTE DECIMAL
  Serial.println("-----------------------------------------------------------");


  Serial.println("Valor do sensor de gás:");
  //Serial.println(porcentualGas);

  delay(2000); //INTERVALO DE 2 SEGUNDOS * NÃO DIMINUIR ESSE VALOR

/* garante funcionamento das conexões WiFi e ao broker MQTT */
  VerificaConexoesWiFIEMQTT();

  //Gas();
  //sprintf(gas_str, "%ld%%", porcentualGas);
  sprintf(ola_str,"%s",texto);
  sprintf(temperatura_str,"%dC", (long)DHT.temperature);
  sprintf(umidade_str, "%dC", (long)DHT.humidity);
  //sprintf(gas_digital_str, "%dC", (long)porcentualGasDigital);
 

  MQTT.publish(TOPICO_PUBLISH_TEMPERATURA,temperatura_str);
  MQTT.publish(TOPICO_PUBLISH_UMIDADE ,umidade_str);  
  MQTT.publish(TOPICO_PUBLICH_GAS,gas_str);
  //MQTT.publish(TOPICO_PUBLISH_GAS_DIGITAL,gas_digital_str);
  MQTT.loop();
  
  delay(2000);
}
   
 
