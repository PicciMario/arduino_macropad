// Keyboard Matrix Tutorial Example
// baldengineer.com
// mariopiccinelli.it
// CC BY-SA 4.0

#include <Keyboard.h>
#include <EncoderButton.h>

// ROWS (inputs)
byte rows[] = {7, 6, 5};
const int rowCount = sizeof(rows)/sizeof(rows[0]);

// COLUMNS (outputs)
byte cols[] = {2, 4, 3};
const int colCount = sizeof(cols)/sizeof(cols[0]);

// Encoder inputs
const byte ENC_A = 0;
const byte ENC_B = 1;
const byte ENC_BUTTON = 21;
EncoderButton eb1(ENC_A, ENC_B, ENC_BUTTON);
volatile int scrollSpeed;

// Current status of switches
byte keys[colCount][rowCount];

// Keyboard value for each switch
char keyValues[3][3] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'}
};


// Encoder callback
void onEb1Encoder(EncoderButton& eb) {
//  Serial.print("eb1 incremented by: ");
//  Serial.println(eb.increment());
//  Serial.print("eb1 position is: ");
//  Serial.println(eb.position());
  if (eb.increment() > 0){
    for (int i = 0; i < scrollSpeed; i++){
      Keyboard.press(KEY_UP_ARROW);
      Keyboard.release(KEY_UP_ARROW);
    }
  }
  else {
    for (int i = 0; i < scrollSpeed; i++){
      Keyboard.press(KEY_DOWN_ARROW);
      Keyboard.release(KEY_DOWN_ARROW);
    }
  }
}

void onEncoderButtonClick(EncoderButton& eb){
//  Serial.print("Button clicked ");
//  Serial.println(eb.clickCount());
  if (scrollSpeed == 1){
    scrollSpeed = 5;
  }
  else {
    scrollSpeed = 1;
  }
}

// #########################################################################################

void setup() {
  
  Serial.begin(115200);
  Keyboard.begin();

  // Set cols (outputs) as inputs
  for(int x=0; x<rowCount; x++) {
    pinMode(rows[x], INPUT);
  }

  // Set rows as inputs with pullup
  for (int x=0; x<colCount; x++) {
    pinMode(cols[x], INPUT_PULLUP);
  }

  // Configure encoder
  pinMode(ENC_BUTTON, INPUT_PULLUP);
  pinMode(ENC_A, INPUT_PULLUP);
  pinMode(ENC_B, INPUT_PULLUP);  
  eb1.setEncoderHandler(onEb1Encoder);
  eb1.setClickHandler(onEncoderButtonClick);
  scrollSpeed = 1;
}

// #########################################################################################

void readMatrix() {
  
  // iterate the columns
  for (int colIndex=0; colIndex < colCount; colIndex++) {
    
    // enable col: set to output and low
    byte curCol = cols[colIndex];
    pinMode(curCol, OUTPUT);
    digitalWrite(curCol, LOW);

    // row: interate through the rows
    for (int rowIndex=0; rowIndex < rowCount; rowIndex++) {
      
      byte rowCol = rows[rowIndex];
      
      pinMode(rowCol, INPUT_PULLUP);
      keys[colIndex][rowIndex] = !digitalRead(rowCol);
      pinMode(rowCol, INPUT);
      
    }
    
    // disable the col (set as input)
    pinMode(curCol, INPUT);
    
  }
  
}

// #########################################################################################

void printKeyboard() {
  for (int rowIndex=0; rowIndex < rowCount; rowIndex++) {
    for (int colIndex=0; colIndex < colCount; colIndex++) { 
      if (keys[rowIndex][colIndex])
        Keyboard.press(keyValues[rowIndex][colIndex]);
      else
        Keyboard.release(keyValues[rowIndex][colIndex]);
    } 
  }
}

// #########################################################################################

void printMatrix() {
  for (int rowIndex=0; rowIndex < rowCount; rowIndex++) {
    if (rowIndex < 10)
      Serial.print(F("0"));
    Serial.print(rowIndex); Serial.print(F(": "));

    for (int colIndex=0; colIndex < colCount; colIndex++) { 
      Serial.print(keys[colIndex][rowIndex]);
      if (colIndex < colCount)
        Serial.print(F(", "));
    } 
    Serial.println("");
  }
  Serial.println("");
}

// #########################################################################################

void loop() {
  
  // Updates encoder
  eb1.update();

  // Read keys
  readMatrix();

  // Print pressed keys
  printKeyboard();

  // Cycle delay
  delay(30); 
  
}
