import argparse, socket, threading, json, time

def handle(conn, addr):
    with conn, conn.makefile("rwb") as f:
        for line in f:
            try:
                req = json.loads(line.decode())
            except Exception:
                continue
            T2 = time.time()
            time.sleep(0.002)
            resp = json.dumps({"T2": T2, "T3": time.time()}).encode() + b"\n"
            f.write(resp); f.flush()

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--bind", default="0.0.0.0")
    ap.add_argument("--port", type=int, default=8003)
    args = ap.parse_args()

    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    s.bind((args.bind, args.port))
    s.listen(32)
    print(f"[TCP-SYNC-SRV] {args.bind}:{args.port}")
    try:
        while True:
            conn, addr = s.accept()
            threading.Thread(target=handle, args=(conn, addr), daemon=True).start()
    finally:
        s.close()

if __name__ == "__main__":
    main()
