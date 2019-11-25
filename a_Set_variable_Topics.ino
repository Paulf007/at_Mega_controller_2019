void setVariableTopics() {
//  This function run once to set all the variuable topics for the after the main topic was set 
 
//sprintf(outTopicV ,"%s%s%s",stat,maintopicV,"/state");
sprintf(inTopicV ,"%s%s%s",cmd,maintopicV,Powera);          // Relay Command Topic 
sprintf(inPOWERcommand  ,"%s%s%s",cmd,maintopicV,Power);    // Used to compare the Power command to remove char
sprintf(outRelayTopicV ,"%s%s%s",stat,maintopicV,Power);    // Used to report the Status of the relay
sprintf(outPinTopicV ,"%s%s%s",stat,maintopicV,Switch);     // Report the Status of the input Pins
sprintf(LWTV ,"%s%s%s","tele/",maintopicV,"/LWT");          // Last Will and testament 
sprintf(chTopicV ,"%s%s%s",cmd,maintopicV,cnhgeT);          // Change Topic
sprintf(linkTopic ,"%s%s%s",cmd,maintopicV,linkT);          // relaylink/# - Link Switch with relay
sprintf(linkTopic1 ,"%s%s%s",cmd,maintopicV,relaylink);     // Link topic to compare 
//sprintf(outLinkRw ,"%s%s%s%s",stat,maintopicV,link,"#");  // This is used for reporting in the Webpage only is now located at the page ;
//sprintf(recState ,"%s%s%s",tele,maintopicV,state);
//sprintf(status_5 ,"%s%s%s",stat,maintopicV,"/STATUS5");     // Status5 Topic
//sprintf(status_0 ,"%s%s%s",stat,maintopicV,"/STATUS");
sprintf(statusCmnd ,"%s%s%s",cmd,maintopicV,"/STATUS");       // Used for comparing incomming strings
//sprintf(sensorTopic ,"%s%s%s",tele,maintopicV,"/SENSOR");
//Serial.print("Variable Topic Set:");
//Serial.println(outLinkRw);
//Serial.println(recState);
 
  
}
