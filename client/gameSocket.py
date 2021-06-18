from google import protobuf
# from snake_pb2 import Header
import snake_pb2 as serialMsg
import socket
import sys
from serializable import Serializable
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

        print(len(headerSize))
        print(header._size)
        print(object._size)
        
        btarray = headerSize + header._data + object._data
        
        print(len(btarray))
        file = open('socksnake.data','wb')
        file.write(btarray)
        self.sock.sendto(btarray,(self.host,self.port)) 
        return True

    def recvHeader(self):
        print(f'start rcHeader')
        buf = self.sock.recv(2000,socket.MSG_PEEK)
        file = open('recdata','wb')
        file.write(buf)
        
        print(buf)
        rc = Header(serialMsg.MessageID.LOGINPETITION)
        n = 0
        hdSize, n =_DecodeVarint32(buf,n)
        print(f'header starts at {n}')
        print(f'header of size {hdSize}')
        rc.from_bin(buf[n:n+hdSize])
        n+=hdSize
        print(f'obj starts at {n}')
        return (rc,n)

    def loadObject(self,object,startPos):
        print('start load')
        buf = self.sock.recv(2000)
        object.from_bin(buf[startPos:])
        print('loaded')