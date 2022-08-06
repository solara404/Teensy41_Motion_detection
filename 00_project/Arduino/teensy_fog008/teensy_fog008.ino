#include <TimeLib.h>    // time
#include <DS1307RTC.h>  // time a basic DS1307 library that returns time as a time_t
#include <Wire.h>               //bno sensor
//#include <i2c_driver_wire.h>
#include <Adafruit_BNO055.h>    //bno sensor
#include <utility/imumaths.h>   //bno sensor
#include <Adafruit_Sensor.h>    //bno sensor
#include <SD.h>                 //sd card
#include <SPI.h>                //sd card
#include <Arduino.h>            //
#include <Adafruit_NeoPixel.h>  //neopixel led
#include <Bounce.h>             //for interrupt only for debugging



//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
// physical switches information //
// 1: turn on teensy
// 2: switch pin 11 save to sd-card
// 3: switch pin 10
// 4: turn on power (general power switch)
// 5:
// 6: turn on arduino

// goblal variables //
//byte num_s = 4;
//byte num_m = 1;

// error message //
//bool error_bool = false;

// serial info //
bool s_info = false; //(serial_info) if true prints information of variable - use for debugging
//int  info_delay = 0; // time to delay the debugging serial
byte digits_var = 15; //how many digits after the comma should be displayed (15 is all digits);
const byte interrupt_pin = 40;
Bounce pushbutton = Bounce(interrupt_pin, 30); //10ms debounce // for interrupt
volatile bool interrupt_waiter = true; //for debugging makes the controller wait indef until changes
bool interrupt_paused = true; //to control which sections debuggin should happen

// performance //
unsigned long performance_ptime = 0;
unsigned long performance_ctime = 0;


// led //
const byte ledPin      = 13;
//const byte neoPixelPin =  9;
//const byte numNeoPixel =  1;
//Adafruit_NeoPixel pixels(numNeoPixel, neoPixelPin, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels(1, 9, NEO_GRB + NEO_KHZ800);

// data //
bool generate_info        = true; //if true generates info_string
byte data_index           =  0;   //index for data[] variable
//byte data_index_length;         //stores the value of actual data values
const int  data_length    = 72;   //setting length of data[] var array, ...
//must be changed if more sensors are used
//3 values per m_type, 6 m_types, 4 sensors
//3 * 6 * 4 = 72 data values
byte  current_data_length =  0;   //get data_length from function ????
int data[data_length];         //var to store all the data values of one cycle

// bno //
// by default i2c address is 0x29 or 0x28 (ex.: Adafruit_BNO055(id, address))
// two use different sda und scl pins look up following link
//https://forum.pjrc.com/threads/59547-Teensy-4-0-use-of-2-different-i2c-bus

Adafruit_BNO055 bno1 = Adafruit_BNO055(55, 0x28, &Wire); //over first i2c connection (wire)
Adafruit_BNO055 bno2 = Adafruit_BNO055(55, 0x29, &Wire); //
Adafruit_BNO055 bno3 = Adafruit_BNO055(54, 0x28, &Wire1); //over second i2c connection (wire1)
Adafruit_BNO055 bno4 = Adafruit_BNO055(54, 0x29, &Wire1); //
// bool m_ori = false, m_gyro = false, m_linacc = false;
// bool m_mag = false, m_acc  = false, m_grav   = false;
// bool m_types[6] = {m_ori, m_gyro, m_linacc, m_mag, m_acc, m_grav};
//bool sensor_1 = false, sensor_2 = false, sensor_3 = false, sensor_4 = false; //for motor to know
//which motors are connected
//bool sensor_active[4] = { sensor_1, sensor_2, sensor_3, sensor_4 };

// ram //
//bool         ram_full         = false;  //if true ram is full
const int    ram_byte_size    = 16000000; //total size of the ram in bytes
unsigned int ram_data_length  = 0; //sets the ram_data_array size(val need to be calculated)
//EXTMEM double ram_data_array[2097152]; // maximum size of int array
//EXTMEM int ram_data_array[4194304]; // maximum size of double array
EXTMEM int ram_data_array[4000000];
byte precision = 2; // how many digits after the comma should be stored
// values are converted from double to int
// eg. precision = 3, double result will be multiplied by 10^3
// precision only needed when values are stored as int into ram
unsigned int prediction_counter = 0; //index of the ram_data_array
unsigned int prediction       = 0; //made by ram_fortune_teller function //test value 10000
unsigned int ram_index        = 0; //index for the ram_data_array

// sd //
File dataFile;
const byte max_char = 50;  //max symbole lenght size for file name
char file_name[max_char] = "datalogger"; //file name for sd file
uint32_t file_name_counter = 0;
bool write2sd_now = false;

// info //
String buff        = "";  //to store all values in one variable
String info_string = "";  //stores what is stored and in which order

// fog //
bool fog_detected = false; //ture when fog is detected

//unsigned long fog_kuckuck_ptime = 0;
//unsigned long fog_kuckuck_ctime = 0;

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
// prototypes //
// this is done because sometimes the concatenation does not work
// to prevent compiling errors put prototypes for all functions in the tabs

// serial // serial_func
void setup_serial(bool wait4serial, int baud_val);
void setup_serial1(bool wait4serial, int baud_val);
void write_serial1();

// interrupt // interrupt_func
void setup_debug_waiter();
void debug_waiter();
void interrupt_fkt();

// sd card // sd_func
void setup_sd();
void write_sd(byte case_val, String data_string);
void file_name_gen();
void setup_sd_toggle_swtich();
void read_sd_switch();

// ram // ram_func
void ram_prediction(double * in_data);
void write_ram(double * in_data);
void write_ram_test(double * in_data);

// indexer // indexer_func
byte indexer();
void generate_indexer_info(String info_text, byte index_count);
void indexer_set_end();
void m_time(int * in_data);

// converter // converter_func
float double2float(double in_val);
int double2int(double in_val);
String float2string(float in_val);
String ulong2string(unsigned long in_val);
String byte2string(byte in_val);
String int2string(int in_val);

// bno // bno_func
void read_bno(byte sensor_var);
void setup_bno(byte bno_index);

// motor // motor_func
void setup_motor();
void motor(byte motor_index, bool motor_on);
void hudhud(byte timer_index, unsigned long time_diff, char case_var);
bool kuckuck(byte timer_index, unsigned long time_diff, char case_var);


// rtc // rtc_func
void setup_time();
time_t getTeensy3Time();
String get_time(char date_time_var, char time_unit_var);

// fog // fog_func
void fog_reader();

// neopixel // neo_func
void setup_neo();
void write_neo(byte red, byte green, byte blue);
void set_neo(char color);

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
// setup //

void setup() {

  setup_led(100, 20); // led feed back to know that the mc is ON, internal led

  setup_neo();
  set_neo('y'); //set neopixel to yellow



  //  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  //  pixels.setPixelColor(0, pixels.Color(0, 150, 0));
  //  pixels.show();

  //communication with serial monitor (pc)
  //if true: waits for serial monitor
  //if false: starts without serial monitor
  //setup_serial(bool, baudrate)
  setup_serial(false, 230400);

  //communication with arduino nano 33 ble
  //if true: waits for serial monitor
  setup_serial1(false, 230400);
  //Serial1.begin(230400);   //fastest baud that works with the arduino BLE

  //time setup //
  setup_time();

  //interrupt for debugging setup
  //setup_debug_waiter(); //setup the interrupt for the debug_waiter

  //I2C setup
  //Wire.begin();
  //Wire1.begin();

  //motor setup
  setup_motor(); // vibrates one motor after the other
  // motor connected to wire0 (I) are left-hand side
  // motor connected to wire1 (II) are on the right-hand side
  // motor odd indices (1,3) are on the upper knee (frontal)
  // motor even indices (2,4) are on the uppper ankle (frontal)


  //setup for bno055 sensor
  //setup_bno(sensor number 1-4)
  setup_bno(1);
  setup_bno(2);
  setup_bno(3);
  setup_bno(4);

  //setup for sd-card
  setup_sd();
  setup_sd_toggle_swtich();
  pinMode(10, INPUT_PULLUP); //for testing put me in a setup function

  // test run //
  /*
    test run to generate 'infor_string' for user to know which value is which in the *.csv file
    test run to generate 'current_array_length' for ram functions and datahandling
    test run to generate 'prediction' for ram functions
  */

  //adding time to data array
  m_time(data);
  //reading the bno055 sensors
  read_bno(1); //reads sensor one
  read_bno(2); //reads sensor two
  read_bno(3); //reads sensor three
  read_bno(4); //reads sensor four

  //interrupt_paused = false;  // only for debugging; debugging start here again

  ram_prediction(data); //for 'prediction' value for ram

  indexer_set_end(); //reset indexer and stop generating 'info_string'


  // read sd switch
  // if switch in on position controller stops and displays a led indication
  read_sd_switch();
  if (write2sd_now) {
    set_neo('r');
    Serial.print("\n'Write to SD Swicht' is in 'ON' position!");
    while (write2sd_now) {
      read_sd_switch();
      led_indicator(100, 1);
    }
  }

  // only for debugging
  if (s_info) {
    Serial.println("Setup END");
    debug_waiter(); //waits till user input; only for debuggin
  }


  // sending info string to pc
  Serial.println();
  Serial.println(info_string);
  // sending info string to controller
  Serial1.println();
  Serial1.println(info_string);

  //  while(1){
  //    read_sd_switch();
  //    if(write2sd_now){
  //      Serial.println("HIGH");
  //      delay(1000);
  //    } else {
  //      Serial.println("LOW");
  //      delay(1000);
  //    }
  //  }

  //indication led
  //loop now starting
  led_indicator(100, 5);


  //  pixels.setPixelColor(0, pixels.Color(0, 0, 0)); // neopixel yellow
  //  pixels.show();
  set_neo('x');

}

//////////////////////////////////////////////////////////////////////////////
// main //

void loop() {
  //  performance(true); //starts timer to messure time

  //  if (s_info) { //loop start information
  //    Serial.println("\n\n\n *** \n\n\n");
  //    Serial.println("Loop START "); //information about the loop start
  //
  //    //ram variables information
  //    Serial.println("RAM - var.Information: ");
  //    //    Serial.print("ram_full = ");
  //    //    Serial.println(ram_full);
  //    Serial.print("ram_data_length = ");
  //    Serial.println(ram_data_length);
  //    Serial.print("prediction_counter = ");
  //    Serial.println(prediction_counter);
  //    Serial.print("prediction = ");
  //    Serial.println(prediction);
  //    Serial.print("ram_index = ");
  //    Serial.println(ram_index);
  //
  //    //data variables information
  //    Serial.println("Data - var.Information: ");
  //    Serial.print("generate_info = ");
  //    Serial.println(generate_info);
  //    Serial.print("data_index = ");
  //    Serial.println(data_index);
  //    //    Serial.print("data_index_length = ");
  //    //    Serial.println(data_index_length);
  //    Serial.print("data_length = ");
  //    Serial.println(data_length);
  //    Serial.print("current_data_length = ");
  //    Serial.println(current_data_length);
  //    Serial.println();
  //    debug_waiter();
  //
  //    Serial.println("data[]: "); //prints all data in data[]
  //    for (byte i = 0; i < data_length; i++) {
  //      Serial.print(data[i], digits_var);
  //      if (i < data_length - 1) {
  //        Serial.print(",");
  //      } else {
  //        Serial.println();
  //      }
  //    }
  //    debug_waiter();
  //
  //    // info variables information
  //    Serial.println("INFO - var.Information");
  //    Serial.print("buff = ");
  //    Serial.println(buff);
  //    Serial.print("info_string = ");
  //    Serial.println(info_string);
  //    Serial.println();
  //    debug_waiter();
  //  }

  // adding time to data array
  m_time(data);
  // read sensors
  read_bno(1); //reads sensor one with choosen m_types (messurment types)
  read_bno(2);
  read_bno(3);
  read_bno(4);
  indexer_set_end(); //resets the indexer function

  // write ram
  write_ram(data);  //writes ram till full and than writes to sd
  //write_ram_test(data); //only for debugging fills ram fast

  // write sd
  read_sd_switch();
  if (prediction_counter >= prediction || write2sd_now) {
    set_neo('g');
    file_name_gen();  // generates a unique filename with date and time
    write_sd(1, "");  // 1: writes all values to sd-card
    //delay(500);
    set_neo('y');

    // led feedback that everything has been transfered to sd card
    while (write2sd_now) {
      read_sd_switch();
      digitalWrite(ledPin, LOW);    // set the LED off
      //set_neo('x');
      //write_neo(50, 40, 0);
      delay(1000);                  // wait for a second
      digitalWrite(ledPin, HIGH);   // set the LED on
      //set_neo('y');
      delay(1000);
    } // ????????????????????????????????????????????????????????????????? maybe make a check if sd card is available again


    prediction_counter = 0;
    ram_index  = 0;
    indexer_set_end();

  }
  //sends data_array to arduino nano ble
  write_serial1();

  //prints data from specific start index to pc serial
  Serial_print_data(1);

  if (prediction_counter > 5) {
    fog_reader();
  }

  if (fog_detected) {
    //    pixels.setPixelColor(0, pixels.Color(0, 0, 255)); // neopixel
    //    pixels.show();
    set_neo('b');

  } else {
    //    pixels.setPixelColor(0, pixels.Color(0, 10, 0)); // neopixel
    //    pixels.show();
    set_neo('m');
  }

  //only for testing replace me with a time function when fog is detected vibration should activate
  while (!digitalRead(10)) {
    set_neo('r');
    motor(2, false);
    motor(3, false);
    delay(150);
    motor(0, true);
    motor(1, true);
    delay(1000);
    motor(0, false);
    motor(1, false);
    delay(150);
    motor(2, true);
    motor(3, true);
    delay(1000);
  }

  motor(1, false);
  motor(2, false);
  motor(3, false);
  motor(4, false);

  //  performance(false); //ends timer and returns timedifference
  //  Serial.print(prediction_counter); // for debuggin, prints prediction_counter
  //  Serial.print(" / ");
  //  Serial.print(prediction);
  //  Serial.println((String) "\t runtime = " + millis() + "ms");
}



//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
// functions //

// led //
void setup_led(int wait_val, byte repeat_val) {
  // indicates that the mc is starting

  pinMode(ledPin, OUTPUT); // init the digital pin as output for the internal led

  for (byte i = 0; i < repeat_val; i++) {
    digitalWrite(ledPin, LOW);    // set the LED off
    delay(wait_val / 2);                // wait for a second
    digitalWrite(ledPin, HIGH);   // set the LED on
    delay(wait_val / 2);                // wait for a second
  }
}

void led_indicator(int wait_val, byte repeat_val) {
  // gives a light feedback from the internal led
  for (byte i = 0; i < repeat_val; i++) {
    digitalWrite(ledPin, LOW);    // set the LED off
    delay(wait_val / 2);                // wait for a second
    digitalWrite(ledPin, HIGH);   // set the LED on
    delay(wait_val / 2);                // wait for a second
  }
}

//////////////////////////////////////////////////////////////////////////////
// Serial print data array //

void Serial_print_data(byte start_val) {
  //prints serial
  //prints data from a specific starting point
  for (byte i = start_val; i < current_data_length; i++) {
    Serial.print(data[i]);
    Serial.print(", ");
  }
  Serial.println();
}

//////////////////////////////////////////////////////////////////////////////
// performance //

double herz_array[255];
byte average_index = 0;
double average_herz = 0;

void performance(bool start_timer) {
  //prints performance information to pc serial
  //this happens when the s_info bool is set to false
  //informations includes the time each cycle takes in msec
  //informations includes the freq each cycle takes in hz
  if (!s_info) {
    if (start_timer) {
      performance_ptime = micros();
    } else {
      performance_ctime = micros();
      unsigned long time_diff = performance_ctime - performance_ptime;
      double double_time_diff = time_diff;
      double herz = 1000000.00 / double_time_diff;
      double time_diff_sec = time_diff / 1000.00;

      if (average_index < 255) {
        herz_array[average_index++] = herz;
      } else {
        for (byte i = 0; i < 255; i++) {
          average_herz += herz_array[i];
        }
        average_herz /= 255;
        average_index = 0;
      }

      //serial to monitor (pc)
      Serial.println();
      Serial.print((String) "performance time = " + time_diff + " usec\t" + time_diff_sec + " msec\t\t");
      Serial.print((String) "performance freq. = " + herz + " Hz\t\t");
      Serial.print((String) "average per freq. = " + average_herz + " Hz\n");

      //serial to arduino nano 33 ble
      //      Serial1.println();
      //      Serial1.print("performance time = ");
      //      Serial1.print(time_diff);
      //      Serial1.print(" micro seconds \t\t performance freq. = ");
      //      Serial1.print(herz);
      //      Serial1.println(" Hz;");

      debug_waiter();
    }
  }
}
