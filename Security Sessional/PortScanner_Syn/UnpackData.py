import socket
import textwrap
import struct
from Ethernet_Frame import  Ethernet_Frame
from IP_Packet import  IP_Packet
from TCP_Packet import TCP_Packet

class UnpackData:

    # Ethernet Frame, IP_Packet, TCP_Packet objects are stored ...
    ethernet_frame = Ethernet_Frame()
    ip_packet = IP_Packet()
    tcp_packet = TCP_Packet()

    def getMacAddressStringFormat(self, address_bytes): # eg. FF:AA:BB:FF:CD format
        address_str = ''
        address_str = map('{:02x}'.format, address_bytes)
        address_str = ':'.join(address_str).upper()
        return  address_str

    def unpack_ethernet_frame(self, data):
        destination_MAC, source_MAC, protocol_Ethernet = struct.unpack('! 6s 6s H', data[:14])
                                                                        # First 14 bits are ethernet header
        self.ethernet_frame.mac_source = self.getMacAddressStringFormat(source_MAC)
        self.ethernet_frame.mac_destination = self.getMacAddressStringFormat(destination_MAC)
        self.ethernet_frame.protocol_ethernet = socket.htons(protocol_Ethernet)
        self.ethernet_frame.payload_ethernet = data[14:]

    def unpack_ip_header(self):
        ip_data = self.ethernet_frame.payload_ethernet  # ethernet's payload is ip's full data

        version_ihl_together = ip_data[0]  # first byte
        # Using bit-wise operation
        self.ip_packet.ip_version = (version_ihl_together >> 4)
        self.ip_packet.ip_header_length = (version_ihl_together & 15) * 4

        # all header info for ip_pkt is 20 bytes long  '\'   # for new line
        self.ip_packet.ttl, self.ip_packet.protocol_ip, self.ip_packet.source_ip_address, \
        self.ip_packet.destination_ip_address = struct.unpack('! 8x B B 2x 4s 4s', ip_data[:20])

        self.ip_packet.destination_ip_address = self.ip_packet.format_ip_address(self.ip_packet.destination_ip_address)
        self.ip_packet.source_ip_address = self.ip_packet.format_ip_address(self.ip_packet.source_ip_address)

        self.ip_packet.payload_ip = ip_data[self.ip_packet.ip_header_length:]  # start at header_len: go to the end

        self.ip_packet.printIPPacket()

    def unpack_tcp_header(self):
        data = self.ip_packet.payload_ip
        (sourcePort, destPort, sequence, acknowledgement, offset_reserved_flags_together) = struct.unpack('! H H L L H', data[:14])
        offset = (offset_reserved_flags_together >> 12) * 4

        flag_urgent = (offset_reserved_flags_together & 32) >> 5
        flag_ack = (offset_reserved_flags_together & 16) >> 4
        flag_psh = (offset_reserved_flags_together & 8) >> 3
        flag_rst = (offset_reserved_flags_together & 4) >> 2
        flag_syn = (offset_reserved_flags_together & 2) >> 1
        flag_fin = (offset_reserved_flags_together & 1) >> 0

        self.tcp_packet.urg = flag_urgent
        self.tcp_packet.ack = flag_ack
        self.tcp_packet.psh = flag_psh
        self.tcp_packet.rst = flag_rst
        self.tcp_packet.syn = flag_syn
        self.tcp_packet.fin = flag_fin


