from robots.status import Status
from robots.command import Command


class Robot(object):
    def __init__(self, robot_name):
        self._name = str(robot_name)
        try:
            #self._status = Status(self._name)
            self._command = Command(self._name)
        except FileNotFoundError:
            raise RuntimeError(f"Could not connect to robot {self._name}") from None

    def __enter__(self):
        return self

    def __exit__(self, t, value, traceback):
        #self._status.close()
        self._command.close()

    def drive(self, percentage: float):
        motor_power = int(round(1024 * (min(max(percentage, -100), 100)) / 100))
        self._send_command(f'MOTOR {motor_power} {motor_power}')

    def stop(self):
        self.drive(0)

    def turn(self, percentage: float):
        motor_power = int(round(1024 * (min(max(percentage, -100), 100)) / 100))
        self._send_command(f'MOTOR {motor_power} {-motor_power}')

    def motors(self, percentage_left: float, percentage_right: float):
        motor_left = int(round(1024 * (min(max(percentage_left, -100), 100)) / 100))
        motor_right = int(round(1024 * (min(max(percentage_right, -100), 100)) / 100))
        self._send_command(f'MOTOR {motor_left} {motor_right}')

    # private:
    def _send_command(self, command_text: str):
        print(command_text, end='')
        self._command.send(command_text)
        # reply = self._status.read()
        # print(' ' + reply)
