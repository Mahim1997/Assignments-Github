from SynScanner import port_scan_syn

import argparse

# --------------------------- for full tcp port scanning -------------------------------
import socket
import threading
from queue import Queue
import time

lock_thread = threading.Lock()
SERVER = "www.google.com"  # "www.pythonprogramming.net"  # "www.buet.ac.bd"


def port_scan_full_tcp(port):
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    # print("-->>TCP Full SCAN, SERVER = " + str(SERVER))
    try:
        con = sock.connect((SERVER, port))
        with lock_thread:
            print('+++ Port: ' + str(port) + " is OPEN")
        con.close()
    except:
        # no need to waste print statement
        # print('--- Port: ' + str(port) + " is closed")
        pass


def thread_scheduling():  # gets worker from queue
    while True:
        port = queue.get()
        port_scan_full_tcp(port)
        queue.task_done()  # to empty out the queue


queue = Queue()

# --------------------------- for full tcp port scanning --------------------------------

# add arguments
# parse arguments

parser = argparse.ArgumentParser(description='Port Scanner 1505022')

parser.add_argument('-option', '--OPTION', type=str, metavar='', help='-option SYN or -option TCP')

parser.add_argument('-p', '--PORT1', type=int, metavar='', help='-p (For single port, for range use with -pf)')
parser.add_argument('-pf', '--PORT2', type=int, metavar='', help='-p (For range of ports eg. -p 90 -pf 100)')

# --- mutually exclusive ---
parser.add_argument('-scan', '--IP_or_URL', type=str, metavar='', help='Need ip or url')


def run_threaded_port_scan_tcp(serverIP_or_URL, port1, port2):
    SERVER = serverIP_or_URL
    del_port = port2 - port1
    # print("FULL TCP SCAN >> server = " + str(serverIP_or_URL) + " , port1 = " + str(port1) + " , port2 = " + str(port2))
    if del_port >= 40:
        num_workers = 20
    else:
        num_workers = 1

    for worker_iter in range(num_workers):  # fixed 20 threads
        t = threading.Thread(target=thread_scheduling)
        t.daemon = True  # should die when main dies ...
        t.start()

    for ports in range(port1, port2):  # number of ports
        queue.put(ports)

    queue.join()


if __name__ == '__main__':
    args = parser.parse_args()
    port1 = args.PORT1
    port2 = args.PORT2
    option = "SYN"  # default
    option = args.OPTION
    print("port1 = " + str(port1))
    print("port2 = " + str(port2))
    print("args.IP_or_URL = " + str(args.IP_or_URL))
    print("option (default SYN) = " + str(args.OPTION))

    if option == "SYN":
        # use SYN scanning
        if port1 is None and port2 is None:
            # all ports
            for port_to_scan in range(1, 65535):
                port_scan_syn(args.IP_or_URL, port_to_scan)
        elif port1 is not None and port2 is None:
            # one port
            port_scan_syn(args.IP_or_URL, port1)
        else:
            # both exists
            for port_to_scan in range(port1, port2):
                port_scan_syn(args.IP_or_URL, port_to_scan)

    elif option == "TCP":
        SERVER = args.IP_or_URL
        if port1 is None and port2 is None:
            port1 = 1
            port2 = 65535
            run_threaded_port_scan_tcp(args.IP_or_URL, port1, port2)
        elif port1 is not None and port2 is None:
            sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            print("-->>TCP Full SCAN [SINGLE PORT], SERVER = " + str(SERVER))
            try:
                con = sock.connect((SERVER, port1))
                print('+++ Port: ' + str(port1) + " is OPEN")
                con.close()
            except:
                pass
                # print('--- Port:  ' + str(port1) + " is closed")
        else:
            # nothing to do here
            run_threaded_port_scan_tcp(args.IP_or_URL, port1, port2)
