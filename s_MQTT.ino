void subscribe_intopic (){
  client.subscribe(inTopicV);
 // client.subscribe("inTopic/test2");
 // client.subscribe(chTopicV);
 // client.subscribe(linkTopic);
 
  int idcheck = EEPROM.read(0);
     if (idcheck != ID){  } // Check if there is a value
     if (idcheck == ID){
       String recivedData;
       recivedData = read_String(55);
       String intopic = "cmnd/"+recivedData+"loop" ;
       const char* mtopic = intopic.c_str();
       client.subscribe(mtopic);
    }
}


String val;



void callback(char* topic, byte* payload, unsigned int length) {
// - use to confirm if MQTT recieve command
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
Serial.println ();
//
if(strcmp(topic, chTopicV) == 0){
  Serial.println ("Change Topic Recived:");
  String msgIN = "";
for (int i=0;i<length;i++)
{
msgIN += (char)payload[i];
Serial.print((char)payload[i]);
}
val = msgIN; // this is the payload from MQTT
  
  stop_publish = 1 ;
     delay(1000);
    // String value = String((char*)payload);
     Serial.println("Writning to EEPROM");
     Serial.println(val);
     writeString(55,val);
     EEPROM.write(0, 0x92);
     Serial.println("Writning to EEPROM Done");
     wdt_enable(WDTO_4S); 
     delay(1500);
 }else  if (strcmp(topic, statusCmnd) == 0){
  //Serial.print ("Command topic recived:");
  //Serial.println((char)payload[0]);
  byte a = (char)payload[0];
  sendStatusData((char)payload[0]) ; 
  if ((char)payload[0] == '1'){
    sendStatusData('1');
  } else if ((char)payload[0] == '5'){
   sendStatusData('5') ; 
   //Serial.println("Data 5 Sent");
  } else if  ((char)payload[0] == '9'){         // STATUS = 9 Will activate web page
    webpageActive = 1 ;
    webTimeOut.start(300000);
  }
}else{ 
 
byte TestTopic = testForPOWERcommand(topic);
if(TestTopic == 1){
      //Serial.println(F("POWER Command Recived :"));
      char* token = strtok(topic,inPOWERcommand); // Remove the Test topic so that only the number is available char* to int
      int posInArray = atoi(token);
      char* switchState = (char)payload[0];
      sendRelayCommand ((posInArray-1) ,switchState);
  }else if (TestTopic == 2){
    Serial.print("Custom Link Activated:");
    char* token = strtok(topic,linkTopic1); // Remove the Link topic so that only the number is available
    int link = atoi(token);
    Serial.println(link);
    char* payld = payload ;
    int cmdNr = atoi(payld);
    Serial.println(cmdNr);
      stop_publish = 1 ;
          Serial.print("Link :");
          Serial.println(link);
          if (link >= 100 && link <= (NUM_LINKS+101) ){   // To make sure that the Data is correct
            Serial.println("Update EEPROM with relay link :");
            int RelayPl = (link*2)-97 ;// +2 is to make sure that two places is used fo the int
  String msgIN = "";
for (int i=0;i<length;i++)
{
msgIN += (char)payload[i];
Serial.print((char)payload[i]);
}
val = msgIN; // this is the payload from MQTT
int cmdNr1 = val.toInt();
            write_data(RelayPl,cmdNr1);
          }else if (link >= 200 && link <= 221 ){
            Serial.print("Link Switch Activated:");
            Serial.println(cmdNr);
                        if (link == 201){ // The Code to activate or deactivate the Auto Switch
                            //cmnd = atol(val.c_str());
                            if (cmdNr  == 1 or cmdNr  == 0){ // Verify that the correct command was sent. 
                            write_data(201,cmdNr);
                            update_sw (201); 
                            sprintf(outLinkR ,"%s%s%s%d",stat,maintopicV,link,201);
                            //cmnd.toCharArray( payLd,2 ); 
                            Serial.println("Activate and Send MQTT");
                            client.publish(outLinkR,String(cmnd).c_str()); 
                           }  
                      }else if (link == 202) {  // Switch to activate manual overide 
                              Serial.print("Link Number:");
                              Serial.println(cmdNr);
                              int i = cmdNr ;
                              Serial.println(i);
                              write_data(202,i);
                              update_sw (202);                    
                        }
          } 

      stop_publish = 0 ; 
    }
  } 
  
 





  
}

// #############################################################################################################


// Connect to MQTT Serer
boolean reconnect() {
 String clientId = "Arduino-";
 clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str(), LWTV , 1, true, "Offline")) {
      Serial.println("connected");
      client.publish(LWTV,"Online", true);
      mqttActive = 1;
      readLinks ();
      flashLED = 0 ;
      sensorData ();
     // sprintf(mqttStat ,"%s %s","MQTT is:",vOnline); 
     // mqttIpadr ();
      digitalWrite(13, HIGH);
      subscribe_intopic ();   // Call to subscribe to topics
      show_mqtt_status(1);    // Print to OLED
      run_send_data();       // Send all Status Data
      reportInputStates();
      
      //customLink = 1;
      //overide = 0 ;
  }
  return client.connected();
  //sprintf(mqttStat ,"%s %s","MQTT is:",vOffLine); 
}
