#ifndef _SIGNAL_H_
#define _SIGNAL_H_

#include <stdlib.h>

typedef struct _Signal Signal;

typedef int	(*fptrGet_wifi_signal)(Signal* const);
typedef void	(*fptr_delete)(Signal* const) ;

typedef struct _Signal
{
	void* pDerivedObj;
	char* pWifiName;      //210
	char* pOptionName;    //signal
	//interface to access member functions
	fptrGet_wifi_signal Strength;
	fptrDelete		Delete;
}Signal;

Signal* new_Signal(const char* pWifiName, const char* pOptionName);	//constructor
void delete_Signal(Signal* const pSignalObj);	//destructor

void Signal_Get_strength(Signal* const pSignalObj);


extern int init_Person(Signal* pObj,const char* pFName, const char* pLName);

#endif