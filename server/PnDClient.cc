#include "SnakeClient.h"

int main(int argc, char** argv){
    SnakeClient client(argv[1],argv[2]);

    client.Connect();


    return 0;
}