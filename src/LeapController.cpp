#include "LeapController.h"
#include <process.h>

LEAP_CONNECTION      LeapController::_connectionHandle = NULL;
bool                 LeapController::_running          = false;
bool                 LeapController::_connected        = false;
HANDLE               LeapController::_pollingThread;
CRITICAL_SECTION     LeapController::_dataLock;
LEAP_TRACKING_EVENT* LeapController::_lastFrame = NULL;

//LeapController::LeapController() {}
LeapController::~LeapController() {
  closeLeapConnection();
  LeapDestroyConnection(_connectionHandle);
}

void LeapController::openLeapConnection() {
  if (_running) {
    return;
  }

  if (LeapController::_connectionHandle || LeapCreateConnection(NULL, &_connectionHandle) == eLeapRS_Success) {
    eLeapRS result = LeapOpenConnection(_connectionHandle);
    if (result == eLeapRS_Success) {
      _running = true;
      InitializeCriticalSection(&_dataLock);
      _pollingThread = (HANDLE)_beginthread(messageLoop, 0, NULL);
    }
  }
}

void LeapController::closeLeapConnection() {
  if (!_running) {
    return;
  }
  _running = false;
  LeapCloseConnection(_connectionHandle);
  WaitForSingleObject(_pollingThread, INFINITE);
  //CloseHandle(_pollingThread);
}

void LeapController::setFrame(const LEAP_TRACKING_EVENT* frame) {
  EnterCriticalSection(&_dataLock);
  if (!_lastFrame) {
    _lastFrame = (LEAP_TRACKING_EVENT*)malloc(sizeof(*frame));
  }
  *_lastFrame = *frame;
  LeaveCriticalSection(&_dataLock);
}

LEAP_TRACKING_EVENT* LeapController::getFrame() {
  LEAP_TRACKING_EVENT* currentFrame;
  EnterCriticalSection(&_dataLock);
  currentFrame = _lastFrame;
  LeaveCriticalSection(&_dataLock);
  return currentFrame;
}

void LeapController::messageLoop(void* unused) {
  eLeapRS                 result;
  LEAP_CONNECTION_MESSAGE msg;
  while (_running) {
    unsigned int timeout = 1000;
    result               = LeapPollConnection(_connectionHandle, timeout, &msg);

    if (result != eLeapRS_Success) {
      continue;
    }

    switch (msg.type) {
      case eLeapEventType_Connection:
        handleConnectionEvent(msg.connection_event);
        break;
      case eLeapEventType_ConnectionLost:
        handleConnectionLostEvent(msg.connection_lost_event);
        break;
      case eLeapEventType_Tracking:
        handleTrackingEvent(msg.tracking_event);
        break;
      default:
        break;
    }
  }
}

void LeapController::handleConnectionEvent(const LEAP_CONNECTION_EVENT* connection_event) {
  _connected = true;
}

void LeapController::handleConnectionLostEvent(const LEAP_CONNECTION_LOST_EVENT* connection_lost_event) {
  _connected = false;
}

void LeapController::handleTrackingEvent(const LEAP_TRACKING_EVENT* tracking_event) {
  setFrame(tracking_event);  //support polling tracking data from different thread
}
