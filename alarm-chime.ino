/*
 * alarm-chime.ino created by Dylan Brenneis September 17, 2020
 * 
 * PINS:
 * Digital 2: triggering signal
 * Digital 4: direction signal
 * Digital 5: pwm signal
 * 
 * ADDITIONAL HARDWARE:
 * DFRobot DRI0044-A motor driver (https://www.digikey.ca/en/products/detail/dfrobot/DRI0044-A/8598405)
 * SparkFun DC Brushed motor (https://www.digikey.ca/en/products/detail/sparkfun-electronics/ROB-11696/6163657)
 *
 */

//Pin declarations
const int trgPin = 2;
const int dirPin = 4;
const int pwmPin = 5;

//Alarm parameters
const int chimeInterval = 10; //10 seconds between chimes
const int numberOfChimes = 3; //3 chimes before snoozing
const int snoozeInterval = 600; //10 minutes snooze

//Motor parameters
const short MAX_PWM = 102; // 40% (of 255) is max PWM to keep the motor from getting too hot at stall current
const int COUNTER_CLOCKWISE = 1;


// The fast PWM affects millis() and delay(), making them also 64 times faster than "real-time"
#define adjusted_millis() (millis() / 64) // adjust millis() to account for faster timing

void setup(){
  // Use fast PWM to eliminate high-pitched motor buzzing
  TCCR0B = TCCR0B & B11111000 | B00000001; // for PWM carrier freq. 62500.00 Hz

  // Initialize pins
  pinMode(trgPin, INPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(pwmPin, OUTPUT);

  // Set up motor to spin counter-clockwise
  digitalWrite(dirPin, COUNTER_CLOCKWISE);
}

void loop(){
  // Main loop. Continually check for alarm condition; if true, sound alarm.
  while (triggered()){
    soundAlarm();
  }
  wait(1); // Give the arduino time to cycle when not triggered.
}

bool triggered(){
  // Return true if the trigger pin is high
  return bool(digitalRead(trgPin));
}

void soundAlarm(){
  /* Sound one cycle of the alarm each time this is called.
  *  One cycle: Ding numberOfChimes times at chimeInterval intervals, then snooze for snoozeInterval.
  *  Check throughout for low trigger signal, turn off alarm if this is low.
  */ 

  //Chime cycle:
  for (int i = 0; i < numberOfChimes; i++){
    if (triggered()){
      pealBell();
      wait(chimeInterval);
    }
    else{
      break;
    }
  }
  
  //Snooze cycle:
  if (triggered()){
    wait(snoozeInterval);
  }
}

void pealBell(){
  // Ring the bell once.
  analogWrite(pwmPin, MAX_PWM);
  delay(3200); // write the torque command for 50 ms (adjusted x64 for fast PWM)
  analogWrite(pwmPin, 0);
}

void wait(int seconds){
  // Wait for the specified number of seconds
  unsigned long current_time = adjusted_millis();
  while ((adjusted_millis() - current_time) / 1000 < seconds){
    //pass the time in 100 ms increments.
    delay(6400);
    /* fast pwm means that millis() will overflow in 18.64 h instead of ~50 days,
     * making it more likely to happen during a wait() loop. Check for that, 
     * and simply exit wait() if it happened.
     */
    if (adjusted_millis() < current_time){
      break;
    }
  }
}
