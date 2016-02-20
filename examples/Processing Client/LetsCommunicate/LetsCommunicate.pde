import processing.serial.*;

Serial port;
Communicate lets;

void setup() {
  size(200, 200);
  String portName = Serial.list()[3];
  println(Serial.list()[3]);
  port = new Serial(this, portName, 115200);
  lets = new Communicate(port);
}

void draw() { } 

void serialEvent(Serial p) { 
  String inString = p.readString(); 
  System.out.print(inString);
  //lets.inComingSerial();
}