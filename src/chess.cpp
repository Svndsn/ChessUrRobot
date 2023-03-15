#include "chess.hpp"
#include <assert.h>
#include <iostream>
Chess::Chess(string path, const char* ip,const char *device,int baud):sf(path), ur(ip),at(device,baud)
{

}

void Chess::urMove(){
    std::cout<<"here"<<std::endl;
    int * coordArray = parseMove(sf.getEngineMove());
    
    ur.makeMove(coordArray[0],coordArray[1],1);//input first 2 from array and z=1
    assert(ur.readWhenChanged(128) == 0);
    ur.makeMove(coordArray[0],coordArray[1], 0); // same input but z=0
    // communicate with atmega
    // assert grip
    ur.makeMove(coordArray[0],coordArray[1], 1); // input first 2 from array and z=1
    assert(ur.readWhenChanged(128) == 0);
    ur.makeMove(coordArray[2],coordArray[3], 1); // input last 2 from array and z=1
    assert(ur.readWhenChanged(128) == 0);
    ur.makeMove(coordArray[2],coordArray[3], 1); // input last 2 from array and z=0
    assert(ur.readWhenChanged(128) == 0);
    // communicate with atmega
    // assert no grip
    ur.makeMove(coordArray[2],coordArray[3], 1); // input last 2 from array and z=1
    assert(ur.readWhenChanged(128) == 0);
    ur.makeMove(9, 9, 2); // input default pos
    assert(ur.readWhenChanged(128) == 0);
}


int * Chess::parseMove(std::string coordinates){
    int array[4];
    array[0]=coordinates[0]-97;
    array[1]=coordinates[1]-49;
    array[2]=coordinates[2]-97;
    array[3]=coordinates[3]-49;
    return array;

}

void Chess::userMove(std::string coordinates){
    sf.sendUserMove(coordinates);
}

bool Chess::isGameOver(){
    return sf.readGameover();
}