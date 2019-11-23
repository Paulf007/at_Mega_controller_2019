void time_data () {
if (stop_publish == 0){  
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
   update_sw (201);     // Links to Relays Will be send every 5 min
  
  
}


void sensorData (){
  //int chk = DHT.read11(DHT11_PIN);
  //temp = DHT.temperature;
  //hum = DHT.humidity;
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    float hic = dht.computeHeatIndex(t, h, false);
  show_tempR4 (t,h);// to OLED
  int anl = analogRead(photocellPin);
  Serial.println(anl);
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
 
  if (stat == 49){ // 49 is the byte version of 1
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
      client.publish(status_0, buffer,n);
     //Serial.print("freeMemory()=");
   // Serial.println(freeMemory());
    doc.clear();

  } else if (stat == 84){ // is the byte version of 5
      //Serial.println("Case 5 Run");
      StaticJsonDocument<50> doc5;
      JsonObject Status5  = doc5.createNestedObject("StatusNET");  
      Status5["IPAddress"] =ip2CharArray(Ethernet.localIP());
      Status5["Mac"] = macS;
      char buffer5[75];
      serializeJson(doc5, buffer5);
      size_t n5 = serializeJson(doc5, buffer5);
      client.publish(status_5, buffer5,n5);
    //Serial.print("freeMemory()=");
    //Serial.println(freeMemory());

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


/*
void dht11 () {
 
  int chk = DHT.read11(DHT11_PIN);
  temp = DHT.temperature;
  hum = DHT.humidity;
  show_tempR4 (temp,hum);// to OLED
  int anl = analog ();
  //Serial.print("Analog = ");
  //Serial.println(anl);
 //Serial.print("Humidity = ");
 //Serial.println(DHT.humidity);
 // Serial.println (hum);
       //convert ip Array into String
  ipmq = String (Ethernet.localIP()[0]);
  ipmq = ipmq + ".";
  ipmq = ipmq + String (Ethernet.localIP()[1]);
  ipmq = ipmq + ".";
  ipmq = ipmq + String (Ethernet.localIP()[2]);
  ipmq = ipmq + ".";
  ipmq = ipmq + String (Ethernet.localIP()[3]);
  //Serial.println(ip);
 // client.publish(outTopic, ipmq.c_str());
 

  String  payload1 = "{";
          payload1 += "\"Ver\":"; payload1 += ver; payload1 += ",";
          payload1 += "\"ip\":\""; payload1 += ipmq.c_str();  payload1 += "\",";
          payload1 += "\"Overide\":"; payload1 += customLink; payload1 += ",";
          payload1 += "\"Anl\":"; payload1 += anl; payload1 += ",";
          payload1 += "\"DHT11\":{"; 
          payload1 += "\"Temp\":"; 
         // payload1 += DHT.temperature; 
          payload1 += ",";
          //payload1 += "\"P_m\":"; payload1 += temp_m; payload1 += ",";
          //payload1 += "\"P_a\":"; payload1 += temp_a; payload1 += ",";
          //payload1 += "\"ip\":"; payload1 += "\"192.168.1.11\""; payload1 += ",";
         // payload1 += "\"Hum\":"; payload1 += DHT.humidity;
          payload1 += "}";
          payload1 += "}";
  char payLd[100] ;
  payload1.toCharArray( payLd, 100 );         
  client.publish(outTopicV,payLd);
 // Serial.println ();
 //Serial.print (F("MQTT Data Sent"));
 // Serial.println(freeMemory());
  previousMillis = millis();
//  }
// }  
}
// Needs to be activated in setup
int analog (){
  //if (analogSw == 1){
     long sum = 0;
    for(int i=0; i<100; i++)
    {
       sum += analogRead(photocellPin);
    }
 
    sum = sum/100;
    return sum ;
    Serial.println(sum);
    //delay(10);
//  }
}
*/
