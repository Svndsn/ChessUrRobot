#include "modbus.hpp"
#include <unistd.h>
Modbus::Modbus(Modbus * in){
    ctx=in->ctx;
}
Modbus::Modbus(const char *ip){
    ctx = modbus_new_tcp(ip, 502);
    if (ctx == NULL) {
        fprintf(stderr, "Unable to create the libmodbus context\n");
    }
    if (modbus_connect(ctx) == -1)
    {
        fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
        modbus_free(ctx);
    }
}
Modbus::Modbus(const char *device, int slave_id){
    ctx = modbus_new_rtu(device,38400,'N',8,1);
    if (ctx == NULL) {
        fprintf(stderr, "Unable to create the libmodbus context\n");
    }
    if (modbus_connect(ctx) == -1)
    {
        fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
        modbus_free(ctx);
    }
    int rc = modbus_set_slave(ctx,slave_id);
    if (rc == -1) {
        fprintf(stderr, "Invalid slave ID\n");
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
        sleep(1);
        rc = modbus_read_registers(ctx, reg, 1, tab_reg);
        if (rc == -1)
        {
            fprintf(stderr, "%s\n", modbus_strerror(errno));
        }
    }
    return tab_reg[0];
}

void Modbus::write(int reg, int val){
    int rc =modbus_write_register(ctx,reg,val);
    if (rc == -1)
        {
            fprintf(stderr, "%s\n", modbus_strerror(errno));
        }
}

void Modbus::makeMove(int x,int y,int z){
    write(129,x);//x1
    write(130,y);//y1
    write(131,z);//z1
    write(128,1);//start
}