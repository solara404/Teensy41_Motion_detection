//////////////////////////////////////////////////////////////////////////////
// sd card // variables
const byte switch_pin_sd = 11; //switch nr 2
bool toggle_bool = false;



// sd card // functions
void setup_sd_toggle_swtich() {
  pinMode(switch_pin_sd, INPUT_PULLUP); //connected to ground

}

void read_sd_switch() {
  //reads the toggle switch position and sets the write2sd_now bool
  //switch nr 2 on the prototype
  if (digitalRead(switch_pin_sd) == LOW) {
    write2sd_now = true;
    delay(300); //debouncing toggle switch
  } else {
    write2sd_now = false;
    delay(300); //debouncing toggle switch
  }
}

void file_name_gen() {
  //generates a unique filename automaticly
  //format: datalogger_yyyymmdd_hhmmss.csv

  String file_name_var = "";

  //declareing where the file came from
  file_name_var += "datalogger";

  file_name_var += "_";

  //get date and put into char
  file_name_var += get_time('d', 'y');
  file_name_var += get_time('d', 'm');
  file_name_var += get_time('d', 'd');

  file_name_var += "_";

  //get time and put into char
  file_name_var += get_time('t', 'h');
  file_name_var += get_time('t', 'm');
  file_name_var += get_time('t', 's');

  //declaring file type
  file_name_var += ".txt";

  //writing the file_name global variable
  file_name_var.toCharArray(file_name, max_char);

  if (s_info) {
    Serial.print("\n\n\nfile_name = ");
    Serial.print(file_name);
  }
}


void setup_sd() {
  // setting up sd

  // if no sd card available 
  while (!SD.begin(BUILTIN_SDCARD)) { //for teensy 4.1
    //status_led(1);  //red led
    Serial.println("failed to init sd!");

    // led indication
    for (byte i = 0; i < 5; i++) {
      digitalWrite(13, LOW);    // set the LED off
      delay(300);                // wait for a second
      digitalWrite(ledPin, HIGH);   // set the LED on
      delay(300);                // wait for a second
    }
  }

  //Wire.begin();
  Serial.println("initialized SD");
}

void write_sd(byte case_val, String in_string) {
  // writes the sd card
  // default: writes in_string to sd card and closes the file
  // case 1: stores all the data (all stored values) to sd-card
  // case 100: only opens a file on sd card and makes it writeable
  // case 255: only closes a file on sd card and makes it unwritable

  switch (case_val) {
    ////////////////////////////////
    default:
      dataFile = SD.open( file_name, FILE_WRITE);
      dataFile.println(in_string);
      //dataFile.println("");
      dataFile.close();

      if (s_info) {
        Serial.println();
        Serial.print("write_sd(): case default: data_string = ");
        Serial.println(in_string);
        //delay(info_delay);
        debug_waiter();
      }

      break;

    ////////////////////////////////
    case 1:
      ram_index = 0;
      //stores all the data (all stored values) to sd-card
      dataFile = SD.open( file_name, FILE_WRITE);

      //writing info_string on top of every file
      dataFile.print("Precision = ");
      dataFile.println(precision);
      dataFile.println(info_string);
      dataFile.println("***\n");

      for (unsigned int ii = 0; ii < prediction_counter; ii++) {
        for (unsigned int i = 0; i < current_data_length; i++) {
          buff += ram_data_array[i + (current_data_length * ii)];
          if ((i + 1) < current_data_length) {
            buff += ",";
          } else {
            buff += ";";
          }
        }

        if (s_info) {
          Serial.println();
          Serial.print("write_sd(): case 1: buff = ");
          Serial.println(buff);
          Serial.print("ii = "); Serial.println(ii);
          //Serial.print("i = "); Serial.println(i);
          //delay(info_delay);
          debug_waiter();
        }

        dataFile.println(buff);
        buff = "";
      }

      //reset all ram counter vars
      prediction_counter = 0;
      ram_index = 0;

      dataFile.close();
      break;

    ////////////////////////////////
    case 100:
      dataFile = SD.open( file_name, FILE_WRITE);
      break;

    ////////////////////////////////
    case 255:
      dataFile.close();
      break;
  }

}
