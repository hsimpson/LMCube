#pragma once
#include "LeapC.h"
#include <thread>
#include <mutex>

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
  void setFrame(const LEAP_TRACKING_EVENT* frame);

 private:
  LEAP_CONNECTION      _connectionHandle;
  std::thread          _pollingThread;
  std::mutex           _dataLock;
  bool                 _running   = false;
  bool                 _connected = false;
  LEAP_TRACKING_EVENT* _lastFrame = nullptr;
};
