#include "LeapController.h"

LeapController::LeapController() {}

LeapController::~LeapController() {
  closeLeapConnection();
  LeapDestroyConnection(_connectionHandle);
}

void LeapController::openLeapConnection() {
  if (_running) {
    return;
  }

  if (!_connectionHandle) {
    spdlog::debug("Creating LeapConnection...");
    LEAP_CONNECTION_CONFIG config = {
        sizeof(LEAP_CONNECTION_CONFIG),
        eLeapConnectionConfig::eLeapConnectionConfig_MultiDeviceAware,
        NULL,
        eLeapTrackingOrigin::eLeapTrackingOrigin_DeviceCenter,
    };
    // when using the above config, then polling times out
    // LEAPC_CHECK(LeapCreateConnection(&config, &_connectionHandle));
    LEAPC_CHECK(LeapCreateConnection(NULL, &_connectionHandle));
  }

  LEAPC_CHECK(LeapOpenConnection(_connectionHandle));
  _running       = true;
  _pollingThread = std::thread(&LeapController::messageLoop, this);
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
    unsigned int timeout = 10000;
    spdlog::trace("LeapController polling connection...");
    LEAPC_CHECK(LeapPollConnection(_connectionHandle, timeout, &msg));

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
      case eLeapEventType_Device:
        handleDeviceEvent(msg.device_event);
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
  spdlog::trace("LeapController tracking event");
  setFrame(tracking_event);  // support polling tracking data from different thread
}

void LeapController::handleDeviceEvent(const LEAP_DEVICE_EVENT* device_event) {
  spdlog::debug("LeapController device event");
  LEAP_DEVICE leap_device;
  LEAPC_CHECK(LeapOpenDevice(device_event->device, &leap_device));

  LEAP_DEVICE_INFO device_info;
  memset(&device_info, 0, sizeof(device_info));

  char serial[1000];
  memset(serial, 0, sizeof(serial));
  device_info.serial_length = sizeof(serial) - 1;
  device_info.serial        = serial;
  device_info.size          = sizeof(device_info);

  LEAPC_CHECK(LeapGetDeviceInfo(leap_device, &device_info));

  spdlog::debug("LeapController device info serial: {}, h_fov: {}, v_fov {}", device_info.serial, device_info.h_fov, device_info.v_fov);
}
