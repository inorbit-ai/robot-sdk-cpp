/*
 * This example shows how to use the InOrbit Robot SDK for C++11 to publish
 * custom key-value data and the robot pose.
 *
 * The InOrbit agent must be running before executing this program for it to
 * work properly.
 */
#include "../include/inorbit/inorbit.hpp"
#include <iostream>
#include <stdio.h>
#include <unistd.h>

// The `start` file is used as a signal
// mechanism to trigger the simulation
#define DEMO_START_TMP_FILE "/tmp/start"
#define DEMO_DATA_POINT_DELAY 1

using namespace std;

// Position struct used on Pose
struct Position {
  Position(const float &x, const float &y, const float &z)
    : x(x), y(y), z(z) {}
  float x, y, z;
};

// Rotation struct used on Pose
struct Rotation {
  Rotation(const float &i, const float &j, const float &k, const float &w)
    : i(i), j(j), k(k), w(w) {}
  float i, j, k, w;
};

// Data point struct used on the simulation
struct RobotDataPoint {
  RobotDataPoint(const Position &position, const Rotation &rotation,
                 const string &status, const float &battery)
      : position(position), rotation(rotation), status(status), battery(battery) {}
  Position position;
  Rotation rotation;
  string status;
  float battery;
};

inline bool file_exists(const std::string& name) {
  struct stat buffer;   
  return (stat (name.c_str(), &buffer) == 0); 
}

int main(void) {

  // Data points used on the simulation
  list<RobotDataPoint> demo_data = {
    RobotDataPoint(Position{5, 5, 0}, Rotation{0.707, 0, 0, 0.707}, string("idle"), 100.0),
    RobotDataPoint(Position{5, 5, 0}, Rotation{0.707, 0, 0, 0.707}, string("idle"), 100.0),
    RobotDataPoint(Position{5, 5, 0}, Rotation{0.707, 0, 0, 0.707}, string("idle"), 100.0),
    RobotDataPoint(Position{7, 7, 0}, Rotation{0.653, 0.271, 0.271, 0.653}, string("mission"), 95.0),
    RobotDataPoint(Position{9, 9, 0}, Rotation{0.653, 0.271, 0.271, 0.653}, string("mission"), 85.0),
    RobotDataPoint(Position{11, 11, 0}, Rotation{0.653, 0.271, 0.271, 0.653}, string("mission"), 80.0),
    RobotDataPoint(Position{13, 13, 0}, Rotation{0.653, 0.271, 0.271, 0.653}, string("mission"), 80.0),
    RobotDataPoint(Position{15, 15, 0}, Rotation{0.653, 0.271, 0.271, 0.653}, string("mission"), 75.0),
    RobotDataPoint(Position{15, 15, 0}, Rotation{0.683, 0.183, 0.183, 0.683}, string("mission"), 70.0),
    RobotDataPoint(Position{15, 15, 0}, Rotation{0.701, 0.092, 0.092, 0.701}, string("mission"), 70.0),
    RobotDataPoint(Position{15, 15, 0}, Rotation{0.707, 0, 0, 0.707}, string("mission"), 65.0),
    RobotDataPoint(Position{17, 15, 0}, Rotation{0.707, 0, 0, 0.707}, string("mission"), 65.0),
    RobotDataPoint(Position{19, 15, 0}, Rotation{0.707, 0, 0, 0.707}, string("mission"), 55.0),
    RobotDataPoint(Position{21, 15, 0}, Rotation{0.707, 0, 0, 0.707}, string("mission"), 55.0),
    RobotDataPoint(Position{23, 15, 0}, Rotation{0.707, 0, 0, 0.707}, string("mission"), 50.0),
    RobotDataPoint(Position{25, 15, 0}, Rotation{0.707, 0, 0, 0.707}, string("mission"), 50.0),
    RobotDataPoint(Position{25, 15, 0}, Rotation{0.683, 0.183, 0.183, 0.683}, string("mission"), 45.0),
    RobotDataPoint(Position{25, 15, 0}, Rotation{0.612, 0.354, 0.354, 0.612}, string("mission"), 45.0),
    RobotDataPoint(Position{25, 15, 0}, Rotation{0.5, 0.5, 0.5, 0.5}, string("mission"), 45.0),
    RobotDataPoint(Position{25, 17, 0}, Rotation{0.5, 0.5, 0.5, 0.5}, string("mission"), 45.0),
    RobotDataPoint(Position{25, 19, 0}, Rotation{0.5, 0.5, 0.5, 0.5}, string("mission"), 42.0),
    RobotDataPoint(Position{25, 21, 0}, Rotation{0.5, 0.5, 0.5, 0.5}, string("mission"), 42.0),
    RobotDataPoint(Position{25, 23, 0}, Rotation{0.5, 0.5, 0.5, 0.5}, string("mission"), 40.0),
    RobotDataPoint(Position{25, 25, 0}, Rotation{0.5, 0.5, 0.5, 0.5}, string("mission"), 38.0),
    RobotDataPoint(Position{25, 25, 0}, Rotation{0.354, 0.612, 0.612, 0.354}, string("mission"), 38.0),
    RobotDataPoint(Position{25, 25, 0}, Rotation{0.183, 0.683, 0.683, 0.183}, string("mission"), 38.0),
    RobotDataPoint(Position{25, 25, 0}, Rotation{0, 0.707, 0.707, 0}, string("mission"), 35.0),
    RobotDataPoint(Position{23, 25, 0}, Rotation{0, 0.707, 0.707, 0}, string("mission"), 35.0),
    RobotDataPoint(Position{21, 25, 0}, Rotation{0, 0.707, 0.707, 0}, string("mission"), 35.0),
    RobotDataPoint(Position{19, 25, 0}, Rotation{0, 0.707, 0.707, 0}, string("mission"), 33.0),
    RobotDataPoint(Position{17, 25, 0}, Rotation{0, 0.707, 0.707, 0}, string("mission"), 32.0),
    RobotDataPoint(Position{15, 25, 0}, Rotation{0, 0.707, 0.707, 0}, string("mission"), 30.0),
    RobotDataPoint(Position{15, 25, 0}, Rotation{0.183, -0.683, -0.683, 0.183}, string("mission"), 30.0),
    RobotDataPoint(Position{15, 25, 0}, Rotation{0.354, -0.612, -0.612, 0.354}, string("mission"), 29.0),
    RobotDataPoint(Position{15, 25, 0}, Rotation{0.500, -0.500, -0.500, 0.500}, string("mission"), 28.0),
    RobotDataPoint(Position{15, 25, 0}, Rotation{0.500, -0.500, -0.500, 0.500}, string("mission"), 27.0),
    RobotDataPoint(Position{15, 23, 0}, Rotation{0.500, -0.500, -0.500, 0.500}, string("mission"), 26.0),
    RobotDataPoint(Position{15, 21, 0}, Rotation{0.500, -0.500, -0.500, 0.500}, string("mission"), 25.0),
    RobotDataPoint(Position{15, 19, 0}, Rotation{0.500, -0.500, -0.500, 0.500}, string("mission"), 24.0),
    RobotDataPoint(Position{15, 17, 0}, Rotation{0.500, -0.500, -0.500, 0.500}, string("mission"), 23.0),
    RobotDataPoint(Position{15, 15, 0}, Rotation{0.500, -0.500, -0.500, 0.500}, string("mission"), 22.0),
    RobotDataPoint(Position{15, 15, 0}, Rotation{0.271, -0.653, -0.653, 0.271}, string("mission"), 21.0),
    RobotDataPoint(Position{13, 13, 0}, Rotation{0.271, -0.653, -0.653, 0.271}, string("mission"), 20.0),
    RobotDataPoint(Position{11, 11, 0}, Rotation{0.271, -0.653, -0.653, 0.271}, string("mission"), 14.0),
    RobotDataPoint(Position{9, 9, 0}, Rotation{0.271, -0.653, -0.653, 0.271}, string("mission"), 12.0),
    RobotDataPoint(Position{7, 7, 0}, Rotation{0.271, -0.653, -0.653, 0.271}, string("mission"), 10.0),
    RobotDataPoint(Position{5, 5, 0}, Rotation{0.271, -0.653, -0.653, 0.271}, string("mission"), 8.0),
    RobotDataPoint(Position{5, 5, 0}, Rotation{0.500, -0.500, -0.500, 0.500}, string("mission"), 6.0),
    RobotDataPoint(Position{5, 5, 0}, Rotation{0.653, -0.271, -0.271, 0.653}, string("mission"), 5.0),
    RobotDataPoint(Position{5, 5, 0}, Rotation{0.707, 0, 0, 0.707}, string("charging"), 10.0),
    RobotDataPoint(Position{5, 5, 0}, Rotation{0.707, 0, 0, 0.707}, string("charging"), 20.0),
    RobotDataPoint(Position{5, 5, 0}, Rotation{0.707, 0, 0, 0.707}, string("charging"), 30.0),
    RobotDataPoint(Position{5, 5, 0}, Rotation{0.707, 0, 0, 0.707}, string("charging"), 40.0),
    RobotDataPoint(Position{5, 5, 0}, Rotation{0.707, 0, 0, 0.707}, string("charging"), 50.0),
    RobotDataPoint(Position{5, 5, 0}, Rotation{0.707, 0, 0, 0.707}, string("charging"), 60.0),
    RobotDataPoint(Position{5, 5, 0}, Rotation{0.707, 0, 0, 0.707}, string("charging"), 70.0),
    RobotDataPoint(Position{5, 5, 0}, Rotation{0.707, 0, 0, 0.707}, string("charging"), 80.0),
    RobotDataPoint(Position{5, 5, 0}, Rotation{0.707, 0, 0, 0.707}, string("charging"), 90.0),
    RobotDataPoint(Position{5, 5, 0}, Rotation{0.707, 0, 0, 0.707}, string("charging"), 100.0),
    RobotDataPoint(Position{5, 5, 0}, Rotation{0.707, 0, 0, 0.707}, string("idle"), 100.0),
  };

  // Create an InOrbitSDK object
  inorbit::InOrbitSDK sdk;

  // Run forever
  while (true) {

    // Wait for the signal to start running the demo
    cout << "Waiting for signal to start the demo" << endl;
    while (!file_exists(DEMO_START_TMP_FILE)) {
      sleep(1);
    }

    // Remove the signal file for next iteration
    if (remove(DEMO_START_TMP_FILE) != 0) {
      perror( "Error deleting file" );
      return -1;
    }

    cout << "Running the demo" << endl;

    for (list<RobotDataPoint>::iterator it = demo_data.begin(); it != demo_data.end(); ++it) {
      // Publish the pose
      double location[] = {it->position.x, it->position.y, 0};
      double rotation_quaternion[] = {it->rotation.i, it->rotation.j, it->rotation.k, it->rotation.w};
      sdk.sendPose("map", location, rotation_quaternion);
      // Publish other key value pairs
      sdk.sendKeyValue("battery percent", it->battery);
      sdk.sendKeyValue("mission status", it->status);

      // Wait between data points
      sleep(DEMO_DATA_POINT_DELAY);
    }
    cout << "No more data points" << endl;
  }

  return 0;
}
