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

        j = dict()
        j["ID"] = headerId;

        #TODO buscar una forma mas elegante de hacer esto
        if(isinstance(object,Serializable)):
            print('serializando un gameobject')
            object.to_bin()
            j["OBJ"] = object._json
        else:
            print('enviando un json')
            print(object)
            j["OBJ"] = object
            print(j)
            print(self.host)
            print(self.port)
            

        self.sock.sendto(json.dumps(j).encode(),(self.host,self.port)) 
        return True

    def recvObj(self):
        buf = self.sock.recv(32768)
        j = json.loads(buf)
        return j