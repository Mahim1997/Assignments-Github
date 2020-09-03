import binascii
import socket
from struct import *

from IP_Details import IP_Details
from TCP_Details import TCP_Details


class RawPacket:
    def __init__(self, src_ip, dest_ip, dest_port):
        self.source_port = 7799  # randomly keeping any port on 'my' machine

        self.source_ip = src_ip
        self.destination_ip = dest_ip

        self.ip_details = IP_Details(src_ip, dest_ip)  # form IP packet
        self.tcp_details = TCP_Details(self.source_port, dest_port)  # form TCP packet

        # --- Save in binary format ----
        self.tcp_header = b""
        self.ip_header = b""
        self.packet = b""
        self.TIMEOUT_SET = 2.0  # default 2 seconds



    def generate_packet(self):  # full packet generation
        # IP header + checksum
        final_header_ip = self.ip_details.getFinalIPHeader()

        # TCP header + checksum
        temp_header_tcp = self.tcp_details.generate_temporary_tcp_header()

        pseudo_header = pack("!4s4sBBH", self.ip_details.src_addr, self.ip_details.dest_addr,
                             self.ip_details.header_checksum, self.ip_details.protocol,
                             len(temp_header_tcp))

        header_intermediate_tcp = pseudo_header + temp_header_tcp

        final_tcp_header = self.tcp_details.getFinalTCPHeader(header_intermediate_tcp)

        # ------- Finalize headers ----------
        self.ip_header = final_header_ip
        self.tcp_header = final_tcp_header
        self.packet = final_header_ip + final_tcp_header

    def send_packet(self):
        s = socket.socket(socket.AF_INET, socket.SOCK_RAW, socket.IPPROTO_TCP)
        s.setsockopt(socket.IPPROTO_IP, socket.IP_HDRINCL, 1)  # so that kernel does not put headers itself
        s.sendto(self.packet, (self.ip_details.dest_ip, 0))
        s.settimeout(self.TIMEOUT_SET)
        data = None
        try:
            data = s.recv(1024)  # receive 1024 bytes
        except:
            print("(~) IP: " + self.ip_details.dest_ip + ", Port " + str(self.tcp_details.dest_port) + " is Filtered [Timeout of " + str(self.TIMEOUT_SET) + " s ]")
        s.settimeout(None)
        s.close()
        return data

    def checkPortStatus(self, ipAddr, port, response):
        cont = binascii.hexlify(response)
        if cont[65:68] == b"012":  # 0001 0010 [12] ==> [CWR ECE URG ACK] [PSH RST SYN FIN] = [0 0 0 1] [0 0 1 0] ... SYN = 1, ACK = 1
            print("+ + + IP: " + ipAddr + ", Port " + str(port) + " is OPEN")
        else:
            print("- - - IP: " + ipAddr + ", Port " + str(port) + " is closed")

def obtain_self_ip():
    import socket
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    s.connect(("8.8.8.8", 80))
    ip_addr = (s.getsockname()[0])
    s.close()
    return ip_addr

def port_scan_syn(ipAddress, port):
    # obtain source ip
    # for port in [21, 22, 80, 435, 8080]:  # list of ports to check
    ipAddress = socket.gethostbyname(ipAddress)  # ip will be mapped back to ip itself
    # print("SYN SCAN >> Scanning for ip: " + str(ipAddress) + " , port = " + str(port))
    src_ip = obtain_self_ip()
    rawPacket = RawPacket(src_ip, ipAddress, port)
    rawPacket.generate_packet()
    resultSet = rawPacket.send_packet()
    if resultSet is not None:
        rawPacket.TIMEOUT_SET = 1.0  # 2 seconds timeout
        rawPacket.checkPortStatus(ipAddress, port, resultSet)


# ------------ main ----------------

willRun=False
if willRun:
    port_scan_syn("172.217.163.142", 100)