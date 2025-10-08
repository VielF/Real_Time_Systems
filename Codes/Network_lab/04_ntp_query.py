import argparse, time, statistics
import ntplib

def query_once(host: str):
    c = ntplib.NTPClient()
    resp = c.request(host, version=4, timeout=2.0)
    return {
        "host": host,
        "offset": resp.offset,
        "delay": resp.delay,
        "root_dispersion": getattr(resp, "root_dispersion", float("nan")),
        "stratum": resp.stratum,
        "ref_id": resp.ref_id,
    }

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--hosts", nargs="+", default=["pool.ntp.org"])
    ap.add_argument("--repeat", type=int, default=5)
    ap.add_argument("--sleep_s", type=float, default=1.0)
    args = ap.parse_args()

    results = {h: [] for h in args.hosts}
    for i in range(args.repeat):
        for h in args.hosts:
            try:
                r = query_once(h)
                results[h].append(r)
                print(f"[{h}] offset={r['offset']*1000:7.3f} ms  delay={r['delay']*1000:7.3f} ms  stratum={r['stratum']} ref={r['ref_id']}")
            except Exception as e:
                print(f"[{h}] erro: {e}")
        time.sleep(args.sleep_s)

    print("\n--- Sumário ---")
    for h, arr in results.items():
        if not arr:
            print(f"{h}: sem dados")
            continue
        offs = [x["offset"] for x in arr]
        dels = [x["delay"] for x in arr]
        p95 = statistics.quantiles(offs, n=20)[-1] if len(offs) >= 20 else statistics.mean(offs)
        print(f"{h}: offset médio {statistics.mean(offs)*1000:7.3f} ms | jitter≈p95 {p95*1000:7.3f} ms | delay médio {statistics.mean(dels)*1000:7.3f} ms")

if __name__ == "__main__":
    main()
