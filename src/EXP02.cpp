#include <FEH.h>
#include <Arduino.h>
#include <FEHUtility.h>
#include <FEHServo.h>
#include <FEHLCD.h>

// Declare things like Motors, Servos, etc. here
// For example:
// FEHMotor leftMotor(FEHMotor::Motor0, 6.0);
// FEHServo servo(FEHServo::Servo0);



void ERCMain5()
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
    DigitalEncoder left_encoder(FEHIO::Pin8);
    DigitalEncoder right_encoder(FEHIO::Pin9);
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
    //go 14 inches
    left_motor.SetPercent(25);
    right_motor.SetPercent(25);
    LCD.Write("Waiting for counts");
    while (left_encoder.Counts() < 486) {
        Sleep(0.01);
    }
    LCD.Clear();
    left_encoder.ResetCounts();
    right_encoder.ResetCounts();
    left_motor.SetPercent(0);
    right_motor.SetPercent(0);
    Sleep(1.0);
    //turn left
    left_motor.SetPercent(-20);
    right_motor.SetPercent(25);
    LCD.Write("Waiting for counts");
    while (right_encoder.Counts() < 243) {
        Sleep(0.01);
    }
    LCD.Clear();
    left_encoder.ResetCounts();
    right_encoder.ResetCounts();
    left_motor.SetPercent(0);
    right_motor.SetPercent(0);
    Sleep(1.0);
    //go 10 inches
    left_motor.SetPercent(60);
    right_motor.SetPercent(60);
    LCD.Write("Waiting for counts");
    while (left_encoder.Counts() < 243) {
        Sleep(0.01);
    }
    LCD.Clear();
    left_encoder.ResetCounts();
    right_encoder.ResetCounts();
    left_motor.SetPercent(0);
    right_motor.SetPercent(0);
    Sleep(1.0);
    //turn right
    left_motor.SetPercent(25);
    right_motor.SetPercent(-20);
    LCD.Write("Waiting for counts");
    while (left_encoder.Counts() < 243) {
        Sleep(0.01);
    }
    LCD.Clear();
    left_encoder.ResetCounts();
    right_encoder.ResetCounts();
    left_motor.SetPercent(0);
    right_motor.SetPercent(0);
    Sleep(10.0);
    //go 4 inches
    left_motor.SetPercent(40);
    right_motor.SetPercent(40);
    LCD.Write("Waiting for counts");
    while (left_encoder.Counts() < 121) {
        Sleep(0.01);
    }
    LCD.Clear();
    left_encoder.ResetCounts();
    right_encoder.ResetCounts();
    left_motor.SetPercent(0);
    right_motor.SetPercent(0);
    Sleep(1.0);

    LCD.Write("done");
}