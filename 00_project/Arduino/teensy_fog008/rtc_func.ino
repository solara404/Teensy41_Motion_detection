/*
   TimeRTC.pde
   example code illustrating Time library with Real Time Clock.

*/


void setup_time()  {
  // set the Time library to use Teensy 3.0's RTC to keep time
  setSyncProvider(getTeensy3Time);

  if (timeStatus() != timeSet) {
    Serial.println("Unable to sync with the RTC");
    Serial.println((String) "date: yyyymmdd = " + get_time('d','y') + get_time('d','m') + get_time('d','d'));
    Serial.println((String) "time: hhmmss   = " + get_time('t','h') + get_time('t','m') + get_time('t','s'));
  } else {
    Serial.println("RTC has set the system time");
    Serial.println((String) "date: yyyymmdd = " + get_time('d','y') + get_time('d','m') + get_time('d','d'));
    Serial.println((String) "time: hhmmss   = " + get_time('t','h') + get_time('t','m') + get_time('t','s'));
  }
}

time_t getTeensy3Time()
{
  return Teensy3Clock.get();
}

/*  code to process time sync messages from the serial port   */
#define TIME_HEADER  "T"   // Header tag for serial time sync message

unsigned long processSyncMessage() {
  unsigned long pctime = 0L;
  const unsigned long DEFAULT_TIME = 1357041600; // Jan 1 2013

  if (Serial.find(TIME_HEADER)) {
    pctime = Serial.parseInt();
    return pctime;
    if ( pctime < DEFAULT_TIME) { // check the value is a valid time (greater than Jan 1 2013)
      pctime = 0L; // return 0 to indicate that the time is not valid
    }
  }
  return pctime;
}


String get_time(char date_time_var, char time_unit_var) {
  //returns a string with the date or the time with leading zero
  //date_time_var 'd' = date, 't' = time
  //time_unit_var 'y' = year, 'm' = month or minute, 'd' = day, 'h' = hour, 's' = second

  String return_string = "";

  //date: year, month, day /////////////////////////////////////////////
  switch (date_time_var) {
    case 'd':

      //year //////////////////////////////////
      switch (time_unit_var) {
        case 'y':
          return_string = String(year());
          return return_string;
          break;

        //month //////////////////////////////
        case 'm':
          if (month() < 10) {
            return_string = "0" + String(month());
          } else {
            return_string = String(month());
          }
          return return_string;
          break;

        //day ////////////////////////////////
        case 'd':
          if (day() < 10) {
            return_string = "0" + String(day());
          } else {
            return_string = String(day());
          }
          return return_string;
          break;
      }
      break;



    //time: hour, minute, second /////////////////////////////////////////
    case 't':

      switch (time_unit_var) {
        // hour ///////////////////////////
        case 'h':
          if (hour() < 10) {
            return_string = "0" + String(hour());
          } else {
            return_string = String(hour());
          }
          return return_string;
          break;

        // minutes ////////////////////////
        case 'm':
          if (minute() < 10) {
            return_string = "0" + String(minute());
          } else {
            return_string = String(minute());
          }
          return return_string;
          break;

        // seconds ////////////////////////
        case 's':
          if (second() < 10) {
            return_string = "0" + String(second());
          } else {
            return_string = String(second());
          }
          return return_string;
          break;

      }
      break;
  }

  // gives a feedback of a false input
  Serial.println("\nWrong date_time_var input in time_digits() function!");
  return return_string;
}
