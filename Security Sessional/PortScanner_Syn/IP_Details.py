import socket
from struct import *


class IP_Details:
    def __init__(self, source_ip_address, destination_ip_address):
        # IP header details
        self.version_ip = 0x4  # ipv4 [version = 4]
        self.ip_header_length = 0x5  # header-len = 5
        self.type_of_service = 0x0
        self.total_length = 0x28  # HEX: 0x28
        self.identification = 0xabcd  # random [4 hex]
        self.flags = 0  # for now, keeping all zero
        self.fragment_offset = 0
        self.time_to_live = 255  # using max possible
        self.protocol = 6  # TCP -> 6
        self.header_checksum = 0  # to be calculated later

        self.src_ip = source_ip_address
        self.dest_ip = destination_ip_address

        self.src_addr = socket.inet_aton(self.src_ip)
        self.dest_addr = socket.inet_aton(self.dest_ip)
        self.version_ip_header_length_together = (self.version_ip << 4) + self.ip_header_length  # should be together
        self.flags_offset_together = (self.flags << 13) + self.fragment_offset  # should be together

    def generate_temporary_ip_header(self):  # to have in tcp header
        tmp_ip_header = pack("!BBHHHBBH4s4s", self.version_ip_header_length_together, self.type_of_service, self.total_length,
                             self.identification, self.flags_offset_together,
                             self.time_to_live, self.protocol, self.header_checksum,
                             self.src_addr,
                             self.dest_addr
                             )
        return tmp_ip_header

    def calculate_checksum_ip(self, msg):  # to calculate checksum using existing method
        s = 0
        for i in range(0, len(msg), 2):
            w = (msg[i] << 8) + msg[i + 1]
            s = s + w
        # s = 0x119cc
        s = (s >> 16) + (s & 0xffff)
        # s = 0x19cd
        s = ~s & 0xffff
        # s = 0xe632
        return s

    def getFinalIPHeader(self):
        temp_ip_header = self.generate_temporary_ip_header()
        final_ip_header = pack("!BBHHHBBH4s4s",
                               self.version_ip_header_length_together,
                               self.type_of_service,
                               self.total_length,
                               self.identification,
                               self.flags_offset_together,
                               self.time_to_live,
                               self.protocol,
                                   # use temporary IP header 'pseudo' header to compute checksum
                               self.calculate_checksum_ip(temp_ip_header),
                               self.src_addr,
                               self.dest_addr)
        return final_ip_header

    def printIPDetails(self):
        print("Version_IP_HEADER_LEN_Together: " + str(self.version_ip_header_length_together)
              + ", TypeOfService: " + str(self.type_of_service) + ", Total_len = " + str(self.total_length)
              + ", Identifictaion: " + str(self.identification) + ", Flags_offset = " + str(self.flags_offset_together)
              + ", TTL: " + str(self.time_to_live) + ", Src addr: " + str(self.src_addr)
              + ", Dest addr: " + str(self.dest_addr))