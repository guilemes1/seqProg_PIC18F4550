#ifndef MCP4725_H
#define MCP4725_H

void dac_init ( void );
void MCP4725(unsigned int Vout);
//void MCP4725X (unsigned int Vout1 , unsigned int Vout2 );
//void dac_read ( unsigned char *ptr);

struct MCP4725_t
{
    void (*vout)(unsigned int Vout );
};

#endif