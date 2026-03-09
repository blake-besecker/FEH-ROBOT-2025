#include <FEH.h>
#include <Arduino.h>
#include <FEHUtility.h>
#include <FEHServo.h>
#include <FEHLCD.h>

// Declare things like Motors, Servos, etc. here
// For example:
// FEHMotor leftMotor(FEHMotor::Motor0, 6.0);
// FEHServo servo(FEHServo::Servo0);



void ERCMain3()
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
    //while at least one sensor is on the path
    while (1==1) {
        //b. write sensor values to screen
        LCD.Clear();
        LCD.Write("left: "); 
        LCD.Write(left_cell.Value());
        LCD.Write(" middle: ");
        LCD.Write(middle_cell.Value());
        LCD.Write(" right: ");
        LCD.Write(right_cell.Value());
        Sleep(0.2);
    }
    LCD.Write("done");
}