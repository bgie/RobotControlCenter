import time
from robots import Robot
from robots.camera_parser import *

line = "bat:100 x:-300 y:-125 a:181;id:0 d:456 a:-71;id:1 d:202 a:53;id:2 d:563 a:-128;id:3 d:487 a:168;id:45 d:388 a:-148;id:47 d:222 a:-126;id:48 d:338 a:-124"
print(extract_markers_info(line))

# robot = Robot('7')
#
# end_time = time.monotonic() + 3
# while time.monotonic() < end_time:
#     robot.camera.read()
#     robot.command.send("TEST\n")
#     print(robot.angle)
