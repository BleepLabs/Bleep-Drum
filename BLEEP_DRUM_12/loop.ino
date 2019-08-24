
void loop() {

  midi_note_check = midi_note_on();

  pbutton1 = button1;
  pbutton2 = button2;
  pbutton3 = button3;
  pbutton4 = button4;

  debouncerRed.update();
  debouncerBlue.update();
  debouncerGreen.update();
  debouncerYellow.update();
  
  button1 = debouncerRed.read();
  // button1 = digitalRead(2);
  
  button2 = debouncerBlue.read();
  //button2 = digitalRead(19);
  
  button3 = debouncerGreen.read();
  //button3 = digitalRead(17);
  
  button4 = debouncerYellow.read();
  //button4 = digitalRead(18);
  
  tapb = digitalRead(8);

  if (button1 == 0 && pbutton1 == 1) {
    bf1 = 1;
  }
  else {
    bf1 = 0;
  }
  if (button2 == 0 && pbutton2 == 1) {
    bf2 = 1;
  }
  else {
    bf2 = 0;
  }

  if (button3 == 0 && pbutton3 == 1) {
    bf3 = 1;
  }
  else {
    bf3 = 0;
  }
  if (button4 == 0 && pbutton4 == 1) {
    bf4 = 1;
  }
  else {
    bf4 = 0;
  }
  if (tapb == 0 && ptapb == 1) {
    bft = 1;
  }
  else {
    bft = 0;
  }
  if (midi_note_check == 58) {
    miditap2 = 1;
    //   digitalWrite(5,HIGH);
  }
  else {
    //      digitalWrite(9,HIGH);
    miditap2 = 0;
  }
  if (midi_note_check == 57) {
    midistep = 1;
    miditempo = 1;
    digitalWrite(5, HIGH);
  }

  else {
    //      digitalWrite(9,HIGH);
    midistep = 0;
  }

  if (midi_note_check == 67) {

    play++;
    play %= 2;
    //   digitalWrite(5,HIGH);
  }

  if (midi_note_check == 69) {
    playmode++;
    playmode %= 2;
    //   digitalWrite(5,HIGH);
  }


  if (midi_note_check == 70) {
    midinoise = 1;
    shift_latch = 1;
    noise_mode++;
    noise_mode %= 2;
    //   digitalWrite(5,HIGH);
  }

  if (midi_note_check == 72) {
    banko = 0; //blue
  }
  if (midi_note_check == 74) {
    banko = 31; // yellow
  }
  if (midi_note_check == 76) {
    banko = 63; //red
  }
  if (midi_note_check == 77) {
    banko = 95; //green
  }



  ptapb = tapb;
  pmiditap = miditap;
  pmidistep = midistep;

  LEDS();
  BUTTONS();
  RECORD();

  if (noise_mode == 0) {
    pot1 = ((analogRead(1)) >> 1) + 2;
    pot2 = ((analogRead(0)) >> 2) + 96;
  }

  if (noise_mode == 1) {

    if (midinoise == 1) {
      pot1 = (midicc1 >> 1) + 1;
      pot2 = (midicc2 >> 2) + 1;
      pot3 = (midicc3 + 1) << 4;
      pot4 = (midicc4 + 1) << 2;

    }
    if (midinoise == 0) {


      if (shift_latch == 0) {
        pot1 = ((analogRead(1)) >> 1) + 1;
        pot2 = ((analogRead(0)) >> 2) + 1;
      }
      if (shift_latch == 1) {

        pot3 = (analogRead(1)) << 4; ////////////////MAKE ME BETTERERER
        pot4 = analogRead(0) << 2;

      }
    }
  }

  trigger_in_read = digitalRead(16);

  if (trigger_in_read == 1 && prev_trigger_in_read == 0) {
    trigger_input = 1;
  }
  else {
    trigger_input = 0;

  }
  prev_trigger_in_read = trigger_in_read;

  eigth = loopstep % 4;

  if (tiggertempo == 0) {

    if (eigth == 0) {
      digitalWrite(12, HIGH);
      // tl++;
    }
    else {
      digitalWrite(12, LOW);
    }

  }

  //////////////////////////////////////////// intput trigger



  prev_trigger_in_read = trigger_in_read;

  trigger_in_read = digitalRead(12);

  if (trigger_in_read == 0 && prev_trigger_in_read == 1) {
    tiggertempo = 1;
    trigger_step = 1;
    //digitalWrite(6,HIGH);

  }

  else {
    trigger_step = 0;
    //digitalWrite(6,LOW);
  }



  /////////////////////////////////////////////////////////////////  loopstep


  taptempof = taptempo;

  recordoffsettimer = micros() - prev ;
  offsetamount = taptempof - (taptempof >> 2 );

  if ((recordoffsettimer) > (offsetamount))
  {

    loopstepf = loopstep + 1;
    loopstepf %= 32;

  }

  if (play == 1) {

    if (onetime == 1) {
      taptempo = 4000000;
      onetime = 0;
    }
    else {
      prevloopstep = loopstep;
      preva = eigth;

      if (recordmode == 1 && miditempo == 0 && tiggertempo == 0) {
        if (micros() - prev > (taptempof) ) {
          prev = micros();

          loopstep++;
          if (loopstep > 31)
          {
            loopstep = 0;
          }
        }

      }

      if (miditempo == 1) {

        if (midistep == 1) {

          loopstep++;
          if (loopstep > 31)
          {
            loopstep = 0;
          }
        }
      }


      if (tiggertempo == 1) {

        if (trigger_step == 1 && ptrigger_step == 0) {

          triggerled = 30;

          loopstep++;
          if (loopstep > 31)
          {
            loopstep = 0;
          }

        }
        else
          triggerled = 0;

      }

      ptrigger_step = trigger_step;

    }

    B4_loop_trigger = B4_sequence[loopstep + banko];
    B1_loop_trigger = B1_sequence[loopstep + banko];
    B2_loop_trigger = B2_sequence[loopstep + banko];
    B3_loop_trigger = B3_sequence[loopstep + banko];
  }

  if (play == 0) {
    loopstep = 31;
    prev = 0;
    B4_loop_trigger = 0;
    B1_loop_trigger = 0;
    B2_loop_trigger = 0;
    B3_loop_trigger = 0;

  }

  if (loopstep != prevloopstep && B3_loop_trigger == 1) {

    B3_seq_trigger = 1;
    //freq3=kickfreqsequence[loopstepf];
  }
  else {
    B3_seq_trigger = 0;
  }

  if (loopstep != prevloopstep && B2_loop_trigger == 1) {

    B2_seq_trigger = 1;
    //freq3=kickfreqsequence[loopstepf];
  }
  else {
    B2_seq_trigger = 0;
  }

  if (loopstep != prevloopstep && B4_loop_trigger == 1) {

    B4_seq_trigger = 1;
    //freq3=kickfreqsequence[loopstepf];
  }
  else {
    B4_seq_trigger = 0;
  }

  if (loopstep != prevloopstep && B1_loop_trigger == 1) {

    B1_seq_trigger = 1;
  }
  else {
    B1_seq_trigger = 0;
  }



  if (B3_trigger == 1 || B3_seq_trigger == 1) {
    index3 = 0;
    accumulator3 = 0;
    B3_latch = 1;
  }

  if (B4_trigger == 1 || B4_seq_trigger == 1) {
    index4 = 0;
    accumulator4 = 0;
    B4_latch = 1;
  }
  if (B1_trigger == 1) {
    index = 0;
    accumulator = 0;
    B1_latch = 1;
  }

  if (B1_seq_trigger == 1) {
    index_freq_1 = 0;
    accu_freq_1 = 0;
    B1_seq_latch = 1;
  }
  if (B2_seq_trigger == 1) {
    index_freq_2 = 0;
    accu_freq_2 = 0;
    B2_seq_latch = 1;
  }

  if (B2_trigger == 1) {
    index2 = 0;
    accumulator2 = 0;
    B2_latch = 1;
  }





  //////////////////////////////////////////////////////////////// T A P



  if (shift == 1) {

    if (bft == 1 || miditap2 == 1) {
      tiggertempo = 0;

      t++;
      t %= 2;
      tapbank[t] = ((micros()) - prevtap) >> 2;
      taptempo = ((tapbank[0] + tapbank[1]) >> 1);
      prevtap = micros();

    }
    else {
    }
  }


}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void RECORD() {

  playbutton = digitalRead(3);
  if (playbutton != pplaybutton && playbutton == LOW && shift == 1) {
    miditempo = 0;
    play++;
    play %= 2;
  }
  else {
  }
  pplaybutton = playbutton;


  recordbutton = digitalRead(4);
  if (recordbutton == LOW && recordbutton != precordbutton) {
    record++;
    record %= 2;
    play = 1;
  }

  else {
  }

  if (play == 0) {
    record = 0;
  }


  ////////////////////////////////////////////////////////////////////erase

  precordbutton = recordbutton;

  if (playbutton == LOW && recordbutton == LOW) {
    eee++;
    if (eee >= 84) {
      erase = 1;
      play = 1;
      record = 0;
      B1_sequence[ee + banko] = 0;
      B2_sequence[ee + banko] = 0;
      B4_sequence[ee + banko] = 0;
      B3_sequence[ee + banko] = 0;
      ee++;
      if (ee == 32) {
        ee = 0;
        eee = 0;
      }
    }

  }
  else {
    erase = 0;
  }



  if (record == 1 && tiggertempo == 0 && miditempo == 0)
  {

    if (B1_trigger == 1) {
      B1_sequence[loopstepf + banko] = 1;
      B1_freq_sequence[loopstepf + banko] = pot1;

    }

    if (B2_trigger == 1) {
      B2_sequence[loopstepf + banko] = 1;
      B2_freq_sequence[loopstepf + banko] = (pot2);
    }

    if (B4_trigger == 1) {
      B4_sequence[loopstepf + banko] = 1;
    }

    if (B3_trigger == 1) {
      B3_sequence[loopstepf + banko] = 1;
    }

  }

  if (record == 1)
  {
    if (tiggertempo == 1 || miditempo == 1)
    {

      if (B1_trigger == 1) {
        B1_sequence[loopstep + banko] = 1;
        B1_freq_sequence[loopstep + banko] = pot1;

      }

      if (B2_trigger == 1) {
        B2_sequence[loopstep + banko] = 1;
        B2_freq_sequence[loopstep + banko] = (pot2);
      }

      if (B4_trigger == 1) {
        B4_sequence[loopstep + banko] = 1;
      }

      if (B3_trigger == 1) {
        B3_sequence[loopstep + banko] = 1;
      }

    }

  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void LEDS() {
  //  analogWrite(9,0);
  //       analogWrite(5,0);
  analogWrite(9, bout >> 1); //Blue
  analogWrite(6, (gout >> 1) + triggerled); //green
  analogWrite(5, rout >> 1);

  if (noise_mode == 1) {
    rout = r;
    gout = g;
    bout = b;
    if (shift_latch == 1) {
      if (record == 0 && play == 0 ) {
        r += sample >> 3;
        b += 4;
      }
    }
    if (shift_latch == 0) {

      if (record == 0 && play == 0 ) {
        g += sample >> 3;
        b += 2;

      }

    }
  }

  else {
  }

  preveigth = eigth;

  if (erase == 1) {
    e = 16;
  }
  if (erase == 0) {
    e = 0;
  }

  if (g > 1) {
    g--;
  }
  if (g <= 1) {
    g = 0;
  }

  if (r > 1) {
    r--;
  }
  if (r <= 1) {
    r = 0;
  }

  if (b > 1) {
    b--;
  }
  if (b <= 1) {
    b = 0;
  }
  if (noise_mode == 0) {
    if (record == 0 && play == 0 ) {

      rout = 16;
      gout = 16;
      bout = 16;
    }

  }


  if (play == 1 && record == 0) {
    bout = b;
    rout = r;
    gout = g;


    ///////////////////////////////////////////////////CHANGE TO ONLY += WHEN LOOPSTEP != PREVLOOPSTEP

    if ( loopstep == 0 && prevloopstep == 1 ) {
      r = 64;
      g = 64;
      b = 64;

    }
    /*
    else  if( loopstep==16 ){
     r=4;
     g=64;
     b=4;
     }
     */
    else if ( loopstep % 4 == 0 && prevloopstep % 4 != 0 ) {
      r += 64;
      g += 64;
      b += 64;

    }
    else {
      b = bankpb;
      r = bankpr;
      g = bankpg;
    }


  }

  if (play == 1 && record == 1 ) {
    bout = b;
    rout = r;
    gout = g;

    if ( loopstep == 0 ) {
      r = 32;
      g = 16;
      b = 6;
    }
    else  if ( loopstep == 16 ) {
      r = 32;
      g = 0;
      b = 0;
    }

    else if ( loopstep == 4 || loopstep == 8 || loopstep == 12   || loopstep == 20 || loopstep == 24 || loopstep == 28) {
      r = 48;
      g = 0;
      b = 0;
    }
    else {
      b = bankpb;
      r = bankpr;
      g = bankpg;
    }


  }


}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void BUTTONS() {
  shift = digitalRead(7);

  if (shift == 0 && prevshift == 1) {
    shift_latch++;
    shift_latch %= 2;
  }

  prevshift = shift;
  ///////////////////////////////////////////////////sequence select

  if (shift == 0 && recordbutton == 1) {
    prevpot2 = pot2;
    if (button1 == 0 ) { //red
      banko = 63;

    }
    if (button4 == 0) { //yellow
      banko = 31;
      bankpr = 4;
      bankpg = 4;
      bankpb = 0;
    }
    if (button2 == 0 || banko == 0) { //blue
      banko = 0;
      bankpr = 0;
      bankpg = 0;
      bankpb = 8;
    }
    if (button3 == 0) { //green
      banko = 95;
      bankpr = 0;
      bankpg = 3;
      bankpb = 0;

    }


    if (tapb == LOW) {
      play = 1;
      ratepot = (analogRead(14));
      taptempo = ratepot << 14;
    }
    revbutton = digitalRead(3);
    if (revbutton == 0 && prevrevbutton == 1) {
      playmode++;
      playmode %= 2;

    }
    prevrevbutton = revbutton;
  }

  if ( banko == 63) {

    bankpr = 4;
    bankpg = 0;
    bankpb = 0;
  }
  if ( banko == 31) {
    bankpr = 4;
    bankpg = 4;
    bankpb = 0;
  }
  if ( banko == 0) {
    bankpr = 0;
    bankpg = 0;
    bankpb = 8;
  }

  if ( banko == 95) {
    bankpr = 0;
    bankpg = 3;
    bankpb = 0;

  }


  if (shift == 1) {
    //       if (bf1==1){

    if (bf1 == 1 || midi_note_check == 60) {
      B1_trigger = 1;
    }
    else {
      B1_trigger = 0;
    }
    //    if (bf4==1){

    if (bf4 == 1 || midi_note_check == 65) {
      B4_trigger = 1;
    }
    else {
      B4_trigger = 0;
    }
    //    if (bf2==1){

    if (bf2 == 1 || midi_note_check == 62) {
      B2_trigger = 1;
    }
    else {
      B2_trigger = 0;
    }
    //   if (bf3==1){
    if (bf3 == 1 || midi_note_check == 64) {
      B3_trigger = 1;
    }
    else {
      B3_trigger = 0;
    }

  }



  ////////////////////////////////////////////

}

int midi_note_on() {

  int type, note, velocity, channel, d1, d2;
  byte r = MIDI.read();
  if (r == 1) {                  // Is there a MIDI message incoming ?
    byte type = MIDI.getType();
    switch (type) {
      case 0x90: //note on. For some reasong "NoteOn" won't work enven though it's declared in midi_Defs.h
        note = MIDI.getData1();
        velocity = MIDI.getData2();
        if (velocity == 0) {
          note = 0;
        }

        break;
      case 0x80: //note off
        note = 0;
        break;

      case 0xB0: //control change
        if (MIDI.getData1() == 70) {
          midicc1 = (MIDI.getData2() << 2) + 3;
        }

        if (MIDI.getData1() == 71) {
          midicc2 = (MIDI.getData2() << 2) + 3;
        }

        if (MIDI.getData1() == 72) {
          midicc3 = (MIDI.getData2() << 2);
        }

        if (MIDI.getData1() == 73) {
          midicc4 = (MIDI.getData2() << 2);
        }
      /*
                       if (MIDI.getData1()==74){
              midicc5 = (MIDI.getData2());
             }

                       if (MIDI.getData1()==75){
              midicc6 = (MIDI.getData2());
             }

                       if (MIDI.getData1()==76){
              midicc7 = (MIDI.getData2());
             }

                       if (MIDI.getData1()==77){
              midicc8 = (MIDI.getData2());
             }
         */


      default:
        note = 0;

    }
  }

  else {
    note = 0;
  }

  return note;
}
