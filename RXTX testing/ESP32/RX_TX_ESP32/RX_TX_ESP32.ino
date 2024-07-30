// ESP32 Transmit and Recieve 
#include <HardwareSerial.h>
HardwareSerial mySerial(1);
void setup() {
  // put your setup code here, to run once:
  mySerial.begin(115200, SERIAL_8N1, 14, 12);
  Serial.begin(115200);

}
int counter = 0;
void loop() {
  // put your main code here, to run repeatedly:
  delay(1000);
  //Serial.println("from esp32 to vga");
  mySerial.printf("from esp32 to vga * %d * ", counter);
  counter++;
  if(mySerial.available())
  {
    String data = mySerial.readString();
    Serial.print("Received: ");
    Serial.println(data);
  }

}