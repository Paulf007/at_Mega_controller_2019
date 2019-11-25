// This code will use MQTT to set the main topic for the skecth

byte testForPOWERcommand (char* topic){
  // Test if it is a POWER command 
char* test = strstr(topic,inPOWERcommand);
byte testLenth = (strlen(test));
//Serial.println(testLenth);
char* testLinkTopic = strstr(topic,linkTopic1);
//Serial.println(testLinkTopic);
byte testLinkTopicln = (strlen(testLinkTopic));
if(testLenth > 1){
  return 1;
  }else if (testLinkTopicln> 1){
  return 2;  
  }else{
   return 0; 
  }
  
}

void sendRelayCommand (byte posInArray ,int switchState ){
 // Serial.print(F("Relay Command"));
 // Serial.print(switchState);
  if (switchState == 2) {
    if (relayStates[posInArray] == HIGH) {
      switchState = '1';
    }else {
      switchState = '0';
      }
        }else if (switchState == 1){
          switchState = '1';
          }else{
            switchState = '0';
            } 
 // if (isMutuallyExclude) {
 //   mutuallyExcludePair(switchState, posInArray);
 // }
  switchRelay(switchState, posInArray);
//  if (switchState == '1') {
//   doSwitch = true;
//  }
  delayTimers();
}


void writeString(char add,String data){
  int _size = data.length();
  int i;
  for(i=0;i<_size;i++)
  {
    EEPROM.write(add+i,data[i]);
  }
  EEPROM.write(add+_size,'\0');   //Add termination null character for String Data
}

//byte valuerom;
String read_String(char add){
  int i;
  char data[200]; //Max 100 Bytes
  int len=0;
  unsigned char k;
  k=EEPROM.read(add);
  while(k != '\0' && len<1000)   //Read until null character
  {    
    k=EEPROM.read(add+len);
    data[len]=k;
    len++;
  }
  data[len]='\0';
  return String(data);
}
