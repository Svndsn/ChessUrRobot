#include "chess.hpp"
#include <assert.h>
#include <iostream>
#include <vector>
Chess::Chess(string path, Modbus* in):sf(path),ur(*in)
{
}

void Chess::urMove()
{
    int *coordArray = parseMove(nextEngineMove);
    bool kill=0;
    kill = moveIsKill(sf.getFen(), coordArray[2], coordArray[3]);
    ur.write(128,1);
    sf.makeMove(nextEngineMove);
    cout << "here"<<endl;
    sf.getFen();
    if (kill)
    {
        ur.makeMove(coordArray[2], coordArray[3], 1); // input last 2 from array and z=1
        assert(ur.readWhenChanged(128) == 0);
        ur.makeMove(coordArray[2], coordArray[3], 0); // input last 2 from array and z=0
        assert(ur.readWhenChanged(128) == 0);
        // at.write(1000,1);
        // assert(at.readWhenChanged(1000)==0);
        ur.makeMove(coordArray[2], coordArray[3], 1); // input last 2 from array and z=1
        assert(ur.readWhenChanged(128) == 0);
        ur.makeMove(10, 10, 1); // input pile pos and z=1
        assert(ur.readWhenChanged(128) == 0);
        ur.makeMove(10, 10, 3);
        assert(ur.readWhenChanged(128) == 0);
        // at.write(1000,2);
        // assert(at.readWhenChanged(1000)==0);
        ur.makeMove(10, 10, 1); // input pile pos and z=1
        assert(ur.readWhenChanged(128) == 0);
        
    }
    
        cout << "here2" << endl;
        ur.makeMove(1, 1, 1); // input first 2 from array and z=1
        cout << "here3" << endl;
        assert(ur.readWhenChanged(128) == 0);
        cout << "here4" << endl;
        ur.makeMove(coordArray[0], coordArray[1], 0); // same input but z=0
        // at.write(1000,1);
        // assert(at.readWhenChanged(1000)==0);
        ur.makeMove(coordArray[0], coordArray[1], 1); // input first 2 from array and z=1
        assert(ur.readWhenChanged(128) == 0);
        ur.makeMove(coordArray[2], coordArray[3], 1); // input last 2 from array and z=1
        assert(ur.readWhenChanged(128) == 0);
        ur.makeMove(coordArray[2], coordArray[3], 0); // input last 2 from array and z=0
        assert(ur.readWhenChanged(128) == 0);
        // at.write(1000,2);
        // assert(at.readWhenChanged(1000)==0);
        ur.makeMove(coordArray[2], coordArray[3], 1); // input last 2 from array and z=1
        assert(ur.readWhenChanged(128) == 0);
        ur.makeMove(9, 9, 2); // input default pos
        assert(ur.readWhenChanged(128) == 0);
    
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