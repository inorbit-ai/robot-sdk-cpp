/*
 * This example shows how to use the InOrbit Robot SDK for C++11 to publish
 * custom key-value data and the robot pose.
 *
 * The InOrbit agent must be running before executing this program for it to
 * work properly.
 */
#include "include/inorbit/inorbit.hpp"
#include <iostream>

using namespace std;

int main(void) {
  // Create an InOrbitSDK object
  inorbit::InOrbitSDK sdk;

  // Publish custom data of different types: string, double, integer
  sdk.sendKeyValue("vendor", "Spoonlift");
  sdk.sendKeyValue("battery", 0.56);
  sdk.sendKeyValue("pending_tasks", 5);

  // Publish the pose
  double location[] = {5, 4, 0};
  double rotation_quaternion[] = {0, 0, -0.131, 0.991};
  sdk.sendPose("my_reference_frame", location, rotation_quaternion);

  cout << "You should see the reported data at InOrbit" << endl;

  return 0;
}
