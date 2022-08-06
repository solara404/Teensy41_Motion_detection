//const byte fog_array_size = 15;
//bool fog_bool_array[fog_array_size]; //var for detection of fog
// bool fog_detected = false;
byte fog_index = 0;
int threashold = 2200; //looking for acc over this value (multi with precision)
unsigned long fog_counter = 0; //
unsigned long fog_detected_threashold = 15;

unsigned long fog_time_window = 300; //milliseconds time window


unsigned long fog_kuckuck_ptime = 0;
unsigned long fog_kuckuck_ctime = 0;


// functions //

bool fog_kuckuck(unsigned long time_diff) {
  // gives time based feedback true if time_diff has passed


  if (fog_kuckuck_ptime == 0) {
    fog_kuckuck_ptime = millis();
  }
  
  fog_kuckuck_ctime = millis();
  


  //calc time diff and returning bool
  if (time_diff <= ( fog_kuckuck_ctime - fog_kuckuck_ptime )) {
//    Serial.println((String) "\nfog_kuckuck_ctime " + fog_kuckuck_ctime);
//    Serial.println((String) "fog_kuckuck_ptime " + fog_kuckuck_ptime);
//    Serial.println((String) "p-c " + (fog_kuckuck_ctime - fog_kuckuck_ptime));
//    Serial.println("true"); delay(5000);
    fog_kuckuck_ptime = 0; // used as a marker
    return true; //time_diff reached
  }

  return false;
}

void fog_reader() {
  //looks if fog occures

  unsigned int fog_index;

  fog_index = ram_index - current_data_length;
  debug_waiter();


  for (byte i = 1; i < current_data_length; i++) {
    //Serial.print((String) ", " + ram_data_array[fog_index + i]);
    if (ram_data_array[fog_index + i] > threashold) {
      fog_counter++;
      Serial.println((String) "\ncounter raiser number: " + ram_data_array[fog_index + i]);
      Serial.println((String) "fog_counter = " + fog_counter);
    }

    if (s_info) {
      Serial.println((String) "fog_index = " + fog_index);
      //Serial.println((String) "fog_bool_array[" + (fog_index - 1) + "] = " + fog_bool_array[fog_index]);
      Serial.println((String) "ram_data_array[" + fog_index + "] = " + ram_data_array[fog_index]);
      //Serial.println(in_data[i], digits_var);
    }

  }

  if ( fog_kuckuck(fog_time_window) ) {
    if (fog_counter > fog_detected_threashold) {
      fog_detected = true;
      Serial.println("\nfog detected!");
    } else {
      fog_detected = false;

      //Serial.println((String) "fog_counter = " + fog_counter);
      //Serial.print(".");
    }

    //reset the fog_counter
    fog_counter = 0;
  }
}
