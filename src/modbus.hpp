#pragma once
#include <string>
#include <modbus/modbus.h>
class Modbus
{
private:
    modbus_t *ctx;
    uint16_t tab_reg[32];
public:
    int read(int reg);
    Modbus(Modbus *in);
    Modbus(const char *ip);
    Modbus(const char *device, int slave_id);
    ~Modbus();
    void write(int reg, int val);
    int readWhenChanged(int reg);    
    void makeMove(int x,int y,int z);
};