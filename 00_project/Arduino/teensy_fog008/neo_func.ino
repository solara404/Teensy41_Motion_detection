

// functions

// setup
void setup_neo() {
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  //pixels.clear(); // sets all neopixel leds to zero (turn em off)
  pixels.setPixelColor(0, pixels.Color(0, 0, 0));
  pixels.show();
}

void write_neo(byte red, byte green, byte blue) {
  pixels.setPixelColor(0, pixels.Color(red, green, blue));
  pixels.show();
}

void set_neo(char color) {
  switch (color) {
    //red
    case 'r': write_neo(255, 0, 0); break;

    //green
    case 'g': write_neo(0, 255, 0); break;

    //blue
    case 'b': write_neo(0, 0, 255); break;

    //cyan
    case 'c': write_neo(0, 255, 255); break;

    //yellow
    case 'y': write_neo(255, 230, 0); break;

    //magenta
    case 'm': write_neo(255, 0, 255); break;

    //white
    case 'w': write_neo(255, 255, 255); break;

    //black
    default: write_neo(0, 0, 0); break;
  }

  //pixels.show();
}
