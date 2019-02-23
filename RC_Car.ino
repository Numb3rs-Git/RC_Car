/* Input pins ******************************** */
#define JOYSTICK_X   A0
#define JOYSTICK_Y   A1
#define GRAY_WIRE     2
#define BLUE_WIRE     3

/* Output Pins ******************************* */
#define BROWN_WIRE    4
#define WHITE_WIRE    5
#define THROTTLE_ENBL 6
#define THROTTLE_1    7
#define THROTTLE_2    8
#define STEER_ENBL    9
#define STEER_1      10
#define STEER_2      11

/* Control Values **************************** */
#define STEER_RIGHT   0
#define STEER_LEFT    1
#define STEER_CENTER  2
#define THROTTLE_FWD  0
#define THROTTLE_REV  1

/* Global Variables ************************** */
int16_t joystick_x, joystick_y;
uint8_t steeringMode, lastSteerMode;
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
    delay(100); // required for h-bridge board
    
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
  steeringMode = 0;
  lastSteerMode = 0;
  throttleMode = 0;
  lastThrotMode = 0;
  throttleLevel = 0;
}

void loop() {
  readControls();
  handleThrottle();
  handleSteering();
  delay(20);
}