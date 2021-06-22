#pragma once
#include "SnakeGames.h"
#include "Serializable.h"

class LoginPetition : public Serializable
{
public:
    std::string nick;
    LoginPetition(const std::string &name) : nick(name) {}
    virtual void to_bin()
    {
        _json["nick"] = nick;
        std::string dump = _json.dump();
        _data = dump.c_str();
        _size = dump.size();
    }
    virtual int from_bin(json data)
    {

        nick = data["nick"];
        std::cout << "loaded logpet\n";
        return 0;
    }
};

class LoginResponse : public Serializable
{
public:
    uint32_t uid;
    uint32_t arenaSize;
    LoginResponse(size_t id, uint32_t arenaSz) : uid(id), arenaSize(arenaSz){};
    ~LoginResponse() = default;
    virtual void to_bin()
    {
        _json["playerId"] = uid;
        _json["arenaSize"] = arenaSize;
        std::string dump = _json.dump();
        _data = dump.c_str();
        _size = dump.size();
    }
    virtual int from_bin(json data)
    {
        uid = data["playerId"];
        arenaSize = data["arenaSize"];
        return 0;
    }
};

class StartGame : public Serializable
{
public:
    StartGame() {};
    ~StartGame() = default;
    virtual void to_bin()
    {
        _json = {};
        std::string dump = _json.dump();
        _data = dump.c_str();
        _size = dump.size();
    }
    virtual int from_bin(json data) {return 0;}
};
