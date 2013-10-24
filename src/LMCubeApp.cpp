#include "LMCubeApp.h"
#include <cmath>

LMCubeApp::LMCubeApp()
  : _windowWidth(1024),
    _windowHeight(768)
{
}

LMCubeApp::~LMCubeApp()
{
}



void LMCubeApp::setup()
{
  setWindowSize(_windowWidth, _windowHeight);
  setFrameRate(60);
  setFullScreen(true);
  hideCursor();
}

void LMCubeApp::mouseDown( MouseEvent event )
{
}

void LMCubeApp::update()
{
}

void LMCubeApp::draw()
{
  if(_leapCtrl.isConnected()) //controller is a Controller object
  {
    Leap::Frame frame = _leapCtrl.frame(); //The latest frame
    int number_of_hands = frame.hands().count();
    if(number_of_hands == 1) // on hand active
    {
      Leap::Hand h = frame.hands()[0];
      if(h.fingers().count() > 1) // no fist
      {
        const float rotation_factor = 0.025f;
        const float translation_factor = 0.025f;
        const float y_offset = 150.0f;
        const float rotation_threshold_radians = 10.0f * (float)(M_PI/180.0f);
        const float translation_threshold_mm = 20.0f;

        // Translation
        Leap::Vector palm_position = h.palmPosition();

        float x = palm_position.x;
        float y = palm_position.y;
        float z = palm_position.z;
        /*
        console() << "x=" << x
                  << " y=" << y
                  << " z=" << z
                  << std::endl;
        */
        
        // reduce y by fixed offset because we whant the y origin be above the controller :-)
        y -= y_offset;
        
        x = ((std::abs(x) - translation_threshold_mm) <= 0.0f) ? 0.0f : x;
        y = ((std::abs(y) - translation_threshold_mm) <= 0.0f) ? 0.0f : y;
        z = ((std::abs(z) - translation_threshold_mm) <= 0.0f) ? 0.0f : z;
        
        x *= translation_factor;
        y *= translation_factor;
        z *= translation_factor;

        _translation += Vec3f(x, -y, z);

        // Rotation
        float pitch = h.direction().pitch();
        float yaw   = h.direction().yaw();
        float roll  = h.palmNormal().roll();
        pitch = ((std::abs(pitch) - rotation_threshold_radians) <= 0.0f) ? 0.0f : pitch;
        yaw =   ((std::abs(yaw)   - rotation_threshold_radians) <= 0.0f) ? 0.0f : yaw;
        roll =  ((std::abs(roll)  - rotation_threshold_radians) <= 0.0f) ? 0.0f : roll;
        /*
        console() << "pitch=" << pitch
                  << " yaw=" << yaw
                  << " roll=" << roll
                  << std::endl;
        */
        pitch *= rotation_factor;
        yaw   *= rotation_factor;
        roll  *= rotation_factor;
        
        _rotation *= Quatf(-pitch, -yaw, -roll);
      }
    }
  }



	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 

  gl::enableAlphaBlending();
  gl::enableDepthRead();
  gl::enableDepthWrite();
  glEnable( GL_CULL_FACE );
  glFrontFace( GL_CW ); // the default camera inverts to a clockwise front-facing direction


  gl::pushModelView();
    glCullFace( GL_BACK );    
    gl::translate(_translation + Vec3f(getWindowCenter()));
    gl::scale( Vec3f( 200.0f, 200.0f, 200.0f ) );	
    gl::rotate( _rotation );
    gl::drawColorCube( Vec3f::zero(), Vec3f( 1, 1, 1 ) );
  gl::popModelView();
}

void LMCubeApp::keyDown( KeyEvent event )
{
  if( event.getCode() == KeyEvent::KEY_ESCAPE ) {
    quit();
  }
  if(event.getChar() == 'f') { // toggle fullscreen
    setFullScreen(!isFullScreen());
  }
  if(event.getChar() == 'r') { // reset translation and rotation
    _translation = Vec3f();
    _rotation = Quatf();
  }
  const float deg = 5;
  float rad = deg*(float)(M_PI/180);
  
  if(event.getCode() == KeyEvent::KEY_LEFT )
    _rotation *= Quatf(0, -rad, 0);
  if(event.getCode() == KeyEvent::KEY_RIGHT )
    _rotation *= Quatf(0, +rad, 0);

  if(event.getCode() == KeyEvent::KEY_UP )
    _rotation *= Quatf(+rad, 0, 0);
  if(event.getCode() == KeyEvent::KEY_DOWN )
    _rotation *= Quatf(-rad, 0, 0);


  if(event.getCode() == KeyEvent::KEY_LEFT && event.isShiftDown() )
    _rotation *= Quatf(0, 0, -rad);
  if(event.getCode() == KeyEvent::KEY_RIGHT && event.isShiftDown())
    _rotation *= Quatf(0, 0, +rad);

}



CINDER_APP_NATIVE( LMCubeApp, RendererGl )
