import time
from robots import Robot

robot = Robot('{fake1}')

end_time = time.monotonic() + 3
while time.monotonic() < end_time:
    robot.camera.read()
    print(robot.angle)
