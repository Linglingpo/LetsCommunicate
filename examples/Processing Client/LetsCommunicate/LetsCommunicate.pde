import processing.serial.*;
Serial communicate;

void setup() {
  size(200, 200);

  String portName = Serial.list()[4]; //3
  println(Serial.list());
  communicate = new Serial(this, portName, 115200);
  communicate.bufferUntil('\n');
}

void draw() {
}

/********************************************************************************/
/*  THE METHODS BELOW HANDLE ALL COMMUNICATIONS STATE WITH A CONNECTED ARDUINO  */
/********************************************************************************/

void serialEvent(Serial p) {
  /* ENSURE WHILE(... > 0) && port.bufferUntil('\n') */
  
  while(p.available() > 0) {
    /* CHECK FOR HELLO = 126 | '~' */
    int available = p.available();
    short temp = (short) p.read();
    if ( (temp == HELLO) && ( !discovered ) ) {
      discover(p, temp, available, discovered);
    } else if ( (temp == HELLO) && ( discovered ) ) {
      discover(p, temp, available, discovered);
    }
  }
  p.clear();
}

short[] receive(Serial p, short t, int available) {
  
  /* INIT RESPONSE BUFFER BASED ON SERIAL AVAILABLE */
  short[] receive = new short[available - 1];
  receive[0] = t;

  System.out.println("Serial BYTES #: " + (available - 1));
  
  for (int i = 1; i < available - 1; i++)
    receive[i] = (short) p.read();

  //if ( receive[ 6 ] != CNT ) { p.clear(); }
  
  /* CHECK IF WE HAVE A PAYLOAD FOLLOWING THE PREAMBLE */
  //if ( receive[ 6 ] == CNT ) {
  //  receive.add( p.read() );
  //  int _temp = receive.get( 7 );
  //  switch(_temp) {
  //   case DIG: 
  //     if( p.read() != DIGSIZE ) return null;
  //     //payload = new short[DIGSIZE];
  //   break;
  //   case DXT: 
  //     //if( (short)p.read() != DIGSIZE + DXTSIZE ) return false;
  //     //payload = new short[DIGSIZE + DXTSIZE];
  //   break;
  //   case ANA:

  //   break;
  //   case ALL: 
  //   break;
  //  }

  //  for (int i = 0; i < 12; i++) {
  //   //payload[i] = (short) p.read();
  //   System.out.print(payload[i]);
  //   System.out.print(" ");
  //   }
  //   System.out.print("PAYLOAD ");
  //  }
  return receive;
}

void send(Serial p, short[] r) {
System.out.print("PROCESS SENT: ");
    for (int i = 0; i < PREAMBLE_SIZE; i++) {
      System.out.print( r[i] );
      System.out.print(" ");
      this.communicate.write( r[ i ] );
    }

System.out.print(" ");
System.out.print("\n\n");
}

short[] check(short[] incoming, boolean d) {
 
  /* RESPONSE BACK FROM PROCESSING BASED ON INCOMING MSG */
 short[] response = {0};
 
 /* CHECK PREAMBLE SIZE = WHAT WE EXPECT FOR THE PREAMBLE SIZE! */ 
 if ( incoming[ 1 ] != PREAMBLE_SIZE ) return null;
 
 /* DEBUG! WHAT PROCESSING RECEIVED */
 System.out.print("ARDUINO SENT: ");
 for (int i = 0; i < incoming.length; i++) {
    System.out.print( incoming[i] );
    System.out.print(" ");
  }
  System.out.println(" ");
  
 /* SELECT WHAT TO DO - DISCOVERED OR NOT */
 if(!d) {
   /* THIS MUST BE A SYNCHRONISE MSG FROM SOURCE */
   if ( ( response = this.peek(incoming) ) != null ) { return response; }
 }  
 
  if(d) {
   /* THIS MUST BE A SYNCHRONISE MSG FROM SOURCE */
   if ( ( response = this.peek(incoming) ) != null ) { return response; }
 }  
return response;
}


/* ----- INITIAL SYNCHRONISATION ----- */
void discover(Serial p, short t, int available, boolean d) {
  short[] response; 
  if ( (response = check( receive(p, t, available), d )) != null ) { send( p, response ); }
}


short[] peek(short[] type) {
 
 /* BUILD PROCESSING RESPONSE */
 short _temp[] = new short[PREAMBLE_SIZE];
 _temp[0] = HELLO;
 _temp[1] = PREAMBLE_SIZE;
 _temp[2] = MYID;
 _temp[3] = ( !discovered ) ? source = type[2] : source;

 switch(type[6]) {
 case SYN:
   /* SYNCHRONISE */
   _temp[4] = ( !discovered ) ? syn = (short)random(0, 255) : ++syn;
   _temp[5] = ( !discovered ) ? ack = type[4] : ack; 
   _temp[6] = SYN; // Payload Type
   System.out.println("SYNCHRONISE!");
   break;
 case URG:
   /* URGENT */
   break;
 case RST:
   /* RESET */
   break;
 case CNT:
   ///* CONTINUE */
   //System.out.println("CONTINIUING...");
   ////check Syn
   //if ( checkSyn() == type[2] ) {
   //  _temp[4] = checkSyn(); //SNY
   //  _temp[5] = ( ack = type[2] ); //ACK
   //  _temp[6] = CNT;
   //}
   //break;
 case FIN:
   /* FINISH */
   _temp[4] = ++syn;
   _temp[5] = (ack = type[4]);
   _temp[6] = FIN;
   System.out.println("FINISHED!");
   if ( !discovered ) discovered = true;
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
short preamble[][] = new short[3][PREAMBLE_SIZE];
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