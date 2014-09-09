#ifndef __GUIAPP_MEASURE_H
#define __GUIAPP_MEASURE_H

#include "DIALOG.h"
#include "sys.h" 
#include "ff.h" 

extern u8 measure_recode_flag; //记录标志，0不记录，1记录



WM_HWIN Create_GUIAPP_MEASURE(void);
void SaveDataIntoFile(u8 * v[4]);


#endif
