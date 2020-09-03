from struct import *


class TCP_Details:

    def __init__(self, source_port, dest_port):
        self.src_port = source_port
        self.dest_port = dest_port
        self.seq_no = 0  # should be 0
        self.ack_no = 0  # should be 0
        self.data_offset = 5  # offset can be anythin
        self.reserved = 0

        # ------------- FLAGS --------------------
        self.syn = 1  # KEEP SYN = 1 ... rest all 0
        self.ack = 0
        self.urg = 0
        self.cwr = 0
        self.ns = 0
        self.psh = 0
        self.fin = 0
        self.rst = 0
        self.ece = 0

        # ----------------- FLAGS DONE ------------------
        self.window_size = 0x7110  # socket.htons (5840)
        self.checksum = 0  # for now
        self.urg_pointer = 0

        self.data_offset_reserved_flags_together = (self.data_offset << 12) + \
                                                   (self.reserved << 9) + \
                                                   (self.ns << 8) + \
                                                   (self.cwr << 7) + \
                                                   (self.ece << 6) + \
                                                   (self.urg << 5) + \
                                                   (self.ack << 4) + \
                                                   (self.psh << 3) + \
                                                   (self.rst << 2) + \
                                                   (self.syn << 1) + \
                                                   (self.fin << 0)

    def generate_temporary_tcp_header(self):
        tmp_tcp_header = pack("!HHLLHHHH",
                              self.src_port,
                              self.dest_port,
                              self.seq_no,
                              self.ack_no,
                              self.data_offset_reserved_flags_together,
                              self.window_size,
                              self.checksum,
                              self.urg_pointer)
        return tmp_tcp_header

    def calculate_checksum_tcp(self, msg):  # to calculate checksum using existing method
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

    def getFinalTCPHeader(self, header_intermediate_tcp):
        final_tcp_header = pack("!HHLLHHHH", self.src_port, self.dest_port,
                                self.seq_no,
                                self.ack_no,
                                self.data_offset_reserved_flags_together, self.window_size,
                                self.calculate_checksum_tcp(header_intermediate_tcp), self.urg_pointer)
        return final_tcp_header
