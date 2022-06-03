from robots.camera import Camera
from robots.command import Command
from robots.camera_parser import extract_robot_info, extract_markers_info


class Robot(object):
    def __init__(self, robot_name):
        self._name = str(robot_name)
        try:
            self._camera = Camera(self._name)
            self._camera.on_line_read = lambda line: self._on_camera_line_read(line)
            self._command = Command(self._name)
        except FileNotFoundError:
            raise RuntimeError(f"Could not connect to robot {self._name}") from None
        self.x = None
        self.y = None
        self.angle = None
        self.markers = {}

    def __enter__(self):
        return self

    def __exit__(self, t, value, traceback):
        self._camera.close()
        self._command.close()

    def get_camera(self):
        return self._camera
    camera = property(get_camera)

    def get_command(self):
        return self._command
    command = property(get_command)

    def drive(self, percentage: float):
        motor_power = int(round(1024 * (min(max(percentage, -100), 100)) / 100))
        self._command.send(f'MOTOR {motor_power} {motor_power}')

    def stop(self):
        self.drive(0)

    def turn(self, percentage: float):
        motor_power = int(round(1024 * (min(max(percentage, -100), 100)) / 100))
        self._command.send(f'MOTOR {motor_power} {-motor_power}')

    def turn_by_angle(self, delta_angle: int):
        target_angle = None
        while self.camera.read():
            if target_angle is None:
                target_angle = self.angle + delta_angle
                print("Target angle " + str(target_angle))
            print("Current angle " + str(self.angle))
            if self.angle < target_angle - 30:
                self.turn(100)
            elif self.angle < target_angle - 15:
                self.turn(70)
            elif self.angle > target_angle + 30:
                self.turn(-100)
            elif self.angle > target_angle + 15:
                self.turn(-70)
            else:
                self.stop()
                break

    def servo(self, angle1: int, angle2: int = 90):
        a1 = max(0, min(180, int(angle1)))
        a2 = max(0, min(180, int(angle2)))
        self._command.send(f'SERVO {a1} {a2}')

    def _on_camera_line_read(self, line: str) -> bool:
        d = extract_robot_info(line)
        position_found = False
        if d:
            self.battery = d['bat']
            self.x = d['x']
            self.y = d['y']
            self.angle = d['a']
            position_found = True
        else:
            self.battery = None
            self.x = None
            self.y = None
            self.angle = None
        self.markers = extract_markers_info(line)
        return position_found

