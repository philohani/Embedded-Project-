#include "mbed.h"
//=================================Hardware setup============================
DigitalOut trigger(D7);
DigitalIn  echo(D9);
DigitalOut IN1(D5);
DigitalOut IN2(D4);
DigitalOut IN3(D3);
DigitalOut IN4(D2);
Timer      pulse_timer;
PwmOut     ENA(D6);
PwmOut     ENB(D11);

//===========================Measuring Distance Value=========================
float get_distance() {
    pulse_timer.reset();
    // 1. Send Trigger pulse
    trigger = 1;
    wait_us(10);
    trigger = 0;

    // 2. Wait for Echo to start (with a simple timeout to prevent hanging)
    uint32_t timeout_cnt = 0;
    while (echo == 0) {
        timeout_cnt++;
        if (timeout_cnt > 100000) return -1; // No sensor detected
        }

    // 3. Start timing
    pulse_timer.start();
    // 4. Wait for Echo to stop
    while (echo == 1);
    // 5. Stop and read
    pulse_timer.stop();

    uint32_t duration = pulse_timer.read_us();
    // Calculate distance: (time * speed of sound) / 2
    return (duration * 0.0343) / 2.0;
}

//=============================Motors Actions===========================
void MotorsFwd(){
    IN1=1;
    IN2=0;
    IN3=1;
    IN4=0;
}

void MotorsOff(){
    IN1=0;
    IN2=0;
    IN3=0;
    IN4=0;
}

void MotorsBwd(){
    IN1=1;
    IN2=0;
    IN3=1;
    IN4=0;
}

//==============================Main Function===========================
int main() {
    ENA.write(1.0);
    ENB.write(1.0);
    while (true) {
        float dist = get_distance();
        if (dist < 0)  printf("Error: Sensor Timeout\n");
        else printf("Distance: %d cm\n", (int)dist);

        // Wait before the next measurement to let echoes dissipate
        wait_us(200);

        //Writing the conditions for the distance
        if(dist<9) MotorsBwd();
        else if(dist>9 && dist<11) MotorsOff();
        else MotorsFwd();
    }
}






