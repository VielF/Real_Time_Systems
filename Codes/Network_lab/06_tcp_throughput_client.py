import argparse, socket, time, os

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--host", default="127.0.0.1")
    ap.add_argument("--port", type=int, default=8002)
    ap.add_argument("--megs", type=float, default=50.0, help="quantidade a enviar em MB")
    ap.add_argument("--chunk_kb", type=int, default=64, help="tamanho do chunk")
    ap.add_argument("--nodelay", action="store_true")
    args = ap.parse_args()

    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    if args.nodelay:
        s.setsockopt(socket.IPPROTO_TCP, socket.TCP_NODELAY, 1)
    s.connect((args.host, args.port))
    print(f"[TCP-THR-CLI] alvo {args.host}:{args.port} enviar {args.megs} MB chunk={args.chunk_kb} KB nodelay={args.nodelay}")
    chunk = os.urandom(args.chunk_kb*1024)
    total_bytes = int(args.megs*1e6)
    sent = 0
    t0 = time.perf_counter()
    while sent < total_bytes:
        n = min(len(chunk), total_bytes - sent)
        s.sendall(chunk[:n])
        sent += n
    s.shutdown(socket.SHUT_WR)
    try:
        while s.recv(64*1024):
            pass
    except Exception:
        pass
    t1 = time.perf_counter()
    mb = sent/1e6
    print(f"[TCP-THR-CLI] enviado {mb:.2f} MB em {(t1-t0):.3f}s => {(mb/(t1-t0)):.2f} MB/s")
    s.close()

if __name__ == "__main__":
    main()
