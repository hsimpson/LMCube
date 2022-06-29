#pragma once

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "LeapController.h"

class LMCubeApp : public cinder::app::App {
 public:
  LMCubeApp();
  virtual ~LMCubeApp();
  virtual void setup();
  virtual void resize();
  virtual void update();
  virtual void draw();
  virtual void mouseDown(cinder::app::MouseEvent event);
  virtual void keyDown(cinder::app::KeyEvent event);  

 private:
  cinder::quat        _rotation;
  cinder::vec3        _translation;
  cinder::CameraPersp _camera;
  LeapController      _leapCtrl;
  int64_t             _lastFrameID = 0;
};
