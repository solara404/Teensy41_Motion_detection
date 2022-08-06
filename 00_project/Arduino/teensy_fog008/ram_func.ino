//////////////////////////////////////////////////////////////////////////////
// ram //

void ram_prediction(int * in_data) {
  // calculates how often values can be moved to ram till full
  if (prediction == 0) {
    //    unsigned int in_data_size = sizeof(in_data[0]); // get size of array[0]
    //    unsigned int in_data_size2 = sizeof(in_data); //get size of whole array
    //    prediction = ram_size / in_data_size;
    //    unsigned int prediction2 = ram_size / in_data_size2;
    unsigned int in_data_size = sizeof(in_data[0]) * data_index; //get size in byte of data
    prediction = ram_byte_size / in_data_size;

    if (s_info) {
      Serial.println();
      Serial.println("ram_prediction():");
      Serial.print("data_index = ");
      Serial.println(data_index);
      Serial.print("in_data_size = ");
      Serial.println(in_data_size);
      //    Serial.print("in_data_size2 = ");
      //    Serial.println(in_data_size2);
      Serial.print("prediction = ");
      Serial.println(prediction);
      //    Serial.print("prediction2 = ");
      //    Serial.println(prediction2);

      debug_waiter();
    }
  }
}

void write_ram(int * in_data) {
  // writes to ram until ram full, than writes all values to sd-card
  if (s_info) {
    Serial.println();
    Serial.println("write_ram(): ");
    //delay(info_delay);
    debug_waiter();

  }

  if (prediction_counter < prediction ) {
    if (s_info) {
      Serial.print("prediction, prediction_counter = ");
      Serial.print(prediction);
      Serial.print(", ");
      Serial.println(prediction_counter);
      delay(15);
      debug_waiter();

    }

    for (int i = 0; i < current_data_length; i++) {
      ram_data_array[ram_index + i] = in_data[i];

      if (s_info) {
        Serial.print("ram_index = ");
        Serial.print(ram_index + i);
        Serial.print("\t i , in_data[] = ");
        Serial.print(i);
        Serial.print(", ");
        Serial.println(in_data[i], digits_var);
      }

    }
    ram_index += current_data_length;
    prediction_counter++;
    debug_waiter();

    //} else {
    //reset everything
    //set ram_full bool
    //ram_full = true;
    //    prediction_counter = 0;
    //    ram_index = 0;
  }
}


void write_ram_test(double * in_data) {
  // fills the ram with values from in_data
  // only for testing

  //s_info = true;
  //interrupt_paused = false;

  // only for debuggin
  // waits for debugging till predictior is almost full
  //if (prediction_counter > prediction - sub_val) {
  //s_info = true;
  //interrupt_paused = false;
  //debug_waiter();
  //}



  if (s_info) {
    Serial.println();
    Serial.println("write_ram_test(): ");
    debug_waiter();
  }

  for (unsigned int j = prediction_counter; j < prediction; j++) {
    if (s_info) {
      Serial.print("prediction, prediction_counter = ");
      Serial.print(prediction);
      Serial.print(", ");
      Serial.println(prediction_counter);
      debug_waiter();
    }

    //int in_data_array_size = data_length; //giveMeArraySize(in_data);

    for (int i = 0; i < current_data_length; i++) {
      if (i == 0) {
        ram_data_array[ram_index + i] = j;
      } else {
        ram_data_array[ram_index + i] = i;
      }

      if (s_info) {
        Serial.print("ram_index = ");
        Serial.print(ram_index + i);
        Serial.print("\t i , in_data[] = ");
        Serial.print(i);
        Serial.print(", ");
        Serial.println(in_data[i], digits_var);
      }

      //ram_index++;
    }
    ram_index += current_data_length;
    prediction_counter++;

    //        if (s_info) {
    //          Serial.println();
    //          Serial.print("ram_index = ");
    //          Serial.println(ram_index);
    //          Serial.print("prediction_counter = ");
    //          Serial.println(prediction_counter);
    //          debug_waiter();
    //        }

  }

  s_info = true;
  interrupt_paused = false;
  //ram_full = true;
  Serial.println("***ram full***");
  debug_waiter();
}
