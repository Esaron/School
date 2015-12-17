#!/usr/bin/python

# distMeasurement.py
# Usage:
#   sudo python distMeasurement.py

import sys, matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plot
from scipy.stats.stats import pearsonr
from rttHopsDist import get_rtt_hops_distance

def main():
    # Init
    scatter = plot.scatter
    exitCode = 1
    # Read in targets
    print 'Reading host file...'
    f = open('targets.txt', 'r')
    targets = f.read().splitlines()
    f.close()
    rttData = []
    hopsData = []
    distData = []
    # Get RTT, hops, and distance to each target host
    for target in targets:
        code = 1
        print '========================='
        print '       %s' % target
        print '========================='
        try:
            code,rtt,hops,dist = get_rtt_hops_distance(target, 33434, 32, False)
            rttData.append(rtt.microseconds)
            hopsData.append(hops)
            distData.append(dist)
        except:
            exitCode = 1
        exitCode = exitCode or code

    assert len(targets) == len(rttData) == len(hopsData) == len(distData)

    # Save results
    print 'Saving results'
    output = open('results.txt', 'w')
    for i in range(0, len(targets)):
        output.write('Target: \t\t%s\nRTT (microseconds): \t%d\nHops: \t\t\t%d\nDistance (miles): \t%d\n\n' % (targets[i],rttData[i],hopsData[i],distData[i]))
    # Correlation coefficient
    output.write('\nPearson Correlation Coefficients:\n\n')
    output.write('Hops, RTT: \t\t%f\n' % pearsonr(hopsData, rttData)[0])
    output.write('Hops, Distance: \t%f\n' % pearsonr(hopsData, distData)[0])
    output.write('RTT, Distance: \t\t%f\n' % pearsonr(rttData, distData)[0])
    output.close()

    # Plot data
    print 'Creating plots...'
    f1 = plot.figure()
    p1 = scatter(hopsData, rttData)
    plot.xlabel('Number of Hops')
    plot.ylabel('Round Trip Time (microseconds)')
    f1.savefig('hopsRtt.jpg')
    plot.close()
    f2 = plot.figure()
    p2 = scatter(rttData, distData)
    plot.xlabel('Round Trip Time (microseconds)')
    plot.ylabel('Distance (miles)')
    f2.savefig('rttDist.jpg')
    plot.close()
    f3 = plot.figure()
    p3 = scatter(hopsData, distData)
    plot.xlabel('Number of Hops')
    plot.ylabel('Distance (miles)')
    f3.savefig('hopsDist.jpg')
    plot.close()

    return exitCode

if __name__ == "__main__":
    sys.exit(main())
