import argparse, time, random, json
from utils import now_monotonic_s, make_udp_socket

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--host", required=True)
    ap.add_argument("--port", type=int, required=True)
    ap.add_argument("--rate_hz", type=float, default=50.0)
    ap.add_argument("--jitter_ms", type=float, default=0.0)
    args = ap.parse_args()

    period = 1.0 / args.rate_hz
    s = make_udp_socket()
    seq = 0
    next_t = now_monotonic_s()

    print(f"[SENDER] {args.host}:{args.port} rate={args.rate_hz:.1f}Hz jitter={args.jitter_ms}ms")
    while True:
        now = now_monotonic_s()
        if now < next_t:
            time.sleep(next_t - now)
        if args.jitter_ms > 0:
            time.sleep(random.uniform(0, args.jitter_ms) / 1000.0)

        ts_send = time.time()
        msg = {"seq": seq, "ts_send": ts_send}
        s.sendto(json.dumps(msg).encode(), (args.host, args.port))
        seq += 1
        next_t += period

if __name__ == "__main__":
    main()
