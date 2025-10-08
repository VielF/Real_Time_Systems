import argparse, time, json, statistics
from utils import make_udp_socket, busy_work_ms

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--bind", default="0.0.0.0")
    ap.add_argument("--port", type=int, required=True)
    ap.add_argument("--deadline_ms", type=float, default=30.0)
    ap.add_argument("--work_ms", type=int, default=0)
    args = ap.parse_args()

    s = make_udp_socket(bind=args.bind, port=args.port, timeout=2.0)
    latencies = []
    violations = 0
    total = 0
    print(f"[RECV] {args.bind}:{args.port} deadline={args.deadline_ms}ms work={args.work_ms}ms")

    while True:
        try:
            data, addr = s.recvfrom(4096)
        except Exception:
            continue
        t_arrive = time.time()
        pkt = json.loads(data.decode())
        ts_send = pkt["ts_send"]
        seq = pkt["seq"]

        if args.work_ms > 0:
            busy_work_ms(args.work_ms)

        t_done = time.time()
        latency_ms = (t_done - ts_send) * 1000.0
        latencies.append(latency_ms)
        total += 1
        viol = latency_ms > args.deadline_ms
        if viol:
            violations += 1

        if total % 20 == 0:
            sample = latencies[-100:] if len(latencies) >= 1 else [latency_ms]
            avg = statistics.mean(sample)
            p95 = statistics.quantiles(sample, n=20)[-1] if len(sample) >= 20 else avg
            print(f"[RECV] seq={seq:6d} lat={latency_ms:7.2f}ms | avg100={avg:6.2f}ms p95~{p95:6.2f}ms | viol={violations}/{total}")

if __name__ == "__main__":
    main()
