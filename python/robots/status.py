class Status(object):
    def __init__(self, robot_name: str):
        self._pipe = open(f'/robots/robot-{robot_name}-status', "r")

    def close(self):
        self._pipe.close()

    def read(self) -> str:
        return self._pipe.readline()
