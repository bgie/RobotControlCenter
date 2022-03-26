from robots.camera import Camera
from robots.command import Command
from robots.camera_parser import extract_robot_info, extract_markers_info


class Robot(object):
    def __init__(self, robot_name: str):
        self._camera = Camera(robot_name)
        self._camera.on_line_read = lambda line: self._on_camera_line_read(line)
        self._command = Command(robot_name)
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

    def turn(self, percentage: float):
        motor_power = int(round(1024 * (min(max(percentage, -100), 100)) / 100))
        self._command.send(f'MOTOR {motor_power} {-motor_power}')

    def _on_camera_line_read(self, line: str):
        d = extract_robot_info(line)
        if d:
            self.battery = d['bat']
            self.x = d['x']
            self.y = d['y']
            self.angle = d['a']
        else:
            self.battery = None
            self.x = None
            self.y = None
            self.angle = None
        self.markers = extract_markers_info(line)

