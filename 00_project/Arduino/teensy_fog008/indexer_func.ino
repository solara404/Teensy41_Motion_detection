//////////////////////////////////////////////////////////////////////////////
// indexer //

byte indexer() {
  //returns index for filling a array with dataFile
  //after running command data_index variable is raised by one

  // needed global vars //
  // data_index

  //  byte return_index;
  //  return_index = data_index++;
  //  return return_index;

  if (s_info) { //only for debugging, returns information in serial monitor
    Serial.println();
    Serial.print("indexer(): data_index = ");
    Serial.println(data_index);
    //delay(info_delay);
    debug_waiter();
  }

  return data_index++;
}

void generate_indexer_info(String info_text, byte index_count) {
  // info-generation for user
  // generates an info_string to know what is stored where
  // generates info_string only once but
  // needs to be stopped with func: indexer_set_end

  // needed global vars //
  // generate_info

  if (generate_info) {
    if (index_count == 0) {
      info_string += info_text;
      info_string += " ";
    } else {
      info_string += info_text;
      info_string += ": ";
      for (byte i = 0; i < index_count; i++) {
        info_string += data_index + i + 1; info_string += ",";
      }
      //      info_string += data_index + 2; info_string += ",";
      //      info_string += data_index + 3; info_string += " ";

      info_string += " ";

    }
  }
  // Serial.println(info_string); //displays the infostring
  // if(Serial){ dealy(3000); }

  if (s_info) { //only for debugging, returns information in serial monitor
    Serial.println();
    Serial.print("generate_indexer_info(): generate_info = ");
    Serial.println(generate_info);
    Serial.print("info_string = ");
    Serial.println(info_string);
    //delay(info_delay);
    debug_waiter();

  }

}

void indexer_set_end() {
  // stops generation of infostring
  // resets data_index to zero

  if (generate_info) {
    generate_info = false;
    current_data_length = data_index;
  }

  data_index = 0;

  if (s_info) { //only for debugging, returns information in serial monitor
    Serial.println();
    Serial.print("indexer_set_end(): generate_info = ");
    Serial.print(generate_info);
    Serial.print("// data_index = ");
    Serial.println(data_index);
    //delay(info_delay);
    debug_waiter();
  }
}

void m_time(int * in_data) {
  generate_indexer_info("time_ms", 1);
  in_data[indexer()] = millis();
}
