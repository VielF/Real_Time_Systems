import argparse, socket, time

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--bind", default="0.0.0.0")
    ap.add_argument("--port", type=int, default=8002)
    ap.add_argument("--echo", action="store_true", help="ecoar de volta (loopback de throughput)")
    args = ap.parse_args()

    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    s.bind((args.bind, args.port))
    s.listen(16)
    print(f"[TCP-THR-SRV] {args.bind}:{args.port} echo={args.echo}")
    try:
        while True:
            conn, addr = s.accept()
            print(f"[TCP-THR-SRV] conexão {addr}")
            total = 0
            t0 = time.perf_counter()
            with conn:
                while True:
                    data = conn.recv(64*1024)
                    if not data:
                        break
                    total += len(data)
                    if args.echo:
                        conn.sendall(data)
            t1 = time.perf_counter()
            mb = total/1e6
            print(f"[TCP-THR-SRV] {addr} recebeu {mb:.2f} MB em {(t1-t0):.3f}s => {(mb/(t1-t0)):.2f} MB/s")
    finally:
        s.close()

if __name__ == "__main__":
    main()
