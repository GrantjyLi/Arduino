#include "FirebaseCom.h"

namespace FirebaseCom{

    void firebaseSetup(){
        config.api_key = FIREBASE_KEY;
        config.database_url = DB_URL;
        
        /* Sign up */
        if (Firebase.signUp(&config, &auth, "", "")){
            Serial.println("Firebase Sign up Confirmed");
        }
        else{
            Serial.print("!ERROR: ");
            Serial.printf("%s\n", config.signer.signupError.message.c_str());
        }

        Firebase.begin(&config, &auth);
        Firebase.reconnectWiFi(true);
    }

    void addEntry(String& SSID, String& PW){
        if(Firebase.RTDB.setString(&FBdata, "Networks\\" + SSID, PW)){
            Serial.println("Password stored.");
        } else {
            Serial.println("Failed to store");
            Serial.println("REASON: " + FBdata.errorReason());
        }
    }

}