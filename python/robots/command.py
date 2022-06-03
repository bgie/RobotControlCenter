class Command(object):
    def __init__(self, robot_name: str):
        self._pipe = open(f'/robots/robot-{robot_name}-cmd', "w")

    def close(self):
        self._pipe.close()

    def send(self, command: str):
        self._pipe.write(command + '\n')
        self._pipe.flush()
