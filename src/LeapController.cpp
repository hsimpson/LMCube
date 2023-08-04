#include "LeapController.h"
#include "spdlog/spdlog.h"

LeapController::LeapController() {}

LeapController::~LeapController() {
  closeLeapConnection();
  LeapDestroyConnection(_connectionHandle);
}

void LeapController::openLeapConnection() {
  if (_running) {
    return;
  }

  spdlog::debug("Creating LeapConnection...");
  if (_connectionHandle || LeapCreateConnection(NULL, &_connectionHandle) == eLeapRS_Success) {
    eLeapRS result = LeapOpenConnection(_connectionHandle);
    if (result == eLeapRS_Success) {
      _running       = true;
      _pollingThread = std::thread(&LeapController::messageLoop, this);
    }
  }
}

void LeapController::closeLeapConnection() {
  if (!_running) {
    return;
  }
  _running = false;
  LeapCloseConnection(_connectionHandle);
  _pollingThread.join();
}

void LeapController::setFrame(const LEAP_TRACKING_EVENT* frame) {
  _dataLock.lock();
  if (!_lastFrame) {
    _lastFrame = (LEAP_TRACKING_EVENT*)malloc(sizeof(*frame));
  }
  *_lastFrame = *frame;
  _dataLock.unlock();
}

LEAP_TRACKING_EVENT* LeapController::getFrame() {
  LEAP_TRACKING_EVENT* currentFrame;
  _dataLock.lock();
  currentFrame = _lastFrame;
  _dataLock.unlock();
  return currentFrame;
}

void LeapController::messageLoop() {
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
  spdlog::debug("LeapController connection event");
  _connected = true;
}

void LeapController::handleConnectionLostEvent(const LEAP_CONNECTION_LOST_EVENT* connection_lost_event) {
  spdlog::debug("LeapController connection lost event");
  _connected = false;
}

void LeapController::handleTrackingEvent(const LEAP_TRACKING_EVENT* tracking_event) {
  // spdlog::debug("LeapController tracking event");
  setFrame(tracking_event);  // support polling tracking data from different thread
}
