class Camera(object):
    def __init__(self, robot_name: str):
        self._pipe = open(f'/robots/robot-{robot_name}-cam', "r")
        self._last_line = None

    def close(self):
        self._pipe.close()

    def read(self) -> bool:
        self._last_line = self._pipe.readline()
        return self.on_line_read(self._last_line)

    # callback method intended to be overwritten
    def on_line_read(self, line: str) -> bool:
        return False
