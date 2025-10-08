import argparse, socket, time, statistics, os

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--host", default="127.0.0.1")
    ap.add_argument("--port", type=int, default=8001)
    ap.add_argument("--count", type=int, default=500)
    ap.add_argument("--payload_bytes", type=int, default=64)
    ap.add_argument("--rate_hz", type=float, default=100.0, help="envios por segundo")
    ap.add_argument("--nodelay", action="store_true", help="habilita TCP_NODELAY (desabilita Nagle)")
    args = ap.parse_args()

    payload = os.urandom(args.payload_bytes)

    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    if args.nodelay:
        s.setsockopt(socket.IPPROTO_TCP, socket.TCP_NODELAY, 1)
    s.connect((args.host, args.port))
    print(f"[TCP-RTT] alvo {args.host}:{args.port} count={args.count} rate={args.rate_hz}Hz nodelay={args.nodelay}")

    rtts = []
    period = 1.0/args.rate_hz if args.rate_hz > 0 else 0.0
    next_t = time.perf_counter()

    for i in range(args.count):
        now = time.perf_counter()
        if period and now < next_t:
            time.sleep(next_t - now)

        t0 = time.perf_counter()
        s.sendall(payload)
        got = 0
        while got < len(payload):
            chunk = s.recv(len(payload)-got)
            if not chunk:
                break
            got += len(chunk)
        t1 = time.perf_counter()
        rtt_ms = (t1 - t0)*1000.0
        rtts.append(rtt_ms)
        if (i+1) % 50 == 0:
            sample = rtts[-200:]
            p50 = statistics.median(sample)
            p95 = statistics.quantiles(sample, n=20)[-1] if len(sample) >= 20 else p50
            print(f"[{i+1:4d}] rtt={rtt_ms:7.3f}ms | p50={p50:6.2f}ms p95≈{p95:6.2f}ms")
        next_t += period if period else 0.0

    if rtts:
        print("\n--- Resumo RTT ---")
        print(f"média={statistics.mean(rtts):.3f} ms | mediana={statistics.median(rtts):.3f} ms")
        if len(rtts) >= 20:
            print(f"p95≈{statistics.quantiles(rtts, n=20)[-1]:.3f} ms  p99≈{statistics.quantiles(rtts, n=100)[-1]:.3f} ms")
    s.close()

if __name__ == "__main__":
    main()
