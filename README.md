# hw2-4

(1) how to setup and run your program

I use 4 Thread: thread1, thread2, thread3, thread4. Thread 1 is for PWM signal.
Thread 2 is to make light sensor to sense lightness. Thread 2 will send Ain into Aout with 1ms period, which is an ADC. Thread 3 will use ADC value to send signal to pinA, and PinA will send signal to PinB to make a Interrupt signal. So Interrupt will call print() when rise and fall, which will show sample on the uLCD.

(2) what are the results

The ADC values for light sensor cause Interrupt to make uLCD print current sample number.
