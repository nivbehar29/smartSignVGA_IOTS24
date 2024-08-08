#pragma once

#include <Firebase_ESP_Client.h>
//#include <FirebaseESP32.h>
// #include <FirebaseClient.h>
//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

#include <Arduino_JSON.h>

#include "keys/openweathermap_key.h"
#include "DBAux.h"
extern DB_parkingLot* db_parkingLot;

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

      // if(!signupOK)
      // {
        if (Firebase.signUp(&config, &auth, "", ""))
        {
          Serial.println("Firebase signUp success");
          signupOK = true;
        }
        else{
          Serial.println("Firebase signUp failed");
          Serial.printf("%s\n", config.signer.signupError.message.c_str());
        }
      // }

      /* Assign the callback function for the long running token generation task */
      // config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
      
      Firebase.begin(&config, &auth);
      Firebase.reconnectWiFi(false);
      
      Serial.println("setup End");
    }

    void setIntFlotTest(int num)
    {
       Serial.println("setIntFlotTest()");
      if (Firebase.ready() && signupOK)
      {
        Serial.println("setIntFlotTest(): 1");
        //sendDataPrevMillis = millis();

        // Write an Int number on the database path test/int
        if (Firebase.RTDB.setInt(&fbdo, "test/int", num))
        {
          Serial.println("setIntFlotTest(): 2");
          Serial.println("PASSED");
          Serial.println("PATH: " + fbdo.dataPath());
          Serial.println("TYPE: " + fbdo.dataType());
        }
        else
        {
          Serial.println("setIntFlotTest(): 3");
          Serial.println("FAILED");
          Serial.println("REASON: " + fbdo.errorReason());
        }
        count++;
        
        // Write a Float number on the database path test/float
        if (Firebase.RTDB.setFloat(&fbdo, "test/float", 0.01 + random(0,100)))
        {
          Serial.println("setIntFlotTest(): 4");
          Serial.println("PASSED");
          Serial.println("PATH: " + fbdo.dataPath());
          Serial.println("TYPE: " + fbdo.dataType());
        }
        else
        {
          Serial.println("setIntFlotTest(): 5");
          Serial.println("FAILED");
          Serial.println("REASON: " + fbdo.errorReason());
        }
      }
      Serial.println("setIntFlotTest() end");
    }

    bool getBool(String path, bool* target)
    {
      if (Firebase.ready() && signupOK)
      {
        if (Firebase.RTDB.getBool(&fbdo, path))
        {
          *target = fbdo.boolData();
          return true;
        }
        else
        {
          Serial.println("Error getting data: " + fbdo.errorReason());
          return false;
        }
      }
      return false;
    }

    bool setBool(String path, bool val)
    {
      if (Firebase.ready() && signupOK)
      {
        if (Firebase.RTDB.setBool(&fbdo, path, val))
        {
          return true;
        }
        else
        {
          Serial.println("Error setting data: " + fbdo.errorReason());
          return false;
        }
      }
      return false;
    }

    bool getInt(String path, int* target)
    {
      if (Firebase.ready() && signupOK)
      {
        if (Firebase.RTDB.getInt(&fbdo, path))
        {
          *target = fbdo.intData();
          return true;
        }
        else
        {
          Serial.println("Error getting data: " + fbdo.errorReason());
          return false;
        }
      }
      return false;
    }

 
    bool getString(String path)
    {
     
        if (Firebase.RTDB.getString(&fbdo, path))
        {
          
             if (fbdo.dataType() == "string")

                {  
                  int len=strlen(fbdo.stringData().c_str());
                  db_parkingLot->adv= (char*)malloc(sizeof(char)*len);
                  strcpy(db_parkingLot->adv,fbdo.stringData().c_str());
                }
        }
        else
        {
          Serial.println("Error getting data: " + fbdo.errorReason());
          return false;
        }
        return true;
    
    }

    bool getDB2()
    {
      bool is_error = false;
      int numFloors;
      

      if(Firebase.ready() && signupOK)
      {
        
        if(getInt("numFloors", &numFloors))
        {
          db_parkingLot = (DB_parkingLot*)malloc(sizeof(DB_parkingLot));

          db_parkingLot->num_floors = numFloors;
          db_parkingLot->floors = (DB_floor*)malloc(sizeof(DB_floor) * numFloors);
          // db_parkingLot->adv=(char*)malloc(sizeof(char)*50);
          getString("ad");
          

          for(int i = 0; i < numFloors && !is_error; i++)
          {
            fbdo.setBSSLBufferSize(16384 /* Rx buffer size in bytes from 512 - 16384 */, 512 /* Tx buffer size in bytes from 512 - 16384 */);
            String floor_path = "/floor" + String(i);

            bool is_data = Firebase.RTDB.getJSON(&fbdo, floor_path);
            
            if(is_data)
            {
              Serial.printf("Get json... %s\n",  fbdo.to<FirebaseJson>().raw());
              FirebaseJson json = fbdo.to<FirebaseJson>();
              String jsonString;
              json.toString(jsonString);

              JSONVar myObject =  JSON.parse(jsonString);
              // Serial.println(myObject);

              int numSlots = myObject["numSlots"];

              db_parkingLot->floors[i].num_slots = numSlots;
              db_parkingLot->floors[i].slots = (DB_parkSlot*)malloc(sizeof(DB_parkSlot) * numSlots);
              
              for(int j = 0; j < numSlots; j++)
              {
                db_parkingLot->floors[i].slots[j].is_changed = false;

                String parkSlot_s = "P" + String(j);
                db_parkingLot->floors[i].slots[j].is_taken = myObject[parkSlot_s]["t"];
                db_parkingLot->floors[i].slots[j].pos_x = myObject[parkSlot_s]["Pos"]["x"];
                db_parkingLot->floors[i].slots[j].pos_y = myObject[parkSlot_s]["Pos"]["y"];
                db_parkingLot->floors[i].slots[j].width = myObject[parkSlot_s]["Dim"]["w"];
                db_parkingLot->floors[i].slots[j].height = myObject[parkSlot_s]["Dim"]["h"];
                db_parkingLot->floors[i].slots[j].type = myObject[parkSlot_s]["K"];
              }

            }
            else
            {
              Serial.println("Error occurred while pulling floor " + String(i));
              Serial.println("Firebase Error Reason:" + fbdo.errorReason());
              is_error = true;
            }
          }
        }
        else
        {
          Serial.println("Error occurred while pulling numFlors");
          is_error = true;
        }
      }
      
      else
      {
        Serial.printf("Firebase not ready!!!!!!!!!!!!!!!!!!!!!!!");
        is_error = true;
      }

      if (is_error)
      {
        // TODO: Free allocated memory
        Serial.println("Error occurred while pulling database, set db_parkingLot to null");
        db_parkingLot = nullptr;
      }

      return !is_error;
    }
    

    bool setDB()
    {
      if(db_parkingLot != nullptr)
      {
        int numFloors = db_parkingLot->num_floors;
        for(int i = 0; i < numFloors; i++)
        {
          int numSlots = db_parkingLot->floors[i].num_slots;
          for(int j = 0; j < numSlots; j++)
          {
            if(db_parkingLot->floors[i].slots[j].is_changed)
            {
              bool val_to_set = db_parkingLot->floors[i].slots[j].is_taken;
              if(Firebase.RTDB.setBool(&fbdo, "floor" + String(i) + "/P" + String(j) + "/t", val_to_set))
              {
                db_parkingLot->floors[i].slots[j].is_changed = false;
                Serial.println("set floor: " + String(i) + ", slot: " + String(j));
                Serial.println("Succeed while setting park slot to DB!");
              }
              else
              {
                Serial.println("Failed while setting park slot to DB!");
                return false;
              }
            }
          }
        }
      }
      else
      {
        Serial.println("Failed while setting park slot to DB! db_parkingLot is null");
        return false;
      }

      return true;
    }

    void EndFB()
    {
      Serial.println("EndFB() Start");
      // Ensure to remove listeners or streaming if used
      Firebase.RTDB.endStream(&fbdo);

      // Clear the FirebaseData object
      //fbdo.clear();
      Firebase.RTDB.end(&fbdo);
      Serial.println("EndFB() End");

      // Optionally, you can also end Firebase altogether
      // Firebase.end(&fbdo);
    }

};