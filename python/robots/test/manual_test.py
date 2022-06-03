import time
from robots import Robot

robotname = "1"

with Robot(robotname) as my_robot:
    while my_robot.camera.read():
        if 4 in my_robot.markers:
            m = my_robot.markers[4]
            print("Marker direction angle " + str(m.angle))
            my_robot.turn_by_angle(m.angle)

    my_robot.stop()
