#pragma once
#include <Windows.h>
#include "LeapC.h"

class LeapController {
 public:
  //LeapController();
  virtual ~LeapController();
  void                 openLeapConnection();
  void                 closeLeapConnection();
  bool                 isConnected() { return LeapController::_connected; }
  LEAP_TRACKING_EVENT* getFrame();

 private:
  static void messageLoop(void* unused);
  static void handleConnectionEvent(const LEAP_CONNECTION_EVENT* connection_event);
  static void handleConnectionLostEvent(const LEAP_CONNECTION_LOST_EVENT* connection_lost_event);
  static void handleTrackingEvent(const LEAP_TRACKING_EVENT* tracking_event);
  static void setFrame(const LEAP_TRACKING_EVENT* frame);

 private:
  static LEAP_CONNECTION      _connectionHandle;
  static HANDLE               _pollingThread;
  static CRITICAL_SECTION     _dataLock;
  static bool                 _running;
  static bool                 _connected;
  static LEAP_TRACKING_EVENT* _lastFrame;
};
