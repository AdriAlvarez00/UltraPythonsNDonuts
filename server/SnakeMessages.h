#pragma once
#include "SnakeGames.h"
#include "Serializable.h"

class LoginPetition : public Serializable{
    public:
    std::string nick;
    LoginPetition(const std::string& name):nick(name){}
    virtual void to_bin(){
        _json["nick"] = nick;
        std::string dump = _json.dump();
        _data = dump.c_str();
        _size = dump.size();
    }
    virtual int from_bin(json data){
        
        nick = data["nick"];
        std::cout << "loaded logpet\n";
        return 0;
    }
};

class LoginResponse: public Serializable
{
public:
    uint32_t uid;
    LoginResponse(size_t id):uid(id){};
    ~LoginResponse()=default;
        virtual void to_bin(){
        _json["playerId"] = uid;
        std::string dump = _json.dump();
        _data = dump.c_str();
        _size = dump.size();
    }
    virtual int from_bin(json data){
        uid = data["playerId"];
        return 0;
    }
};
