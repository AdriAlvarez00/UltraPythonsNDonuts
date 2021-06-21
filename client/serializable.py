import abc
import json

class Serializable:
    @abc.abstractmethod
    def from_json(self,msg):
        pass
    @abc.abstractmethod
    def get_json(self):
        pass
    def __init__(self):
        self._data = None
        self._size = -1
        self._json = None
    def to_bin(self):
        self._json = self.get_json()
        self._data =  json.dumps(self._json)
        self._size = len(self._data)
    def from_bin(self, bin):
        self._json = bin;
        self.from_json(bin)