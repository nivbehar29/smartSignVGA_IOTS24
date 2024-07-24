#include "fabgl.h"
#include "bitmapsWarehouse.h"
#include "fabui.h"

#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>

#include <Firebase_ESP_Client.h>
//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

#include "keys/openweathermap_key.h"

#include "ParkingApp.h"

// main VGA Controller
fabgl::VGA16Controller DisplayController;
fabgl::PS2Controller   PS2Controller;

// bitmap for parking image
// Bitmap park_bitmap = Bitmap(92, 146, NBitmapsWarehouse::park, PixelFormat::RGBA2222);
// Bitmap red_light_bitmap = Bitmap(60, 52, NBitmapsWarehouse::red_light, PixelFormat::RGBA2222);

// sprites (images) to show on the screen
// Sprite sprites[4];

// main Canvas
// fabgl::Canvas Canvas(&DisplayController);
//fabgl::uiApp runner;

// WIFI
extern String openWeatherMapApiKey;
extern const char* ssid;
extern const char* password;


// Weather parameters
String city = "Haifa";
String countryCode = "IL";
// THE DEFAULT TIMER IS SET TO 10 SECONDS FOR TESTING PURPOSES
// For a final application, check the API call limits per hour/minute to avoid getting blocked/banned
unsigned long lastTime = 0;
// Timer set to 10 minutes (600000)
//unsigned long timerDelay = 600000;
// Set timer to 10 seconds (10000)
unsigned long timerDelay = 10000;

String jsonBuffer;

// Firebase stuff
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
unsigned long sendDataPrevMillis = 0;
int count = 0;
bool signupOK = false;

void printMem()
{
  Serial.printf("Free 8bit: %d KiB\n", heap_caps_get_free_size(MALLOC_CAP_8BIT) / 1024);
  Serial.printf("Free 32bit: %d KiB\n", heap_caps_get_free_size(MALLOC_CAP_32BIT) / 1024);
}

void setup(){
  Serial.begin(115200);  // Start the serial communication

  delay(1000);

  Serial.printf("Memory at init:\n");
  printMem();

  // Setup Mouse / Keyboard
  PS2Controller.begin(PS2Preset::KeyboardPort0_MousePort1, KbdMode::GenerateVirtualKeys);

  Serial.printf("Memory after init PS2Controller:\n");
  printMem();

  // Setup display controller
  DisplayController.begin();
  DisplayController.setResolution(VGA_640x480_60Hz); // VGA_640x350_70Hz , VGA_640x480_60Hz

  // Serial.printf("Memory after init fabgl with VGA_640x480_60Hz:\n");
  // printMem();

  // DisplayController.setResolution(VGA_256x192_50Hz); // VGA_640x350_70Hz , VGA_640x480_60Hz

  // Serial.printf("Memory after changing resolution to VGA_256x192_50Hz:\n");
  // printMem();

  // DisplayController.end();

  // Serial.printf("Memory after ending fabgl:\n");
  // printMem();

  // PS2Controller.end();

  // Serial.printf("Memory after ending PS2Controller:\n");
  // printMem();

  // DisplayController.begin();
  // DisplayController.setResolution(VGA_256x192_50Hz ); // VGA_640x350_70Hz , VGA_640x480_60Hz

  // Serial.printf("Memory after init fabgl with  VGA_256x192_50Hz:\n");
  // printMem();

  // Wifi
  delay(1000);

  WiFi.mode(WIFI_STA); //Optional
  WiFi.begin(ssid, password);
  Serial.println("\nConnecting");

  Serial.printf("Memory after init Wifi:\n");
  printMem();

  unsigned long startAttemptTime = millis();

  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 10000)
  {
      Serial.print(".");
      delay(100);
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nConnected to the WiFi network");
    Serial.print("Local ESP32 IP: ");
    Serial.println(WiFi.localIP());


    Serial.printf("Memory after connecting Wifi:\n");
    printMem();

    // Setup Firebase
    // Assign the api key (required)
    config.api_key = API_KEY;

    // Assign the RTDB URL (required)
    config.database_url = DATABASE_URL;
    Serial.println(API_KEY);
    Serial.println(DATABASE_URL);

    // if (Firebase.signUp(&config, &auth, "", ""))
    // {
    //   Serial.println("ok");
    //   signupOK = true;
    // }
    // else{
    //   Serial.println("not ok");
    //   Serial.printf("%s\n", config.signer.signupError.message.c_str());
    // }

    // Serial.println("next step");

    // /* Assign the callback function for the long running token generation task */
    // // config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
    
    // Firebase.begin(&config, &auth);
    // Firebase.reconnectWiFi(true);

  }
  else
  {
    Serial.println("\nFailed to connect to the WiFi network within 10 seconds");

  }

}

bool done_with_weather = false;
bool weather_succeeded = false;
JSONVar myObject;

void loop() {

  // Send an HTTP GET request
  if ((millis() - lastTime) > timerDelay && !done_with_weather)
  {
    Serial.println("Starting Weather");

    // Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED)
    {
      Serial.println("Wifi status OK");
      
      String serverPath = "http://api.openweathermap.org/data/2.5/weather?q=" + city + "," + countryCode + "&APPID=" + openWeatherMapApiKey;
      
      jsonBuffer = httpGETRequest(serverPath.c_str());
      Serial.println(jsonBuffer);
      myObject = JSON.parse(jsonBuffer);
  
      // JSON.typeof(jsonVar) can be used to get the type of the var
      // if 'cod' is not 200, according to weather api, we didnt get the right data (can be becuase of bad api key for example)
      if (JSON.typeof(myObject) == "undefined" || (int) myObject["cod"] != 200)
      {
        Serial.println("Parsing input failed!");
        done_with_weather = true;
        weather_succeeded = false;
      }
    
      else
      {
        done_with_weather = true;
        weather_succeeded = true;

        Serial.print("JSON object = ");
        Serial.println(myObject);
        Serial.print("Temperature: ");
        Serial.println(myObject["main"]["temp"]);
        Serial.print("Pressure: ");
        Serial.println(myObject["main"]["pressure"]);
        Serial.print("Humidity: ");
        Serial.println(myObject["main"]["humidity"]);
        Serial.print("Wind Speed: ");
        Serial.println(myObject["wind"]["speed"]);
      }

    }
    else
    {
      WiFi.disconnect(true, true);
      Serial.println("WiFi Disconnected");
      done_with_weather = true;
      weather_succeeded = false;
    }
    
    lastTime = millis();
  }

  if(done_with_weather)
  {
    Serial.println("Done with weather!");

    // Disconnect from WiFi
    if(WiFi.status()== WL_CONNECTED)
    {
      Serial.println("Disconnecting from WiFi...");
      WiFi.disconnect(true, true);
    }

    // ParkingApp().runAsync(&DisplayController, 3500).joinAsyncRun();
    if(weather_succeeded)
      ParkingApp(&myObject).runAsync(&DisplayController, 3500).joinAsyncRun();
    else
      ParkingApp(nullptr).runAsync(&DisplayController, 3500).joinAsyncRun();
  }

  // if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0)){
  //   sendDataPrevMillis = millis();
  //   // Write an Int number on the database path test/int
  //   if (Firebase.RTDB.setInt(&fbdo, "test/int", count)){
  //     Serial.println("PASSED");
  //     Serial.println("PATH: " + fbdo.dataPath());
  //     Serial.println("TYPE: " + fbdo.dataType());
  //   }
  //   else {
  //     Serial.println("FAILED");
  //     Serial.println("REASON: " + fbdo.errorReason());
  //   }
  //   count++;
    
  //   // Write an Float number on the database path test/float
  //   if (Firebase.RTDB.setFloat(&fbdo, "test/float", 0.01 + random(0,100))){
  //     Serial.println("PASSED");
  //     Serial.println("PATH: " + fbdo.dataPath());
  //     Serial.println("TYPE: " + fbdo.dataType());
  //   }
  //   else {
  //     Serial.println("FAILED");
  //     Serial.println("REASON: " + fbdo.errorReason());
  //   }
  // }

}

String httpGETRequest(const char* serverName) {
  WiFiClient client;
  HTTPClient http;
    
  // Your Domain name with URL path or IP address with path
  http.begin(client, serverName);
  
  // Send HTTP POST request
  int httpResponseCode = http.GET();
  
  String payload = "{}"; 
  
  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
}

