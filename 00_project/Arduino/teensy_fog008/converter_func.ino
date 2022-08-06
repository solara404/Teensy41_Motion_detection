//////////////////////////////////////////////////////////////////////////////
// converter functions //

float double2float(double in_val) {
  String str_val = "";
  float return_val;

  str_val.concat(in_val);
  return_val = str_val.toFloat();
  return return_val;
}

int double2int(double in_val) {
  String str_val = "";
  int return_val;

  in_val = in_val * 100;
  str_val.concat(in_val);
  return_val = str_val.toInt();
  return_val /= 100;
  return return_val;
}

String float2string(float in_val) {
  String str_val = "";

  str_val.concat(in_val);
  return str_val;
}

String ulong2string(unsigned long in_val) {
  String str_val = "";

  str_val.concat(in_val);
  return str_val;
}

String byte2string(byte in_val) {
  String str_val = "";

  str_val.concat(in_val);
  return str_val;
}

String int2string(int in_val) {
  String str_val = "";

  str_val.concat(in_val);
  return str_val;
}
