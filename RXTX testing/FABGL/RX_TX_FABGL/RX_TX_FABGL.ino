// FABGL transmit and receive
// FABGL              ESP32
// TX: IO 2         RX: IO 14
// RX: IO 34        TX: IO 12
// GND              GND

#include <HardwareSerial.h>
HardwareSerial mySerial(1);

void setup() {
  // Initialize serial communication at 115200 baud rate
  Serial.begin(115200);
  mySerial.begin(115200, SERIAL_8N1, 34, 2);
  // Print a message on the Serial Monitor
  Serial.println("Serial Communication Initialized");
}

int counter = 0;

void loop() {
  // Send a test message via Serial
  mySerial.printf("Hello from FABGL VGA32 * %d *", counter);
  counter++;

  // Check if data is available on Serial
  if (mySerial.available()) {
    String data = mySerial.readString();
    Serial.print("Received: ");
    Serial.println(data);

   
  }



//Serial.println("end loop ");

  // Add a delay to avoid flooding the serial port
  delay(800);
}