#include <FEH.h>
#include <Arduino.h>
#include <FEHUtility.h>
#include <FEHServo.h>
#include <FEHLCD.h>

// Declare things like Motors, Servos, etc. here
// For example:
// FEHMotor leftMotor(FEHMotor::Motor0, 6.0);
// FEHServo servo(FEHServo::Servo0);



void ERCMain()
{
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
    //constants
    const int MOVE_UP_RAMP = 1550;
    const int TURN_LEFT=220;
    const int TURN_BLUE=30;
    const int TURN_RED=30;
    const int START=175;
    const int TURN_RAMP=320;
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
    //move to light
    left_encoder.ResetCounts();
    left_motor.SetPercent(20);
    right_motor.SetPercent(20);
    boolean done = false;
    while(1 ==1 ) {
        LCD.Clear();
        LCD.WriteLine(cds.Value());
        if (cds.Value() < 0.35 && cds.Value() > 0.3) {
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
        done = true;
    }
    else if (cds.Value() < 0.15) {
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
        done = true;
    }
    Sleep(0.01);
    if (done) break;
    }
    //read light, and depending on what it says write the color



}