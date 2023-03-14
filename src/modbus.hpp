#include <string>
#include "modbus/modbus.h"
class Modbus
{
private:
    modbus_t *ctx;
    uint16_t tab_reg[32];
public:
    Modbus(const char *ip);
    Modbus(const char *device, int baud, char parity, int data_bit, int stop_bit);
    ~Modbus();
    void write(int reg, int val);
    int read(int reg);
    int readWhenChanged(int reg);    
    void makeMove(int x,int y,int z);
};

Modbus::Modbus(const char *ip){
    ctx = modbus_new_tcp(ip, 502);
    if (modbus_connect(ctx) == -1)
    {
        fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
        modbus_free(ctx);
    }
}
Modbus::~Modbus(){
    modbus_close(ctx);
    modbus_free(ctx);
}

int Modbus::read(int reg){
    int rc = modbus_read_registers(ctx,reg,1,tab_reg);
    if (rc == -1)
        {
            fprintf(stderr, "%s\n", modbus_strerror(errno));
            
        }
    return (int) tab_reg[0];
}
int Modbus::readWhenChanged(int reg){
    int rc = modbus_read_registers(ctx, reg, 1, tab_reg);
    if (rc == -1)
    {
        fprintf(stderr, "%s\n", modbus_strerror(errno));
    }
    int initial = tab_reg[0];
    while (tab_reg[0] == initial){
        rc = modbus_read_registers(ctx, reg, 1, tab_reg);
        if (rc == -1)
        {
            fprintf(stderr, "%s\n", modbus_strerror(errno));
        }
    }
    return tab_reg[0];
}

void Modbus::write(int reg, int val){
    modbus_write_register(ctx,reg,val);
}

void Modbus::makeMove(int x,int y,int z){
    write(129,x);//x1
    write(130,y);//y1
    write(131,z);//z1
    write(128,1);//start
}