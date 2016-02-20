import processing.*;
import processing.serial.*;

class Communicate {
  /* HISTORY & PAYLOAD SIZES */
  final short HISTORY_SIZE = 3; 
  final short PREAMBLE_SIZE = 7;
  final short PAYLOAD_DIGITAL_SIZE = 3;
  final short PAYLOAD_ANALOG_SIZE = 12; 
  /* PREAMBLE TYPES */
  final short HELLO = 126; // ~ Start of Message
  final short SYN = 0; // SYN = SYNCHRONISE
  final short URG = 1; // URG = ACT NOW
  final short RST = 2; // RST = CLEAR?
  final short CNT = 3; // CNT = MORE THAN ONE MESSAGE - CHECK FOR FIN
  final short FIN = 4; // FIN = COMMUNICATIONS FINISHED
  /* PAYLOAD MESSAGE TYPE */
  final short PWM = 251;
  final short DIG = 252;
  final short DXT = 253;
  final short ANA = 254;
  final short ALL = 255;
  /* PAYLOAD MESSAGE LENGTHS */
  final short OFFSET = 2;
  final short DIGSIZE = 14;
  final short DXTSIZE = 6;
  final short ANASIZE = 6;
  /* SYN RESET CONTROL */
  final short MAXMSGS = 255;
  /* COMMUNICATIONS TYPE */
  final short HARDSERIAL = 0;
  final short SOFTSERIAL = 1;
  final short ISQUAREDC = 2;
  
  Serial port;
  String inString;
  
  static boolean discovered = false;
  static short syn = 0;    // SYN_ID
  static short ack = 0;    // ACK_ID
  static short source = 0; // SENDER_ID
  static short target = 0; // MY_ID
  static short msg = 0;
  static short preamble[];
  static short payload_digital[];
  static short payload_analog[];
  
  Communicate(Serial p) {
    System.out.println("Hello World");
    this.port = p;
  }
  
  void inComingSerial() { 
    String inString = port.readString(); 
    System.out.print(inString);
  }
}