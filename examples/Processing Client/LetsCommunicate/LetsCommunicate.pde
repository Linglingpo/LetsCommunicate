import processing.serial.*;

Serial port;

void setup() {
  size(200, 200);
  String portName = Serial.list()[5];
  println(Serial.list());
  port = new Serial(this, portName, 115200);
  port.bufferUntil('\n');
}

void draw() {
}

/********************************************************************************/
/*  THE METHODS BELOW HANDLE ALL COMMUNICATIONS STATE WITH A CONNECTED ARDUINO  */
/********************************************************************************/

void serialEvent(Serial p) {
  /* ENSURE WHILE(... > 0) && port.bufferUntil('\n') */
  while (p.available() > 0) {
    /* CHECK FOR HELLO = 126 | '~' */
    short _temp = (short)p.read();
    if ( (_temp == HELLO) && ((!discovered)) ) {
      discover(p);
    } else if ( (_temp == HELLO) && ((discovered)) ) {
      handle(p);
    }
  }
}

void discover(Serial p) {
  System.out.println("IN DISCOVERED ---------------------------------");
  short _temp = (short)p.read(); // preamble size
  if (_temp == PREAMBLE_SIZE) {
    preamble = new short[PREAMBLE_SIZE - OFFSET]; // no ~, no Preamble Size
    System.out.print("ARDUINO DISCOVERED PREAMBLE: ");
    for (int i = 0; i < PREAMBLE_SIZE - OFFSET; i++) {
      preamble[i] = (short)p.read();
      System.out.print(preamble[i]); 
      System.out.print(" ");
    }
    println();

    /* RESPONSE FROM PROCESSING */
    short[] _preamble = peek(preamble);
    System.out.print("PROCESSING DISCOVERED PREAMBLE: ");
    for (int i = 0; i < PREAMBLE_SIZE; i++) {
      System.out.print(_preamble[i]); 
      System.out.print(" ");
      port.write(_preamble[i]);
    }
    println();

    if (!discovered) {
      System.out.println("COME ON ARDUINO ----------");
    } else {
      System.out.println("DISCOVERED ----------");
    }
    // PREAMBLE_SIZE is not 7
  } else {
    System.out.println("ERROR !!!!!");
  }
}


void handle(Serial p) {
  System.out.println("IN HANDLE ---------------------------------");

  // ------------------ PREAMBLE ---------------------//
  short _temp = (short)p.read(); // preamble size
  if (_temp == PREAMBLE_SIZE) {
    preamble = new short[PREAMBLE_SIZE - OFFSET]; // no ~, no Preamble Size
    System.out.print("ARDUINO HANDLE PREAMBLE: ");
    for (int i = 0; i < PREAMBLE_SIZE - OFFSET; i++) {
      preamble[i] = (short)p.read();
      System.out.print(preamble[i]); 
      System.out.print(" ");
    }
    println();
  } // End of preamble


  // ------------------ DATA ---------------------//
  short _payloadType = (short)p.read(); // DIG/DXT/ANA/ALL
  short _dataSize = (short)p.read();

  System.out.print("Payload Type: "); 
  System.out.println(_payloadType); 
  System.out.print("DATA Size: "); 
  System.out.println(_dataSize); 

  if (_payloadType == DIG) {
    System.out.println("IN DIG ---------------------------------"); 
    data = new short[_dataSize - OFFSET]; // no Payload Type, no Data Size
    System.out.print("DATA MSG: ");
    for (int i = 0; i < _dataSize - OFFSET; i++) {
      data[i] = (short)p.read(); // reading data state
      System.out.print(data[i]); 
      System.out.print(" ");
    }
  } // end of DIG
  
  println();
  
  /* RESPONSE FROM PROCESSING */
  short[] _preamble = peek(preamble);
  System.out.print("PROCESSING HANDLE PREAMBLE: ");
  for (int i = 0; i < PREAMBLE_SIZE; i++) {
    System.out.print(_preamble[i]); 
    System.out.print(" ");
    port.write(_preamble[i]);
  }
  
  println();
}


short[] peek(short[] type) {
  short _temp[] = new short[PREAMBLE_SIZE];

  _temp[0] = HELLO;
  _temp[1] = PREAMBLE_SIZE;
  _temp[2] = MYID;
  _temp[3] = ( !discovered ) ? source = type[0] : source; // Arduino ID

  switch(type[4]) {
  case SYN:
    /* SYNCHRONISE */
    _temp[4] = ( !discovered ) ? syn = (short)random(0, 255) : ++syn; //SNY
    //System.out.println("SYN IS: " + syn);
    _temp[5] = ( ack = type[2] ); // ACK
    _temp[6] = SYN; // Payload Type
    break;
  case URG:
    /* URGENT */
    break;
  case RST:
    /* RESET */
    break;
  case CNT:
    /* CONTINUE */
    System.out.println("IN CNT ------------------------------------");
    //check Syn
    if (++syn == type[2]) {
      _temp[4] = ++syn; //SNY
      _temp[5] = ( ack = type[2] ); //ACK
      _temp[6] = CNT; // Preamble Type
    }
    //Capture the data msg
    break;
  case FIN:
    /* FINISH */
    //System.out.println("SYN IS: " + syn);
    //System.out.println("MSGID IS: " + type[2]);
    //checking the Syn form arduino
    if (++syn == type[2]) {
      //processing Syn update
      _temp[4] = ++syn;
      if (!discovered) { 
        discovered = true;
      }
    }
    _temp[5] = (ack = type[2]);
    _temp[6] = FIN;

    break;
  }
  return _temp;
}

/********************************************************************************/
/* Variables used for the Communication between Processing and Arduino          */
/********************************************************************************/

final short MYID = 0x10; // MY_ID - 16
/* HISTORY & PAYLOAD SIZES */
final short HISTORY_SIZE         = 0x03;
final short PREAMBLE_SIZE        = 0x07; //7
//final short PAYLOAD_DIGITAL_SIZE = 0x03;
final short PAYLOAD_ANALOG_SIZE  = 0x0C; //12 data size

/* PREAMBLE TYPES */
final short HELLO = 0x7E; // ~ Start of Message
final short SYN   = 0x00; // SYN = SYNCHRONISE
final short URG   = 0X01; // URG = ACT NOW
final short RST   = 0x02; // RST = CLEAR?
final short CNT   = 0x03; // CNT = MORE THAN ONE MESSAGE - CHECK FOR FIN
final short FIN   = 0X04; // FIN = COMMUNICATIONS FINISHED

final short PWM = 0xFB; // PWM = PWM OUTPUT ?
final short DIG = 0xFC; // 252 - DIG = DIGITAL
final short DXT = 0xFD; // 253 - DXT = DIGITAL EXTENDED
final short ANA = 0xFE; // 254 - ANA = ANALOG
final short ALL = 0xFF; // 255 - ALL = DIGITAL + ANALOG

/* PAYLOAD MESSAGE LENGTHS (Will possibly change)*/
final short OFFSET  = 0x02;
final short DIGSIZE = 0x0C; //12
final short DXTSIZE = 0x06; //6
final short ANASIZE = 0x06; //6 Pin count

/* SYN RESET CONTROL */
final short MAXMSGS =  0xFF;
/* COMMUNICATIONS TYPE */
final short HARDSERIAL = 0x00;
final short SOFTSERIAL = 0x01;
final short ISQUAREDC  = 0x02;

static boolean discovered = false;
short syn = 0;    // SYN_ID
short ack = 0;    // ACK_ID
short source = 0; // SENDER_ID
short preamble[];
short data[];
short payload_digital[];
short payload_analog[];

/********************************************************************************/
/* Variables used for the Communication between Processing and Arduino          */
/********************************************************************************/