/*
The Bleep Drum v12
bleeplabs.com
August 2015
Dr. Bleep (John-Mike Reed) drbleep@bleeplabs.com

See http://bleeplabs.com/store/bleep-drum-midi/ for more info
All work licensed under a Creative Commons Attribution-ShareAlike 3.0

Now compatible with current versions of MIDI, bounce and pgmspace.
It is no longer necessary to edit MIDI.h

*/



// !!
// Most problems compiling come from having an old version of the midi library of multiple ones installed.
// !!

#include <MIDI.h>
MIDI_CREATE_DEFAULT_INSTANCE();




#include <avr/pgmspace.h>
#include "samples.h"
#include <SPI.h>
#include <Bounce2.h>
#define BOUNCE_LOCK_OUT

Bounce debouncerRed = Bounce(); 
Bounce debouncerGreen = Bounce(); 
Bounce debouncerBlue = Bounce(); 
Bounce debouncerYellow = Bounce(); 

const char noise_table[] PROGMEM = {};

int bass_length = 7031;
int sample_holder1, sample_holder2;
byte eee, ee, ef, eef;
byte shift, bankpg, bankpr, bout, rout, gout, prevpot2;
byte      banko = 0;
byte n1, n2;
int n3;
byte bankpb = 4;
byte beat;
int pot1 = 127;
int pot2 = 4;
long pot3, pot4;
int kick_sample, snare_sample, sample, hat_sample, noise_sample, bass_sample, B2_freq_sample, B1_freq_sample;
uint16_t increment, increment2, increment3, increment4, increment5, increment2v, increment4v;
uint32_t accumulator, accumulator2, accumulator3, accumulator4, accumulator5, accu_freq_1, accu_freq_2;
int rando;
//byte B1_sequence[16]={0,1,0,1 ,1,1,0,0 ,0,1,0,1 ,1,1,1,1};
//byte B4_sequence[16]={1,0,1,0 ,1,0,1,0 ,1,0,1,0 ,1,1,1,1};
byte B2_sequence[128] = {};
byte B3_sequence[128] = {};
byte B1_sequence[128] = {};
byte B4_sequence[128] = {
  1, 0, 0, 0 , 0, 0, 0, 0 , 1, 0, 0, 0 , 0, 0, 0, 0 , 1, 0, 0, 0 , 0, 0, 0, 0 , 1, 0, 0, 0 , 0, 0, 0, 0,
  1, 0, 0, 0 , 0, 0, 0, 0 , 1, 0, 0, 0 , 0, 0, 0, 0 , 1, 0, 0, 0 , 0, 0, 0, 0 , 1, 0, 0, 0 , 0, 0, 0, 0,
  0, 0, 0, 0 , 0, 0, 0, 0 , 0, 0, 0, 0 , 0, 0, 0, 0 , 0, 0, 0, 0 , 0, 0, 0, 0 , 0, 0, 0, 0 , 0, 0, 0, 0,
  1, 1, 1, 1 , 1, 1, 1, 1 , 1, 1, 1, 1 , 1, 1, 1, 1 , 0, 0, 0, 0 , 0, 0, 0, 0 , 0, 0, 0, 0 , 0, 0, 0, 0,
};

int B2_freq_sequence[128] = {
  0, 0, 0, 0 , 0, 0, 0, 0 , 0, 0, 0, 0 , 0, 0, 0, 0 , 0, 0, 0, 0 , 0, 0, 0, 0 , 0, 0, 0, 0 , 0, 0, 0, 0,
  0, 0, 0, 0 , 0, 0, 0, 0 , 0, 0, 0, 0 , 0, 0, 0, 0 , 0, 0, 0, 0 , 0, 0, 0, 0 , 0, 0, 0, 0 , 0, 0, 0, 0,
  0, 0, 0, 0 , 0, 0, 0, 0 , 0, 0, 0, 0 , 0, 0, 0, 0 , 0, 0, 0, 0 , 0, 0, 0, 0 , 0, 0, 0, 0 , 0, 0, 0, 0,
  0, 0, 0, 0 , 0, 0, 0, 0 , 0, 0, 0, 0 , 0, 0, 0, 0 , 0, 0, 0, 0 , 0, 0, 0, 0 , 0, 0, 0, 0 , 0, 0, 0, 0,

};
int B1_freq_sequence[128] = {};
int sample_sum;
int j, k, freq3, cc;
int kf, pf, holdkf, kfe;
int shiftcount = 0;
int t1, c1, count1, dd;
byte noise_type;
uint16_t index, index2, index3, index4, index5, index4b, index_freq_1, index_freq_2, index4bv;
uint16_t indexr, index2r, index3r, index4r, index4br, index2vr, index4vr;
int osc, oscc;
byte ledstep;
byte noise_mode = 1;
unsigned long freq, freq2;

int wavepot, lfopot, arppot;

byte loopstep = 0;
byte loopstepf = 0;

byte recordbutton, prevrecordbutton, record, looptrigger, prevloopstep, revbutton, prevrevbutton, preva, prevb;
int looprate;
//int clapbank[16]={1,0,0,0,1,0,1,0,1,1,1,1,0,0,0,0};
long prev, prev2, prev3;
byte playmode = 1;
byte play = 0;
byte playbutton, pplaybutton;
byte prevbanktrigger, banktrigger;
byte pkbutton, kbutton, B4_trigger, B4_latch, cbutton, pcbutton, B4_loop_trigger, B1_trigger, kick, B1_latch, clap, B1_loop_trigger, B4_seq_trigger, B3_seq_trigger;
byte ptbutton, tbutton, ttriger, B1_seq_trigger, B3_latch, B2_trigger, bc, B2_loop_trigger, B3_loop_trigger;
byte B2_latch, B3_trigger, B2_seq_trigger, pbutton, ppbutton;
byte kicktriggerv, B2_seq_latch, kickseqtriggerv,  B1_seq_latch, pewseqtriggerv, precordbutton;
byte measure, half;
byte recordmode = 1;
byte tap, tapbutton, ptapbutton, eigth;
long tapholder, prevtap;
unsigned long taptempo = 8000000;
unsigned long ratepot;
byte r, g, b, erase, e, preveigth;
//Bounce bouncertap = Bounce(8, 100);
byte trigger_input, trigger_output,   trigger_out_latch, tl;
byte onetime = 1;
//Bounce bouncer1 = Bounce(2, 200); //not actuall 2 seconds since timers are running at 64kHz.
//Bounce bouncer4 = Bounce(18, 200);
//Bounce bouncer2 = Bounce(19, 200);
//Bounce bouncer3 = Bounce( 17, 200);
byte button1, button2, button3, button4, tapb;
byte pbutton1, pbutton2, pbutton3, pbutton4, ptapb;
byte prev_trigger_in_read, trigger_in_read, tiggertempo, trigger_step, triggerled, ptrigger_step;

byte bf1, bf2, bf3, bf4, bft;
uint16_t midicc3 = 128;
uint16_t midicc4 = 157;
uint16_t midicc1, midicc2, midicc5, midicc6, midicc7, midicc8;

byte midi_note_check;

byte prevshift, shift_latch;
byte tick;
byte t;
long tapbank[4];
//int what,pwhat;
byte  mnote, mvelocity, miditap, pmiditap, miditap2, midistep, pmidistep, miditempo, midinoise;

unsigned long recordoffsettimer, offsetamount, taptempof;
int potX;

#define red_pin 2
#define blue_pin 19
#define green_pin 17
#define yellow_pin 18

void setup() {
  //dac.setGain(1);


  // analogWrite(9,2); //Blue


  randomSeed(analogRead(0));
  // delay(100);


  //  delay(200);
  // MIDI.setHandleControlChange(cc1);
  //  MIDI.setHandleNoteOn(noteon);
  //  MIDI.setHandleNoteOff(noteOff);

  cli();
  taptempo = 4000000;

  pinMode (12, OUTPUT); pinMode (13, OUTPUT); pinMode (11, OUTPUT); pinMode (10, OUTPUT);
  pinMode (9, OUTPUT); pinMode (5, OUTPUT);  pinMode (6, OUTPUT);
  pinMode (16, OUTPUT);

  pinMode (3, INPUT);     digitalWrite(3, HIGH);  //play
  pinMode (4, INPUT);     digitalWrite (4, HIGH); //rec
  pinMode (8, INPUT);     digitalWrite (8, HIGH); //tap
  pinMode (7, INPUT);     digitalWrite(7, HIGH);   //shift
  pinMode (12, INPUT);    digitalWrite (12, HIGH);

  pinMode (green_pin, INPUT_PULLUP);   //low left clap green
  pinMode (yellow_pin, INPUT_PULLUP);   // low right kick yellow
  pinMode (blue_pin, INPUT_PULLUP);    //Up Right tom Blue
  pinMode (red_pin, INPUT_PULLUP);   // Up right pew red

  debouncerGreen.attach(green_pin);
  debouncerGreen.interval(2); // interval in ms
  debouncerYellow.attach(yellow_pin);
  debouncerYellow.interval(2); // interval in ms  
  debouncerBlue.attach(blue_pin);
  debouncerBlue.interval(2); // interval in ms
  debouncerRed.attach(red_pin);
  debouncerRed.interval(2); // interval in ms

  delay(100);

  if (digitalRead(17) == LOW) {
    analogWrite(6, 64); //green
    MIDI.begin(3);
    delay(20000);

  }
  else if (digitalRead(2) == LOW) {
    analogWrite(5, 64); //RED
    MIDI.begin(1);
    delay(20000); // we're messing with the timers so this isn't actually 20000 Millis

  }
  else if (digitalRead(19) == LOW) {
    analogWrite(9, 64); //Blue
    MIDI.begin(2);
    delay(20000);

  }
  else if (digitalRead(18) == LOW) {
    analogWrite(5, 48); //yellow
    analogWrite(6, 16);

    MIDI.begin(4);
    delay(20000);

  }

  else {
    MIDI.begin(0);
  }

  MIDI.turnThruOff();
  
  //pinMode (16, INPUT); digitalWrite (16, HIGH);
  digitalWrite(16, HIGH);
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  /* Enable interrupt on timer2 == 127, with clk/8 prescaler. At 16MHz,
     this gives a timer interrupt at 15625Hz. */
  TIMSK2 = (1 << OCIE2A);
  OCR2A = 128;
  //OCR2B = 127;

  /* clear/reset timer on match */
  TCCR2A = 1 << WGM21 | 0 << WGM20; /* CTC mode, reset on match */
  TCCR2B = 0 << CS22 | 1 << CS21 | 1 << CS20; /* clk, /8 prescaler */

  //dac
  //   SPCR = 0x50;
  //   SPSR = 0x01;
  //   DDRB |= 0x2E;
  ///   PORTB |= (1<<1);

  //pwm
  TCCR0B = B0000001;
  TCCR1B = B0000001;

  //    TCCR0B = TCCR0B & 0b11111000 | 0x03;
  //   TCCR1B = TCCR1B & 0b11111000 | 0x03;


  sei();
  if (digitalRead(7) == 0) {
    noise_mode = 1;
  }
  else {
    noise_mode = 0;
  }


}




