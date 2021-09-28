/*
 InOrbit Robot SDK for C++11

 This library allows C++ programs to communicate with the InOrbit Agent to
 send and receive data from the InOrbit Platform.

 Copyright 2021 InOrbit, Inc.

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
*/

#ifndef INORBIT_SDK_HPP
#define INORBIT_SDK_HPP

#define INORBIT_SDK_DEFAULT_HOST "localhost"
#define INORBIT_SDK_DEFAULT_PORT 5000

#include <algorithm>
#include "thirdparty/httplib.h"

namespace inorbit {

/**
 * Implements the communication with the Agent and provides methods to send
 * and receive data to/from it.
 */
class InOrbitSDK {
public:
  /**
   * Initializes the SDK
   *
   * @param agent_host The hostname or IP address where the InOrbit agent is
   * running, default is "localhost"
   * @param agent_port Agent TCP port, default is 5000.
   * @param log_errors Log errors to std::cerr, default is true
   */
  explicit InOrbitSDK(const std::string &agent_host, int agent_port,
                      bool log_errors);

  /**
   * Initializes the SDK
   *
   * @overload
   */
  explicit InOrbitSDK(const std::string &agent_host, int agent_port);

  /**
   * Initializes the SDK
   *
   * @overload
   */
  explicit InOrbitSDK();

  /**
   * Sends a key-value pair
   *
   * @param key
   * @param value
   * @return true if the data was sent, false if there was an error
   */
  bool sendKeyValue(const std::string &key, const std::string &value);

  /**
   * Sends a key-value pair
   *
   * @overload
   */
  bool sendKeyValue(const std::string &key, const double &value);

  /**
   * Sends a key-value pair
   *
   * @overload
   */
  bool sendKeyValue(const std::string &key, const int &value);

  /**
   * Sends the robot pose
   *
   * @param frameId reference frame identification
   * @param t Robot location
   * @param r Robot orientation specified as a quaternion
   * @return true if the data was sent, false if there was an error
   */
  bool sendPose(const std::string &frameId, const double t[3],
                const double r[4]);

protected:
  const std::string agent_host;
  const int agent_port;
  const bool log_errors;
  bool doPost(const std::string &endpoint, const std::string &data,
              const std::string &content_type);

private:
  std::unique_ptr<httplib::Client> http_;
};

inline InOrbitSDK::InOrbitSDK()
    : InOrbitSDK(INORBIT_SDK_DEFAULT_HOST, INORBIT_SDK_DEFAULT_PORT) {}

inline InOrbitSDK::InOrbitSDK(const std::string &agent_host, int agent_port)
    : agent_host(agent_host), agent_port(agent_port), log_errors(true),
      http_(std::unique_ptr<httplib::Client>(
          new httplib::Client(agent_host, agent_port))) {}

inline InOrbitSDK::InOrbitSDK(const std::string &agent_host, int agent_port,
                              bool log_errors)
    : agent_host(agent_host), agent_port(agent_port), log_errors(log_errors),
      http_(std::unique_ptr<httplib::Client>(
          new httplib::Client(agent_host, agent_port))) {}

inline bool InOrbitSDK::sendKeyValue(const std::string &key, const int &value) {
  return sendKeyValue(key, std::to_string(value));
}

inline bool InOrbitSDK::sendKeyValue(const std::string &key,
                                     const double &value) {
  return sendKeyValue(key, std::to_string(value));
}

inline bool InOrbitSDK::sendKeyValue(const std::string &key,
                                     const std::string &value) {
  std::string escaped_key = key;
  std::replace(escaped_key.begin(), escaped_key.end(), '/', '_');
  return doPost("/api/v1/data/" + escaped_key, value, "text/plain");
}

inline bool InOrbitSDK::sendPose(const std::string &frameId, const double t[3],
                                 const double r[4]) {
  const std::string data =
      "{ \"frame_id\": \"" + frameId + "\", \"x\": " + std::to_string(t[0]) +
      ", \"y\": " + std::to_string(t[1]) + ", \"z\": " + std::to_string(t[2]) +
      ", \"q\": { \"x\": " + std::to_string(r[0]) +
      ", \"y\": " + std::to_string(r[1]) + ", \"z\": " + std::to_string(r[2]) +
      ", \"w\": " + std::to_string(r[3]) + "  } }";
  return doPost("/api/v1/pose", data, "application/json");
}

inline bool InOrbitSDK::doPost(const std::string &endpoint,
                               const std::string &data,
                               const std::string &content_type) {

  auto res = http_->Post(endpoint.c_str(), data.c_str(), content_type.c_str());
  if (!res) {
    if (log_errors) {
      std::cerr << "InOrbitSDK error: " << res.error() << std::endl;
    }
    return false;
  }
  if (res->status < 200 || res->status > 300) {
    if (log_errors) {
      std::cerr << "InOrbitSDK error: " << res->status << " " << res->body
                << std::endl;
    }
    return false;
  }
  return true;
}

} // end namespace inorbit

#endif