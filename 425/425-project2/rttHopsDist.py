#!/usr/bin/python

# rttHopsDist.py
# Usage:
# sudo python rttHopsDist.py
#   [-p|--port port]
#   [-t|--ttl initialTTL]
#   [-v|--verbose]
#   targetHost

import optparse, sys, os, ctypes, socket, select, struct, requests, xml.etree.ElementTree as ET, geoip2.database
from datetime import datetime
from uuid import uuid4
from math import radians, cos, sin, asin, sqrt

# Begin http://bt3gl.github.io/black-hat-python-building-a-udp-scanner.html
# Define ICMP class
class ICMP(ctypes.Structure):
    _fields_ = [
    ('icmp_type',    ctypes.c_ubyte),
    ('code',         ctypes.c_ubyte),
    ('checksum',     ctypes.c_ushort),
    ('unused',       ctypes.c_ushort),
    ('next_hop_mtu', ctypes.c_ushort)]

    def __new__(self, socket_buffer):
        return self.from_buffer_copy(socket_buffer)

    def __init__(self, socket_buffer):
        pass
# End http://bt3gl.github.io/black-hat-python-building-a-udp-scanner.html
    def __str__(self):
        return ('========== ICMP Header ==========\n'
                'Type: %d\n'
                'Code: %d\n'
                'Checksum: %d\n'
                'Next Hop MTU: %d') % (self.icmp_type, self.code, self.checksum,
                                     self.next_hop_mtu)

# Define IP class
class IP(ctypes.Structure):
    _fields_ = [
    ('version_ihl',    ctypes.c_ubyte),
    ('dscp_ecn',       ctypes.c_ubyte),
    ('length',         ctypes.c_ushort),
    ('identification', ctypes.c_ushort),
    ('reserved_frags', ctypes.c_ushort),
    ('ttl',            ctypes.c_ubyte),
    ('protocol',       ctypes.c_ubyte),
    ('checksum',       ctypes.c_ushort),
    ('sa1',            ctypes.c_char),
    ('sa2',            ctypes.c_char),
    ('sa3',            ctypes.c_char),
    ('sa4',            ctypes.c_char),
    ('da1',            ctypes.c_char),
    ('da2',            ctypes.c_char),
    ('da3',            ctypes.c_char),
    ('da4',            ctypes.c_char)]

    version = None
    ihl = None
    source_addr = None
    dest_addr = None

    def __new__(self, socket_buffer):
        return self.from_buffer_copy(socket_buffer)

    def __init__(self, socket_buffer):
        self.version = self.version_ihl >> 4
        self.ihl = (self.version_ihl & 0xF) * 4
        self.source_addr = socket.inet_ntoa(self.sa1 + self.sa2 + self.sa3 + self.sa4)
        self.dest_addr = socket.inet_ntoa(self.da1 + self.da2 + self.da3 + self.da4)

    def __str__(self):
        return ('=========== IP Header ===========\n'
                'Version: %d\n'
                'IHL: %d\n'
                'Total Length: %d\n'
                'TTL: %d\n'
                'Protocol: %d\n'
                'Checksum: %d\n'
                'Source: %s\n'
                'Destination: %s') % (self.version, self.ihl, self.length, self.ttl,
                                       self.protocol, self.checksum, self.source_addr,
                                       self.dest_addr)

# Define UDP class
class UDP(ctypes.Structure):
    _fields_ = [
    ('source',   ctypes.c_ushort),
    ('dest',     ctypes.c_ushort),
    ('length',   ctypes.c_ushort),
    ('checksum', ctypes.c_ushort)]

    def __new__(self, socket_buffer):
        return self.from_buffer_copy(socket_buffer)

    def __init__(self, socket_buffer):
        pass

    def __str__(self):
        return ('=========== UDP Header ==========\n'
                'Source Port: %d\n'
                'Destination Port: %d\n'
                'Length: %d\n'
                'Checksum: %d') % (self.source, self.dest, self.length, self.checksum)

# Define globals
ICMP_TYPE = socket.getprotobyname('icmp')
UDP_TYPE = socket.getprotobyname('udp')
DATA_SIZE = 8
NO_DATA = (None,0,0,False)
BASE_GEOIP_URL = 'http://freegeoip.net/xml/'

def trace(send_socket, recv_socket, dest_addr, dest_port, timeout):
    """
    Send a datagram id (uuid) so we can track whether we received the
    correct response to the destination host and port, await response,
    and parse results to obtain hops (reduction to TTL). Time the
    single send-receive for RTT.
    """
    # Set up a polling object to check for data on the receive socket
    poller = select.poll()
    poller.register(recv_socket.fileno(), select.EPOLLIN)
    # Start the timer to get RTT
    start = datetime.now()
    if DBG:
        print '<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<'
    # Generate the unique id to verify packet identity
    datagram_id = str(uuid4())
    # Send data
    sent = send_socket.sendto(datagram_id, (dest_addr, dest_port))
    # Wait for data for one second (or until received)
    ready = poller.poll(timeout)
    # Record the time data is available for read
    end = datetime.now()
    # Interpret data if received
    if ready:
        # Read the data
        data,curr_addr = recv_socket.recvfrom(65535)
        # curr_addr is represented as a tuple
        curr_addr = curr_addr[0]
        # Calculate RTT
        rtt = end - start
        # Unpack IP header of response into IP object
        ip_header = IP(data[:ctypes.sizeof(IP)])
        if DBG:
            print ip_header
        # Unpack ICMP header
        icmp_offset = ip_header.ihl + ctypes.sizeof(ICMP)
        icmp_header = ICMP(data[ip_header.ihl:icmp_offset])
        # Check for destination unreachable response
        destination_reached = icmp_header.icmp_type == 3
        if DBG:
            print icmp_header
        # Unpack the originally sent IP header
        ip_offset = icmp_offset + ctypes.sizeof(IP)
        orig_ip_header = IP(data[icmp_offset:ip_offset])
        ip_header_ttl = orig_ip_header.ttl
        if DBG:
            print orig_ip_header
        # Unpack the originally sent UDP data
        udp_offset = ip_offset + 8
        orig_datagram = struct.unpack('!4H', data[ip_offset:udp_offset])
        orig_dest_port = orig_datagram[1]
        # Verify packet identity by checking port and id (if id is null, we didn't get the data back
        # It was worth a shot.  TODO use header checksums/ids
        orig_datagram_id = data[udp_offset:]
        if orig_dest_port == dest_port and (not orig_datagram_id or orig_datagram_id == datagram_id):
            if DBG:
                print '>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>'
            return curr_addr,ip_header_ttl,rtt,destination_reached
    if DBG:
        print '>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>'
    return NO_DATA
# End https://gist.github.com/pklaus/856268

# Begin https://github.com/leonidg/Poor-Man-s-traceroute/blob/master/traceroute.py
def create_sockets(ttl):
    """
    Sets up sockets necessary for the traceroute.  We need a receiving
    socket and a sending socket.
    """
    recv_socket = socket.socket(socket.AF_INET, socket.SOCK_RAW, ICMP_TYPE)
    send_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, UDP_TYPE)
    send_socket.setsockopt(socket.SOL_IP, socket.IP_TTL, ttl)
    return recv_socket, send_socket
# End https://github.com/leonidg/Poor-Man-s-traceroute/blob/master/traceroute.py

def get_ip():
    ip = None
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    try:
        s.connect(("8.8.8.8",80))
        ip = s.getsockname()[0]
    finally:
        s.close()
    return ip

def get_lat_lon(addr):
    lat = None
    lon = None
    try:
        # Get the geo ip xml and parse out latitude and longitude
        r = requests.get(BASE_GEOIP_URL + addr)
        tree = ET.fromstring(r.text)
        for el in tree.iter('Latitude'):
            lat = float(el.text)
        for el in tree.iter('Longitude'):
            lon = float(el.text)
    except:
        # Fallback to file-based method
        reader = geoip2.database.Reader(os.path.abspath('GeoLite2-City.mmdb'))
        if not addr:
            addr = get_ip()
        response = reader.city(addr)
        lat = response.location.latitude
        lon = response.location.longitude
    return lat,lon

# Begin http://stackoverflow.com/questions/4913349/haversine-formula-in-python-bearing-and-distance-between-two-gps-points
def haversine(lat1, lon1, lat2, lon2):
    """
    Calculate the great circle distance between two points 
    on the earth (specified in decimal degrees)
    """
    # convert decimal degrees to radians 
    lon1, lat1, lon2, lat2 = map(radians, [lon1, lat1, lon2, lat2])

    # haversine formula 
    dlon = lon2 - lon1 
    dlat = lat2 - lat1 
    a = sin(dlat/2)**2 + cos(lat1) * cos(lat2) * sin(dlon/2)**2
    c = 2 * asin(sqrt(a)) 
    r = 3956 # Radius of earth. 6371 for kilometers. 3956 for miles.
    return c * r
# End http://stackoverflow.com/questions/4913349/haversine-formula-in-python-bearing-and-distance-between-two-gps-points

def get_rtt_hops_distance(dest_name, port, ttl, verbose):
    # Init
    global DBG
    DBG = verbose
    exitCode = 1
    retries = 3
    retry = 1
    timeout = 3000
    dest_addr = socket.gethostbyname(dest_name)
    curr_addr = None
    recv_socket, send_socket = create_sockets(ttl)
    recv_socket.bind(('', port))
    recv_socket.setblocking(0)
    curr_name = None
    data = None
    ip_header_ttl = ttl
    destination_reached = False
    start = 0
    end = 0
    rtt = 0
    out = NO_DATA

    #
    # Perform single-probe RTT and Hops calculation
    #

    print
    print 'Getting RTT and Hops...'
    try:
        while retries > 0 and out == NO_DATA:
            print 'Try %d' % retry
            out = trace(send_socket, recv_socket, dest_addr, port, timeout)
            retries -= 1
            retry += 1
        curr_addr,ip_header_ttl,rtt,destination_reached = out
        try:
            curr_name = socket.gethostbyaddr(curr_addr)[0]
        except:
            print "Couldn't get hostname."
            curr_name = curr_addr
    except socket.error:
        print 'Error sending/receiving data on socket(s)'
    finally:
        send_socket.close()
        recv_socket.close()

    # Perform some formatting
    if curr_addr is not None:
        curr_host = "%s (%s)" % (curr_name, curr_addr)
    else:
        curr_host = "*"
    hops = ttl - ip_header_ttl

    # Output results
    print "RTT: %s\nTotal Hops: %d\nResponding Host: %s\nDestination Reached?: %s" \
          % (rtt, hops, curr_host, destination_reached)

    # Get geographical distance if destination was reached
    distance = 0
    if destination_reached:
        print
        print 'Getting geographical distance...'

        # Get source and destination latitude and longitude
        src_lat,src_lon = get_lat_lon('')
        dest_lat,dest_lon = get_lat_lon(curr_addr)

        # Calculate distance
        distance = haversine(src_lat, src_lon, dest_lat, dest_lon)
        print "Distance (miles): %d\n" % distance

        # Set success code
        exitCode = 0
    else:
        print "Did not reach %s. Skipping geographical distance calculation." % dest_addr

    return exitCode,rtt,hops,distance

def main(dest_name, port, ttl, verbose):
    exitCode,_,_,_ = get_rtt_hops_distance(dest_name, port, ttl, verbose)
    return exitCode

if __name__ == "__main__":
    parser = optparse.OptionParser(usage="%prog [options] hostname")
    parser.add_option("-t", "--ttl", dest="ttl",
                      help="TTL for the probe [default: %default]",
                      default=32, metavar="TTL")
    parser.add_option("-p", "--port", dest="port",
                      help="Port number to use [default: %default]",
                      default=33434, metavar="PORT")
    parser.add_option("-v", dest="verbose",
                      action="store_true")
    options, args = parser.parse_args()
    if len(args) != 1:
        parser.error()
    else:
        dest_name = args[0]
    sys.exit(main(dest_name=dest_name,
                  port=int(options.port),
                  ttl=int(options.ttl),
                  verbose=options.verbose))
