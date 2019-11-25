void write_data (int addr , int info ){ // update the custom pin link
    Serial.print(F("Data Receved to Write in ROM:"));
    Serial.println(info); 
    if (info <= pinCount){              // Test to make sure that the Correct value is placed
      EEPROM.write(addr,info);
      //readLinks ();
     wdt_enable(WDTO_2S); 
     delay(1500);
     }
     

   }

void readEEOROMip (){
  valuerom = EEPROM.read(address);
  Serial.print(address);
  Serial.print("\t");
  Serial.print(valuerom, DEC);
  Serial.println();
  address = address + 1;
  if (address == EEPROM.length()) {
    address = 0;
  }
  delay(500);
 }
// Get the maintpoic value from the EEOROM and set for later use
 void readEEOROM (char add){
  Serial.print(F("Read Main Topic"));
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
  //return String(data);
  strcpy(maintopicV, data);
  //const char* emaintopic = data;
  Serial.print(F("Main Topic is:"));
  Serial.println(data);
  mt_len = strlen(data); // get the lenth of the main topic so that it can be used in the MQTT recieved payload
  // Set the variable funtions
  setVariableTopics();
  }
