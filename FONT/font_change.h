#ifndef __FONT_CHANGE_H__
#define __FONT_CHANGE_H__	 
#include "sys.h"







//gbkתunicode
u16 GBK_To_UNI(u8 *src,u16 *dst);


//gbkתutf8
void GBK_To_UTF8(u8 *src,u8 *dst);


#endif
