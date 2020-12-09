# at_Mega_controller_2019
Complete New Updated version

Relay and Input Controller via MQTT

Start up from scratch : 1. Default Ip for config is 192.168.8.0 2. Go to http://192.168.0.2/setup and set the nessasary network settings press submit 3. Your device will restart and connect to your broker. (Red LED will be on permanent if connected) 4. Go to http://”new device ip” to get the config topic – Should add your own custom main topic The default topic on startup is : cmnd//changetopic

The Command topic will be :  cmd//changetopic and 	
Payload 			 :  mainhouse
This needs to change as the commands need identify your custom name – below is an example to the custom topic used “mainhouse” Topics for MQTT Server General info Topic : stat/mainhouse/state Relay Command topic : cmnd/mainhouse/POWER# Where "# = 1 (Relay 1) payload = 1 - ON Where"payload = 0  - OFF Where payload = 2 - Toggle - (cmnd/mainhouse/POWER1) - Full Command

Status of relay is reported on : stat/mainhouse/POWER# Status of Input Pins : stat/mainhouse/SWITCH Last will and testament : stat/mainhouse/LWT Main topic can be changed at : cmnd/atmegarelay/mainhouse/changetopic
NOTE THE MAIN TOPIC MUST BE UNIQUE !

Commands :

Only Switch 0 – 21 can be used to link to relays

To link the Relay with the Switch and save it to the eeprom so that it is used in future - Use Topic : cmnd/atmegarelay/%yourTpopic%/relayLink/100 - where switchpin number + 100 with the payload being the Relay Pin number so to link Switch 0 with Relay 2 the command will be: cmnd/mainhouse/relayLink/100 Payload : 2 Note - the Board will be restarted after each command to ensure that he link is added

To Change the Auto Command from ON or OFF use the following command

cmnd/mainhouse/relayLink/201 Payload : 1 – ON or 2 – OFF

A Watch dog timer was added to make sure the MQTT connection stays active. It will send a payload of 10 to cmnd/~devName~/STATUS this will then recieve the payload and reset the timer. Should the timer run out (Miss two payloads) the board will be restarted.
The kick will happen every 5 min with the status updates. 
