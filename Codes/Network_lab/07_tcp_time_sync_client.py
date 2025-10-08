import argparse, socket, time, json, statistics
from utils import SoftClock

def est_delay_offset(T1, T2, T3, T4):
    delay = (T4 - T1) - (T3 - T2)
    offset = ((T2 - T1) + (T3 - T4)) / 2.0
    return delay, offset

def robust_mean(values):
    if len(values) < 3:
        return statistics.mean(values)
    k = max(1, int(0.2 * len(values)))
    vals = sorted(values)[k:-k] if len(values) > 2*k else values
    return statistics.mean(vals)

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--host", default="127.0.0.1")
    ap.add_argument("--port", type=int, default=8003)
    ap.add_argument("--samples", type=int, default=10)
    ap.add_argument("--sleep_ms", type=int, default=300)
    ap.add_argument("--fake_offset_ms", type=float, default=0.0)
    ap.add_argument("--fake_ppm", type=float, default=0.0)
    ap.add_argument("--nodelay", action="store_true")
    args = ap.parse_args()

    clk = SoftClock(offset_ms=args.fake_offset_ms, ppm=args.fake_ppm)
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    if args.nodelay:
        s.setsockopt(socket.IPPROTO_TCP, socket.TCP_NODELAY, 1)
    s.connect((args.host, args.port))
    f = s.makefile("rwb")
    print(f"[TCP-SYNC-CLI] {args.host}:{args.port} samples={args.samples} nodelay={args.nodelay} fake_offset={args.fake_offset_ms}ms fake_ppm={args.fake_ppm}")

    delays, offsets = [], []
    for i in range(args.samples):
        T1 = clk.now_soft()
        f.write(json.dumps({"T1": T1}).encode() + b"\n"); f.flush()
        line = f.readline()
        if not line:
            print("conexão encerrada"); break
        T4 = clk.now_soft()
        resp = json.loads(line.decode())
        T2, T3 = resp["T2"], resp["T3"]
        delay, offset = est_delay_offset(T1, T2, T3, T4)
        delays.append(delay); offsets.append(offset)
        print(f"[{i+1:02d}] delay={delay*1000:7.3f}ms offset={offset*1000:7.3f}ms")
        time.sleep(args.sleep_ms/1000.0)

    if delays and offsets:
        print("\n--- Resumo ---")
        print(f"delay médio ≈ {statistics.mean(delays)*1000:.3f} ms | mediana ≈ {statistics.median(delays)*1000:.3f} ms")
        print(f"offset médio ≈ {statistics.mean(offsets)*1000:.3f} ms | mediana ≈ {statistics.median(offsets)*1000:.3f} ms | média robusta ≈ {robust_mean(offsets)*1000:.3f} ms")
    s.close()

if __name__ == "__main__":
    main()
