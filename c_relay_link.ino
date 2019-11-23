
// Read all the input pins and check if they are linked to any relays:
void readLinks (){
//int i;
    for (int i = 0; i < 32; i++)      //declaring analog inputs
  {
  //int s = buttPins[i];
  int GetAdd = ((i+100)*2)-97 ; 
  int sw = EEPROM.read(GetAdd);
    StaticJsonDocument<30> doc;
    doc["Relay"] =sw;
    char buffer[30];
    serializeJson(doc, buffer);
    size_t n = serializeJson(doc, buffer);
    //serializeJson(doc, Serial);
    sprintf(outLinkR ,"%s%s%s%d",stat,maintopicV,link,i);
    client.publish(outLinkR, buffer,n);
    doc.clear();  
  
  }
}





void int_link(int addr){
  int switchState;
  // first check if int link as active or not.
  Serial.print("Custom link is read from set is :");
  Serial.println(customLink); 
  if (customLink == 1){
  int relay;
  int GetAdd = ((addr+100)*2)-97 ; // this is how the pin was saved in the memory so that two places was added for each link.  
  //Serial.print("Adress to search:");
  //Serial.println(addr);
  // read the pin in the memory
  // Serial.print("Get Relay Pin Info from EEPROM");
  EEPROM.get(GetAdd,relay);
  int Relaycheck = EEPROM.read(0);
  if (relay!= 0){
  //Serial.print("Link in Mem Pin:");
  //Serial.println(addr);
  //Serial.print("Relay Pin:");
  //Serial.println(relay);
  //check_relay(relay);
  switchState = check_relay(relay-1);
  switchRelay(switchState,relay-1);
    }else{
    //Serial.print("No Link for Pin:"); 
    //Serial.println(addr);     
    }
  }
}




void update_sw (int val){
     int i ;
     EEPROM.get(201,i);
     Serial.print("Custom Link is now: "); 
     Serial.println(i);  
     customLink = i;      
     Serial.print("Custom Link is: ");
     Serial.println(customLink);
     
}
