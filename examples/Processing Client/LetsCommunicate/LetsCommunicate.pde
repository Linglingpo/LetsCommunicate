import processing.serial.*;

Serial port;

void setup() {
  size(200, 200);

  String portName = Serial.list()[3];
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
    if ( (_temp == HELLO) && ( !discovered ) ) {
      discover(p);
    } else if ( (_temp == HELLO) && ( discovered ) ) {
      handle(p);
    }
  }
}

boolean receive(Serial p) {
  boolean _receive = false;
  /* CHECK PREAMBLE SIZE */
  if ( (short)p.read() != PREAMBLE_SIZE ) return false;

  /* OK - PREAMBLE SIZE APPEARS TO BE CORRECT - LETS CONTINUE */
  preamble = new short[PREAMBLE_SIZE - OFFSET]; // no ~, no Preamble Size

  for (int i = 0; i < PREAMBLE_SIZE - OFFSET; i++) {
    preamble[i] = (short)p.read();
    System.out.print(preamble[i]);
    System.out.print(" ");
  }
  println();

  /* CHECK IF WE HAVE A PAYLOAD FOLLOWING THE PREAMBLE */
  if ( preamble[4] == CNT ) {
    System.out.print("PAYLOAD ");

    if (p.available() > 0) { 

      short[] temp = new short[14];

      for (int i = 0; i < 14; i++) {
        temp[i] = 0;
        temp[i] = (short) p.read();
        System.out.print(temp[i]);
        System.out.print(" ");
      }
    }
  }

  //short _temp = (short)p.read();
  //switch(_temp) {
  //  case DIG: 
  //    //if( (short)p.read() != DIGSIZE ) return false;
  //    //payload = new short[DIGSIZE];
  //  break;
  //  case DXT: 
  //    //if( (short)p.read() != DIGSIZE + DXTSIZE ) return false;
  //    //payload = new short[DIGSIZE + DXTSIZE];
  //  break;
  //  case ANA:

  //  break;
  //  case ALL: 
  //  break;
  //}

  //for (int i = 0; i < 12; i++) {
  //  //payload[i] = (short) p.read();
  //  System.out.print(payload[i]);
  //  System.out.print(" ");
  //  }
  //  System.out.print("PAYLOAD ");
  //}
  return true;
}

void send(Serial p) {
  /* RESPONSE FROM PROCESSING */
  short[] _preamble = peek(this.preamble);
  if (_preamble != null) {
    for (int i = 0; i < PREAMBLE_SIZE; i++) {
      System.out.print(_preamble[i]);
      System.out.print(" ");
      port.write(_preamble[i]);
      _preamble[i] = 0;
    }
  }
  println();  
  println();
}

/* ----- INITIAL SYNCHRONISATION ----- */
void discover(Serial p) {
  if (receive(p)) { 
    send(p);
  }
}

void handle(Serial p) {
  if (receive(p)) { 
    send(p);
  }
}

//------------ Start messy around with the SYN -----------// 
short checkSyn() {

  if (syn == ack +1) {
    synCorrected = true; 
  }
  
  if (synCorrected){
  if (syn == maxSynNumer) {
      syn = 0;
    } else {
      ++syn;
    }
  } else {
    print("Arduino send more than one msg... ");
    println();
  }

  print ("In Chenck SYN: ");
  println(syn);
  return syn;
}

//------------ Start messy around with the SYN -----------// 
short[] peek(short[] type) {
  short _temp[] = new short[PREAMBLE_SIZE];

  _temp[0] = HELLO;
  _temp[1] = PREAMBLE_SIZE;
  _temp[2] = MYID;
  _temp[3] = ( !discovered ) ? source = type[0] : source; // Arduino ID

  switch(type[4]) {
  case SYN:
    /* SYNCHRONISE */
    System.out.println("SYNCHRONISING...");
    checkSyn();
    //_temp[4] = ( !discovered ) ? syn = (short)random(0, 255) : ++syn; //SNY
    _temp[4] = ( !discovered ) ? syn = (short)random(0, maxSynNumer) : checkSyn(); //SNY


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
    System.out.println("CONTINIUING...");
    //check Syn
    if ( checkSyn() == type[2] ) {
      _temp[4] = checkSyn(); //SNY
      _temp[5] = ( ack = type[2] ); //ACK
      _temp[6] = CNT;
    }
    break;
  case FIN:
    /* FINISH */
    System.out.println("FINISHING...");
    //checking the Syn form arduino
    if ( checkSyn() == type[2] ) {

      _temp[4] = checkSyn();

      if ( !discovered ) {
        discovered = true;
      }
    }
    _temp[5] = (ack = type[2]);
    _temp[6] = FIN;
    break;
  default:
    return null;
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
short payload[];

//Syn number goes wrong when it is too high
short maxSynNumer = 200;
boolean synCorrected = false;

short payloadType;
short dataSize;
short data[];
short payload_digital[];
short payload_analog[];

/********************************************************************************/
/* Variables used for the Communication between Processing and Arduino          */
/********************************************************************************/