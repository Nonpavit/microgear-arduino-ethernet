# microgear-arduino-ethernet

microgear-arduino-ethernet is a client library that is used to connect an Arduino board to the NETPIE Platform's service for developing IoT applications. For more details on the NETPIE Platform, please visit https://netpie.io . 

## Compatibility
This library can be used with Arduino Mega 2560 and  Ethernet Shield

**Usage Example**
```c++
#include <Ethernet.h>
#include <MicroGear.h>

#define APPID   <APPID>
#define KEY     <APPKEY>
#define SECRET  <APPSECRET>
#define ALIAS   "anything"

EthernetClient client;
AuthClient *authclient;

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
MicroGear microgear(client);
int timer = 0;

void onMsghandler(char *topic, uint8_t* msg, unsigned int msglen) {
  Serial.print("Incoming message --> ");
  msg[msglen] = '\0';
  Serial.println((char *)msg);
}

void onFoundgear(char *attribute, uint8_t* msg, unsigned int msglen) {
  Serial.print("Found new member --> ");
  for (int i=0; i<msglen; i++)
    Serial.print((char)msg[i]);
  Serial.println();  
}

void onLostgear(char *attribute, uint8_t* msg, unsigned int msglen) {
  Serial.print("Lost member --> ");
  for (int i=0; i<msglen; i++)
    Serial.print((char)msg[i]);
  Serial.println();
}

void onConnected(char *attribute, uint8_t* msg, unsigned int msglen) {
  Serial.println("Connected to NETPIE...");
  microgear.setAlias(ALIAS);
}

void setup() {  
    Serial.begin(9600);
    Serial.println("Starting...");

    microgear.on(MESSAGE,onMsghandler);
    microgear.on(PRESENT,onFoundgear);
    microgear.on(ABSENT,onLostgear);
    microgear.on(CONNECTED,onConnected);

    if (Ethernet.begin(mac)) {
      Serial.println(Ethernet.localIP());
      microgear.init(KEY,SECRET,ALIAS);
      microgear.connect(APPID);
    }
}

void loop() {
	if (microgear.connected()) {
		Serial.println("connected");
		microgear.loop();
		if (timer >= 1000) {
  			microgear.chat(ALIAS,"Hello");
	    	timer = 0;
	    } 
    	else timer += 100;
	}
	else {
	    Serial.println("connection lost, reconnect...");
    	if (timer >= 5000) {
          microgear.connect(APPID);
			timer = 0;
		}
		else timer += 100;
	}
	delay(100);
}

```
## Library Usage
---
To initial a microgear use one of these methods :

**int MicroGear::init(char* *key*, char* *secret* [,char* *alias*])**

**arguments**
* *key* - is used as a microgear identity.
* *secret* - comes in a pair with gearkey. The secret is used for authentication and integrity.
* *alias* - specifies the device alias (optional).  

```c++
microgear.init("sXfqDcXHzbFXiLk", "DNonzg2ivwS8ceksykGntrfQjxbL98", "myplant");
```

---

**void MicroGear:: setEEPROMOffset(int offset)**

Shift the offset of an EEPROM address where a microgear token is stored. This command will be useful if your application wants to store some other data in an EEPROM as well. The default offset value is 0.

**arguments**
* *offset* - The EEPROM address offset vale. The default is 0.

---

**void MicroGear::on(unsigned char event, void (* callback)(char*, uint8_t*,unsigned int))**

Add a callback listener to the event.

**arguments**
* *event* - a name of the event (MESSAGE|CONNECTED|PRESENT|ABSENT).
* *callback* - a callback function .

---

**bool MicroGear::connect(char* appid)**

Connect to NETPIE. If succeed, a CONNECTED event will be fired.

**arguments**
* *appidt* - an App ID.

---

**bool MicroGear::connected(char* appid)**

Check the connection status, return true if it is connected.

**arguments**
* *appidt* - an App ID.

---

**void MicroGear::setAlias(char* alias)**

microgear can set its own alias, which to be used for others make a function call chat(). The alias will appear on the key management portal of netpie.io .

**arguments**
* *alias* - an alias.

---

**bool MicroGear::chat(char* target, char* message)**<br/>
**bool MicroGear::chat(char* target, int message)**<br/>
**bool MicroGear::chat(char* target, double message)**<br/>
**bool MicroGear::chat(char* target, double, int decimal)**<br/>
**bool MicroGear::chat(char* target, String message)**<br/>
		
**arguments**
* *target* - the alias of the microgear(s) that a message will be sent to.
* *message* - message to be sent.
* *decimal* - number of digits after the deimal point.

---

**bool MicroGear::publish(char* topic, char* message [, bool retained])**<br/>
**bool MicroGear::publish(char* topic, double message [, bool retained])**<br/>
**bool MicroGear::publish(char* topic, double message, int decimal [, bool retained])**<br/>
**bool MicroGear::publish(char* topic, int message [, bool retained])**<br/>
**bool MicroGear::publish(char* topic, String message [, bool retained])**<br/>

In the case that the microgear want to send a message to an unspecified receiver, the developer can use the function publish to the desired topic, which all the microgears that subscribe such topic will receive a message.

**arguments**
* *topic* - name of topic to be send a message to.
* *message* - message to be sent.
* *decimal* - number of digits after the deimal point.
* *retained* - retain a message or not, the default is false (optional))

---

**void MicroGear::subscribe(char* topic)**

microgear may be interested in some topic. The developer can use the function subscribe() to subscribe a message belong to such topic. If the topic used to retain a message, the microgear will receive a message everytime it subscribes that topic.

**arguments**
* *topic* - name of topic to be send a message to.

---

**void MicroGear::unsubscribe(char* topic)**

cancel subscription

**arguments**
* *topic* - name of topic to be send a message to.

---

**void microgear.writeFeed (char* feedid, char *datajson)**<br/>
**void microgear.writeFeed (char* feedid, char *datajson, char *apikey)**<br/>
**void microgear.writeFeed (char* feedid, String datajson)**<br/>
**void microgear.writeFeed (char* feedid, String datajson, char *apikey)**<br/>

write time series data to a feed storage

**arguments**
* *feedid* - name of the feed 
* *datajson* - data string in json format 
* *apikey* - apikey for authorization. If apikey is not specified, you will need to allow the AppID to access feed and then the default apikey will be assigned automatically.

```js
microgear.writeFeed("homesensor","{temp:25.7,humid:62.8,light:8.5}");
microgear.writeFeed("homesensor","temp:25.7,humid:62.8,light:8.5");
```
---

**void MicroGear::resetToken()**

To send a revoke token control message to NETPIE and delete the token from cache. As a result, the microgear will need to request a new token for the next connection.

---

**void MicroGear::loop()**

This method has to be called regularly in the arduino loop() function to keep connection alive and to handle incoming messages.
