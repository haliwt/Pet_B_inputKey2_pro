#include "bsp_smg.h"
#include "bsp.h"


void Run_Keep_Heat_Setup_Digital_Numbers(uint8_t value)
{

  Smg_Display_Digital_Numbers_Changed(value);

}

void Repeat_Keep_Heat_Setup_Digital_Numbers(uint8_t value)
{
    Repeat_Smg_Display_Digital_Numbers_Changed(value);

}



