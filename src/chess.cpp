#include "chess.hpp"
#include <assert.h>
#include <iostream>
#include <vector>
#include <unistd.h>
#include <thread>
#include "database.h"
Chess::Chess(string path, Modbus* ur,Modbus* at, ChessRobotDatabase* db):sf(path),ur(*ur),at(*at),db(*db)
{

}

Chess::~Chess(){

}

void Chess::urMove(std::string nextEngineMove)
{
    int *coordArray = parseMove(nextEngineMove);
    bool kill;
    kill = moveIsKill(sf.getFen(), coordArray[2], coordArray[3]);
    ur.write(128,1);
    sf.makeMove(nextEngineMove);

    // Insert black move into db
    db.insertMove(nextEngineMove,0,kill);
    

    if (kill)
    {
        ur.makeMove(coordArray[2], coordArray[3], 1); // input last 2 from array and z=1
        assert(ur.readWhenChanged(128) == 0);
        ur.makeMove(coordArray[2], coordArray[3], 0); // input last 2 from array and z=0
        assert(ur.readWhenChanged(128) == 0);
        at.write(1000,1);
        
        
        assert(at.readWhenChanged(1000)==0);
        ur.makeMove(coordArray[2], coordArray[3], 1); // input last 2 from array and z=1
        assert(ur.readWhenChanged(128) == 0);
        ur.makeMove(10, 10, 1); // input pile pos and z=1
        assert(ur.readWhenChanged(128) == 0);
        ur.makeMove(10, 10, 3);
        assert(ur.readWhenChanged(128) == 0);
        at.write(1000,2);
        
        assert(at.readWhenChanged(1000)==0);
        ur.makeMove(10, 10, 1); // input pile pos and z=1
        assert(ur.readWhenChanged(128) == 0);
        
    }
        cout << coordArray[0] << endl;
        cout << coordArray[1] << endl;
        cout << coordArray[2] << endl;
        cout << coordArray[3] << endl;
        ur.makeMove(coordArray[0], coordArray[1], 1); // same input but z=0
        assert(ur.readWhenChanged(128) == 0);
        ur.makeMove(coordArray[0], coordArray[1], 0); // same input but z=0
        assert(ur.readWhenChanged(128) == 0);
        at.write(1000,1);
        
        //assert(at.readWhenChanged(1000)==0);
        ur.makeMove(coordArray[0], coordArray[1], 1); // input first 2 from array and z=1
        assert(ur.readWhenChanged(128) == 0);
        ur.makeMove(coordArray[2], coordArray[3], 1); // input last 2 from array and z=1
        assert(ur.readWhenChanged(128) == 0);
        ur.makeMove(coordArray[2], coordArray[3], 0); // input last 2 from array and z=0
        assert(ur.readWhenChanged(128) == 0);
        at.write(1000,2);
        
        //assert(at.readWhenChanged(1000)==0);
        ur.makeMove(coordArray[2], coordArray[3], 1); // input last 2 from array and z=1
        assert(ur.readWhenChanged(128) == 0);
        //ur.makeMove(9, 9, 2); // input default pos
        //assert(ur.readWhenChanged(128) == 0);
    
}

int * Chess::parseMove(std::string move){
    static int myArray[4];
    myArray[0]=move[0]-97;
    myArray[1]=move[1]-49;
    myArray[2]=move[2]-97;
    myArray[3]=move[3]-49;
    
    return myArray;

}

void Chess::userMove(std::string move){
    std::string nextEngineMove = sf.sendUserMove(move);
    int *coordArray = parseMove(move);
    bool kill;
    kill = moveIsKill(sf.getFen(), coordArray[2], coordArray[3]);
    // Insert white move into db
    db.insertMove(move,1,kill);
    urMove(nextEngineMove);
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
