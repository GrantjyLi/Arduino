#ifndef FIREBASECOM_H
#define FIREBASECOM_H

#include<Firebase_ESP_Client.h>

#define FIREBASE_KEY "AIzaSyBLr1kYfHeiWlVLlcffqOU3xmJwlRYEvmI" //WEB API KEY, NOT APART OF SERVICE ACCOUNRTS
#define DB_URL "https://esp8266test-64f1b-default-rtdb.firebaseio.com"

#define HOST_PASSWORD "hotspot4Grant"

extern FirebaseData FBdata;
extern FirebaseAuth auth;
extern FirebaseConfig config;

namespace FirebaseCom{
    void firebaseSetup();

    void addEntry(String&, String&);
}

#endif