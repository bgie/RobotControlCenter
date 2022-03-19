from robots import Robot
import time

robot = Robot('{Fake1}')

end_time = time.monotonic() + 3

while time.monotonic() < end_time:
    robot.camera.read()
    print(robot.angle)
