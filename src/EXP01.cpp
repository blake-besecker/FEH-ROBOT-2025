#include <FEH.h>
#include <Arduino.h>
#include <FEHUtility.h>
#include <FEHServo.h>
#include <FEHLCD.h>

// Declare things like Motors, Servos, etc. here
// For example:
// FEHMotor leftMotor(FEHMotor::Motor0, 6.0);
// FEHServo servo(FEHServo::Servo0);


void ERCMain2()
{
    
    FEHServo servo(FEHServo::Servo0); // declare servo arm

    // servo.TouchCalibrate(); // calibrate servo

    // int min = 500;
    // int max = 2290;
    // int light = 5;
    // servo.SetMin(min);
    // servo.SetMax(max);
    // AnalogInputPin cell(FEHIO::Pin1);
    // while (1==1) {
    //     int deg = 180*(cell.Value()/light);
    //     servo.SetDegree(deg);
    //     Sleep(0.35);
    // }

    // while (1==1) {
    //     LCD.Clear();
    //     LCD.WriteLine(cell.Value());
    //     Sleep(0.5);
    // }
    FEHMotor right_motor(FEHMotor::Motor0, 9); 
    FEHMotor left_motor(FEHMotor::Motor1, 9); 

    DigitalInputPin front_left(FEHIO::Pin14);
    DigitalInputPin back_left(FEHIO::Pin15);
    DigitalInputPin front_right(FEHIO::Pin0);
    DigitalInputPin back_right(FEHIO::Pin2);
    //a. wait for first input
    LCD.Write("waiting for input");
    while(back_right.Value()) {
        Sleep(0.01);
    }
    //b. go forward and wait to hit wall
    LCD.Clear();
    LCD.Write("going forward until I hit a wall");
    right_motor.SetPercent(50);
    left_motor.SetPercent(50);
    while(front_left.Value() && front_right.Value()) {
        Sleep(0.01);
    }
    right_motor.SetPercent(0);
    left_motor.SetPercent(0);
    Sleep(1.0);
    //c. stop and turn until you hit the back wall
    LCD.Clear();
    LCD.Write("turning until I hit a wall");
    right_motor.SetPercent(-25);
    while(back_left.Value() || back_right.Value()) {
        Sleep(0.01);
    }

    right_motor.SetPercent(0);
    Sleep(1.0);
    //d. go until you hit the final wall
    LCD.Clear();
    LCD.Write("going forward until I hit the final wall");
    right_motor.SetPercent(50);
    left_motor.SetPercent(50);
    while(front_left.Value() && front_right.Value()) {
        Sleep(0.01);
    }
    right_motor.SetPercent(0);
    left_motor.SetPercent(0);
    //e. turn until you hit the back wall
    LCD.Clear();
    LCD.Write("turning until I hit a wall");
    left_motor.SetPercent(-25);
    while( back_left.Value()) {
        Sleep(0.01);
    }
    right_motor.SetPercent(-25);
    while( back_left.Value() || back_right.Value()) {
        Sleep(0.01);
    }
    right_motor.SetPercent(0);
    left_motor.SetPercent(0);
    Sleep(1.0);
    //f. go forward until you hit the final final wall
    LCD.Clear();
    LCD.Write("going forward until I hit the final final wall");
    right_motor.SetPercent(50);
    left_motor.SetPercent(50);
    while(front_left.Value() && front_right.Value()) {
        Sleep(0.01);
    }
    right_motor.SetPercent(0);
    left_motor.SetPercent(0);
    LCD.Clear();
    LCD.Write("DONE");

}