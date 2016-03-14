/*
   ThrottleSweep
   Waits for a PWM signal from an RC Rx, then steps through
   full range on output servo channel
*/

#include <Servo.h>

Servo throttle;
int cmdpin = 12;
int throtpin = 9;

int hold_time = 5000;   //throttle posn hold time in ms
int throt_low = 1000;   //max throttle cmd in us
int throt_high = 2000;  //max throttle cmd in us
int steps = 20;         //number of steps to take
int interval = (throt_high - throt_low) / steps; //cmd interval in us
int throt_cmd = 0;      //current throttle command in us
int active = 0;         //activation signal value
int active_trig = 1500; //activation signal trigger

void setup() {
  throttle.attach(throtpin);
  pinMode(cmdpin, INPUT);
}

void loop() {
  throttle.writeMicroseconds(throt_low);
  active = pulseIn(cmdpin, HIGH);
  if (active >= active_trig) {
    for (throt_cmd = throt_low + interval; throt_cmd <= throt_high; throt_cmd += interval) {
      throttle.writeMicroseconds(throt_cmd);
      delay(hold_time);
      active = pulseIn(cmdpin, HIGH);
      if (active < active_trig) {
        throttle.writeMicroseconds(throt_low);
        break;
      }
    }
    for (throt_cmd = throt_high - interval; throt_cmd >= throt_low; throt_cmd -= interval) {
      throttle.writeMicroseconds(throt_cmd);
      delay(hold_time);
      active = pulseIn(cmdpin, HIGH);
      if (active < active_trig) {
        throttle.writeMicroseconds(throt_low);
        break;
      }
    }
  }
}
