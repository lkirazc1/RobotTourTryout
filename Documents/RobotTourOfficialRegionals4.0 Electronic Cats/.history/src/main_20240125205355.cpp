#include <Arduino.h>
#include <vector>
#include "Movements.h"
#include "Drivetrain.h"
#include "Mapping.h"


Drivetrain* drivetrain;

int cur_instruction = 0;


//Point points[] = {Point(0, 1), Point(1, 1), Point(1, 3), Point(1, 2), Point(0, 2), Point(1, 2), Point(1, 0), Point(3, 0), Point(2, 0), Point(2, 1), Point(3, 1), Point(2, 1), Point(2, 3), Point(3, 3), Point(3, 2)};
//Point points[] = {Point(0, 1), Point(1, 1), Point(1, 0), Point(3, 0), Point(2, 0), Point(2, 1), Point(3, 1), Point(2, 1), Point(2, 3), Point(3, 3), Point(3, 2)};
//Point points[] = {Point(0, 0), Point(0, 3), Point(0, 0), Point(3, 0), Point(3, 1), Point(2, 1), Point(3, 1), Point(3, 3), Point(3, 2), Point(1, 2), Point(1, 3), Point(2, 3)};
Point points[] = {Point(1, 0)};

const int points_len = sizeof(points)/sizeof(points[0]);
Point starting_point = Point(1, 0);
bool button_pressed = false;

int straight_speed = 63;
std::vector<Instruction> kInstructionsBuffer;
Instruction kInstructions[] = {
  {CMtoSteps(25), Drivetrain::FORWARD, 68},
  {10000, Drivetrain::MOVE_LEFT, 85}
};
int kNumInstructions = sizeof(kInstructions) / sizeof(kInstructions[0]);




void setup()
{ 
  Serial.begin(57600);
  
  Serial.println("hello");

  // kInstructionsBuffer = getPath(points, points_len, starting_point, 0, 87, straight_speed);
  // kNumInstructions = kInstructionsBuffer.size();
  // kInstructions = kInstructionsBuffer.data();
  Wire.begin();
  
  // Serial.print("Is working: ");
  Serial.println(is_sensor_working());
  //setup_sensor(0, 0);

  drivetrain = new Drivetrain();
  Serial.println("Drivetrain initialized");
  setup_motors();
  delay(2000);
}

void loop()
{
  if (buttonPressed() && !button_pressed)
  {
    Serial.println("loop button pressed");
    button_pressed = true;
  }

  if (button_pressed)
  {

    drivetrain->loop();
    if (!drivetrain->is_moving())
    {
      if (cur_instruction >= kNumInstructions)
      {
        return;
      }
      if (cur_instruction != 0)
      {
        if (kInstructions[cur_instruction].movement == Drivetrain::BACKWARD && kInstructions[cur_instruction - 1].movement == Drivetrain::FORWARD)
        {
          //delay(350);
        }
      }

      Serial.println("getting to go");
      drivetrain->Go(kInstructions[cur_instruction].slits, kInstructions[cur_instruction].movement, kInstructions[cur_instruction].speed_perc);
      cur_instruction++;
    }
  }
}
