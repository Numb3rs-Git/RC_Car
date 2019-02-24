/* Input pins ******************************** */
#define JOYSTICK_X   A4
#define JOYSTICK_Y   A5
#define GRAY_WIRE    A0
#define BLUE_WIRE    A1

/* Output Pins ******************************* */
#define BROWN_WIRE   A2
#define WHITE_WIRE   A3
#define STEER_ENBL    2
#define STEER_1       3
#define STEER_2       4
#define THROTTLE_ENBL 5
#define THROTTLE_1    6
#define THROTTLE_2    7

/* Control Values **************************** */
#define STEER_RIGHT   0
#define STEER_LEFT    1
#define STEER_CENTER  2
#define THROTTLE_FWD  0
#define THROTTLE_REV  1

/* Steering States *****************************/
#define RIGHT_MAX     0
#define RIGHT         1
#define CENTER        2
#define LEFT          3
#define LEFT_MAX      4
#define INVALID       5

/* Global Variables ************************** */
int16_t joystick_x, joystick_y;
uint8_t steeringMode, lastSteerMode;
uint8_t steeringPos, lastSteerPos;
uint8_t throttleMode, lastThrotMode, throttleLevel;

/* Output Handlers *************************** */

void handleSteering(){
  
}

void handleThrottle(){
  
  // if throttle mode has changed
  if(throttleMode != lastThrotMode){
    
    // brake
    digitalWrite(THROTTLE_1, 0);
    digitalWrite(THROTTLE_2, 0);
    delay(200); // be nice to h-bridge
    
    // set new throttle mode
    digitalWrite(THROTTLE_1, !throttleMode);
    digitalWrite(THROTTLE_2, throttleMode);
    
    // record last mode set
    lastThrotMode = throttleMode;
  }
  
  // set throttle level
  analogWrite(THROTTLE_ENBL, throttleLevel);
  
}

/* Input Handlers **************************** */

void readControls(){
  
  // read joystick values
  joystick_x = analogRead(JOYSTICK_X);
  joystick_y = analogRead(JOYSTICK_Y);

  // get steering mode
  if(joystick_x < 461)
    steeringMode = STEER_RIGHT;
  else if(joystick_x > 562)
    steeringMode = STEER_LEFT;
  else
    steeringMode = STEER_CENTER;
  
  // get determine throttle mode
  joystick_y -= 512;
  if(joystick_y < 0){
    joystick_y *= -1;
    throttleMode = THROTTLE_REV;
  }
  else
    throttleMode = THROTTLE_FWD;
  
  // get throttle level
  if(joystick_y < 50)
    throttleLevel = 0;
  else if(joystick_y > 450)
    throttleLevel = 255;
  else
    throttleLevel = ((joystick_y - 50) * 255) / 400;
  
}

void readSteeringPosition(){
  
  boolean grayVal, blueVal;
  
  // check for maximums
  // set outputs
  digitalWrite(BROWN_WIRE, 1);
  digitalWrite(WHITE_WIRE, 0);
  
  // check inputs
  grayVal = digitalRead(GRAY_WIRE);
  blueVal = digitalRead(BLUE_WIRE);
  
  // max right
  if(!grayVal){
    steeringPos = RIGHT_MAX;
    return;
  }
  
  // max left
  if(!blueVal){
    steeringPos = LEFT_MAX;
    return;
  }
  
  // check for in-between positions
  // set outputs
  digitalWrite(BROWN_WIRE, 0);
  digitalWrite(WHITE_WIRE, 1);
  
  // check inputs
  grayVal = digitalRead(GRAY_WIRE);
  blueVal = digitalRead(BLUE_WIRE);
  
  // center
  if(grayVal && blueVal)
    steeringPos = CENTER;
  
  // right
  else if(grayVal)
    steeringPos = RIGHT;
  
  // left
  else if(blueVal)
    steeringPos = LEFT;
  
  // invalid state
  else
    steeringPos = INVALID;
  
}

/* Debug Stuff **************************** */

boolean debug_on = true;

void printSteeringPosition(){
  
  Serial.print("Steering position: ");

  switch(steeringPos){
    
    case RIGHT_MAX:
      Serial.print("RIGHT_MAX");
      break;
      
    case RIGHT:
      Serial.print("RIGHT");
      break;
      
    case CENTER:
      Serial.print("CENTER");
      break;
      
    case LEFT:
      Serial.print("LEFT");
      break;
      
    case LEFT_MAX:
      Serial.print("LEFT_MAX");
      break;
      
    case INVALID:
      Serial.print("INVALID");
      break;
  }
  
  Serial.print("\n");
}

/* ******************************************* */

void setup() {
  
  // inputs
  pinMode(GRAY_WIRE, INPUT_PULLUP);
  pinMode(BLUE_WIRE, INPUT_PULLUP);
  
  // outputs
  pinMode(BROWN_WIRE, OUTPUT);
  pinMode(WHITE_WIRE, OUTPUT);
  pinMode(STEER_ENBL, OUTPUT);
  pinMode(STEER_1, OUTPUT);
  pinMode(STEER_2, OUTPUT);
  pinMode(THROTTLE_1, OUTPUT);
  pinMode(THROTTLE_2, OUTPUT);
  
  // init outputs
  digitalWrite(STEER_ENBL, 0);
  digitalWrite(STEER_1, 0);
  digitalWrite(STEER_2, 0);
  digitalWrite(THROTTLE_1, 0);
  digitalWrite(THROTTLE_2, 0);
  
  // init control variables
  steeringMode = STEER_CENTER;
  lastSteerMode = STEER_CENTER;
  throttleMode = THROTTLE_FWD;
  lastThrotMode = THROTTLE_FWD;
  throttleLevel = 0;
  
  readSteeringPosition();
  lastSteerPos = steeringPos;

  if(debug_on)
    Serial.begin(115200);
}

void loop() {

  //readControls();
  readSteeringPosition();
  //handleThrottle();
  //handleSteering();
  delay(20);
  
  if(debug_on){
    delay(30);
    printSteeringPosition();
  }
}