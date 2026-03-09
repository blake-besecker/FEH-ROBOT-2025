#include <FEH.h>
#include <Arduino.h>
#include <FEHUtility.h>
#include <FEHServo.h>
#include <FEHLCD.h>

// Declare things like Motors, Servos, etc. here
// For example:
// FEHMotor leftMotor(FEHMotor::Motor0, 6.0);
// FEHServo servo(FEHServo::Servo0);



void ERCMain4()
{
    FEHServo servo(FEHServo::Servo0); // declare servo arm
    //motors
    FEHMotor right_motor(FEHMotor::Motor0, 9); 
    FEHMotor left_motor(FEHMotor::Motor1, 9); 
    //switches
    // DigitalInputPin front_left(FEHIO::Pin14);
    // DigitalInputPin back_left(FEHIO::Pin0);
    // DigitalInputPin front_right(FEHIO::Pin0);
    //DigitalInputPin back_right(FEHIO::Pin1);
    //encoders
    DigitalEncoder left_encoder(FEHIO::Pin9);
    DigitalEncoder right_encoder(FEHIO::Pin8);
    //cell sensors
    AnalogInputPin right_cell(FEHIO::Pin12);
    AnalogInputPin middle_cell(FEHIO::Pin13);
    AnalogInputPin left_cell(FEHIO::Pin14);
    //init sensor vars
    float left = 0, middle=0, right=0;
    //wait for back right switch to be hit to start
    LCD.Write("Waiting for input to start(touch 0,0)");
    while (!LCD.Touch(0,0)) {
        Sleep(0.01);
    }
    //update sensor values
    left = left_cell.Value(), middle = middle_cell.Value(), right = right_cell.Value();
    //while at least one sensor is on the path
    while(1==1) {
        //a. update sensor values
        left = left_cell.Value(), middle = middle_cell.Value(), right = right_cell.Value();
        //c. set motor values according to sensor
        //if we're on the left(only left is hit)
        while (left >= 2.5) {
            //turn right and go forward
            left = left_cell.Value(), middle = middle_cell.Value(), right = right_cell.Value();
            left_motor.SetPercent(20);
            right_motor.SetPercent(-40);
            Sleep(0.01);
        }
        //if we're on the right(only right is hit)
        while (right >= 2.5) {
            left = left_cell.Value(), middle = middle_cell.Value(), right = right_cell.Value();
            left_motor.SetPercent(40);
            right_motor.SetPercent(-20);
            Sleep(0.01);
        } 
        //we must be in the middle
        while (middle >= 2.5) {
            left = left_cell.Value(), middle = middle_cell.Value(), right = right_cell.Value();
            left_motor.SetPercent(20);
            right_motor.SetPercent(-20);
            Sleep(0.01);
        }
        //d. sleep (set check interval)
        Sleep(0.01);
        //e. update sensor values
        left = left_cell.Value(), middle = middle_cell.Value(), right = right_cell.Value();
    }
    left_motor.SetPercent(0);
    right_motor.SetPercent(0);
    LCD.Write("done");
}