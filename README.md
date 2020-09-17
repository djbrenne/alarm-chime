# alarm-chime
# Alarm Chime

Arduino code for an alarm-clock chime. Takes a digital signal on pin 2 (signal spliced in from any bypassed alarm clock piezo) to trigger a hobby brushless motor to spin (direction from pin 4, pwm from pin 5). 

The hobby motor is connected to a pendulum, which strikes a chime 3 times at 10 second intervals, then _automatically_ snoozes for 10 minutes. 

No more harsh beeps, no more bad sleeps.
