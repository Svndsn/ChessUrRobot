#pragma once
#include "modbus.hpp"
#include "chess.hpp"
#include <stdio.h>
#include <iostream>

int main()
{
    Modbus ur("127.0.0.1");
    Chess game(ur);
    game.userMove("e2e4");
    
    while(1){//game not over
        //chess.getNextMove()
        //parse move into array[4]
        

        
    }
}