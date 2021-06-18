import snake_pb2 as serialMsg
import abc

class Serializable:
    @abc.abstractmethod
    def from_message(self,msg):
        pass
    @abc.abstractmethod
    def get_message(self):
        pass
    def __init__(self):
        print('init serializable')
        self._data = None
        self._size = -1
        self._msg = None
    def to_bin(self):
        self._msg = self.get_message()
        self._data =  self._msg.SerializeToString()
        self._size = self._msg.ByteSize()
        print(self._size)
    def from_bin(self, bin):
        self._msg.ParseFromString(bin)
        self._data = bin
        self._size = self._msg.ByteSize()
        self.from_msg(self._msg)

class Header(Serializable):
    def __init__(self,id):
        self.id = id
    def get_message(self):
        msg = serialMsg.Header()
        msg.msgID = self.id
        return msg
    def from_message(self,msg):
        self.id = msg.msgID