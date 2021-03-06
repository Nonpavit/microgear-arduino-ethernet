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

   
#define GEARTIMEADDRESS "ga.netpie.io"
#define GEARTIMEPORT 8080

#define CLIENTTYPE "arduino.v1"

#define MINBACKOFFTIME             10
#define MAXBACKOFFTIME             10000
#define MAXENDPOINTLENGTH          200
#define MAXTOPICSIZE               128
#define STRBUFFERSIZE              200

#define KEYSIZE                    16
#define TOKENSIZE                  16
#define TOKENSECRETSIZE            32
#define USERNAMESIZE               65
#define PASSWORDSIZE               28
#define REVOKECODESIZE             28
#define FLAGSIZE                   4

#define EEPROM_STATE_NUL           65
#define EEPROM_STATE_REQ           66
#define EEPROM_STATE_ACC           67

#define EEPROM_STATEOFFSET         0
#define EEPROM_KEYOFFSET           1
#define EEPROM_TOKENOFFSET         17
#define EEPROM_TOKENSECRETOFFSET   33
#define EEPROM_REVOKECODEOFFSET    65
#define EEPROM_ENDPOINTSOFFSET     93

#define MICROGEAR_NOTCONNECT       0
#define MICROGEAR_CONNECTED        1
#define MICROGEAR_REJECTED         2
#define RETRY                      3

#define MQTTCLIENT_NOTCONNECTED    0
#define MQTTCLIENT_CONNECTED       1

#define NETPIECLIENT_CONNECTED     0
#define NETPIECLIENT_NOTCONNECTED  1
#define NETPIECLIENT_TOKENERROR    2

/* event type */
#define MESSAGE                    1
#define PRESENT                    2
#define ABSENT                     3
#define CONNECTED                  4
#define CALLBACK                   5
#define ERROR                      6
#define INFO                       7

class MicroGear {
    private:
        char* appid;
        char* gearname;
        char* gearkey;
        char* gearsecret;
        char* gearalias;
        char* scope;
        char* token;
        char* tokensecret;
        char* endpoint;
        int eepromoffset;
        int backoff, retry;
        unsigned long bootts;

        MQTTClient *mqttclient;
        Client *sockclient;
        AuthClient* authclient;

        bool getHTTPReply(Client*, char*, size_t);
        bool clientReadln(Client*, char*, size_t);

        void syncTime(Client*, unsigned long*);
        void readEEPROM(char*,int, int);
        void writeEEPROM(char*,int, int);
        void initEndpoint(Client*, char*);
        bool getToken(char*, char*, char*, char*, char*);

    public:
        int constate;
        MicroGear(Client&);
        void setName(char*);
        void setAlias(char*);
        int connect(char*);
        bool connected();
        bool publish(char*, char*);
        bool publish(char*, char*, bool);
        bool publish(char*, double);
        bool publish(char*, double, bool);
        bool publish(char*, double, int);
        bool publish(char*, double, int, bool);
        bool publish(char*, int);
        bool publish(char*, int, bool);
        bool publish(char*, String);
        bool publish(char*, String, bool);
        bool writeFeed(char*, char*);
        bool writeFeed(char*, char*, char*);
        bool writeFeed(char*, String);
        bool writeFeed(char*, String, char*);
        bool chat(char*, char*);
        bool chat(char*, int);
        bool chat(char*, double);
        bool chat(char*, double, int);
        bool chat(char*, String);
        void subscribe(char*);
        void unsubscribe(char*);
        void loop();
        void resetToken();
        void setToken(char*, char*, char*);
        int init(char*, char*);
        int init(char*, char*, char*);
        int init(char*, char*, char*, char*);
        void resetEndpoint();
        void strcat(char*, char*);
        void on(unsigned char,void (* callback)(char*, uint8_t*,unsigned int));
        void setEEPROMOffset(int);
        int state();
};

#endif
