#pragma once
#include <thread>
#include <mutex>
#include "LeapC.h"
#include "spdlog/spdlog.h"

// printf("Fatal error in calling function: %s: 0x%X\n", #func, result);
#define LEAPC_CHECK(func)                                                                     \
  do {                                                                                        \
    eLeapRS result = (func);                                                                  \
    if (result != eLeapRS_Success) {                                                          \
      spdlog::critical("Fatal error in calling function: '{}' result: {:#x}", #func, result); \
    }                                                                                         \
  } while (0)

class LeapController {
 public:
  LeapController();
  virtual ~LeapController();
  void                 openLeapConnection();
  void                 closeLeapConnection();
  bool                 isConnected() { return _connected; }
  LEAP_TRACKING_EVENT* getFrame();

 private:
  void messageLoop();
  void handleConnectionEvent(const LEAP_CONNECTION_EVENT* connection_event);
  void handleConnectionLostEvent(const LEAP_CONNECTION_LOST_EVENT* connection_lost_event);
  void handleTrackingEvent(const LEAP_TRACKING_EVENT* tracking_event);
  void handleDeviceEvent(const LEAP_DEVICE_EVENT* device_event);
  void setFrame(const LEAP_TRACKING_EVENT* frame);

 private:
  LEAP_CONNECTION      _connectionHandle;
  std::thread          _pollingThread;
  std::mutex           _dataLock;
  bool                 _running   = false;
  bool                 _connected = false;
  LEAP_TRACKING_EVENT* _lastFrame = nullptr;
};
