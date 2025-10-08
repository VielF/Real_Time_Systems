import time
import socket
from dataclasses import dataclass

def now_monotonic_s() -> float:
    return time.monotonic()

def busy_work_ms(ms: int):
    end = time.perf_counter() + (ms / 1000.0)
    while time.perf_counter() < end:
        pass

@dataclass
class SoftClock:
    offset_ms: float = 0.0
    ppm: float = 0.0
    _t0: float = None
    _mono0: float = None

    def __post_init__(self):
        self._mono0 = time.monotonic()
        self._t0 = time.time()

    def now_soft(self) -> float:
        mono = time.monotonic()
        dt = mono - self._mono0
        scale = 1.0 + (self.ppm * 1e-6)
        return self._t0 + dt * scale + (self.offset_ms / 1000.0)

def make_udp_socket(bind: str = None, port: int = None, timeout: float = None) -> socket.socket:
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    if timeout is not None:
        s.settimeout(timeout)
    if bind is not None and port is not None:
        s.bind((bind, port))
    return s
