//////////////////////////////////////////////////////////////////////////////
// interrupt //

void setup_debug_waiter() {
  pinMode(interrupt_pin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interrupt_pin), interrupt_fkt, FALLING);
}

void debug_waiter() {
  if (!interrupt_paused) {
    while (interrupt_waiter);
    delay(200);
    interrupt_waiter = true;
  }
}

void interrupt_fkt() {
  interrupt_waiter = false;
}
