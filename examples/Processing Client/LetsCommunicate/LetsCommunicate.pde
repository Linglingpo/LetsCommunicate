import processing.serial.*;
import java.util.Arrays;
Serial communicate;

boolean got192 = false;

void setup() {
  size(200, 200);

  String portName = Serial.list()[5]; //3
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

  while (p.available () > 0) {
    /* CHECK FOR HELLO = 126 | '~' */
    int available = p.available();
    short temp = (short) p.read();
    if (temp == HELLO)
      discover(p, temp, available, discovered);
  }
  p.clear();
}

short[] receive(Serial p, short t, int available) {

  /* INIT RESPONSE BUFFER BASED ON SERIAL AVAILABLE */
  short[] temp = new short[available -1];
  //126
  temp[0] = t; 

  System.out.print("Temp Array: ");

  System.out.print(temp[0]);
  System.out.print(" ");

  for (int i = 1; i < temp.length; i++) {
    temp[i] = (short) p.read();
    System.out.print(temp[i]);
    System.out.print(" ");
  }

  if (temp[4] == '!' && temp[6] == '?') {
    System.out.println(" ------- I got ! and ? ------- ");
    got192 = true;
    //skip and put the value in the next index
  } else if (temp[4] != '!' && temp[6] != '?') {
    System.out.println(" ------- I got regular number ------- ");
  }

  short[] receive = new short[7];

  if (got192) {
    for (int i =0; i < 4; i ++) {
      receive[i] = temp[i];
    }
    //skip the ! = temp [4]
    receive [4] = (short) (temp [5] + 1) ;
    //skip the ? = temp [6]
    receive [5] = temp [7] ;
    receive [6] = temp [8] ;
    got192 = false;
  } else {
    receive = temp;
  }

  for (int i = 0; i < receive.length; i++) {
    System.out.print(receive[i]);
    System.out.print(" ");
  }

  System.out.println();
  System.out.println("Serial BYTES #: " + (available - 1));
  
  p.clear();
  return receive;
}




void send(Serial p, short[] r) {
  for (int i = 0; i < PREAMBLE_SIZE; i++) {
    p.write( r[ i ] );
  }
}

/* ------------------------- SYNCHRONISATION ------------------------- */
void discover(Serial p, short t, int available, boolean d) {
  short[] response;
  if ( (response = check( receive(p, t, available), d )) != null ) { 
    send( p, response );
  }
}

short[] check(short[] incoming, boolean d) {

  /* RESPONSE BACK FROM PROCESSING BASED ON INCOMING MSG */
  short[] response = {
    0
  };

  /* CHECK PREAMBLE SIZE = WHAT WE EXPECT FOR THE PREAMBLE SIZE! */
  if ( incoming[ 1 ] != PREAMBLE_SIZE ) return null;
  System.out.println("THE INDEX: " + index);

  /* SAVE TO HISTORY */
  in[ index % 3 ] = Arrays.copyOfRange(incoming, 0, PREAMBLE_SIZE);
  if (in[ index % 3 ][6] == CNT) {
    payload[ index % 3 ] = Arrays.copyOfRange(incoming, 7, incoming.length);
  } else {
    payload[ index % 3 ] = null;
  }

  /* DEBUG! WHAT PROCESSING RECEIVED */
  System.out.print("ARDUINO SENT: ");
  for (int i = 0; i < in[index % 3].length; i++) {
    System.out.print( in[index % 3][i] );
    System.out.print(" ");
  }
  System.out.println(" ");

  /* DEBUG! WHAT PROCESSING RECEIVED */
  System.out.print("ARDUINO SENT: ");
  if ( payload[ index % 3 ] != null) {
    for (int i = 0; i < payload[index % 3].length; i++) {
      System.out.print( payload[index % 3][i] );
      System.out.print(" ");
    }
    System.out.println(" ");
  }

  /* WE NEED TO VERIFY THE INCOMING DATA TO WHAT WE KNOW OF THE NETWORK */
  if (!d || ( in[ index % 3 ][2] != source && in[ index % 3 ][3] != MYID ) ) {

    if ( ( response = this.peek( incoming ) ) != null ) {

      /* SAVE TO HISTORY */
      out[ index % 3 ] = Arrays.copyOfRange(response, 0, PREAMBLE_SIZE);

      /* DEBUG! WHAT PROCESSING SENT */
      System.out.print("PROCESS SENT: ");
      for (int i = 0; i < out[index % 3].length; i++) {
        System.out.print( out[index % 3][i] );
        System.out.print(" ");
      }
      System.out.println(" ");
      System.out.println(" ");
      index++;
      return response;
    }
  }

  if (d) {
    /* THIS MUST BE A SYNCHRONISE MSG FROM SOURCE */
    if ( ( response = this.peek(incoming) ) != null ) { 
      index++; 
      return response;
    }
  }

  return response;
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
    ++syn;
    _temp[4] = ( !discovered ) ? syn = (short)random(150, 255) : ++syn;
    //_temp[4] = ( !discovered ) ? syn = (short)9 : ++syn;

    ack = type[4];
    _temp[5] = ack;
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
    System.out.println("CONTINIUED!");
    ++syn;
    ack = type[4];
    _temp[4] = ++syn; //SNY
    _temp[5] = ack; //ACK
    _temp[6] = CNT;
    break;

  case FIN:
    /* FINISH */
    ++syn;
    _temp[4] = ++syn;
    ack = type[4];
    _temp[5] = ack;
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
//final short PAYLOAD_ANALOG_SIZE  = 0x0C; //12 data size

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

/* PREAMBLE HISTORY - SENT & RECEIVED */
short in[][]  = new short[HISTORY_SIZE][PREAMBLE_SIZE];
short out[][] = new short[HISTORY_SIZE][PREAMBLE_SIZE];
static short index   = 0; // HISTORY INDEX

/* PAYLOAD HISTORY - RECEIVED - RAGGED 2D ARRAY */
short payload[][]  = new short[HISTORY_SIZE][];

short payload_digital[];
short payload_analog[];

/********************************************************************************/
/* Variables used for the Communication between Processing and Arduino          */
/********************************************************************************/
