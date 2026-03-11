#include <FEH.h>
#include <Arduino.h>
#include <FEHUtility.h>
#include <FEHServo.h>
#include <FEHLCD.h>

// Declare things like Motors, Servos, etc. here
// For example:
// FEHMotor leftMotor(FEHMotor::Motor0, 6.0);
// FEHServo servo(FEHServo::Servo0);

    FEHServo servo(FEHServo::Servo0); // declare servo arm
    //motors
    FEHMotor right_motor(FEHMotor::Motor2, 9); 
    FEHMotor left_motor(FEHMotor::Motor0, 9); 
    //encoders
    DigitalEncoder left_encoder(FEHIO::Pin8);
    DigitalEncoder right_encoder(FEHIO::Pin9);
    //switches
    DigitalInputPin back_left(FEHIO::Pin11);
    DigitalInputPin back_right(FEHIO::Pin12);
    //cell sensors
    AnalogInputPin right_cell(FEHIO::Pin5);
    AnalogInputPin middle_cell(FEHIO::Pin3);
    AnalogInputPin left_cell(FEHIO::Pin1);
    //cds cell
    AnalogInputPin cds(FEHIO::Pin14);

void followPath(float time) {
    float left = left_cell.Value(), middle = middle_cell.Value(), right = right_cell.Value();
    float timestart = TimeNow();
    //while at least one sensor is on the path
    while (TimeNow() - timestart <= time) {
        //a. update sensor values
        left = left_cell.Value(), middle = middle_cell.Value(), right = right_cell.Value();
        //c. set motor values according to sensor
        //if we're on the left(only left is hit)
        LCD.Clear();
        LCD.WriteLine(left_cell.Value());
        LCD.WriteLine(middle_cell.Value());
        LCD.WriteLine(right_cell.Value());
        while (left > 3.2 && middle < 3.2 && right < 3.2) {
            //left right and go forward
            left = left_cell.Value(), middle = middle_cell.Value(), right = right_cell.Value();
            left_motor.SetPercent(20);
            right_motor.SetPercent(30);
            Sleep(0.01);
        }
        //if we're on the right(only right is hit)
        while (right > 3.2 && middle < 3.2 && left < 3.2) {
            left = left_cell.Value(), middle = middle_cell.Value(), right = right_cell.Value();
            left_motor.SetPercent(30);
            right_motor.SetPercent(20);
            Sleep(0.01);
        } 
        //we must be in the middle
        while (middle > 3.2) {
            left = left_cell.Value(), middle = middle_cell.Value(), right = right_cell.Value();
            left_motor.SetPercent(20);
            right_motor.SetPercent(20);
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

void ERCMain()
{
    //constants
    const int MOVE_UP_RAMP = 1550;
    const int TURN_LEFT=220;
    const int TURN_BLUE=30;
    const int TURN_RED=30;
    const int START=175;
    const int TURN_RAMP=330;
    //waiting for start light
    LCD.Write("waiting for start light");
    while (cds.Value() > 0.15) {
        Sleep(0.01);
    }
    //backup until hit start button(back right is )
    left_motor.SetPercent(40);
    right_motor.SetPercent(40);
    while (left_encoder.Counts() < START) {
        Sleep(0.01);
    }
    //stop
    left_motor.SetPercent(0);
    right_motor.SetPercent(0);
    //turn to face ramp
    left_encoder.ResetCounts();
    left_motor.SetPercent(-30);
    right_motor.SetPercent(30);
    while(left_encoder.Counts() < TURN_RAMP) {
        Sleep(0.01);
    }
    left_encoder.ResetCounts();
    left_motor.SetPercent(0);
    right_motor.SetPercent(0);
    //drive up ramp
    left_motor.SetPercent(40);
    right_motor.SetPercent(40);
    while(left_encoder.Counts() < MOVE_UP_RAMP) {
        Sleep(0.01);
    }
    left_motor.SetPercent(0);
    right_motor.SetPercent(0);
    //turn to light
    left_encoder.ResetCounts();
    left_motor.SetPercent(-20);
    right_motor.SetPercent(20);
    while(left_encoder.Counts() < TURN_LEFT) {
        Sleep(0.01);
    }
    left_motor.SetPercent(0);
    right_motor.SetPercent(0);

    followPath(20.0);
    left_encoder.ResetCounts();
    left_motor.SetPercent(20);
    right_motor.SetPercent(20);
    while(cds.Value() > 1.0) {
        Sleep(0.01);
    }
    left_motor.SetPercent(0);
    right_motor.SetPercent(0);
    Sleep(0.5);
    LCD.Clear();
    LCD.WriteLine(cds.Value());
        if (cds.Value() > 0.5) {
        LCD.Clear();
        LCD.WriteLine("it's blue");
        left_motor.SetPercent(0);
        right_motor.SetPercent(0);
        Sleep(0.5);
        //code for hitting blue button
        //turn to face blue
        right_encoder.ResetCounts();
        left_motor.SetPercent(0);
        right_motor.SetPercent(10);
        while(right_encoder.Counts() < TURN_BLUE) {
            Sleep(0.01);
        }
        //drive straight
        left_motor.SetPercent(15);
        right_motor.SetPercent(15);
    }
    else if (cds.Value() < 0.5) {
        LCD.Clear();
        LCD.WriteLine("it's red");
        left_motor.SetPercent(0);
        right_motor.SetPercent(0);
        Sleep(0.5);
        //code for hitting red button
        //turn to face red
        left_encoder.ResetCounts();
        left_motor.SetPercent(10);
        right_motor.SetPercent(0);
        while(left_encoder.Counts() < TURN_RED) {
            Sleep(0.01);
        }
        //drive straight
        left_motor.SetPercent(15);
        right_motor.SetPercent(15);
    }
}