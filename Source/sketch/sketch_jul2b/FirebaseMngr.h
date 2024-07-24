#pragma once

#include <Firebase_ESP_Client.h>
// #include <FirebaseClient.h>
//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

#include "keys/openweathermap_key.h"

class FBMngr {

public:

    // Firebase stuff
    FirebaseData fbdo;
    FirebaseAuth auth;
    FirebaseConfig config;
    unsigned long sendDataPrevMillis = 0;
    int count = 0;
    bool signupOK = false;

    FBMngr()  
    {
      count = 0;
      signupOK = false;
    }

    void setup()
    {
      config.api_key = API_KEY;

      // Assign the RTDB URL (required)
      config.database_url = DATABASE_URL;
      Serial.println(API_KEY);
      Serial.println(DATABASE_URL);

      if (Firebase.signUp(&config, &auth, "", ""))
      {
        Serial.println("Firebase signUp success");
        signupOK = true;
      }
      else{
        Serial.println("Firebase signUp failed");
        Serial.printf("%s\n", config.signer.signupError.message.c_str());
      }

      /* Assign the callback function for the long running token generation task */
      // config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
      
      Firebase.begin(&config, &auth);
      Firebase.reconnectWiFi(false);
    }

    void setIntFlotTest()
    {
       Serial.println("setIntFlotTest()");
      if (Firebase.ready() && signupOK /*&& (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0)*/)
      {
        Serial.println("setIntFlotTest(): 1");
        //sendDataPrevMillis = millis();

        // Write an Int number on the database path test/int
        if (Firebase.RTDB.setInt(&fbdo, "test/int", count)){
          Serial.println("setIntFlotTest(): 2");
          Serial.println("PASSED");
          Serial.println("PATH: " + fbdo.dataPath());
          Serial.println("TYPE: " + fbdo.dataType());
        }
        else {
          Serial.println("setIntFlotTest(): 3");
          Serial.println("FAILED");
          Serial.println("REASON: " + fbdo.errorReason());
        }
        count++;
        
        // Write a Float number on the database path test/float
        if (Firebase.RTDB.setFloat(&fbdo, "test/float", 0.01 + random(0,100))){
          Serial.println("setIntFlotTest(): 4");
          Serial.println("PASSED");
          Serial.println("PATH: " + fbdo.dataPath());
          Serial.println("TYPE: " + fbdo.dataType());
        }
        else {
          Serial.println("setIntFlotTest(): 5");
          Serial.println("FAILED");
          Serial.println("REASON: " + fbdo.errorReason());
        }
      }
      Serial.println("setIntFlotTest() end");
    }

    void EndFB()
    {
      // Ensure to remove listeners or streaming if used
      Firebase.RTDB.endStream(&fbdo);

      // Clear the FirebaseData object
      fbdo.clear();

      // Optionally, you can also end Firebase altogether
      // Firebase.end(&fbdo);
    }

};