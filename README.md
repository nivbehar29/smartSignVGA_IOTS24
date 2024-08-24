# VGA Smart Parking Lot IOTS24

---

## **Table of Contents**

1. [Introduction](#introduction)
2. [Features](#Features)
3. [Board](#Board)
4. [Libraries](#libraries)
5. [Usage](#usage)
6. [Poster](#Poster)

---

## **Introduction**

A university project on IoT.
In this project, we are implementing a graphical interface that represents a parking lot, both for the customer side and the parking lot manager side.
For the esp32, the graphical interface is based on fabgl library.

For the customer, various options are displayed upon entering the parking lot, using an ESP32 that shows on a screen the different parking options they wish to select (such as parking for a disabled person, electric car, regular car, etc.).
The customer is presented with a parking lot map with the ability to choose a parking spot from the available spaces even before entering the parking lot.

For the parking lot manager, there is the option to manage and design the parking lot through a website, including adding parking spaces, setting parking types, and more.

The project utilizes Firebase to manage the data.

---

## **Features**

- **Feature 1 (Customer side)**:  
Display location and temperature on the main screen, with the temperature being fetched from the OpenWeatherMap API.

- **Feature 2 (Customer side)**:  
Display advertisements on the main screen.
Advertisements can be presented as text with certain effects, such as scrolling text from right to left.
    
- **Feature 3 (Customer side)**:  
The customer selects the desired type of parking (electric/disabled/etc.).

- **Feature 4 (Customer side)**:  
Display an updated map of the parking lot floors, showing the status of each parking spot (available, occupied), and the customer selects the desired parking spot using the mouse based on the map.

- **Feature 5 (Manager side)**:  
An option to manage and design the parking lot through a website.

- **Feature 6 (Manager side)**:  
An option to add parking spaces in different floors, setting parking types, move and resize parking slots

---

## **Board**
We have used the LILYGO FabGL VGA32 board.
Note: for fabgl library to work we have used esp32 version 2.0.17 by Espressif Systems.

---

## **Libraries**

- **FabGL - version 1.0.9 with some custom changes provided in this repo**:
- **Arduino_JSON - version 0.2.0**:
- **Firebase Arduino_Client Library for ESP8266 and ESP32 - version 4.4.14**:
- **pngle - version 1.0.2 (used in png tests but not in the final project)**:

---

## Usage

**To set up firebase**:
- Note that this project is using a very basic and unsecured firebase environement - it is using anonymous signup to firebase when using it.
- It doesn't depend on users login for using the database. 
- Create a realtime database in google firebase site (We have used this: https://randomnerdtutorials.com/esp32-firebase-realtime-database/).
- Upload the firebase_database.json file to the realtime databse (you can find this file in 'manager_website' folder).

**To set up OpenWeatherMap API**:
- Follow this tutorial to get you api key: https://randomnerdtutorials.com/esp32-http-get-open-weather-map-thingspeak-arduino/
- You should add your api key to the keys.h in the next section.

**To set up the ESP32 environment**:
- Clone this repo to you machine.
- Install arduino IDE and install esp32 version 2.0.17 by Espressif Systems in the boards manager.
- Install the libraries mentioned in the Libraries section.
- Get the files in 'fabgl changes' folder and copy them into: user\Documents\Arduino\libraries\FabGL\src or wherever you fabgl library is.
- In Source/sketch/sketch_jul2b/ folder, create a new folder called keys and inside it create a file 'keys.h'.  
  This file should include some constants and Macros for the relevant keys.  
  An example file is included in the following directory:  
  /keys files examples/Source/sketch/sketch_jul2b/keys/keys.h.
- Compile and flash.

**To set up the manager site**:
- In manager_website folder, create a new folder called 'keys' and place firebaseConfig.js file in it.
  You can find the file inside /keys files examples/manager_website/keys.
  Note that you need to edit this file with your own keys.
- Open terminal in "manager_website" directory.
- Run "python -m http.server <port>". example: python -m http.server 800
- Open browser and navigate to "http://localhost:<port>/" and open parking_manager.html throut it.

---

## Poster
![Project Poster](https://github.com/nivbehar29/smartSignVGA_IOTS24/blob/main/Extra/IOT%20Poster/Poster.jpg)
