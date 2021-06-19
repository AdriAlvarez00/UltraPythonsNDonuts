from google import protobuf
import socket
import sys
from serializable import Serializable
import json

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
        object.to_bin()

        j = dict()
        j["ID"] = headerId;
        j["OBJ"] = object._json

        self.sock.sendto(json.dumps(j).encode(),(self.host,self.port)) 
        return True

    def recvObj(self):
        print('start load')
        buf = self.sock.recv(32768)
        j = json.loads(buf)
        return j