import re

# EXAMPLE camera output line
# bat:100 x:-300 y:-125 a:181;id:0 d:456 a:-71;id:1 d:202 a:53;id:2 d:563 a:-128;id:3 d:487 a:168;id:45 d:388 a:-148;id:47 d:222 a:-126;id:48 d:338 a:-124


def extract_robot_info(line: str):
    m = re.match(r"^bat:(?P<bat>\d+) x:(?P<x>-?\d+) y:(?P<y>-?\d+) a:(?P<a>\d+);", line)
    if m:
        return {key: int(value) for (key, value) in m.groupdict().items()}
    return None
