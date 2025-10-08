import argparse, socket, threading, time
from utils import busy_work_ms

def handle(conn, addr, work_ms, add_delay_ms):
    with conn:
        while True:
            data = conn.recv(4096)
            if not data:
                break
            if work_ms > 0:
                busy_work_ms(work_ms)
            if add_delay_ms > 0:
                time.sleep(add_delay_ms/1000.0)
            conn.sendall(data)

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--bind", default="0.0.0.0")
    ap.add_argument("--port", type=int, default=8001)
    ap.add_argument("--work_ms", type=int, default=0, help="carga CPU por pacote")
    ap.add_argument("--delay_ms", type=int, default=0, help="atraso artificial por pacote")
    args = ap.parse_args()

    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    s.bind((args.bind, args.port))
    s.listen(100)
    print(f"[TCP-ECHO] escutando em {args.bind}:{args.port} work={args.work_ms}ms delay={args.delay_ms}ms")

    try:
        while True:
            conn, addr = s.accept()
            print(f"[TCP-ECHO] conexão de {addr}")
            th = threading.Thread(target=handle, args=(conn, addr, args.work_ms, args.delay_ms), daemon=True)
            th.start()
    finally:
        s.close()

if __name__ == "__main__":
    main()
