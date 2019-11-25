
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


void hardLinkFromMemory (){ // Run the Links so that each pin can be recalled
  for (int i = 0; i < NUM_LINKS; i++)  {
  int GetAdd = ((i+100)*2)-97 ; 
  int sw = EEPROM.read(GetAdd);
  linkArray[i]=sw ;
  //Serial.print(F("Value from Memory:"));
  //Serial.println(sw);
  }
}

void switchOnHarlinkFromStartUp(byte link){
  
 int relayCMND = linkArray[link];         // Check what the Link is
 if (relayCMND > 0){                      // If There is no link (0) to the relay so ignore the command 
  sendRelayCommand (relayCMND-1 ,2);    // Toggle Relay - Number starts at 1 and not at 0
 }

}

/*
void update_sw (int val){
     int i ;
     EEPROM.get(201,i);
     //Serial.print("Custom Link is now: "); 
     //Serial.println(i);  
     customLink = i;      
     //Serial.print("Custom Link is: ");
     //Serial.println(customLink);
}
*/
