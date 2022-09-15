// Keyboard Matrix Tutorial Example
// baldengineer.com
// mariopiccinelli.it
// CC BY-SA 4.0

#include <Keyboard.h>

// ROWS (inputs)
byte rows[] = {7, 6, 5};
const int rowCount = sizeof(rows)/sizeof(rows[0]);

// COLUMNS (outputs)
byte cols[] = {2, 4, 3};
const int colCount = sizeof(cols)/sizeof(cols[0]);

// Current status of switches
byte keys[colCount][rowCount];

char keyValues[3][3] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'}
};

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
    
}

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

void loop() {
  readMatrix();
  if (Serial.read()=='!')
    printMatrix();
  printKeyboard();
  delay(100);
}
