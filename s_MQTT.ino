void subscribe_intopic (){
  client.subscribe(inTopicV);
 
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

// Timer to run and check if Kick Command is Recieved - This will run for 300s 


String val;

void callback(char* topic, byte* payload, unsigned int length) {
/* - use to confirm if MQTT recieve command
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
Serial.println ();
*/
// ------------------- Convert Payload to Int to use later ------------------
  char myNewArray[length+1];
  for (int i=0;i<length;i++) {
    myNewArray[i] = (char)payload[i];
  }
 myNewArray[length] = NULL;
 int command = atoi(myNewArray);  // now an unsigned long
 //Serial.print(F("Payload Command:"));   
 //Serial.println(command);

// ---------- Change Topic Command -------------------   // This will only be called once so Covert to String and write to memory and Restart
if(strcmp(topic, chTopicV) == 0){                 
  Serial.println ("Change Topic Recived:");
    String msgIN = "";
    for (int i=0;i<length;i++)
    { msgIN += (char)payload[i];
      Serial.print((char)payload[i]);
        }
          val = msgIN; // this is the payload from MQTT
          stop_publish = 1 ;
          delay(1000);
         Serial.println("Writning to EEPROM");
         Serial.println(val);
         writeString(55,val);
         EEPROM.write(0, 0x92);
         Serial.println("Writning to EEPROM Done");
         wdt_enable(WDTO_4S); 
         delay(1500);
 }
 // ---------- Status Topic Command -------------------   // This will report the status of the Board - Similar to Sonoff sothat it could be used in a Monitor App
 else if (strcmp(topic, statusCmnd) == 0){  
  //  byte a = atoi((char*)payload);   // There is only a number exspected in this command so atoi can be used. 
    if (command == 1){
    sendStatusData(1);
      }else if (command == 5){
    sendStatusData(5) ; 
     } else if  (command == 9){         // STATUS = 9 Will activate web page
      webpageActive = 1 ;
       webTimeOut.start(300000);
      }else if (command == 10){
        Serial.println("Kick Recieved");
        previousKick = millis();
      }
        }else{ 
 // ----------POWER Topic Command -------------------   // Will come in as POWER# (POWER1 , POWER2)
 
  byte TestTopic = testForPOWERcommand(topic);      // Check if this is a POWER Command
    if(TestTopic == 1){                             // POWER Command Recieved
      if (command < 3){                            // Payload can olny be 0 = OFF , 1 == ON , 2 = Toggle
        char* token = strtok(topic,inPOWERcommand);  // Remove the Topic so that only the number is available char* to int
        //Serial.println(F("Correct POWER Command"));
        int posInArray = atoi(token);
        char* switchState = (char)payload[0];
        sendRelayCommand((posInArray-1) ,command);
      }else{
       // Serial.println(F("Invalid POWER Command"));
      }
    } 
 // ----------LINK Topic Command -------------------    // Custom Link Command Recieved - This will Come in as /# (/100 , /101) number - 100 = Link Command   
      else if (TestTopic == 2){                        
        char* token = strtok(topic,linkTopic1);        // Remove the Link topic so that only the number is available
        int link = atoi(token);
        //stop_publish = 1 ;
          if (link >= 100 && link <= (NUM_LINKS+101) ){   // To make sure that the Data is correct
            if (command <= pinCount) {                    // The Amount if realays Anailable so if the value is more than that I would not work to link it
              int RelayPl = (link*2)-97 ;                 // +2 is to make sure that two places is used fo the int
              write_data(RelayPl,command);
            }
          }
    // No longer need to activate link 200 as the Links is declaired as global viarables.      
    /*      else if (link >= 200 && link <= 221 ){
           //Serial.print("Link Switch Activated:");
           //Serial.println(cmdNr);
                        if (link == 201){ // The Code to activate or deactivate the Auto Switch
                            //cmnd = atol(val.c_str());
                            if (command  == 1 or command  == 0){ // Verify that the correct command was sent. 
                            write_data(201,command);
                            update_sw (201); 
                            sprintf(outLinkR ,"%s%s%s%d",stat,maintopicV,link,201);
                            //cmnd.toCharArray( payLd,2 ); 
                           //Serial.println("Activate and Send MQTT");
                            client.publish(outLinkR,String(cmnd).c_str()); 
                           }  
                      }else if (link == 202) {  // Switch to activate manual overide 
                             //Serial.print("Link Number:");
                             //Serial.println(cmdNr);
                              int i = command ;
                             //Serial.println(i);
                              write_data(202,i);
                              update_sw (202);                    
                        }
          } 

      stop_publish = 0 ; 
      */
    }
  } 
}

// #############################################################################################################


// Connect to MQTT Serer
boolean reconnect() {
 String clientId = "Arduino-";
 clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str(), LWTV , 1, true, "Offline")) {
      Serial.println(F("connected"));
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
