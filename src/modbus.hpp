#pragma once
#include <string>
#include <modbus/modbus.h>
class Modbus
{
private:
    modbus_t *ctx;
    uint16_t tab_reg[32];
public:
    Modbus(const char *ip);
    Modbus(const char *device, int slave_id);
    ~Modbus();
    void write(int reg, int val);
    int read(int reg);
    int readWhenChanged(int reg);    
    void makeMove(int x,int y,int z);
};