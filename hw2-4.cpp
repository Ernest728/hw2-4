#include "mbed.h"
#include "uLCD_4DGL.h"



PwmOut PWM1(D6);
AnalogIn Ain(A0);
AnalogOut Aout(D7);
DigitalOut A(D8), led(LED1);
EventQueue queue(32 * EVENTS_EVENT_SIZE);
InterruptIn B(D5);
Thread thread1, thread2, thread3, thread4;
uLCD_4DGL uLCD(D1, D0, D2);
float threshold = 0.97;
int current;

void print()
{
    led = !led;
    uLCD.cls();
    uLCD.text_width(4); // 4X size text
    uLCD.text_height(4);
    uLCD.color(WHITE);
    uLCD.locate(1, 2);
    uLCD.printf("%2d", current);
    printf("%2d\n", current);
}
void rise_handler()
{
    queue.call(print);
}

void fall_handler()
{
    queue.call(print);
}

void PWM_thread()
{
    PWM1.period_ms(5);
    float R[20];
    int Re[20];
    int i = 0;
    R[i++] = 0;
    for (; i < 5; i++)
        R[i] = i/5.0;
    for (;i < 10; i++)
        R[i] = 1.0;
    for (;i < 15; i++)
        R[i] = 1.0 - (i-10)/5.0;
    for (;i < 20; i++)
        R[i] = 0.0;
    for (i = 0; i < 20; i++)
        Re[i] = int(R[i]*5000);
    while (true) {
        for (int j = 0; j < 20; j++) {
            PWM1.pulsewidth_us(Re[j]);
            ThisThread::sleep_for(50ms);
        }
    }
}

void Light_sense()
{
    while(true) {
        Aout = Ain;
        ThisThread::sleep_for(1ms);
    }
}

void Light_Rupt()
{
    
    while(true){
        float y = 0;
        for(int k=1; k <= 1000; k++)
        {   
            
            y = y + Aout.read();
            if ((k % 50) == 0) {
                y = y / 50.0;
                if((y >= threshold) && (A == 0)) {
                    current = k;
                    A = 1;
                }
                else if((y < threshold) && (A == 1)){
                    current = k;
                    A = 0;
                }
                y = 0;
            }
                
            ThisThread::sleep_for(1ms);
        }
    }
}

int main()
{
    A.write(0);
    thread4.start(callback(&queue, &EventQueue::dispatch_forever));
    thread1.start(PWM_thread);
    thread2.start(Light_sense);
    thread3.start(Light_Rupt);
    
    B.rise(&rise_handler);
    B.fall(&fall_handler);
}
