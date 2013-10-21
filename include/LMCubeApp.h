#ifndef LMCubeApp_h__
#define LMCubeApp_h__

#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "Leap.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class LMCubeApp : public AppNative {
public:
  LMCubeApp();
  virtual ~LMCubeApp();
  virtual void setup();
  virtual void mouseDown( MouseEvent event );	
  virtual void update();
  virtual void draw();
  virtual void keyDown( KeyEvent event );

private:
  int _windowWidth;
  int _windowHeight;
  Quatf _rotation;
  Vec3f _translation;
  Leap::Controller _leapCtrl;
};

#endif // LMCubeApp_h__
