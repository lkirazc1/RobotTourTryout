#include <Arduino.h>
#include "Movements.h"
#include "Drivetrain.h"

Drivetrain* drivetrain;

int cur_instruction = 0;

// 17 slits is a 90 degree turn.
//
// 50 slits is 60cm. 21 slits is 25cm (half a maze square).

const int kTurnSpeed = 50;
const int kForwardSlits = 21;

const struct
{
  int slits;
  Drivetrain::Movement movement;
  int speed_perc;
} kInstructions[] = {
    {CMtoSteps(25), Drivetrain::FORWARD, 50},
    {10000, Drivetrain::MOVE_LEFT, 80},
    {CMtoSteps(50), Drivetrain::FORWARD, 50},
    {10000, Drivetrain::MOVE_RIGHT, 80},
    {CMtoSteps(50), Drivetrain::FORWARD, 50},
    {10000, Drivetrain::MOVE_RIGHT, 80},
    {CMtoSteps(50), Drivetrain::FORWARD, 50},
    {10000, Drivetrain::MOVE_LEFT, 80},
    {CMtoSteps(50), Drivetrain::FORWARD, 50},
    {10000, Drivetrain::MOVE_RIGHT, 80},
    {CMtoSteps(50), Drivetrain::FORWARD, 50},
    {10000, Drivetrain::MOVE_LEFT, 80},
    {CMtoSteps(50), Drivetrain::FORWARD, 50},
    {CMtoSteps(50), Drivetrain::BACKWARD, 50},
    {10000, Drivetrain::MOVE_LEFT, 80},
    {CMtoSteps(100), Drivetrain::FORWARD, 50},
    {10000, Drivetrain::MOVE_RIGHT, 80},
    {CMtoSteps(50), Drivetrain::FORWARD, 50},
    {10000, Drivetrain::MOVE_RIGHT, 80},
    {CMtoSteps(50), Drivetrain::FORWARD, 50},
};
const int kNumInstructions = sizeof(kInstructions) / sizeof(kInstructions[0]);


void setup()
{
  Serial.begin(57600);
  Wire.begin();
  compass.init();
  compass.setCalibration(-1701, 1307, -1743, 1027, -1305, 1391);
  compass.read();
  delay(100);


  drivetrain = new Drivetrain();
  setup_motors();
  drivetrain->Calibrate();
}

void loop()
{
  drivetrain->loop();
  if (!drivetrain->is_moving())
  {
    if (cur_instruction >= kNumInstructions)
    {
      return;
    }
    Serial.println("Starting next instruction after 500ms");
    delay(500);
    drivetrain->Go(kInstructions[cur_instruction].slits, kInstructions[cur_instruction].movement, kInstructions[cur_instruction].speed_perc);
    cur_instruction++;
  }
}
