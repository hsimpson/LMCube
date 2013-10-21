#LMCube: [Leap Motion](https://www.leapmotion.com) controlled 3D#
This APP is a proof of concept for controlling a 3D cube with the [Leap Motion Controller](https://www.leapmotion.com)


##Windows:##
###Dependencies###
1. Visual Studio 2012
2. [Leap Motion SDK](https://developer.leapmotion.com/downloads) (a developer account is needed)
3. [Cinder C++ Library](http://libcinder.org/download/)

###Build###
1. Create source directory e.g.: `C:\LM_Apps`
2. Download the [Leap Motion SDK](https://developer.leapmotion.com/downloads)
3. Extract the `LeapDeveloperKit_release_win_<VERSION>.zip` into the source directory
4. Download the [Cinder C++ Library](http://libcinder.org/download/)
5. Extract the `cinder_<VERSION>_vc2012.zip` into the source directory and rename the `cinder_<VERSION>_vc2012` directory to `Cinder`
6. Open the solution `./vc11/LMCube.sln` in Visual Studio 2012
7. Build and run
8. Output can be found in `./vc11/dist`


##OSX:##
###Dependencies###
1. Xcode 5
2. [Leap Motion SDK](https://developer.leapmotion.com/downloads) (a developer account is needed)
3. [Cinder C++ Library](http://libcinder.org/download/)

###Build###
1. Create source directory e.g.: `~/LM_Apps`
2. Download the [Leap Motion SDK](https://developer.leapmotion.com/downloads)
3. Copy the contents of the `LeapDeveloperKit_release_mac_<VERSION>.dmg` into the source directory
4. Download the [Cinder C++ Library](http://libcinder.org/download/)
5. Extract the `cinder_<VERSION>_mac.zip` into the source directory and rename the `cinder_<VERSION>_mac` directory to `Cinder`
6. Open the solution `./xcode/LMCube.xcodeproj` in Xcode
7. Build and run


##Help: ##
* Hold out one palm and move/rotate the palm.
* Press 'f' key to toggle fullscreen
* Press 'r' to reset the cube position and rotation
* Press 'ESC' to quit the APP