/*********************************************************************
*                                                                    *
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
*                                                                    *
**********************************************************************
*                                                                    *
* C-file generated by:                                               *
*                                                                    *
*        GUI_Builder for emWin version 5.22                          *
*        Compiled Jul  4 2013, 15:16:01                              *
*        (c) 2013 Segger Microcontroller GmbH & Co. KG               *
*                                                                    *
**********************************************************************
*                                                                    *
*        Internet: www.segger.com  Support: support@segger.com       *
*                                                                    *
**********************************************************************
*/

// USER START (Optionally insert additional includes)
// USER END

#include "DIALOG.h"
#include "GUIAPP_MEASURE.h"
#include "GUIAPP_MAIN.h"
#include "sys.h"
#include "rtc.h"
#include "ff.h"
#include <string.h>
#include <stdio.h>

WM_HWIN hWin_Measure;
u8 measure_recode_flag; //记录标志，0不记录，1记录

/*********************************************************************
*                                                                    *
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
*                                                                    *
**********************************************************************
*                                                                    *
* C-file generated by:                                               *
*                                                                    *
*        GUI_Builder for emWin version 5.22                          *
*        Compiled Jul  4 2013, 15:16:01                              *
*        (c) 2013 Segger Microcontroller GmbH & Co. KG               *
*                                                                    *
**********************************************************************
*                                                                    *
*        Internet: www.segger.com  Support: support@segger.com       *
*                                                                    *
**********************************************************************
*/

// USER START (Optionally insert additional includes)
// USER END


/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_WINDOW_0 (GUI_ID_USER + 0x00)
#define ID_LISTVIEW_0 (GUI_ID_USER + 0x02)
#define ID_HEADER_0 (GUI_ID_USER + 0x03)
#define ID_BUTTON_0 (GUI_ID_USER + 0x04)
#define ID_TEXT_0 (GUI_ID_USER + 0x05)
#define ID_TEXT_1 (GUI_ID_USER + 0x06)
#define ID_TEXT_2 (GUI_ID_USER + 0x07)
#define ID_TEXT_3 (GUI_ID_USER + 0x08)
#define ID_TEXT_4 (GUI_ID_USER + 0x09)
#define ID_TEXT_5 (GUI_ID_USER + 0x0A)


// USER START (Optionally insert additional defines)
// USER END

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

// USER START (Optionally insert additional static data)
// USER END

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "Window", ID_WINDOW_0, 0, 0, 240, 300, 0, 0x0, 0 },
  { LISTVIEW_CreateIndirect, "Listview", ID_LISTVIEW_0, 0, 150, 240, 150, 0, 0x0, 0 },
  { HEADER_CreateIndirect, "Header", ID_HEADER_0, 0, 0, 240, 30, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button", ID_BUTTON_0, 170, 77, 50, 30, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "wendu_t", ID_TEXT_0, 25, 45, 50, 30, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "shidu_t", ID_TEXT_1, 25, 77, 50, 30, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "dianya_t", ID_TEXT_2, 25, 109, 50, 30, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "wendu_v", ID_TEXT_3, 77, 45, 80, 30, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "shidu_v", ID_TEXT_4, 77, 77, 80, 30, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "dianya_v", ID_TEXT_5, 77, 109, 80, 30, 0, 0x64, 0 },
  // USER START (Optionally insert additional widgets)
  // USER END
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/

// USER START (Optionally insert additional static code)
// USER END

/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialog(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int     NCode;
  int     Id;
  // USER START (Optionally insert additional variables)
	u8 * pstr;
	u8 value_str[10];
	u8 table_value[4][20];
	u8 * table_p[4];
	u8 i;
  // USER END

  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
    //
    // Initialization of 'Window'
    //
    hItem = pMsg->hWin;
    WINDOW_SetBkColor(hItem, 0x00C0C0C0);
		//WINDOW_SetBkColor(hItem, 0x0085DDF3);
	
    //
    // Initialization of 'Listview'
    //
	
	//温度 Temperature
  //湿度 Humidity
  //电压 Voltage
  //时间 Time
    hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0);
    LISTVIEW_AddColumn(hItem, 40, "Temp", GUI_TA_HCENTER | GUI_TA_VCENTER);
    LISTVIEW_AddColumn(hItem, 40, "Humi", GUI_TA_HCENTER | GUI_TA_VCENTER);
		LISTVIEW_AddColumn(hItem, 40, "Volt", GUI_TA_HCENTER | GUI_TA_VCENTER);
		LISTVIEW_AddColumn(hItem, 120, "Time", GUI_TA_HCENTER | GUI_TA_VCENTER);
		LISTVIEW_AddRow(hItem, NULL);
    LISTVIEW_SetGridVis(hItem, 1);
    
    //
    // Initialization of 'Header'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_HEADER_0);
    HEADER_AddItem(hItem, 240, "超声波空气湿度检测", 14);
		HEADER_SetFont(hItem, &XBF_Font);
		//HEADER_SetBkColor(hItem, 0x0014A8CD);
		
		
		
		// Initialization of 'Button'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
    BUTTON_SetText(hItem, "记录");
		BUTTON_SetFont(hItem, &XBF_Font);
		
		
    //
    // Initialization of 'Text'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    TEXT_SetText(hItem, "温度");
		TEXT_SetFont(hItem, &XBF_Font);
		TEXT_SetBkColor(hItem, 0x00E7B023);
		TEXT_SetTextColor(hItem, 0x00FFFFFF);
    //
    // Initialization of 'Text'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    TEXT_SetText(hItem, "湿度");
		TEXT_SetFont(hItem, &XBF_Font);
		TEXT_SetBkColor(hItem, 0x00E7B023);
		TEXT_SetTextColor(hItem, 0x00FFFFFF);
    //
    // Initialization of 'Text'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_2);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    TEXT_SetText(hItem, "电压");
		TEXT_SetFont(hItem, &XBF_Font);
		TEXT_SetBkColor(hItem, 0x00E7B023);
		TEXT_SetTextColor(hItem, 0x00FFFFFF);
    //
    // Initialization of 'Text'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_3);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    TEXT_SetText(hItem, "");
    TEXT_SetFont(hItem, GUI_FONT_13B_ASCII);
    TEXT_SetTextColor(hItem, 0x00FFFFFF);
		TEXT_SetBkColor(hItem, 0x00E7B023);
    //
    // Initialization of 'Text'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_4);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    TEXT_SetText(hItem, "");
    TEXT_SetFont(hItem, GUI_FONT_13B_ASCII);
    TEXT_SetTextColor(hItem, 0x00FFFFFF);
		TEXT_SetBkColor(hItem, 0x00E7B023);
    //
    // Initialization of 'Text'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_5);
    TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    TEXT_SetText(hItem, "");
    TEXT_SetTextColor(hItem, 0x00FFFFFF);
    TEXT_SetFont(hItem, GUI_FONT_13B_ASCII);
		TEXT_SetBkColor(hItem, 0x00E7B023);
    // USER START (Optionally insert additional code for further widget initialization)
    // USER END
    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_LISTVIEW_0: // Notifications sent by 'Listview'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_SEL_CHANGED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_HEADER_0: // Notifications sent by 'Header'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_MOVED_OUT:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_BUTTON_0: // Notifications sent by 'Button'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
				//if(WM_IsWindow(hWin_Measure))
				//{
					//WM_DeleteWindow(hWin_Measure);			//		
				//}
				hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
				if (measure_recode_flag)
				{
					measure_recode_flag=0;
					BUTTON_SetText(hItem, "记录");
					
				}else
				{
					 measure_recode_flag=1;
					 BUTTON_SetText(hItem, "取消");
				}
				//GUI_Delay(100);
			
			
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    // USER START (Optionally insert additional code for further Ids)
    // USER END
    }
    break;
  // USER START (Optionally insert additional message handling)
	case MSG_MEASURE_UPDATE:
			 pstr=(u8 *)pMsg->Data.p;
			 if ((pstr[0]!=':')||(pstr[1]!='V')||(pstr[2]!=' '))
			 break;
			 
			 pstr=pstr+2;
			 //更新温度
			 i=0;
			 pstr++;
			 while((*pstr!=' ')&&(*pstr!=0))
			 {
					value_str[i]=*pstr;
					i++;
					pstr++;
					if (i>9)
					break;
			 }
			 value_str[i]=0;
			 strcpy((char *)table_value[0],(char *)value_str);
			 hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_3);
       TEXT_SetText(hItem,(const char *)value_str);
			 //更新湿度
			 i=0;
			 pstr++;
			 while((*pstr!=' ')&&(*pstr!=0))
			 {
					value_str[i]=*pstr;
					i++;
					pstr++;
					if (i>9)
					break;
			 }
			 value_str[i]=0;
			 strcpy((char *)table_value[1],(char *)value_str);
			 hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_4);
       TEXT_SetText(hItem,(const char *)value_str);
			 //更新电压
			 i=0;
			 pstr++;
			 while((*pstr!=' ')&&(*pstr!=0))
			 {
					value_str[i]=*pstr;
					i++;
					pstr++;
					if (i>9)
					break;
			 }
			 value_str[i]=0;
			 strcpy((char *)table_value[2],(char *)value_str);
			 hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_5);
       TEXT_SetText(hItem,(const char *)value_str);
			 
			 //时间
			 sprintf((char *)table_value[3],"%04d-%02d-%02d %02d:%02d:%02d",calendar.w_year,calendar.w_month,calendar.w_date,calendar.hour,calendar.min,calendar.sec);
      	
			 
			 if (!measure_recode_flag)//不记录
			 break;
			 
			 
			 
			 table_p[0]=table_value[0];
			 table_p[1]=table_value[1];
			 table_p[2]=table_value[2];
			 table_p[3]=table_value[3];
			 
 
			 hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0);
			 LISTVIEW_InsertRow(hItem,0,(const GUI_ConstString *)table_p);//插入一行
			 LISTVIEW_DeleteRow(hItem,9);//删除最后一行
			 
			 SaveDataIntoFile(table_p);

		break;
  // USER END
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}



/*************************** End of file ****************************/

WM_HWIN Create_GUIAPP_MEASURE(void) {
  WM_HWIN hWin;

  if (WM_IsWindow(hWin_Measure))
  {
  	 return  hWin_Measure;
  }
	measure_recode_flag=0;//不记录
  hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, hWinMain, 0, 0);
  hWin_Measure= hWin;
  return hWin;
}

// USER START (Optionally insert additional public code)
// USER END

/*************************** End of file ****************************/


void SaveDataIntoFile(u8 * v[4])
{
	FIL  hFile;
  UINT  bw;
  FRESULT res;

	char filepath[30];
	char each_recode[40];
	
	strcpy(filepath,"1:/MEASURE/20140314.txt");
	
	filepath[11]=v[3][0];//年
	filepath[12]=v[3][1];
	filepath[13]=v[3][2];
	filepath[14]=v[3][3];
	
	filepath[15]=v[3][5];//月
	filepath[16]=v[3][6];
	
	filepath[17]=v[3][8];//日
	filepath[18]=v[3][9];
	
	
	sprintf(each_recode,"%s %s %s %s\r\n",v[3],v[0],v[1],v[2]);
  
	res = f_open(&hFile, filepath, FA_OPEN_ALWAYS|FA_WRITE);//写上字体文件存放的位置

	if (res != FR_OK) 
  return ; /* Error */
	
	res = f_lseek(&hFile, hFile.fsize);

	if (res != FR_OK) 
  return ; /* Error */

  res = f_write(&hFile,each_recode, strlen(each_recode), &bw); 
		
	if (res != FR_OK) 
  return ; /* Error */	
		
	f_close(&hFile);


}