from robots.camera import Camera
from robots.command import Command
from robots.camera_parser import extract_robot_info

class Robot(object):
    def __init__(self, robot_name: str):
        self._camera = Camera(robot_name)
        self._camera.on_line_read = lambda line: self._on_camera_line_read(line)
        self._command = Command(robot_name)

    def __enter__(self):
        return self

    def __exit__(self, t, value, traceback):
        self._camera.close()
        self._command.close()

    def get_camera(self):
        return self._camera

    def get_command(self):
        return self._command

    command = property(get_command)
    camera = property(get_camera)

    def _on_camera_line_read(self, line: str):
        d = extract_robot_info(line)
        if d:
            self.battery = d['bat']
            self.x = d['x']
            self.y = d['y']
            self.angle = d['a']
