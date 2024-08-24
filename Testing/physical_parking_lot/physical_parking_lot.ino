/*
  IR Proximity Sensor interface code
  Turns on an LED on when obstacle is detected, else off.
  blog.circuits4you.com 2016
 */
const int ProxSensor=13;

void setup() {       
  // Start the serial communication
  Serial.begin(115200);

  // initialize the digital pin as an output.
  // Pin 2 has an LED connected to it
  pinMode(2, OUTPUT);     
  //Pin 2 is connected to the output of proximity sensor
  pinMode(ProxSensor,INPUT);
}

void loop() {
  
  int data = digitalRead(ProxSensor);
  Serial.println(data);
  if(data==LOW)      //Check the sensor output
  {
    Serial.println("set the LED off");
    digitalWrite(2, LOW);   // set the LED on
  }
  else
  {
    digitalWrite(2, HIGH);    // set the LED off
  }
  delay(100);              // wait for a second
}