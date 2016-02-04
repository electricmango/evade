/* virtually copied from http://playground.arduino.cc/Main/MultiplexMAX72xx

   yeah i cite ppl correctly
   
 */

/////////////////////////////ISR Timer Functions ///////////////////////////

ISR(TIMER2_COMPA_vect) {  //This ISR toggles shutdown between the 2MAX7221's
  if(maxInShutdown == red){
    lc.shutdown(green,true);  // The order here is critical - Shutdown first!
    lc.shutdown(red,false);   // . . . Then restart the other.
    maxInShutdown = green;
  } else {
    lc.shutdown(red,true);
    lc.shutdown(green,false);
    maxInShutdown = red;
  }
}  

void setISRtimer(){  // setup ISR timer controling toggleing
  TCCR2A = 0x02;                        // WGM22=0 + WGM21=1 + WGM20=0 = Mode2 (CTC)
  TCCR2B = 0x05;                // CS22=1 + CS21=0 + CS20=1 = /128 prescaler (125kHz)
  TCNT2 = 0;                            // clear counter
  OCR2A = ISR_FREQ;                     // set TOP (divisor) - see #define
}
void startISR(){  // Starts the ISR
  TCNT2 = 0;                            // clear counter (needed here also)
  TIMSK2 |= (1 << OCIE2A);                  // set interrupts=enabled (calls ISR(TIMER2_COMPA_vect)
} void stopISR(){    // Stops the ISR
  TIMSK2 &= ~(1 << OCIE2A);                  // disable interrupts
}

