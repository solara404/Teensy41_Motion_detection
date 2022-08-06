//////////////////////////////////////////////////////////////////////////////
// bno sensor //

void setup_bno(byte bno_index) {
  //setup for the bno sensors

  switch (bno_index) {
    case 1:
      if (!bno1.begin()) {
        Serial.print("failed to init bno055: bno ");
        Serial.println(bno_index);
        //status_led(1);  //red led
        while (1);
        //sensor_1 = false;
        //break;
      } else {
        Serial.print("init bno055: bno ");
        Serial.println(bno_index);
        
        bno1.setExtCrystalUse(true);
        //status_led(2);  //green led
        //sensor_1 = true;
      } break;

    case 2:
      if (!bno2.begin()) {
        Serial.print("failed to init bno055: bno ");
        Serial.println(bno_index);
        //status_led(1);  //red led
        while (1);
        //sensor_2 = false;
        //break;
      } else {
        Serial.print("init bno055: bno ");
        Serial.println(bno_index);
        
        bno2.setExtCrystalUse(true);
        //status_led(2);  //green led
        //sensor_2 = true;
      } break;
    //
    case 3:
      if (!bno3.begin()) {
        Serial.print("failed to init bno055: bno ");
        Serial.println(bno_index);
        //status_led(1);  //red led
        while (1);
        //sensor_3 = false;
        //break;
      } else {
        Serial.print("init bno055: bno ");
        Serial.println(bno_index);
        
        bno3.setExtCrystalUse(true);
        //status_led(2);  //green led
        //sensor_3 = true;
      } break;

    case 4:
      if (!bno4.begin()) {
        Serial.print("failed to init bno055: bno ");
        Serial.println(bno_index);
        //status_led(1);  //red led
        while (1);
        //sensor_4 = false;
        //break;
      } else {
        Serial.print("init bno055: bno ");
        Serial.println(bno_index);
        
        bno4.setExtCrystalUse(true);
        //status_led(2);  //green led
        //sensor_4 = true;
      } break;
    //

    default:
      Serial.println("wrong bno index!");
      //status_led(1);  //red led
      while (1);
      break;
  }
}


void read_bno(byte sensor_var) {
  // func to read the bno sensors and m_types
  // Possible vector values can be:
  // - VECTOR_ACCELEROMETER - m/s^2
  // - VECTOR_MAGNETOMETER  - uT
  // - VECTOR_GYROSCOPE     - rad/s
  // - VECTOR_EULER         - degrees
  // - VECTOR_LINEARACCEL   - m/s^2
  // - VECTOR_GRAVITY       - m/s^2

  //G Range        2, 4, 8, 16g (9.81m/s^2)
  //g LSB range    2000, 4000, 8000, 16000
  
  //gyro range     2000, 1000, 500, 250, 125 dps (degrees per sec)
  //gyro LSB range 32000, 16000, 8000, 4000, 2000

  //orientation 0 - 359 
  
  

  if (s_info) {
    Serial.print("\n /////////////////////////////////////////////// \n");
    Serial.println();
    Serial.print("read_bno() START");
    Serial.println();
  }

  sensors_event_t orientationData, angVelocityData, linearAccelData, magnetometerData, accelerometerData, gravityData;
  double d1, d2, d3; //dumb values to spot errors
  bool ori, gyro, linacc, mag, acc, grav;

  ori   = false;
  gyro  = true;
  linacc = false;
  mag   = false;
  acc   = true;
  grav  = false;

  // messurment types
  //  ori     = in_m_types[0];
  //  gyro    = in_m_types[1];
  //  linacc  = in_m_types[2];
  //  mag     = in_m_types[3];
  //  acc     = in_m_types[4];
  //  grav    = in_m_types[5];

  for (byte j = 0; j < 6; j++) { //m_types, repeats 6 times, if all m_types are choosen then all will be read
    // dumb values to spot errors
    d1 = d2 = d3 = 404.44;

    if (s_info) { // for debugging
      Serial.println("read_bno():");
      Serial.print("i , j = ");
      Serial.print(sensor_var); Serial.print(", ");
      Serial.println(j);
      Serial.print("d1, d2, d3 = ");
      Serial.print(d1, digits_var); Serial.print(", ");
      Serial.print(d2, digits_var); Serial.print(", ");
      Serial.println(d3, digits_var);
      Serial.println();
      //      Serial.print("m_ori = "); Serial.println(m_ori);
      //      Serial.print("m_gyro = "); Serial.println(m_gyro);
      //      Serial.print("m_linacc = "); Serial.println(m_linacc);
      //      Serial.print("m_mag =  "); Serial.println(m_mag);
      //      Serial.print("m_acc = "); Serial.println(m_acc);
      //      Serial.print("m_grav = "); Serial.println(m_grav);

    }

    // orientation // 1
    if (ori) {
      switch (sensor_var) {
        case 1: bno1.getEvent(&orientationData,   Adafruit_BNO055::VECTOR_EULER); generate_indexer_info("bno1", 0); break;
        case 2: bno2.getEvent(&orientationData,   Adafruit_BNO055::VECTOR_EULER); generate_indexer_info("bno2", 0); break;
        case 3: bno3.getEvent(&orientationData,   Adafruit_BNO055::VECTOR_EULER); generate_indexer_info("bno3", 0); break;
        case 4: bno4.getEvent(&orientationData,   Adafruit_BNO055::VECTOR_EULER); generate_indexer_info("bno4", 0); break;
      }

      d1 = orientationData.orientation.x;
      d2 = orientationData.orientation.y;
      d3 = orientationData.orientation.z;
      generate_indexer_info("m_ori", 3);
      //m_ori = false;

      if (s_info) {
        Serial.println("\t ***");
        Serial.println("ori");
        Serial.print("d1, d2, d3 = ");
        Serial.print(d1, digits_var); Serial.print(", ");
        Serial.print(d2, digits_var); Serial.print(", ");
        Serial.println(d3, digits_var);
      }
      // gyroscope // 2
    } else if (gyro) {
      switch (sensor_var) {
        case 1: bno1.getEvent(&angVelocityData,   Adafruit_BNO055::VECTOR_GYROSCOPE); generate_indexer_info("bno1", 0); break;
        case 2: bno2.getEvent(&angVelocityData,   Adafruit_BNO055::VECTOR_GYROSCOPE); generate_indexer_info("bno2", 0); break;
        case 3: bno3.getEvent(&angVelocityData,   Adafruit_BNO055::VECTOR_GYROSCOPE); generate_indexer_info("bno3", 0); break;
        case 4: bno4.getEvent(&angVelocityData,   Adafruit_BNO055::VECTOR_GYROSCOPE); generate_indexer_info("bno4", 0); break;
      }
      d1 = angVelocityData.gyro.x;
      d2 = angVelocityData.gyro.y;
      d3 = angVelocityData.gyro.z;
      generate_indexer_info("gyro", 3);
      //m_gyro = false;

      if (s_info) {
        Serial.println("\t ***");
        Serial.println("gyro");
        Serial.print("d1, d2, d3 = ");
        Serial.print(d1, digits_var); Serial.print(", ");
        Serial.print(d2, digits_var); Serial.print(", ");
        Serial.println(d3, digits_var);
      }

      // linearacc // 3
    } else if (linacc) {
      switch (sensor_var) {
        case 1: bno1.getEvent(&linearAccelData,   Adafruit_BNO055::VECTOR_LINEARACCEL); generate_indexer_info("bno1", 0); break;
        case 2: bno2.getEvent(&linearAccelData,   Adafruit_BNO055::VECTOR_LINEARACCEL); generate_indexer_info("bno2", 0); break;
        case 3: bno3.getEvent(&linearAccelData,   Adafruit_BNO055::VECTOR_LINEARACCEL); generate_indexer_info("bno3", 0); break;
        case 4: bno4.getEvent(&linearAccelData,   Adafruit_BNO055::VECTOR_LINEARACCEL); generate_indexer_info("bno4", 0); break;
      }
      d1 = linearAccelData.acceleration.x;
      d2 = linearAccelData.acceleration.y;
      d3 = linearAccelData.acceleration.z;
      generate_indexer_info("linacc", 3);
      //m_linacc = false;

      if (s_info) {
        Serial.println("\t ***");
        Serial.println("linacc");
        Serial.print("d1, d2, d3 = ");
        Serial.print(d1, digits_var); Serial.print(", ");
        Serial.print(d2, digits_var); Serial.print(", ");
        Serial.println(d3, digits_var);
      }

      // magnetometer // 4
    } else if (mag) {
      switch (sensor_var) {
        case 1: bno1.getEvent(&magnetometerData,  Adafruit_BNO055::VECTOR_MAGNETOMETER); generate_indexer_info("bno1", 0); break;
        case 2: bno2.getEvent(&magnetometerData,  Adafruit_BNO055::VECTOR_MAGNETOMETER); generate_indexer_info("bno2", 0); break;
        case 3: bno3.getEvent(&magnetometerData,  Adafruit_BNO055::VECTOR_MAGNETOMETER); generate_indexer_info("bno3", 0); break;
        case 4: bno4.getEvent(&magnetometerData,  Adafruit_BNO055::VECTOR_MAGNETOMETER); generate_indexer_info("bno4", 0); break;
      }
      d1 = magnetometerData.magnetic.x;
      d2 = magnetometerData.magnetic.y;
      d3 = magnetometerData.magnetic.z;
      generate_indexer_info("mag", 3);
      //m_mag = false;

      if (s_info) {
        Serial.println("\t ***");
        Serial.println("mag");
        Serial.print("d1, d2, d3 = ");
        Serial.print(d1, digits_var); Serial.print(", ");
        Serial.print(d2, digits_var); Serial.print(", ");
        Serial.println(d3, digits_var);
      }

      // accelerometer // 5
    } else if (acc) {
      switch (sensor_var) {
        case 1: bno1.getEvent(&accelerometerData, Adafruit_BNO055::VECTOR_ACCELEROMETER); generate_indexer_info("bno1", 0); break;
        case 2: bno2.getEvent(&accelerometerData, Adafruit_BNO055::VECTOR_ACCELEROMETER); generate_indexer_info("bno2", 0); break;
        case 3: bno3.getEvent(&accelerometerData, Adafruit_BNO055::VECTOR_ACCELEROMETER); generate_indexer_info("bno3", 0); break;
        case 4: bno4.getEvent(&accelerometerData, Adafruit_BNO055::VECTOR_ACCELEROMETER); generate_indexer_info("bno4", 0); break;
      }
      d1 = accelerometerData.acceleration.x;
      d2 = accelerometerData.acceleration.y;
      d3 = accelerometerData.acceleration.z;
      generate_indexer_info("acc", 3);
      //m_acc = false;

      if (s_info) {
        Serial.println("\t ***");
        Serial.println("acc");
        Serial.print("d1, d2, d3 = ");
        Serial.print(d1, digits_var); Serial.print(", ");
        Serial.print(d2, digits_var); Serial.print(", ");
        Serial.println(d3, digits_var);
      }

      // gravity // 6
    } else if (grav) {
      switch (sensor_var) {
        case 1: bno1.getEvent(&gravityData,       Adafruit_BNO055::VECTOR_GRAVITY); generate_indexer_info("bno1", 0); break;
        case 2: bno2.getEvent(&gravityData,       Adafruit_BNO055::VECTOR_GRAVITY); generate_indexer_info("bno2", 0); break;
        case 3: bno3.getEvent(&gravityData,       Adafruit_BNO055::VECTOR_GRAVITY); generate_indexer_info("bno3", 0); break;
        case 4: bno4.getEvent(&gravityData,       Adafruit_BNO055::VECTOR_GRAVITY); generate_indexer_info("bno4", 0); break;
      }
      d1 = gravityData.acceleration.x;
      d2 = gravityData.acceleration.y;
      d3 = gravityData.acceleration.z;
      generate_indexer_info("grav", 3);
      //m_grav = false;

      if (s_info) {
        Serial.println("\t ***");
        Serial.println("grav");
        Serial.print("d1, d2, d3 = ");
        Serial.print(d1, digits_var); Serial.print(", ");
        Serial.print(d2, digits_var); Serial.print(", ");
        Serial.println(d3, digits_var);
      }
    }

    //handing data over to data array
    if (ori || gyro || linacc || mag || acc || grav) {
      //data[indexer()] = double2float(d1);
      //data[indexer()] = double2float(d2);
      //data[indexer()] = double2float(d3);

      data[indexer()] = d1 * pow(10, precision);
      data[indexer()] = d2 * pow(10, precision);
      data[indexer()] = d3 * pow(10, precision);

      if (s_info) {
        Serial.println("\t ***");
        Serial.println("data[]: ");

        byte newline_counter = 0;
        for (byte i = 0; i < data_index; i++) {

          Serial.print(data[i], digits_var);
          Serial.print(", ");
          newline_counter++;
          if (newline_counter == 3) {
            Serial.println();
            newline_counter = 0;
          }

        }

        Serial.println();
        Serial.print("double \t");
        Serial.print("d1, d2, d3 = ");
        Serial.print(d1, digits_var); Serial.print(", ");
        Serial.print(d2, digits_var); Serial.print(", ");
        Serial.println(d3, digits_var);
        Serial.print("float \t");
        Serial.print("d1, d2, d3 = ");
        Serial.print(double2float(d1), digits_var); Serial.print(", ");
        Serial.print(double2float(d2), digits_var); Serial.print(", ");
        Serial.println(double2float(d3), digits_var);
        Serial.println();
        //debug_waiter();
      }

      if (ori) {          // 1
        ori = false;
      } else if (gyro) {  // 2
        gyro = false;
      } else if (linacc) { // 3
        linacc = false;
      } else if (mag) {   // 4
        mag = false;
      } else if (acc) {   // 5
        acc = false;
      } else if (grav) {  // 6
        grav = false;
      }
    }

    if (s_info) {
      Serial.println();
      Serial.print("read_bno(): END");
      Serial.println();
      debug_waiter();
    }
  }
}
