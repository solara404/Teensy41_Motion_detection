//////////////////////////////////////////////////////////////////////////////
// serial setup //

void setup_serial(bool wait4serial, int baud_val) {
  //serial print setup
  //controller waits for serial if wait4serial is set true
  Serial.begin(baud_val);
  if (wait4serial) {
    while (!Serial);
  }
}

void setup_serial1(bool wait4serial, int baud_val) {
  //serial print setup
  //controller waits for serial if wait4serial is set true
  Serial1.begin(baud_val);
  if (wait4serial) {
    while (!Serial1);
    setup_led(50, 10);
  }
}

void write_serial1() {
  //writes information to the arduino nano 33 Ble
  for (byte i = 0; i < current_data_length; i++) {
    buff += data[i];
    if (i < current_data_length - 1) {
      buff += ",";
    } else {
      buff += ";"; //semicolon is used to start new line 
    }
  }

  //only for debugging
  //print buff to monitor
  if (s_info) {
    Serial.println();
    Serial.println("write_serial1: ");
    Serial.print(buff);
    debug_waiter();
  }

  Serial1.println(buff);
  buff = "";

}
