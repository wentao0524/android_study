#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
/*
void create_wifi_signal_txt(char *name)
{
	char buf[30];
	memset(buf,0,sizeof(buf));
	sprintf(buf,"cat > %s",name);
	system(buf);
}
*/

Signal* new_Signal(const char* pWifiName, const char* pOptionName)
{
	Signal* pObj = NULL;
	//allocating memory
	pObj = (Signal*)malloc(sizeof(Signal));
	if (pObj == NULL)
	{
		return NULL;
	}
	pObj->pDerivedObj = pObj; //pointing to itself
	pObj->pWifiName = malloc(sizeof(char)*(strlen(pWifiName)+1));
	strcpy(pObj->pWifiName, pWifiName);
	pObj->pOptionName = malloc(sizeof(char)*(strlen(pOptionName)+1));
	strcpy(pObj->pOptionName, pOptionName);

	//Initializing interface for access to functions
	pObj->Delete = delete_Person;			//destructor pointing to destrutor of itself
	pObj->Strength = Signal_Get_strength;

	return pObj;
}

int init_Person(Signal* pObj,const char* pWifiName, const char* pOptionName)
{
//	Person* pObj = NULL;
	//allocating memory
//	pObj = (Person*)malloc(sizeof(Person));
	if (pObj == NULL)
	{
		return -1;
	}
	pObj->pDerivedObj = pObj; //pointing to itself
	pObj->pWifiName = malloc(sizeof(char)*(strlen(pWifiName)+1));
	strcpy(pObj->pWifiName, pWifiName);
	pObj->pOptionName = malloc(sizeof(char)*(strlen(pOptionName)+1));
	strcpy(pObj->pOptionName, pOptionName);

	//Initializing interface for access to functions
	pObj->Delete = delete_Signal;			//destructor pointing to destrutor of itself
	pObj->Strength = Signal_Get_strength;

	return 0;
}



void delete_Signal(Signal* const pSignalObj)
{
	if(pSignalObj!= NULL)
	{
//		free(pPersonObj->pFirstName);
//		free(pPersonObj->pLastName);
//		free(pPersonObj);
	}
}




int Signal_Get_strength(Signal* const pSignalObj)
{
	
}



int get_index(char *buf)
{
	int index=-1;
	char *buf_flag=NULL;
	char buf_index[]="Siganl";
	buf_flag=strstr(buf,buf_index);
	if(buf_flag!=NULL)
	{
		index=buf_flag-buf;
	}
	return index; 
}

int get_wifi_strength(char *buf,char *name_wifi,int index)
{
	int wifi_strength=0;
	char *buf_flag=NULL;
	buf_flag=strstr(buf,name_wifi);
	if(buf_flag!=NULL)
	{
		wifi_strength=strtol(&buf[index],NULL,10);
	}
	return wifi_strength;
}

int get_wifi_signal()
{
	int ret=-1;
	int flag=0;
	int index=0;
	int wifi_strength=0;
	char buf[128];
	char order[]="aps";
//	char order[]="cat aa.txt";
	char name_wifi[]="211";
	FILE *fp_t;
	
//	create_wifi_signal_txt(order);
	fp_t=popen(order,"r");
	if (!fp_t)
	{
        return -1;
    }
/*	if(fp_t==NULL)
	{
		printf("open file false\n");
		return ret;
	}
*/
//	while(!feof(fp_t))
	while(fgets(buf, sizeof(buf),fp_t) != NULL)
	{
	//	memset(buf,0,sizeof(buf));
	//	fgets(buf,sizeof(buf),fp_t);
		if(flag==0)
		{
			index=get_index(buf);
			if(index>0)
			{
				flag=1;
			}
		}
		else
		{
			wifi_strength=get_wifi_strength(buf,name_wifi,index);
			if(wifi_strength>0)
			{
				printf("%s wifi strength is %d\n",name_wifi,wifi_strength);
				return wifi_strength;
			}
		}
		memset(buf,0,sizeof(buf));
	}
	pclose(fp_t); //关闭管道
	return ret;
}

/*
int main(int argc,char **argv)
{
	int wifi_strength=0;
	wifi_strength=get_wifi_signal();
	if(wifi_strength>0)
	{
		printf("sucess\n");
	}
}
*/
