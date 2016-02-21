import processing.serial.*;

Serial port;

void setup() {
  size(200, 200);
  String portName = Serial.list()[3];
  println(Serial.list()[3]);
  port = new Serial(this, portName, 115200);
  port.bufferUntil('\n');
}

void draw() { } 

/********************************************************************************/
/*  THE METHODS BELOW HANDLE ALL COMMUNICATIONS STATE WITH A CONNECTED ARDUINO  */
/********************************************************************************/
/*  DO NOT ALTER THESE METHODS UNLESS YOU UNDERSTAND HOW THE COMMUNICATION WORK */
/********************************************************************************/

void serialEvent(Serial p) { 
  /* ENSURE WHILE(... > 0) && port.bufferUntil('\n') */
  while(p.available() > 0) {
    /* CHECK FOR HELLO = 126 | '~' */
    short _temp = (short)p.read();
    if( (_temp == HELLO) && ((!discovered)) ) {
      discover(p);
    } else if( (_temp == 126) && ((discovered)) ) {
      //handle();
    }
  }
}

void discover(Serial p){ 
    short _temp = (short)p.read();
    
    preamble = new short[PREAMBLE_SIZE - OFFSET];
    for(int i = 0; i < PREAMBLE_SIZE - OFFSET; i++){
      preamble[i] = (short)p.read();
      //System.out.print(preamble[i]); System.out.print(" ");
    }
    /* RESPONSE FROM PROCESSING */
    short[] _preamble = buildResponsePreamble(preamble);
    for(int i = 0; i < PREAMBLE_SIZE; i++){
      //System.out.print(_preamble[i]); System.out.print(" ");
      port.write(_preamble[i]);
    }
    println();
    
    /* DO NOT REMOVE - LEAVE THIS HERE - PROCESSING NEEDS IT !!!!! */
    //while(p.available() < 0) {
    //  _temp = (short)p.read();
    //  while(_temp != HELLO) {
    //    System.out.println("Wating for Confirmation from Arduino"); 
    //    _temp = (short)p.read(); delay(1000);
    //  }
    //}
    if(!discovered) {
      System.out.println("COME ON ARDUINO");
    } else {
      System.out.println("DISCOVERED");
    }   
}

void handle(){

}

short[] buildResponsePreamble(short[] type){
  short _temp[] = new short[PREAMBLE_SIZE];
  _temp[0] = HELLO;
  _temp[1] = PREAMBLE_SIZE;
  _temp[2] = type[1];
  _temp[3] = type[0];

    switch(type[4]) {
      case SYN:
        /* SYNCHRONISE */
        if(!discovered) { syn = (short)random(0, 255); }
        _temp[4] = syn++;
        ack = type[2];
        _temp[5] = ack;
        _temp[6] = SYN;
      break;
      case URG:
        /* URGENT */
      break;
      case RST:
       /* RESET */
      break;
      case CNT:
        /* CONTINUE */
      break;
      case FIN:
        /* FINISH */
        if(syn == type[2])
          _temp[4] = ++syn;
        if(!discovered)
          discovered = true;
        ack = type[2];
        _temp[5] = ack;
        _temp[6] = FIN;
      break;
    }
  return _temp;
}

/********************************************************************************/
/* Variables used for the Communication between Processing and Arduino          */
/********************************************************************************/

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
  
  static boolean discovered = false;
  static short syn = 0;    // SYN_ID
  static short ack = 0;    // ACK_ID
  static short source = 0; // SENDER_ID
  static short target = 0; // MY_ID
  static short msg = 0;
  static short preamble[];
  static short payload_digital[];
  static short payload_analog[];
  
/********************************************************************************/
/* Variables used for the Communication between Processing and Arduino          */
/********************************************************************************/