add_executable(LMCube
    LMCubeApp.cpp
    LeapController.cpp
)

CPMAddPackage(
    NAME CINDER
    GITHUB_REPOSITORY cinder/Cinder
    GIT_TAG master
)

CPMAddPackage(
    NAME spdlog
    GITHUB_REPOSITORY gabime/spdlog
    GIT_TAG v1.12.0
)

if(APPLE)
    include("/Library/Application Support/Ultraleap/LeapSDK/lib/cmake/LeapCTargets.cmake")
endif()

target_link_libraries(LMCube
    spdlog::spdlog_header_only
    LeapSDK::LeapC
    cinder
)