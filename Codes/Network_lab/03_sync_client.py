import argparse, json, time, statistics
from utils import make_udp_socket, SoftClock

def est_delay_offset(T1, T2, T3, T4):
    delay = (T4 - T1) - (T3 - T2)
    offset = ((T2 - T1) + (T3 - T4) ) / 2.0
    return delay, offset

def robust_mean(values):
    if len(values) < 3:
        return statistics.mean(values)
    k = max(1, int(0.2 * len(values)))
    vals = sorted(values)[k:-k] if len(values) > 2*k else values
    return statistics.mean(vals)

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--host", required=True)
    ap.add_argument("--port", type=int, required=True)
    ap.add_argument("--samples", type=int, default=10)
    ap.add_argument("--sleep_ms", type=int, default=500)
    ap.add_argument("--fake_offset_ms", type=float, default=0.0)
    ap.add_argument("--fake_ppm", type=float, default=0.0)
    args = ap.parse_args()

    clk = SoftClock(offset_ms=args.fake_offset_ms, ppm=args.fake_ppm)
    s = make_udp_socket(timeout=1.0)
    delays, offsets = [], []

    print(f"[SYNC-CLIENT] {args.host}:{args.port} samples={args.samples} fake_offset={args.fake_offset_ms}ms fake_ppm={args.fake_ppm}")
    for i in range(args.samples):
        T1 = clk.now_soft()
        req = {"T1": T1}
        s.sendto(json.dumps(req).encode(), (args.host, args.port))

        try:
            data, addr = s.recvfrom(4096)
        except Exception:
            print("timeout")
            time.sleep(args.sleep_ms/1000.0)
            continue
        T4 = clk.now_soft()
        resp = json.loads(data.decode())
        T2 = resp["T2"]
        T3 = resp["T3"]

        delay, offset = est_delay_offset(T1, T2, T3, T4)
        delays.append(delay); offsets.append(offset)
        print(f"[{i+1:02d}] delay={delay*1000:7.3f}ms  offset={offset*1000:7.3f}ms")

        time.sleep(args.sleep_ms/1000.0)

    if delays and offsets:
        print("\n--- Resumo ---")
        print(f"delay médio ≈ {statistics.mean(delays)*1000:.3f} ms | mediana ≈ {statistics.median(delays)*1000:.3f} ms")
        print(f"offset médio ≈ {statistics.mean(offsets)*1000:.3f} ms | mediana ≈ {statistics.median(offsets)*1000:.3f} ms | média robusta ≈ {robust_mean(offsets)*1000:.3f} ms")

if __name__ == "__main__":
    main()
