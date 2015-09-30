/*
  MicroGear Arduino library
   NetPIE Project
   http://netpie.io

*/

#ifndef MICROGEAR_H
#define MICROGEAR_H

#include <stdio.h>
#include <Ethernet.h>
#include "PubSubClient.h"
#include "MQTTClient.h"
#include <EEPROM.h>
#include "SHA1.h"
#include "AuthClient.h"
//#include "debug.h"

   
#define GEARTIMEADDRESS "gearauth.netpie.io"
#define GEARTIMEPORT 8080

#define CLIENTTYPE "arduino.v1"

#define MINBACKOFFTIME             10
#define MAXBACKOFFTIME             10000
#define MAXENDPOINTLENGTH          200
#define MAXGEARIDSIZE              64
#define MAXTOPICSIZE               128

#define TOKENSIZE                  16
#define TOKENSECRETSIZE            32
#define USERNAMESIZE               65
#define PASSWORDSIZE               28
#define REVOKECODESIZE             28
   
#define EEPROM_STATE_NUL           65
#define EEPROM_STATE_REQ           66
#define EEPROM_STATE_ACC           67
#define EEPROM_STATEOFFSET         0
#define EEPROM_TOKENOFFSET         1
#define EEPROM_TOKENSECRETOFFSET   17
#define EEPROM_REVOKECODEOFFSET    49
#define EEPROM_ENDPOINTSOFFSET     77

#define MICROGEAR_NOTCONNECT       0
#define MICROGEAR_CONNECTED        1
#define MICROGEAR_REJECTED         2
#define RETRY                      3

#define CLIENT_NOTCONNECT          0
#define CLIENT_CONNECTED           1
#define CLIENT_REJECTED            2

/* event type */
#define MESSAGE                    1
#define PRESENT                    2
#define ABSENT                     3
#define CALLBACK                   4


class MicroGear {
	private:
        char* appid;
		char* gearname;
		char* gearkey;
        char* gearsecret;
        char* scope;
		char gearid[MAXGEARIDSIZE];
		char* app_topic;
		char* group_topic;
        char* token;
        char* tokensecret;
        char* endpoint;
		char mqtt_client_type;
		unsigned long bootts;
		int eepromoffset;

        int backoff, retry;
        AuthClient* authclient;

		bool getHTTPReply(Client*, char*, size_t);
		bool clientReadln(Client*, char*, size_t);

		void syncTime(Client*, unsigned long*);
		void readEEPROM(char*,int, int);
		void writeEEPROM(char*,int, int);
        void getToken(char*, char*, char*);

		MQTTClient *mqttclient;
		Client *sockclient;

	public:
		int constate;
		MicroGear(Client&);
		void setName(char*);
		boolean connect(char*);
		boolean connected();
		void publish(char*, char*);
		void subscribe(char*);
		void unsubscribe(char*);
		void chat(char*, char*);
		void loop();
        void resetToken();
        void setToken(char*, char*);
        int init(char*, char*, char*);
		void strcat(char*, char*);
		void on(unsigned char,void (* callback)(char*, uint8_t*,unsigned int));
		void setEEPROMOffset(int);
};

#endif
