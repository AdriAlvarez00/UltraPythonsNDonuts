from snake_pb2 import Header
import socket
import sys
from serializable import Serializable, Header
import google.protobuf.wrappers_pb2 as pbWrapper
from google.protobuf.internal.encoder import _VarintBytes
from google.protobuf.internal.decoder import _DecodeVarint32

class GameSocket():
    def __init__(self):
        self.sock = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
        self.id = None
        self.host = None
        self.port = None
    def connect(self,host,port):
        self.host = host
        self.port = port
        #TODO logica coniexion
        return None
    def send(self,object,headerId):
        header = Header(headerId)
        header.to_bin()
        object.to_bin()

        headerSize = _VarintBytes(header._size)
        
        btarray = headerSize + header._data + object._data
        print(type(btarray))
        self.sock.sendto(btarray,(self.host,self.port)) 
        return True