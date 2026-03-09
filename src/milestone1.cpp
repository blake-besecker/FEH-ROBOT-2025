#include <FEH.h>
#include <Arduino.h>
#include <FEHUtility.h>
#include <FEHServo.h>
#include <FEHLCD.h>

// Declare things like Motors, Servos, etc. here
// For example:
// FEHMotor leftMotor(FEHMotor::Motor0, 6.0);
// FEHServo servo(FEHServo::Servo0);



void ERCMain9()
{
    FEHServo servo(FEHServo::Servo0); // declare servo arm
    //motors
    FEHMotor right_motor(FEHMotor::Motor2, 9); 
    FEHMotor left_motor(FEHMotor::Motor0, 9); 
    //encoders
    DigitalEncoder left_encoder(FEHIO::Pin8);
    DigitalEncoder right_encoder(FEHIO::Pin9);
    //switches
    // DigitalInputPin front_left(FEHIO::Pin14);
    // DigitalInputPin back_left(FEHIO::Pin0);
    // DigitalInputPin front_right(FEHIO::Pin0);
    //DigitalInputPin back_right(FEHIO::Pin1);
    //cell sensors
    AnalogInputPin right_cell(FEHIO::Pin5);
    AnalogInputPin middle_cell(FEHIO::Pin3);
    AnalogInputPin left_cell(FEHIO::Pin1);
    //init sensor vars
    //wait for back right switch to be hit to start
    LCD.Write("Waiting for input to start(touch 0,0)");
    while (!LCD.Touch(0,0)) {
        Sleep(0.01);
    }
    Sleep(0.3);
    left_motor.SetPercent(40);
    right_motor.SetPercent(40);
    while (!LCD.Touch(0,0)) {
        Sleep(0.01);
    }
    left_motor.SetPercent(0);
    right_motor.SetPercent(0);
    Sleep(1.0);
    LCD.Write("Waiting for input #2 to start(touch 0,0)");
    while (!LCD.Touch(0,0)) {
        Sleep(0.01);
    }
    Sleep(1.0);
    left_encoder.ResetCounts();
    left_motor.SetPercent(40);
    right_motor.SetPercent(40);
    while(left_encoder.Counts()<1500) {
        Sleep(0.01);
    }
    left_motor.SetPercent(0);
    right_motor.SetPercent(0);
    left_encoder.ResetCounts();
    Sleep(0.5);
    left_motor.SetPercent(-40);
    right_motor.SetPercent(-40);
}