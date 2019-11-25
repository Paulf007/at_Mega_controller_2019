void time_data () {
if ( mqttActive == 1){  
 unsigned long currentMillis = millis(); // grab current time
  // check if "interval" time has passed (1000 milliseconds)
 if ((unsigned long)(currentMillis - previousMillis) >= interval) {
  run_send_data();
 previousMillis = currentMillis;
    }
    
  } 
}

void run_send_data (){
   send_tele_state ();
   publishStates();     // states of the relays will be sent every 5 min - 
  // reportInputStates(); //states of the imput pins will be reported
  // update_sw (201);     // Links to Relays Will be send every 5 min
  
  
}


void sensorData (){
  char sensorTopic[35]="";
  sprintf(sensorTopic ,"%s%s%s",tele,maintopicV,"/SENSOR"); 
  //int chk = DHT.read11(DHT11_PIN);
  //temp = DHT.temperature;
  //hum = DHT.humidity;
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    float hic = dht.computeHeatIndex(t, h, false);
  show_tempR4 (t,h);// to OLED
  int anl = analogRead(photocellPin);
 //Serial.println(anl);
  StaticJsonDocument<90> doc;
//doc["ver"] = ver;
//doc["ip"] = ip2CharArray(Ethernet.localIP());
JsonObject Status  = doc.createNestedObject("DHT11");  
Status["Temprature"] =  t;
Status["Humidity"] =  h;
Status["Heatindex"] =  hic;
//Status["an"] = anl;
JsonObject ANALOG  = doc.createNestedObject("ANALOG");  
ANALOG["A12"] = anl;


  char buffer[90];
  serializeJson(doc, buffer);
 // Serial.println("Sending message to MQTT topic..");
 // serializeJson(doc, Serial);
 // Serial.println();
 size_t n = serializeJson(doc, buffer);

client.publish(sensorTopic, buffer,n);
doc.clear(); 
}

void sendStatusData(byte stat){
 //Serial.print("MQTT Command Recv:");
 //Serial.println(stat);
 
  if (stat == 1){ // 49 is the byte version of 1
  // Serial.println("Case 1 Run");
    StaticJsonDocument<100> doc;
      JsonObject Status  = doc.createNestedObject("Status");  
      Status["Module"] = 1;
      JsonArray  FriendlyName  = Status.createNestedArray("FriendlyName");
      FriendlyName.add(Fname);
       Status["Topic"] = maintopicV;  
      char buffer[80];
      serializeJson(doc, buffer);
      size_t n = serializeJson(doc, buffer);
      char status_0[40]="";
      sprintf(status_0 ,"%s%s%s","stat/",maintopicV,"/STATUS");
      client.publish(status_0, buffer,n);
     //Serial.print("freeMemory()=");
   // Serial.println(freeMemory());
    doc.clear();
  } else if (stat == 5){ // is the byte version of 5
      char status_5[40]="";
      sprintf(status_5 ,"%s%s%s","stat/",maintopicV,"/STATUS5");
      //Serial.println("Case 5 Run");
      StaticJsonDocument<50> doc5;
      JsonObject Status5  = doc5.createNestedObject("StatusNET");  
      Status5["IPAddress"] =ip2CharArray(Ethernet.localIP());
      Status5["Mac"] = macS;
      char buffer5[75];
      serializeJson(doc5, buffer5);
      size_t n5 = serializeJson(doc5, buffer5);
      //Serial.println(status_5);
      client.publish(status_5, buffer5,n5);
    //Serial.print("freeMemory()=");
    

    doc5.clear();
  }
   // Serial.println(stat);
    
  }


void send_tele_state (){
 StaticJsonDocument<30> doc;
   doc["ver"] = ver;
   doc["WebActive"] = webpageActive;
   char buffer[30];
  serializeJson(doc, buffer);
 size_t n = serializeJson(doc, buffer);
//serializeJson(doc, Serial);
client.publish("tele/garage/STATE", buffer,n);
doc.clear();  
sensorData ();
}
