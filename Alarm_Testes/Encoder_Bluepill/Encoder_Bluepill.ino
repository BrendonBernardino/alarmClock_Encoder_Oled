enum PinAssignments {
  encoderPinA = PA1,   // right
  encoderPinB = PA0,   // left
  selectButton = PB9
};

volatile unsigned int encoderPos = 0;  // a counter for the dial
unsigned int lastReportedPos = 1;   // change management
static boolean rotating = false;    // debounce management

// interrupt service routine vars
boolean A_set = false;
boolean B_set = false;

void setup() {
  pinMode(encoderPinA, INPUT);
  pinMode(encoderPinB, INPUT);
  pinMode(selectButton, INPUT);

  // turn on pullup resistors
  digitalWrite(encoderPinA, HIGH);
  digitalWrite(encoderPinB, HIGH);
  digitalWrite(selectButton, HIGH);

  // encoder pin on interrupt 0 (pin 2)
  attachInterrupt(digitalPinToInterrupt(encoderPinA), doEncoderA, CHANGE);
  // encoder pin on interrupt 1 (pin 3)
  attachInterrupt(digitalPinToInterrupt(encoderPinB), doEncoderB, CHANGE);

  attachInterrupt(digitalPinToInterrupt(selectButton), doSelect, FALLING);

  Serial.begin(115200);  // output
}

// main loop, work is done by interrupt service routines, this one only prints stuff
void loop() {
  rotating = true;  // reset the debouncer
  if (lastReportedPos != encoderPos) {
    Serial.print("Index:");
    Serial.println(encoderPos, DEC);
    lastReportedPos = encoderPos;
  }
}

// Interrupt on A changing state
void doEncoderA() {
    // debounce
  if ( rotating ) delay (1);  // wait a little until the bouncing is done

    // Test transition, did things really change?
  if ( digitalRead(encoderPinA) != A_set ) { // debounce once more
    A_set = !A_set;

    // adjust counter + if A leads B
    if ( A_set && !B_set )
      encoderPos += 1;
      Serial.println("Girou Encoder");

    rotating = false;  // no more debouncing until loop() hits again
  }
}

// Interrupt on B changing state, same as A above
void doEncoderB() {
  if ( rotating ) delay (1);
  if ( digitalRead(encoderPinB) != B_set ) {
    B_set = !B_set;
    //  adjust counter - 1 if B leads A
    if ( B_set && !A_set )
      encoderPos -= 1;
      Serial.println("Girou Encoder");

    rotating = false;
  }
}

void doSelect() {
  Serial.println("Botao pressionado");
}
