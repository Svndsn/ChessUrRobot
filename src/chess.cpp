#include "chess.hpp"
#include <assert.h>
#include <iostream>
#include <vector>
Chess::Chess(string path, const char* ip,const char *device,int baud):sf(path), ur(ip),at(device,baud)
{

}

void Chess::urMove()
{
    int *coordArray = parseMove(nextEngineMove);
    bool kill;
    kill = moveIsKill(sf.getFen(), coordArray[2], coordArray[3]);
    sf.makeMove(nextEngineMove);
    if (kill)
    {
        ur.makeMove(coordArray[2], coordArray[3], 1); // input last 2 from array and z=1
        // assert(ur.readWhenChanged(128) == 0);
        ur.makeMove(coordArray[2], coordArray[3], 0); // input last 2 from array and z=0
        // assert(ur.readWhenChanged(128) == 0);
        //  communicate with atmega
        //  assert grip
        ur.makeMove(coordArray[2], coordArray[3], 1); // input last 2 from array and z=1
        // assert(ur.readWhenChanged(128) == 0);
        ur.makeMove(10, 10, 1); // input pile pos and z=1
        // assert(ur.readWhenChanged(128) == 0);
        //where to drop?
        ur.makeMove(9, 9, 2); // input default pos
        // assert(ur.readWhenChanged(128) == 0);
    }
    
    
        ur.makeMove(coordArray[0], coordArray[1], 1); // input first 2 from array and z=1
        // assert(ur.readWhenChanged(128) == 0);
        ur.makeMove(coordArray[0], coordArray[1], 0); // same input but z=0
        // communicate with atmega
        // assert grip
        ur.makeMove(coordArray[0], coordArray[1], 1); // input first 2 from array and z=1
        // assert(ur.readWhenChanged(128) == 0);
        ur.makeMove(coordArray[2], coordArray[3], 1); // input last 2 from array and z=1
        // assert(ur.readWhenChanged(128) == 0);
        ur.makeMove(coordArray[2], coordArray[3], 0); // input last 2 from array and z=0
        // assert(ur.readWhenChanged(128) == 0);
        //  communicate with atmega
        //  assert no grip
        ur.makeMove(coordArray[2], coordArray[3], 1); // input last 2 from array and z=1
        // assert(ur.readWhenChanged(128) == 0);
        ur.makeMove(9, 9, 2); // input default pos
        // assert(ur.readWhenChanged(128) == 0);
    
}

int * Chess::parseMove(std::string coordinates){
    static int myArray[4];
    myArray[0]=coordinates[0]-97;
    myArray[1]=coordinates[1]-49;
    myArray[2]=coordinates[2]-97;
    myArray[3]=coordinates[3]-49;
    
    return myArray;

}

void Chess::userMove(std::string coordinates){
    nextEngineMove= sf.sendUserMove(coordinates);
}

bool Chess::isGameOver(){
    return sf.readGameover();
}

bool Chess::moveIsKill(std::string fen, int movex, int movey){
    fen = "/"+fen+"/";
    int pos=-1;
    int posend;
    for(int i=0; i<=7-movey;i++){
        pos = fen.find('/',pos+1);
        posend = fen.find('/',pos+1);
    }
    
    std::string row =fen.substr(pos+1,posend-pos-1);
    
    std::vector<int> xPos; 
    for(int i=0;i<row.length();i++){
        if(row[i]+0>47 && row[i]+0<58){
            
            for(int j=0;j<row[i]-48;j++){
                xPos.push_back(0);
            }
        } else
        {
            xPos.push_back(1);
        }
        
    }
    
    return (bool) xPos[movex];

}