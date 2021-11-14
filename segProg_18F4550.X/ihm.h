#ifndef IHM_H
#define IHM_H

#include "dispLCD4vias.h"

struct
{
    void (* init)(void);
    void (* print)( const char * str );
    void (* printpos)( unsigned char lin, unsigned char col, const char * str );
    void (* printv)(  unsigned char lin, unsigned char col, int num, unsigned char tam );
    void (* clr)( void );

} IHM = { dispLCD_init, dispLCD_str, dispLCD, dispLCD_num, dispLCD_clr };

#endif
