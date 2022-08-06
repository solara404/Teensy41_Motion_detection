//////////////////////////////////////////////////////////////////////////////

// motor // variables
const byte m1_pin = 3;
const byte m2_pin = 2;
const byte m3_pin = 4;
const byte m4_pin = 5;
byte motor_array[4] = { m1_pin, m2_pin, m3_pin, m4_pin };

unsigned long kuckuck_ctime[4] = { 0, 0, 0, 0 };
unsigned long kuckuck_ptime[4] = { 0, 0, 0, 0 };

unsigned long hudhud_ctime = 0;//[4] = { 0, 0, 0, 0 };
unsigned long hudhud_ptime = 0;//[4] = { 0, 0, 0, 0 };
bool hudhud_bool[4] = {1, 1, 1, 1};


// motor // functions
void setup_motor() {
  //declaring all motor pins as output
  pinMode(m1_pin, OUTPUT);
  pinMode(m2_pin, OUTPUT);
  pinMode(m3_pin, OUTPUT);
  pinMode(m4_pin, OUTPUT);

  Serial.print("\nsetup_motor(): \n");

  //test run//
  //turn all pins to low (motor off)
  for (byte i = 0; i < (sizeof(motor_array) / sizeof(byte)); i++) {
    digitalWrite(motor_array[i], LOW);
  }

  //turning one at a time high (motor on)
  byte repeat_val = 5;
  uint16_t on_time = 100;
  uint16_t off_time = 50;

  for (byte i = 0; i < (sizeof(motor_array) / sizeof(byte)); i++) {

    Serial.println((String) "testing motor number: " + int(1 + i));
    for (byte ii = 0; ii < repeat_val; ii++) {
      digitalWrite(motor_array[i], HIGH);
      //Serial.println((String) "\nmotor number: " + int(i + 1) + " HIGH");
      delay(on_time);
      digitalWrite(motor_array[i], LOW);
      //Serial.println((String) "\nmotor number: " + int(i + 1) + " LOW");
      delay(off_time);
    }
    delay(2000);
  }

}

// function not compleate
// this function should turn the motor on after a specific time interval
// and the motor should run for another specific interfall
//void motor(byte motor_index, unsigned long time_diff) {
//  if (kuckuck(motor_index, time_diff, 'm')) {
//    digitalWrite(motor_array[motor_index], HIGH);
//  }
//}

void motor(byte motor_index, bool motor_on) {
  if (motor_on) {
    digitalWrite(motor_array[motor_index], HIGH);
  } else {
    digitalWrite(motor_array[motor_index], LOW);
  }
}

// timer for motor //
//
//void hudhud(byte timer_index, unsigned long time_diff, char case_var) {
//  // gives time based feedback true if time_diff has passed
//  // there are four different kuckuck timers available
//  // case 's' for seconds time differences
//  // case 'm' for milliseconds time differences
//  // case 'u' for microseconds time differences
//
//  switch (case_var) {
//    case 's':
//      time_diff *= 1000;
//      if (hudhud_ptime[timer_index] == 0) {
//        hudhud_ptime[timer_index] = millis();
//      } else {
//        hudhud_ctime[timer_index] = millis();
//      }
//      break;
//
//    case 'm':
//      if (hudhud_ptime[timer_index] == 0) {
//        hudhud_ptime[timer_index] = millis();
//      } else {
//        hudhud_ctime[timer_index] = millis();
//      }
//      break;
//
//    case 'u':
//      if (hudhud_ptime[timer_index] == 0) {
//        hudhud_ptime[timer_index] = micros();
//      } else {
//        hudhud_ctime[timer_index] = micros();
//      }
//      break;
//  }
//
//  if (time_diff >= ( hudhud_ctime[timer_index] - hudhud_ptime[timer_index] ) && hudhud_bool) {
//    hudhud_ptime[timer_index] = 0; // used as a marker
//    hudhud_bool[timer_index] = false; //time_diff reached
//  } else (time_diff >= ( hudhud_ctime[timer_index] - hudhud_ptime[timer_index] ) && !hudhud_bool) {
//    hudhud_ptime[timer_index] = 0; // used as a marker
//    hudhud_bool[timer_index] = true; //time_diff still not met
//  }
//}

bool motor_rythm(unsigned long time_diff) {
  // gives time based feedback true if time_diff has passed


  if (hudhud_ptime == 0) {
    hudhud_ptime = millis();
  }
  
  hudhud_ctime = millis();
  


  //calc time diff and returning bool
  if (time_diff <= ( hudhud_ctime - hudhud_ptime )) {
//    Serial.println((String) "\nfog_kuckuck_ctime " + fog_kuckuck_ctime);
//    Serial.println((String) "fog_kuckuck_ptime " + fog_kuckuck_ptime);
//    Serial.println((String) "p-c " + (fog_kuckuck_ctime - fog_kuckuck_ptime));
//    Serial.println("true"); delay(5000);
    fog_kuckuck_ptime = 0; // used as a marker
    return true; //time_diff reached
  }

  return false;
}


bool kuckuck(byte timer_index, unsigned long time_diff, char case_var) {
  // gives time based feedback true if time_diff has passed
  // there are four different kuckuck timers available
  // case 's' for seconds time differences
  // case 'm' for milliseconds time differences
  // case 'u' for microseconds time differences

  switch (case_var) {
    case 's':
      time_diff *= 1000;
      if (kuckuck_ptime[timer_index] == 0) {
        kuckuck_ptime[timer_index] = millis();
      } else {
        kuckuck_ctime[timer_index] = millis();
      }
      break;

    case 'm':
      if (kuckuck_ptime[timer_index] == 0) {
        kuckuck_ptime[timer_index] = millis();
      } else {
        kuckuck_ctime[timer_index] = millis();
      }
      break;

    case 'u':
      if (kuckuck_ptime[timer_index] == 0) {
        kuckuck_ptime[timer_index] = micros();
      } else {
        kuckuck_ctime[timer_index] = micros();
      }
      break;
  }

  if (time_diff >= ( kuckuck_ctime[timer_index] - kuckuck_ptime[timer_index] )) {
    kuckuck_ptime[timer_index] = 0; // used as a marker
    return true; //time_diff reached
  } else {
    return false; //time_diff still not met
  }
}
