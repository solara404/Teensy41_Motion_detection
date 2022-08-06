#include <ArduinoBLE.h>         //ble

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned long current_time = 0;
//float data[40];
String buff = "";
char rbuff = '-';
unsigned int baudrate = 230400;



////ble
//// get the uuid from: https://www.uuidgenerator.net/version1
#define IMUService         "180F"
#define IMUSensorData      "2A19"
//#define IMUSwitchSdWrite   "2A20"
//#define IMUSwitchFOGmarker "2A21"
//#define IMUSwitchVibration "2A22"
//
//// https://www.arduino.cc/en/Reference/ArduinoBLEBLECharacteristicBLECharacteristic
BLEService imu_service(IMUService);
BLEStringCharacteristic imu_sensor_data         (IMUSensorData,      BLERead | BLENotify, 512);
//BLEByteCharacteristic   switchCharacteristicSD  (IMUSwitchSdWrite,   BLERead | BLEWrite);
//BLEByteCharacteristic   switchCharacteristicFOG (IMUSwitchFOGmarker, BLERead | BLEWrite);
//BLEByteCharacteristic   switchCharacteristicVib (IMUSwitchVibration, BLERead | BLEWrite);


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() {

  // receiver code //
  setup_serial(false, baudrate);
  Serial1.begin(baudrate);
  // receiver code end //
  Serial.println("Arduino Nano 33 BLE: Ready to receive data...");

  setup_ble();
  setup_led(200); //(delaytime) // only for nano 33 ble, led indicator that everything is ready

}

void loop() {
  // receiver code //

  while (Serial1.available()) {
    // BLE device
    BLEDevice central = BLE.central();

    // indicator
    status_led(2); //green

    // serial reading
    rbuff = Serial1.read();
    buff += rbuff;
    if (rbuff == ';') {
      Serial.println(buff);
      buff = "";


      // BLE device connected to PC
      while ( central.connected()) {

        // indicator
        status_led(3); //green

        // serial reading
        while (Serial1.available()) {
          rbuff = Serial1.read();
          buff += rbuff;
          if (rbuff == ';') {
            Serial.println(buff);
            write_ble();
            buff = "";
          }
        }
      }

    }
  }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup_ble() {
  //sets up the bluetooth low energy
  if (!BLE.begin()) {
    Serial.println("Failed to initialize BLE");
  } else {
    Serial.println("initialized BLE");
  }

  BLE.setDeviceName("FOG Nano");
  BLE.setLocalName("FOG Nano");
  BLE.setAdvertisedService(imu_service);
  //BLE.setAdvertisedService(testService);

  imu_service.addCharacteristic(imu_sensor_data);
  //imu_service.addCharacteristic(switchCharacteristicSD);
  //imu_service.addCharacteristic(switchCharacteristicFOG);
  //imu_service.addCharacteristic(switchCharacteristicVib);
  //  testService.addCharacteristic(acc_x);

  BLE.addService(imu_service);
  //BLE.addService(testService);
  //BLE.addService(sd_service);

  BLE.advertise();

  Serial.print("Local address is: ");
  Serial.println(BLE.address());
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void write_ble() {
  imu_sensor_data.writeValue(buff);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
String int2string(int in_val) {
  String str_val = "";

  str_val.concat(in_val);
  return str_val;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup_serial(bool wait4serial, int baud_val) {
  //serial print setup
  Serial.begin(baud_val);
  if (wait4serial) {
    while (!Serial);
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
String float2string(float in_val) {
  String str_val = "";

  str_val.concat(in_val);
  return str_val;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void status_led(int case_val) {
  //  digitalWrite(LEDR, HIGH);  //HIGH --> led is off
  //  digitalWrite(LEDG, HIGH);  //LOW  --> led is on
  //  digitalWrite(LEDB, HIGH);

  //(1)red, (2)green, (3)blue, (4)while, (5)magenta, (6)yellow, (7)cyan, () default white

  switch (case_val) {
    case 1: //red
      //status_neopixel(1);
      digitalWrite(LEDR, LOW);   //LOW  --> led is on
      digitalWrite(LEDG, HIGH);  //HIGH --> led is off
      digitalWrite(LEDB, HIGH); break;

    case 2: //green
      //status_neopixel(2);
      digitalWrite(LEDR, HIGH);
      digitalWrite(LEDG, LOW);
      digitalWrite(LEDB, HIGH); break;

    case 3: //blue
      //status_neopixel(3);
      digitalWrite(LEDR, HIGH);
      digitalWrite(LEDG, HIGH);
      digitalWrite(LEDB, LOW); break;

    case 4: //white
      //status_neopixel(4);
      digitalWrite(LEDR, LOW);
      digitalWrite(LEDG, LOW);
      digitalWrite(LEDB, LOW); break;

    case 5: //magenta
      //status_neopixel(5);
      digitalWrite(LEDR, LOW);
      digitalWrite(LEDG, HIGH);
      digitalWrite(LEDB, LOW); break;

    case 6: //yellow
      //status_neopixel(6);
      digitalWrite(LEDR, LOW);
      digitalWrite(LEDG, LOW);
      digitalWrite(LEDB, HIGH); break;

    case 7: //cyan
      //status_neopixel(7);
      digitalWrite(LEDR, HIGH);
      digitalWrite(LEDG, LOW);
      digitalWrite(LEDB, LOW); break;

    default: //white
      //status_neopixel(8);
      digitalWrite(LEDR, LOW);
      digitalWrite(LEDG, LOW);
      digitalWrite(LEDB, LOW); break;
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup_led(int delay_val) {
  //pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  //pixels.clear(); // Set all pixel colors to 'off'

  digitalWrite(LEDR, HIGH); //HIGH --> led is off
  digitalWrite(LEDG, HIGH); //LOW  --> led is on
  digitalWrite(LEDB, HIGH);

  for (int i = 0; i < 5; i++) {
    status_led(1); delay(delay_val);
    status_led(2); delay(delay_val);
    status_led(3); delay(delay_val);
  }
}
