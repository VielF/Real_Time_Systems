import argparse, time, json
from utils import SoftClock, make_udp_socket

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--name", default="NODE")
    ap.add_argument("--bind", default="0.0.0.0")
    ap.add_argument("--port", type=int, required=True)
    ap.add_argument("--peer", default=None)
    ap.add_argument("--offset_ms", type=float, default=0.0)
    ap.add_argument("--ppm", type=float, default=0.0)
    ap.add_argument("--period_ms", type=int, default=500)
    args = ap.parse_args()

    clk = SoftClock(offset_ms=args.offset_ms, ppm=args.ppm)
    s = make_udp_socket(bind=args.bind, port=args.port, timeout=0.3)
    peer = None
    if args.peer:
        host, p = args.peer.split(":")
        peer = (host, int(p))
    seq = 0
    print(f"[{args.name}] {args.bind}:{args.port} offset_ms={args.offset_ms} ppm={args.ppm}")

    while True:
        if peer:
            msg = {"name": args.name, "seq": seq, "t_sys": time.time(), "t_soft": clk.now_soft()}
            s.sendto(json.dumps(msg).encode(), peer)
            seq += 1

        try:
            data, addr = s.recvfrom(4096)
            pkt = json.loads(data.decode())
            print(f"[{args.name}] recv {addr} | seq={pkt['seq']} t_sys={pkt['t_sys']:.6f} t_soft={pkt['t_soft']:.6f}")
        except Exception:
            pass

        print(f"[{args.name}] local t_sys={time.time():.6f} t_soft={clk.now_soft():.6f}")
        time.sleep(args.period_ms / 1000.0)

if __name__ == "__main__":
    main()
