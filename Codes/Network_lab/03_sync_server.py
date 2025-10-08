import argparse, json, time
from utils import make_udp_socket

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--bind", default="0.0.0.0")
    ap.add_argument("--port", type=int, required=True)
    args = ap.parse_args()

    s = make_udp_socket(bind=args.bind, port=args.port, timeout=0.5)
    print(f"[SYNC-SERVER] {args.bind}:{args.port}")
    while True:
        try:
            data, addr = s.recvfrom(4096)
        except Exception:
            continue
        try:
            json.loads(data.decode())
        except Exception:
            continue
        T2 = time.time()
        resp = {"T2": T2}
        time.sleep(0.002)
        resp["T3"] = time.time()
        s.sendto(json.dumps(resp).encode(), addr)

if __name__ == "__main__":
    main()
