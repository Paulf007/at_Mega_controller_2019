
void LEDflash(){
 if(flashLED == 1){ 
    unsigned long currentMillis = millis();

  if (currentMillis - LEDpreviousMillis >= LEDinterval) {
    // save the last time you blinked the LED
    LEDpreviousMillis = currentMillis;

    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }
    //Serial.println(flashLED);
    // set the LED with the ledState of the variable:
    digitalWrite(13, ledState);
    }
  }
}



//+------------------------------------------------------------------+
//| Timers
//+------------------------------------------------------------------+

void check_timers (){
  if (webTimeOut.elapsed()){
  webpageActive = 0 ;
 //Serial.print(F("Webpage Disable"));
  flashLED = 0;
 }
}

void delayTimers() {
  delay_400ms.start(50, AsyncDelay::MILLIS);
  delay_30s.start(1, AsyncDelay::MILLIS);
}

void checkKick(){
 if (millis()- previousKick >= kickInterval){  // TimeOut was reached and board needs to be reset - MQTT Meassage was not recieved in time
         Serial.println("No Kick Recieved Restart Board");
         wdt_enable(WDTO_1S);
         previousKick = millis(); 
         //delay(5000);
 }
}



//+------------------------------------------------------------------+
//| Shield Setp
//+------------------------------------------------------------------+
void ShieldSetup(){
  int idcheck = EEPROM.read(0);
   Serial.println("Connect to Network  ....."); 
  if (idcheck != ID){
    //ifcheck id is not the value as const byte ID,
    //it means this sketch has NOT been used to setup the shield before
    //just use the values written in the beginning of the sketch
    Serial.println(F("Never Setup Before"));
    Ethernet.begin(mac, ip);
    printIPAddress();
    flashLED = 1 ;
  }else if (idcheck == ID){
    //if id is the same value as const byte ID,
    //it means this sketch has been used to setup the shield.
    //So we will read the values out of EERPOM and use them
    //to setup the shield.
    Serial.println(F("Setup was done Before"));
    for (int i = 0; i < 6; i++){
      mac[i] = EEPROM.read(i+1);
    }
    for (int i = 0; i < 4; i++){
      ip[i] = EEPROM.read(i+7);
    }
    for (int i = 0; i < 4; i++){
      subnet[i] = EEPROM.read(i+11);
    }
    for (int i = 0; i < 4; i++){
      gateway[i] = EEPROM.read(i+15);
    }
   //#############
    for (int i = 0; i < 4; i++){         
      mqttserver[i] = EEPROM.read(i+19);
    }
    Ethernet.begin(mac, ip);
    printIPAddress();
    flashLED = 1 ;
  }

  

}
