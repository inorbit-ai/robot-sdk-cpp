# InOrbit Robot SDK C++ Library

The InOrbit Robot SDK C++11 **header-only** library allows C++ programs to communicate with the **InOrbit Agent** that must run also on the robot. It's goal is to ease the integration between InOrbit and any other custom software running on the robot.

The following example shows how to use the library to send custom data and poses to the Agent:

```cpp
#include "include/inorbit/inorbit.hpp"

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
```

When running this program, the Agent takes care of reporting the data to the InOrbit Platform in the cloud.

## Including the Library in a Project

This library can be included in any C++11 project. To do so, you have to copy `include/inorbit` to your project and include the `inorbit.hpp` file from your files.
