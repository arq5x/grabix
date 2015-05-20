import sys
n = int(sys.argv[1]) + 1
for i in range(1, n):
    s = str(i) + "ACTG"
    print "@read_%i" % i
    seq = "".join([s] * 20)[:100]
    print seq
    print "+"
    print seq
