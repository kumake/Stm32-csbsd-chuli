#ifndef __GUIAPP_MAIN_H
#define __GUIAPP_MIAN_H

#include "DIALOG.h"
#include "sys.h" 
#include "ff.h" 

#define ID_ICONVIEW_0 (GUI_ID_USER + 0x08)

//#define GUI_ID_TEXT0  (GUI_ID_USER + 0x09)



/* 用户定义的消息 */
#define MY_MESSAGE_RTC 				(WM_USER+0)



// USER START (Optionally insert additional defines)
// USER END

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/
























extern WM_HWIN hWinMain;

extern WM_HWIN hWinTop;



extern GUI_XBF_DATA XBF_Data;
extern GUI_FONT     XBF_Font;
extern FIL          Fontfile;




#endif

