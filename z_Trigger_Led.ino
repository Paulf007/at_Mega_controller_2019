//+------------------------------------------------------------------+
//| Flash LED
//+------------------------------------------------------------------+
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
