#include "LMCubeApp.h"
#include "cinder/Log.h"
#include <cmath>
#include <thread>
#include "spdlog/spdlog.h"

LMCubeApp::LMCubeApp() {
}

LMCubeApp::~LMCubeApp() {
  _leapCtrl.closeLeapConnection();
}

void LMCubeApp::setup() {
  spdlog::set_level(spdlog::level::debug);  // Set global log level to debug
  _camera.lookAt(cinder::vec3(0, 0, 5), cinder::vec3(0));
  _camera.setFov(60);
  _camera.setNearClip(0.1f);
  _camera.setFarClip(1000.0f);

  // open the Leap connection
  _leapCtrl.openLeapConnection();
  while (!_leapCtrl.isConnected()) {
    spdlog::debug("Wait for LeapController isConnected...");
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
  spdlog::debug("LeapController isConnected");
}

void LMCubeApp::resize() {
  float windowWidth  = (float)getWindowWidth();
  float windowHeight = (float)getWindowHeight();
  _camera.setAspectRatio(windowWidth / windowHeight);
}

void LMCubeApp::mouseDown(cinder::app::MouseEvent event) {
}

void LMCubeApp::update() {
  LEAP_TRACKING_EVENT* frame = _leapCtrl.getFrame();

  if (frame && (frame->tracking_frame_id > _lastFrameID)) {
    // spdlog::debug("Number of hands: " << frame->nHands);
    if (frame->nHands == 1) {  // one hand active
      LEAP_HAND* hand = &frame->pHands[0];
      LEAP_PALM  palm = hand->palm;

      float x = palm.position.x;
      float y = palm.position.y;
      float z = palm.position.z;

      const float rotation_factor            = 0.025f;
      const float translation_factor         = 0.025f;
      const float y_offset                   = 150.0f;
      const float rotation_threshold_radians = 10.0f * (float)(M_PI / 180.0f);
      const float translation_threshold_mm   = 20.0f;

      // reduce y by fixed offset because we want the y origin be above the controller :-)
      y -= y_offset;

      x *= translation_factor;
      y *= translation_factor;
      z *= translation_factor;

      spdlog::debug("Translate: ({}, {}, {})", x, y, z);

      _translation = cinder::vec3(x, y, z);

      // Rotation
      LEAP_QUATERNION orientation = palm.orientation;

      _rotation = cinder::quat(orientation.w, orientation.x, orientation.y, orientation.z);
    }
  }
}

void LMCubeApp::draw() {
  // clear out the window with black
  cinder::gl::clear(cinder::Color(0, 0, 0));
  cinder::gl::enableDepthRead();
  cinder::gl::enableDepthWrite();

  cinder::gl::setMatrices(_camera);

  // auto lambert = cinder::gl::ShaderDef().lambert();
  // auto shader  = cinder::gl::getStockShader(lambert);
  // shader->bind();

  cinder::gl::pushModelView();
  cinder::gl::translate(_translation);
  // cinder::gl::scale(cinder::vec3(1, 1, 1));
  cinder::gl::rotate(_rotation);
  cinder::gl::drawColorCube(cinder::vec3(), cinder::vec3(1, 1, 1));
  cinder::gl::popModelView();
}

void LMCubeApp::keyDown(cinder::app::KeyEvent event) {
  if (event.getCode() == cinder::app::KeyEvent::KEY_ESCAPE) {
    quit();
  }
  if (event.getChar() == 'f') {  // toggle fullscreen
    setFullScreen(!isFullScreen());
  }
  if (event.getChar() == 'r') {  // reset translation and rotation
    _translation = cinder::vec3();
    _rotation    = cinder::quat();
  }
  const float  deg = 5.0f;
  float        rad = deg * (float)(M_PI / 180);
  cinder::quat rotationOffset;

  if (event.getCode() == cinder::app::KeyEvent::KEY_LEFT)
    _rotation = cinder::angleAxis(-rad, cinder::vec3(0, 1, 0)) * _rotation;
  if (event.getCode() == cinder::app::KeyEvent::KEY_RIGHT)
    _rotation = cinder::angleAxis(+rad, cinder::vec3(0, 1, 0)) * _rotation;

  if (event.getCode() == cinder::app::KeyEvent::KEY_UP)
    _rotation = cinder::angleAxis(+rad, cinder::vec3(1, 0, 0)) * _rotation;
  if (event.getCode() == cinder::app::KeyEvent::KEY_DOWN)
    _rotation = cinder::angleAxis(-rad, cinder::vec3(1, 0, 0)) * _rotation;

  if (event.getCode() == cinder::app::KeyEvent::KEY_LEFT && event.isShiftDown())
    _rotation = cinder::angleAxis(-rad, cinder::vec3(0, 0, 1)) * _rotation;
  if (event.getCode() == cinder::app::KeyEvent::KEY_RIGHT && event.isShiftDown())
    _rotation = cinder::angleAxis(+rad, cinder::vec3(0, 0, 1)) * _rotation;
}

CINDER_APP(LMCubeApp,
           cinder::app::RendererGl(cinder::app::RendererGl::Options().msaa(2)),
           [&](cinder::app::App::Settings* settings) {
             settings->setWindowSize(1024, 786);
             settings->setFrameRate(60);

  // setFullScreen(true);
  // hideCursor();

#if defined(CINDER_MSW)
             settings->setConsoleWindowEnabled();
#endif
           });
