/***************************************************************************
*                                                                         *
*  Copyright (C) 1997-1999 Drago Entertainment Corp. All rights reserved  *
*                                                                         *
*  File:       JERROR.H                                                   *
*  Purpose:    Header file for error handling                             *
*                                                                         *
*  Project:    ACTIVATION!                                                *
*                                                                         *
*  Author:     Jan Jasinski                                               *
*  Modified:   99.09.23                                                   *
*  Notes:                                                                 *
*                                                                         *
***************************************************************************/

/*覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧�*
* Preprocessor                                                            *
*覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧�*/
#ifndef __JERROR_H__
#define __JERROR_H__

/*覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧�*
* Defines                                                                 *
*覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧�*/
#define CHECK_DX(x) {if(FAILED(hResult=(x))){ DUMP_F("%s in %s: %d",GetDirectXStatusString(hResult),__FILE__,__LINE__);} }

/*覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧�*
*                                                                         *
* Structures                                                              *
*                                                                         *
*覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧�*/
/*覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧�*
* Struct ERRLIST                                                          *
*覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧�*/
typedef struct tag_ERRLIST
{
	HRESULT hResult;
	LPSTR lpszError;
} ERRLIST;
typedef ERRLIST * LPERRLIST;

/*覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧�*
*                                                                         *
*  Function Prototypes                                                    *
*                                                                         *
*覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧�*/
int   OpenDebugLogFile(LPSTR lpszFileName);
int   WriteDebugLogFile(LPSTR lpszComment);
void  CloseDebugLogFile( void );
void  DUMP_O( char * fmt, ... ); // Dump to output device
void  DUMP_F( char * fmt, ... ); // Dump to file
void  TRY_DX( HRESULT hResult );
LPSTR GetDirectXStatusString(HRESULT DDStatus);

/*覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧�*
* Globals                                                                 *
*覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧�*/
extern BOOL g_bLogFile;

/*覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧�*
* End of code                                                             *
*覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧覧�*/
#endif //__JERROR_H__
/***************************************************************************
* End of file JDX_INIT.H                                                  *
***************************************************************************/
