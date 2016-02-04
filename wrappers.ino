void setboth(int x, int y, int state){
  stopISR();
  lc.setLed(red,x,y,state);
  lc.setLed(green,x,y,state);
  startISR();
}

void setone(int address, int x, int y, int state){
  stopISR();
  lc.setLed(address,x,y,state);
  startISR();
}

void clearall(){
  stopISR();
  lc.clearDisplay(red);
  lc.clearDisplay(green);
}

/////////   Wrappers for LedControl functions . . . //////////
void SetLed(byte Color, byte Row,byte Col, byte State){
  stopISR();            // disable interrupts - stop toggling shutdown when updating
  lc.setLed(Color,Row,Col,State);
  startISR();           // enable interrupts again
}

void SetRow(byte Color, byte Row, byte State){
  stopISR();            // disable interrupts - stop toggling shutdown when updating
  lc.setRow(Color,Row,State);
  startISR();           // enable interrupts again
}

void SetColumn(byte Color, byte Col, byte State){
  stopISR();            // disable interrupts - stop toggling shutdown when updating
  lc.setColumn(Color,Col,State);
  startISR();           // enable interrupts again
}

void ClearMatrix(){
  stopISR();            // disable interrupts - stop toggling shutdown when updating
  lc.clearDisplay(red);
  lc.clearDisplay(green);
  startISR();           // enable interrupts again
}
