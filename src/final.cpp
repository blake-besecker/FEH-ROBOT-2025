#include <FEH.h>
#include <Arduino.h>
#include <FEHUtility.h>
#include <FEHServo.h>
#include <FEHLCD.h>
#include <FEHRCS.h>

// Declare things like Motors, Servos, etc. here
// For example:
// FEHMotor leftMotor(FEHMotor::Motor0, 6.0);
// FEHServo servo(FEHServo::Servo0);
    const int ARM_MIN = 800;
    const int ARM_MAX = 2500;
    FEHServo arm(FEHServo::Servo0); // declare servo arm
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

void followPath() {
    float left = left_cell.Value(), middle = middle_cell.Value(), right = right_cell.Value();
    //while at least one sensor is not on the path
    while (left > 3.2 || right > 3.2 || middle > 3.2) {
        //a. update sensor values
        left = left_cell.Value(), middle = middle_cell.Value(), right = right_cell.Value();
        //c. set motor values according to sensor
        //if we're on the left(only left is hit)
        LCD.Clear();
        LCD.WriteLine(left_cell.Value());
        LCD.WriteLine(middle_cell.Value());
        LCD.WriteLine(right_cell.Value());
        while (left > 3.2) {
            //left right and go forward
            left = left_cell.Value(), middle = middle_cell.Value(), right = right_cell.Value();
            left_motor.SetPercent(20);
            right_motor.SetPercent(30);
            Sleep(0.001);
        } 
        //if we're on the right(only right is hit)
        while (right > 3.2) {
            left = left_cell.Value(), middle = middle_cell.Value(), right = right_cell.Value();
            left_motor.SetPercent(30);
            right_motor.SetPercent(20);
            Sleep(0.001);
        } 
        //we must be in the middle
        while (middle > 3.2) {
            left = left_cell.Value(), middle = middle_cell.Value(), right = right_cell.Value();
            left_motor.SetPercent(20);
            right_motor.SetPercent(20);
            Sleep(0.001);
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

void drive(int motorPercents, int encoderCounts) {
    //drive with given percent until encoder counts are done
    left_motor.SetPercent(motorPercents);
    right_motor.SetPercent(motorPercents);
    while (left_encoder.Counts() < encoderCounts) {
        Sleep(0.01);
    }
    //stop
    left_motor.SetPercent(0);
    right_motor.SetPercent(0);
    left_encoder.ResetCounts();
}

void turn(String direction, int motorPercents, int encoderCounts) {
    int directionMultLeft = 1, directionMultRight = 1;
    if (direction.equals("left")) {
        directionMultLeft = -1;
    } else {
        directionMultRight = -1;
    }
    //drive with given opposite percent until encoder counts are done
    left_motor.SetPercent(motorPercents*directionMultLeft);
    right_motor.SetPercent(motorPercents*directionMultRight);
    while (left_encoder.Counts() < encoderCounts) {
        Sleep(0.01);
    }
    //stop
    left_motor.SetPercent(0);
    right_motor.SetPercent(0);
    left_encoder.ResetCounts();
}

void doButtons() {
    const int MOVE_BUTTON = 375;
    const int TURN_BLUE=35;
    const int TURN_RED=35;
    const int MOVE_LIGHT=120;
    followPath();
    drive(20, MOVE_LIGHT);
    Sleep(0.25);
    LCD.Clear();
    LCD.WriteLine(cds.Value());
    if (cds.Value() > 0.3) {
        LCD.Clear();
        LCD.WriteLine("it's blue");
        LCD.WriteLine(cds.Value());
        //stop
        left_motor.SetPercent(0);
        right_motor.SetPercent(0);
        Sleep(0.5);
        //turn to face blue
        turn("left", 20, TURN_BLUE);
        //drive straight
        drive(40, MOVE_BUTTON);
    }
    else {
        LCD.Clear();
        LCD.WriteLine("it's red");
        LCD.WriteLine(cds.Value());
        left_motor.SetPercent(0);
        right_motor.SetPercent(0);
        Sleep(0.5);
        //code for hitting red button
        //turn to face red
        turn("left", 20, TURN_RED);
        //drive straight
        drive(40, MOVE_BUTTON);
    }
}

void doWindow() {
    const int TURN_WINDOW=360;
    // const int TURN_WINDOW=400;
    const int OPEN_WINDOW=160;
    const int MOVE_WINDOW=840;
    const int PAST_WINDOW=60;
    //turn to face window
    turn("left", 30, TURN_WINDOW);
    //go to window
    drive(40, MOVE_WINDOW);
    Sleep(0.5);
    //move arm to catch window handle
    arm.SetDegree(25);
    //turn to be parallel to window
    turn("right", 30, OPEN_WINDOW);
    Sleep(0.5);
    //move arm to catch window handle
    // arm.SetDegree(0);
    // //turn to be past window
    // turn("right", 30, PAST_WINDOW);
    // Sleep(0.5);
    // //move arm to catch window handle
    // arm.SetDegree(20);
    // //turn to be back at window now
    // turn("left", 30, PAST_WINDOW);
    // Sleep(0.5);
    //turn to close window/reset to starting position
    arm.SetDegree(0);
    turn("left", 30, OPEN_WINDOW);
    Sleep(0.5);
    //go back to start
    drive(-40, MOVE_WINDOW);
}
void doApplesAndLevers() {
    const int START=100;
    const int TURN_AFTER_BUCKET=470;
    const int TURN_BUCKET=95;
    const int MOVE_BUCKET=745;
    const int TURN_RAMP=660;
    const int MOVE_RAMP=1500;
    const int ADJUST=140;
    const int MOVE_ADJUST=280;
    const int MOVE_CRATE=450;
    const int TURN_LEVERS=187;
    const int MOVE_LEVERS=200;
    const int RAMP_OFFSET=0;
    const int ADJUST_OFFSET=10;
    const int BACK_UP=100;
    const int LEVER_ADJUST=20;
    //set arm mins and maxes;
    //turn to face bucket
    turn("left", 30, TURN_BUCKET);
    //flip arm up to get apple
    arm.SetDegree(90);
    //go to it
    drive(40, MOVE_BUCKET);
    Sleep(0.25);
    //flip arm up to hold apple(gradually)
    for (int i = 0; i<75;i++) {
        arm.SetDegree(85-i);
        Sleep(0.005);
    }
    Sleep(0.25);
    //turn to go to start(should be same as bucket)
    turn("right", 30, TURN_AFTER_BUCKET);
    //go to start
    drive(40, MOVE_BUCKET);
    //turn to go to ramp
    turn("right", 30, TURN_RAMP);
    // drive up ramp
    drive(40, MOVE_RAMP);
    Sleep(0.1);
    //turn to dodge stand
    turn("left", 30, ADJUST);
    Sleep(0.1);
    //move to dodge stand
    drive(40, MOVE_ADJUST);
    Sleep(0.1);
    //turn to set back
    turn("right", 30, ADJUST+ADJUST_OFFSET);
    Sleep(0.1);
    //move to crate
    drive(40, MOVE_CRATE);
    //flip arm down to drop apple
    arm.SetDegree(100);
    //back up a bit
    drive(-40, BACK_UP);
    //stop
    //turn to go to levers
    Sleep(0.25);
    turn("left", 30, TURN_LEVERS);
    Sleep(0.25);
    //flip arm up to get on top of levers
    arm.SetDegree(40);
    Sleep(0.5);
    drive(40, MOVE_LEVERS);
    Sleep(0.5);
    //flip arm down to flip switch
    arm.SetDegree(130);
    Sleep(0.5);
    //back up
    drive(-40, BACK_UP);
    Sleep(0.5);
    //turn a little
    Sleep(0.5);
    //flip arm down to prepare to flip up
    arm.SetDegree(180);
    Sleep(0.5);
    //go back(sleep some to give it time)
    drive(40, BACK_UP);    
    Sleep(0.5);
    //flip arm up to flip lever up
    arm.SetDegree(110);
    Sleep(0.5);
}

void doComposter() {
    //composter constants
    const int TURN_COMPOSTER=500;
    const int MOVE_COMPOSTER=640;
    const int TURN_FACE_COMPOSTER=263;
    const int topPosition = 40;
    const int bottomPosition = 180;
    const int BACK_UP = 150;
    const int SMALL_BACK_UP = 20;
    const int FINAL_ARM_OFFSET = 35;

    int difference = bottomPosition-topPosition;
    turn("left", 40, TURN_COMPOSTER);
    drive(40, MOVE_COMPOSTER);
    turn("left", 40, TURN_FACE_COMPOSTER);
    drive(-40, SMALL_BACK_UP);
    Sleep(0.25);
    //if we're just initiating it
    for (int i =0; i<130; i++) {
        arm.SetDegree(i);
        Sleep(0.01);
    }
    Sleep(0.25);
    arm.SetDegree(0);
    Sleep(0.25);
    //if we're actually gonna do it

    // for (int j = 0; j<3; j++) {
    //     for (int i =0; i<difference; i++) {
    //         arm.SetDegree(topPosition+i);
    //         Sleep(0.01);
    //     }
    //     Sleep(0.25);
    //     drive(-40, BACK_UP);
    //     Sleep(0.25);
    //     arm.SetDegree(topPosition);
    //     Sleep(0.25);
    //     drive(40, BACK_UP);
    //     Sleep(0.25);
    // }
    // for (int i =0; i<(difference-FINAL_ARM_OFFSET); i++) {
    //     arm.SetDegree(topPosition+i);
    //     Sleep(0.01);
    // }

    drive(40, SMALL_BACK_UP);
    Sleep(0.25);
    turn("right", 40, TURN_FACE_COMPOSTER);
    Sleep(0.25);
    arm.SetDegree(0);
    drive(-40, MOVE_COMPOSTER);
    Sleep(0.25);
    turn("right", 40, TURN_COMPOSTER);
    Sleep(0.25);
}

void ERCMain()
{
    const int START=125;
    //going to buttons constants
    const int TURN_TO_BUTTONS=320;
    const int DRIVE_TO_BUTTON_START=550;
    const int ALIGN_BUTTON_START=350;
    //finishing constants
    const int BACK_OFF_BUTTONS=700;
    const int TURN_TO_END=200;
    const int GO_TO_END=1600;
    //set arm mins and maxes;
    arm.SetMin(ARM_MIN);
    arm.SetMax(ARM_MAX);
    //move arm fully up
    arm.SetDegree(0);
    RCS.InitializeTouchMenu("0150F3VKF");
    WaitForFinalAction();
    // waiting for start light
    // LCD.Write("waiting for start light");
    // while (cds.Value() > 0.15) {
    //     Sleep(0.01);
    // }
    Sleep(30.0);
    drive(40, START);
    Sleep(0.25);
    //COMPOSTER
    doComposter();
    //WINDOW
    //do window(ends with a reset back to start)
    doWindow();
    //APPLES AND LEVERS
    //do apples(pickup apple bucket and drop it off in the crate)
    doApplesAndLevers();
    arm.SetDegree(0);
    //HUMIDIFIER BUTTONS
    //turn and drive to button corner
    turn("left", 40, TURN_TO_BUTTONS);
    Sleep(0.5);
    drive(40, DRIVE_TO_BUTTON_START);
    Sleep(0.5);
    turn("right", 40, ALIGN_BUTTON_START);
    Sleep(0.5);
    //do buttons(assumes you are in front of black line facing buttons)
    arm.SetDegree(0);
    doButtons();
    //GO BACK TO END
    //back up
    drive(-40, BACK_OFF_BUTTONS);
    turn("left", 40, TURN_TO_END);
    //turn to face end button
    //drive back and hit end button
    drive(40, GO_TO_END);
}