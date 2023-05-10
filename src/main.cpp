#include <FastLED.h>

#define NUM_LEDS 18*18
#define NUM_CELLS 30*30
#define DATA_PIN 20
#define CLOCK_PIN 21

CRGB leds[NUM_LEDS];

bool cell_matrix[NUM_CELLS];
bool next_cell_matrix[NUM_CELLS];
bool LED_matrix[NUM_LEDS];

bool ones = true;

void setup() { 
  Serial.begin(9600);
  FastLED.addLeds<DOTSTAR, DATA_PIN, CLOCK_PIN, BGR>(leds, NUM_LEDS);
  FastLED.setBrightness(80);
}

void set_cell(int cell_x, int cell_y, bool kill = false, bool next_gen = false) {
  //cell_x += (sqrt(NUM_CELLS)-sqrt(NUM_LEDS))/2;
  //cell_y += (sqrt(NUM_CELLS)-sqrt(NUM_LEDS))/2;

  int cell_num = cell_x + cell_y*sqrt(NUM_CELLS);

  if (kill) {
    if (next_gen) {
      next_cell_matrix[cell_num] = false;
    } else {
      cell_matrix[cell_num] = false;
    }
  } else {
    if (next_gen) {
      next_cell_matrix[cell_num] = true;
    } else {
      cell_matrix[cell_num] = true;
    }
  }
}

bool return_cell(int cell_x, int cell_y, bool next_gen = false) {
  int cellNum = cell_x + cell_y*sqrt(NUM_CELLS);
  if (next_gen) {
    return next_cell_matrix[cellNum];
  }
  return cell_matrix[cellNum];
}

void next_cell_gen() {

  for (int cell=0; cell<NUM_CELLS; cell++) { next_cell_matrix[cell] = cell_matrix[cell];}

  int liveNeighbours;

  //for every non edging cell
  for (int x=1; x<sqrt(NUM_CELLS)-1; x++) {

    for (int y=1; y<sqrt(NUM_CELLS)-1; y++) {

      liveNeighbours = 0;

      //check neighbouring cells
      if (return_cell(x-1, y  )) { liveNeighbours++;}
      if (return_cell(x-1, y-1)) { liveNeighbours++;}
      if (return_cell(x  , y-1)) { liveNeighbours++;}
      if (return_cell(x+1, y-1)) { liveNeighbours++;}
      if (return_cell(x+1, y  )) { liveNeighbours++;}
      if (return_cell(x+1, y+1)) { liveNeighbours++;}
      if (return_cell(x  , y+1)) { liveNeighbours++;}
      if (return_cell(x-1, y+1)) { liveNeighbours++;}
    
      //rules
      if (liveNeighbours <= 1) { set_cell(x, y, true, true);}
      if (liveNeighbours >= 4) { set_cell(x, y, true, true);}
      if (liveNeighbours == 3) { set_cell(x, y, false, true);}
    }
  }
  for (int cell=0; cell<NUM_CELLS; cell++) { cell_matrix[cell] = next_cell_matrix[cell];} // current gen = next gen
  for (int cell=0; cell<NUM_CELLS; cell++) { next_cell_matrix[cell] = false;} // reset next gen
}

void set_LED_from_cell(int cell_x, int cell_y) {

  int pixel;

  if (cell_y%2 == 0) { //if column divisible by two
    pixel = cell_y*sqrt(NUM_LEDS) + cell_x;
  }
  else {
    pixel = (cell_y+1)*sqrt(NUM_LEDS)-1 - cell_x;
  }
  LED_matrix[pixel] = true;
}

void cell_matrix_to_LED_matrix() {

  // difference between ledmatrix and cellmatrix size
  int standOff = (sqrt(NUM_CELLS)-sqrt(NUM_LEDS))/2;

  //loop over cellmatrix n cellmatrix
  for (int x = standOff; x < sqrt(NUM_CELLS)-standOff; x++) {
    for (int y = standOff; y < sqrt(NUM_CELLS)-standOff; y++) {
      if (return_cell(x, y)) {
        set_LED_from_cell(x-standOff, y-standOff);
      }
    }
  }
}

void turnOnLeds() {
  for (int pixel = 0; pixel < NUM_LEDS; pixel++) {
    if (LED_matrix[pixel]) {
      leds[pixel] = CRGB::Green;
    }
  }
}

void resetLEDs() {
  for (int i=0; i<NUM_LEDS; i++) {
    LED_matrix[i] = false;
  }
}

void glider180degreesKickback() {
  set_cell(12, 13);

  set_cell(11, 14);

  set_cell(11, 15);
  set_cell(12, 15);
  set_cell(13, 15);


  set_cell(12, 18);
  set_cell(13, 18);
  set_cell(11, 19);
  set_cell(13, 19);
  set_cell(13, 20);
}

void binary101() {

  bool other_matrix[30*30] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
  };
  for (int i = 0; i<900; i++) { cell_matrix[i] = other_matrix[i];}
}

void loop() { 

  resetLEDs();
  FastLED.clear();

  delay(500);
  next_cell_gen();

  if (ones) {

    binary101();

    ones = false;
  }

  cell_matrix_to_LED_matrix();
  
  turnOnLeds();
  FastLED.show();
}