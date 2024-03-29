#    RobotControlCenter
#    Copyright (C) 2021-2022 Kuppens Brecht
#
#    This program is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program.  If not, see <https://www.gnu.org/licenses/>.
#
cmake_minimum_required(VERSION 3.14)

add_library(lib
  aruco/CalibrationController.cpp
  aruco/Aruco.cpp
  aruco/MarkerTracker.cpp
  aruco/SceneTracker.cpp
  aruco/Plane3d.cpp
  aruco/MarkerList.cpp
  aruco/Marker.cpp
  ai/IAgent.cpp
  ai/PythonAgent.cpp
  ai/AdvancedProcessor.cpp
  kalman/RotationCounter.cpp
  kalman/KalmanTracker1D.cpp
  kalman/KalmanTracker3D.cpp
  joystick/SDL2EventLoop.cpp
  joystick/JoystickManager.cpp
  joystick/IJoystick.cpp
  joystick/Joystick.cpp
  joystick/GamePad.cpp
  joystick/GamePadManager.cpp
  camera/ICamera.cpp
  camera/Camera.cpp
  camera/CameraReader.cpp
  camera/ICameraManager.cpp
  camera/MultiCameraManager.cpp
  camera/CameraManager.cpp
  camera/ReplayCamManager.cpp
  camera/ReplayCam.cpp
  camera/ReplayThread.cpp
  camera/CameraController.cpp
  settings/SettingsController.cpp
  settings/AppSettings.cpp
  robot/IRobotManager.cpp
  robot/RobotNetwork.cpp
  robot/MultiRobotManager.cpp
  robot/FakeRobotManager.cpp
  robot/RobotSettings.cpp
  robot/Robot.cpp
  socket/RobotSocketManager.cpp
  socket/RobotSocket.cpp
  util/FactoryMethod.cpp
  game/WorldEdge.cpp
  game/PythonGameMode.cpp
  game/GameScene.cpp
  game/MarkerSceneItem.cpp
  performance/PerformanceMonitor.cpp
  performance/LogPerformance.cpp
)

target_include_directories(lib PUBLIC .)
target_include_directories(lib PUBLIC ${OpenCV_INCLUDE_DIRS})

target_link_libraries(lib Qt5::Core Qt5::Quick Qt5::Network)
target_link_libraries(lib SDL2)
target_link_libraries(lib ${OpenCV_LIBS})
