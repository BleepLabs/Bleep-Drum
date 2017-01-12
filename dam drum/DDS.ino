ISR(TIMER2_COMPA_vect) {
  OCR2A = 40;
  if (playmode == 0) { //reverse
    snare_sample = (pgm_read_byte(&snare_table[(indexr)])) - 127;
    kick_sample = (pgm_read_byte(&kick_table[(index2r)])) - 127;
    hat_sample = (pgm_read_byte(&tick_table[(index3r)])) - 127;
    bass_sample = (((pgm_read_byte(&bass_table[(index4r)])))) - 127;

    B1_freq_sample = pgm_read_byte(&tick_table[(index2vr)]) - 127;
    B2_freq_sample = (pgm_read_byte(&bass_table[(index4vr)])) - 127;
  }

  if (playmode == 1) {
    snare_sample = (pgm_read_byte(&snare_table[(index3)])) - 127;
    kick_sample = (pgm_read_byte(&kick_table[(index4)])) - 127;
    hat_sample = (pgm_read_byte(&tick_table[(index)])) - 127;
    bass_sample = (((pgm_read_byte(&bass_table[(index2)])))) - 127;
    B1_freq_sample = pgm_read_byte(&tick_table[(index_freq_1)]) - 127;
    B2_freq_sample = (pgm_read_byte(&bass_table[(index_freq_2)])) - 127;

    noise_sample = (((pgm_read_byte(&noise_table[(index5)])))) - 127;

  }
  sample_sum = snare_sample + kick_sample + hat_sample + bass_sample + B1_freq_sample + B2_freq_sample;

  if (noise_mode == 1) {

    sample_holder1 = (sample_sum ^ (noise_sample >> 1)) + 127;


    if (B1_latch == 0 && B2_latch == 0  && B3_latch == 0  && B4_latch == 0 ) {
      sample = 127 ;
    }
    else {
      sample = sample_holder1;
    }

  }

  if (noise_mode == 0) {
    sample = ((snare_sample + kick_sample + hat_sample + bass_sample + B1_freq_sample + B2_freq_sample)) + 127;

  }
  byte sample_out = constrain(sample, 0, 255);


  uint16_t dac_out = (0 << 15) | (1 << 14) | (1 << 13) | (1 << 12) | ( sample_out << 4 );
  digitalWrite(10, LOW);
  SPI.transfer(dac_out >> 8);
  SPI.transfer(dac_out & 255);
  digitalWrite(10, HIGH);

  ///////////////////////////////////////////////////////////////////////////////

  if (playmode == 0) {
    indexr = (index3 - snare_length) * -1;
    index2r = (index4 - kick_length) * -1;
    index3r = (index - tick_length) * -1;
    index4r = (index2 - bass_length) * -1;
    index2vr = (index_freq_1 - tick_length) * -1;
    index4vr = (index_freq_2 - bass_length) * -1;

  }

  if (B1_latch == 1) {
    if (midicc1 > 4) {
      accumulator += midicc1;
    }
    else {
      accumulator += pot1;
    }
    index = (accumulator >> (6));
    if (index > tick_length) {

      index = 0;
      accumulator = 0;
      B1_latch = 0;
    }
  }

  if (B2_latch == 1) {
    if (midicc2 > 4) {
      accumulator2 += midicc2;
    }
    else {
      accumulator2 += pot2;
    }
    index2 = (accumulator2 >> (6));
    if (index2 > bass_length) {

      index2 = 0;
      accumulator2 = 0;
      B2_latch = 0;
    }
  }

  if (B3_latch == 1) {
    accumulator3 += (midicc3);
    index3 = (accumulator3 >> 6);
    if (index3 > snare_length) {

      index3 = 0;
      accumulator3 = 0;
      B3_latch = 0;
    }
  }

  if (B4_latch == 1) {
    accumulator4 += (midicc4);
    // index4b=(accumulator4 >> (6));
    index4 = (accumulator4 >> (6));
    if (index4 > kick_length) {

      index4 = 0;
      accumulator4 = 0;
      B4_latch = 0;
    }
  }

  accu_freq_1 += kf;
  index_freq_1 = (accu_freq_1 >> (6));
  if (B1_seq_trigger == 1 && tiggertempo == 0) {
    kf = B1_freq_sequence[loopstepf + banko];
    kfe = kf;
  }

  if (B1_seq_trigger == 1 && tiggertempo == 1) {
    kf = B1_freq_sequence[loopstep + banko];
    kfe = kf;
  }

  if (index_freq_1 > tick_length) {
    kf = 0;
    index_freq_1 = 0;
    accu_freq_1 = 0;
    B1_seq_latch = 0;
  }


  accu_freq_2 += pf;
  index_freq_2 = (accu_freq_2 >> (6));

  if (B2_seq_trigger == 1 && tiggertempo == 0) {
    pf = B2_freq_sequence[loopstepf + banko];
  }

  if (B2_seq_trigger == 1 && tiggertempo == 1) {
    pf = B2_freq_sequence[loopstep + banko];
  }
  if (index_freq_2 > bass_length) {
    pf = 0;
    index_freq_2 = 0;
    accu_freq_2 = 0;
    B2_seq_latch = 0;
  }


  if (noise_mode == 1) {
    accumulator5 += (pot3);
    // index4b=(accumulator4 >> (6));
    index5 = (accumulator5 >> (6));
    if (index5 > pot4) {
      index5 = 0;
      accumulator5 = 0;
    }
  }
}



