#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <EEPROM.h>

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;
char data[80];
char message_buff[100];
int lastValue = 0;

// ID
const char* name = "cortina_quarto"; // local do equipamento
const char* service = "WindowCovering"; // tipo do equipamento
const char* service_name = "cortina"; // nome do equipamento
//Set
const char* ssid = "Luiz"; // nome da rede
const char* password = "12345678"; // senha da rede
const char* mqtt_server = "172.20.10.3"; // ip do broker
const char* listenTopic = "homebridge/from/set";
const char* confirmTopic = "homebridge/from/set/confirm";
const char* confirmMsg = "Cortina 01 Conectado";
const char* setTopic = "homebridge/to/set/cortina";
float configDelay = 100; // numero pra definar o quanto tempo o motor vai girar, ex: 100 = 10 posicoes no homekit = 1 segundo

void setup_wifi() {

  delay(10);
  Serial.println();
  Serial.print("Conectando a ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());
  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  char inData[length];
  Serial.print("payload: ");
  
  for(int i =0; i<length; i++){
    inData[i] = (char)payload[i];
  }
  
  Serial.println(inData);
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(inData);
  
  int valueJson = root["value"];
  const char* nomeJson = root["service_name"];
  if (strcmp (service_name,nomeJson) == 0 ) // checagem pra saber se os nomes conferem
  {
  if(valueJson <= lastValue){
    
    value = lastValue - valueJson;
    girarEsquerda(value, valueJson);
    lastValue = valueJson;
  
  }else if(valueJson >= lastValue){
  
    value = valueJson - lastValue;
    girarDireita(value, valueJson);
    lastValue = valueJson;
    
  }

  else
    {
      Serial.print("Erro\n");
    }
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Connectando ao Homekit...");
    String clientId = "ESP8266-";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      Serial.println("Conectado!");
      client.publish(confirmTopic, confirmMsg);
      client.subscribe(listenTopic); // topico que ele vai escutar os comandos
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 2 seconds");
      delay(2000);
    }
  }
}

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     
  Serial.begin(115200);
  setup_wifi();
  pinMode(14, OUTPUT); // Motor horário
  pinMode(12, OUTPUT); // Motor antiHorário
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

}

void girarEsquerda(int value, int target){
  char bufferrCurrent[50];
  char msg[50];
  sprintf(msg, "Girando o motor sentido Horário por aproximadamente: %d segundos",value/10);
  Serial.println(msg);
  sprintf(bufferrCurrent, "%s:%s:%s:PositionState:%d",name,service_name,service, target);
  digitalWrite(14, LOW);
  client.publish(setTopic,bufferrCurrent);
  digitalWrite(12, HIGH);
  char bufferrPosition[50];
  sprintf(bufferrPosition, "%s:%s:%s:CurrentPosition:%d",name,service_name,service, target);
  delay(value*configDelay);
  client.publish(setTopic, bufferrPosition);
  digitalWrite(12, LOW);
}


void girarDireita(int value, int target){
  char bufferrCurrent[50];
  char msg[50];
  sprintf(msg, "Girando o motor sentido Anti-Horário por aproximadamente: %d segundos",value/10);
  Serial.println(msg);
  sprintf(bufferrCurrent, "%s:%s:%s:PositionState:%d",name,service_name,service, target);
  digitalWrite(12, LOW);
  client.publish(setTopic,bufferrCurrent);
  digitalWrite(14, HIGH);
  char bufferrPosition[50];
  sprintf(bufferrPosition, "%s:%s:%s:CurrentPosition:%d",name,service_name,service, target);
  delay(value*configDelay);
  client.publish(setTopic, bufferrPosition);
  digitalWrite(14, LOW);
}

