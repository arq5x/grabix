import sys
import subprocess
import gzip

f = sys.argv[1]
lines = ["EMPTY"] + [x.strip() for x in gzip.open(f).readlines()]

print "checking indexing at bounds:"

def check(gzname, start, end=None):
    run = subprocess.check_output
    exp = lines[start:start+1] if end is None else lines[start:end + 1]
    obs = run("./grabix grab %s %d" % (gzname, start), shell=True) if end is None \
            else run("./grabix grab %s %d %d" % (gzname, start, end), shell=True)
    obs = [x.strip() for x in obs.strip().split("\n")]
    sys.stdout.write(".")
    sys.stdout.flush()
    assert exp == obs, (exp, obs)
    if start % 4 == 1:
        assert obs[0][0] == "@"
    else:
        assert obs[0][0] != "@"

for i in range(9990, 10010):
    check(f, i)
    check(f, i, i + 11)
    check(f, i, i + 101)

for i in range(19990, 20010):
    check(f, i)
    check(f, i, i + 1)
    check(f, i, i + 4)

for i in range(1, 200):
    check(f, i)
    check(f, i, i + 1)
    check(f, i, i + 4)

n = len(lines)

check(f, n - 1)

print("\nPASS")

