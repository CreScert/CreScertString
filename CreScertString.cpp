//�ַ����Ĵ���ͳһ�����з������ܷ����ַ��������β��д��ݡ�
//��ȡ���ļ�ĩβʱ�������ַ���ΪNULL���жϣ��Է���Ϊ-1���жϡ�
#include "CreScertString.h"

int Length;
DWORD Return;


//���ļ�,���handleΪ0����ôfilename�ʹ��ļ�������handle��Ϊ0�����ô��ļ�
HANDLE CreScertOpenFile(char *FileName,HANDLE Handle,int MemAllocLength)
{

	//��Ҫ���ļ�
	if(Handle==0) 
	{ 
		Handle=CreateFile(FileName,GENERIC_WRITE|GENERIC_READ,0,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);	

		if(Handle==INVALID_HANDLE_VALUE)
		{	
			MessageBox(NULL,"Create Faile","Tip",MB_OK);
			return 0;
		}

	}
	//�ƶ�ָ�뵽����ȡ�ļ���С
	Length=SetFilePointer(Handle,0,0,FILE_END);

	if(Length==-1||Length>=20000)
	{
		char *ErrorCode=(char *)malloc(25);

		sprintf(ErrorCode,"���ȴ��� ����:%d",GetLastError());

		MessageBox(NULL,ErrorCode,"ERROR",MB_OK);
	}
	//�ָ�ָ���ڳ�ʼλ��
	SetFilePointer(Handle,0,0,FILE_BEGIN);


	return Handle;
}

//������к������ⲿ����
//���ص��Ƕ�ȡÿ�еĳ���
int GetLineString(char *FileName,HANDLE Handle,int Point,int MemAllocLength,char *LineString)
{

	int i;
	//�������Handle�Ƿ����0���費��Ҫ�ر��ļ����
	int Flag=0;

	/******************************************************************************/
	//�ر�ע��  ���������������� ������ָ��������󡣡�
	//char *LineString=(char *)malloc(300);
	ZeroMemory(LineString,100);

	//handleΪ0������Ҫ�ر��ļ������flag��һ
	if((int)Handle==0)
		Flag=1;
	//����ֵ��Ϊhandle����
	Handle=CreScertOpenFile(FileName,Handle,MemAllocLength);

	HGLOBAL hMem;

 
	if(MemAllocLength==0)
	{			
		//�����ڴ�
		hMem=GlobalAlloc(GMEM_MOVEABLE,Length);
	}
	else 	
	{
		hMem=GlobalAlloc(GMEM_MOVEABLE,MemAllocLength);
	}

	if(hMem==NULL)
	{
		char *ErrorCode=(char *)malloc(25);

		sprintf(ErrorCode,"�ڴ�ռ����ʧ�� ����:%d",GetLastError());

		MessageBox(NULL,ErrorCode,"ERROR",MB_OK);
		return 0;
	}


	//�����ڴ棬���õ�ָ���ַ
	char *String=(char *)GlobalLock(hMem);

	//���㣬��С��������ڴ�Ĵ�С
	memset(String,0,GlobalSize(hMem));

	if(String==NULL)
	{	//�����ڴ�����

		MessageBox(NULL,"�ڴ�����ʧ��","ERROR",MB_OK);

		return 0;
	}

	//��ȡ�ļ������ļ����浽ָ��
	ReadFile(Handle,String,Length,&Return,NULL);
	//*(String+Length)='\0';

	//	MessageBox(NULL,String,":d",MB_OK);
	for(i=0;*(String+Point)!=10&&Point<Length;Point++,i++)
	{
		*(LineString+i)=*(String+Point);
		//		printf("\n%d",*(LineString+i));
		if(Point>Length||*(String+Point)==0)
			break;
	}

	*(LineString+i)='\0';

	//�����ڴ�����
	GlobalUnlock(hMem);
	GlobalFree(hMem);


	//�ر��ļ����
	if(Flag==1)
	{	
		CloseHandle(Handle);
	}

	if(Point>Length) 
		return -1;

	else 
		return strlen(LineString);

}

//�����һ���ַ����ĺ������ⲿ����
//���ض�ȡÿ�еĳ���
int GetPastLineString(char *FileName,HANDLE Handle,int Point,int MemAllocLength,char * LineString)
{
	if(LineString==NULL)return -1;
	//�������Handle�Ƿ����0���費��Ҫ�ر��ļ����
	int Flag=0;

	//	char *LineString=(char *)malloc(100);
	//memset(LineString,0,100);
	ZeroMemory(LineString,sizeof(100));

	//handleΪ0������Ҫ�ر��ļ������flag��һ
	if((int)Handle==0)
		Flag=1;

	//����ֵ��Ϊhandle����
	Handle=CreScertOpenFile(FileName,Handle,MemAllocLength);

	HGLOBAL hMem;

	if(MemAllocLength==0)
	{			
		//�����ڴ�
		hMem=GlobalAlloc(GMEM_MOVEABLE,Length);
	}
	else 	
	{
		hMem=GlobalAlloc(GMEM_MOVEABLE,MemAllocLength);
	}


	if(hMem==NULL)
	{
		MessageBox(NULL,"�ڴ�ռ����ʧ��","ERROR",MB_OK);
		return 0;
	}

	//�����ڴ棬���õ�ָ���ַ
	char *String=(char *)GlobalLock(hMem);

	if(String==NULL)
	{	
		MessageBox(NULL,"�ڴ�����ʧ��","ERROR",MB_OK);
		return 0;
	}

	//��ȡ�ļ������ļ����浽ָ��
	ReadFile(Handle,String,Length,&Return,NULL);
	//*(String+Length)='\0';

	//��ǰ����Ѱ�һ��з���
	for(;(*(String+Point)!='\r')&&Point>=0;)
	{
		if(((*(String+Point-1)=='\r')&&(*(String+Point-2)=='\r'))||((*(String+Point-1)=='\r')&&(*(String+Point-3)=='\r')))
			Point--;
		if(((*(String+Point-1)=='\n')&&(*(String+Point-2)=='\n'))||((*(String+Point-1)=='\n')&&(*(String+Point-3)=='\n')))
			Point--;
		Point--;
	}

	//�������з�
	Point--;

	//�ٴ���ǰѰ�һ��з��ţ��ҵ���+2�������з��ſ�ʼ�õ���һ�е�����
	for(;(*(String+Point)!='\r')&&Point>=0;)	
		Point--;

	//����ļ�λ��Ϊ���ţ�����
	//�����������з���
	if(Point<=0)
		Point=0;
	else
		Point+=2;		//�������з���

	//�����ڴ�����
	GlobalUnlock(hMem);
	GlobalFree(hMem);

	//��ȡ��һ�е����ݣ��������һ��ֻ�л��з������ȡ��һ�з���

	//LineString=GetLineString(FileName,Handle,Point,0);

	//Point+=strlen(LineString);

	Point+=GetLineString(FileName,Handle,Point,0,LineString);
	//��ӻ��з���
	*(LineString+strlen(LineString))='\0';

	//�ر��ļ����
	if(Flag==1)
	{	
		CloseHandle(Handle);
	}

	return strlen(LineString);

}
//ʵ�ֵ����ѯ
int StringContainEx(char *ContainString,char *NewString,int Number,BOOL Sort)
{
	//�ȼ��������ַ����ĳ��ȣ�����Ҫ��һ
	int Length=strlen(ContainString)-1;
	int NewLength=strlen(NewString)-1;

	//�����ַ��������⿽������������Ŀ������Ϊ��ʵ��������Ĳ����ַ������������ַ�����������const�ģ������޸��ַ�������
	char *tempContainString=(char *)malloc(Length+1);
	char *tempNewString=(char *)malloc(NewLength+1);

	strcpy(tempContainString,ContainString);
	strcpy(tempNewString,NewString);

	//�ߵ��ַ�����Ϊ����߷�0Ϊ����
	if(!Sort)
	{
		//��ʱ������ַ�
		char TempString;

		//���������ַ��ߵ�
		//���������ַ���
		for(int j=0;j<=Length/2;j++)
		{
			TempString=*(tempContainString+j);
			*(tempContainString+j)=*(tempContainString+Length-j);
			*(tempContainString+Length-j)=TempString;
		}
		//Ҫ���İ������ַ���
		for(int j=0;j<=NewLength/2;j++)
		{
			TempString=*(tempNewString+j);
			*(tempNewString+j)=*(tempNewString+NewLength-j);
			*(tempNewString+NewLength-j)=TempString;
		}	
	}

	//�������
	int OrderSortSum=StringContain(tempContainString,tempNewString,Number);

	//�����FALSE,Ҳ���ǵ��򣬽�����ֵ�ļӼ�������ȷ
	if(!Sort)
	{
		if(OrderSortSum==-1)return -1;
		OrderSortSum=Length-OrderSortSum-NewLength;

	}

	return OrderSortSum;

}
//�����ַ��ж�,����Number�ǵڼ��������жϵ��ַ�
//�����ַ�		NewString
//Ҫ�����ַ�	ContainString
int StringContain(char *ContainString,char *NewString,int Number)
{

	//Ҫ�����ַ��±�
	int Sub=0;
	//�����ַ�
	int NewSub=0;

	//��ȴ���
	int EquNum=0;

	//������±�
	int SaveSub=0;

	//�Ȼ�ȡ�������ַ���С���������Ҫ�����Ĵ�С��ʧ�ܷ���-1
	int nLength=strlen(NewString);
	int nContainStringLength=strlen(ContainString);

	if(nLength>nContainStringLength)
		return -1;


	//��һ���ж��Ƕ�Number���ж�
	for(int m_Number=0;m_Number<=Number&&Sub<=nContainStringLength;)
	{
		//�ж��ַ��Ƿ�һ��
		for(;Sub<=nContainStringLength;)
		{
			//��������ַ����
			if(*(ContainString+Sub)==*(NewString+NewSub))
			{
				EquNum++;
				NewSub++;
				if(EquNum==1)
					SaveSub=Sub;
				Sub++;
			}
			else
			{
				NewSub=0;
				EquNum=0;
				SaveSub++;
				Sub=SaveSub;
			}
			//�����ȴ������ڰ����ַ��Ĵ�С���������ȫ���
			if(EquNum==nLength)
			{
				m_Number++;

				break;
			}
		}
		if(m_Number>=Number)
		{
			break;
		}
		else
		{
			NewSub=0;
			EquNum=0;
			SaveSub++;
			Sub=SaveSub; 	
		}
	}

	if(EquNum!=nLength) return -1;
	return SaveSub;
}

//��ȫ�Ƚϣ��Ƚ�ָ�����ַ����Ƿ�����ָ��λ�ã����ƥ�䷵��TRUE,��ƥ�䷵��FALSE
BOOL StringCmpOfStart(char *ContainString,char *NewString,int Start,int End ,BOOL Sort)
{
	int StartNow=Start;
	//ƥ��Ŀ�ʼλ�÷���
	if(StartNow==StringContain(ContainString,NewString,0))
	{
		for(int i=0;i<strlen(NewString);i++,StartNow++)
		{
			if(*(ContainString+StartNow)!=*(NewString+i))
				return FALSE;
		}
		if(End==-1)
		{
			if(StartNow!=strlen(NewString)+Start)
				return FALSE;
		}
		else
		{
			if(StartNow!=End)
				return FALSE;
		}
		return TRUE;
	}

	else
		return FALSE;
	return FALSE;
}
//�������ݣ���start��end�����ݣ����endΪ-1��������ĩβ,IdentifierΪ��ʶ������˼��������ʲô�ַ������������ո񣬻��н���
//����ֵ-1����ʧ��
//DesStringΪ�ͳ��ַ�
int StringCopy(char *DesString,char *ScrString,int Start,int End,char Identifier)
{

	if(ScrString==NULL)
	{
		ZeroMemory(DesString,End-Start);
		return 0;
	}
	//���EndΪ-1������������β
	if(-1==End)
	{
		End=strlen(ScrString);
	}

	//��ʼ��������
	for(int iStart=0;Start<=End;iStart++,Start++)
	{
		*(DesString+iStart)=*(ScrString+Start);
		*(DesString+iStart+1)='\0';
		if(Identifier!=NULL)
		{
			if(*(ScrString+Start)==Identifier)
			{
				break;		
			}
		}
	}

	return 0;
}
//�ض��ַ���
char *StringCat(char *String,int Start,int End,BOOL IsCopy)
{
	char *TempString=(char *)malloc(strlen(String));

	End++;
	//�����ַ������ȱ���סԭ�����ַ���
	strcpy(TempString,String);

	for(;*(String+Start)!=0;Start++,End++)
	{
		*(TempString+Start)=*(String+End);
	}

	*(TempString+Start)='\0';

	if(!IsCopy)
		strcpy(String,TempString);

	return TempString;

}

//���ַ���ת��������
int StringToSum(char *String)
{
	if(String==0)
		return 0;
	//��ȡ�ַ����Ĵ�С
	int Length=strlen(String);
	//���������
	int Sum=0;

	//�Ƚ���һ���ַ�����Sum
	Sum=(*(String+0)-48);

	//���ַ�������߿�ʼ����10�Դ���������
	//1253
	//((1*10+2)*10+5)*10+3=1253
	for(int i=0;i<Length-1;i++)
	{
		Sum*=10;
		Sum+=(*(String+i+1)-48);
	}

	//���ؼ���õ�����
	return Sum;
}

//�����ƶ�λ��
char * StringReplace(char *DesString,char *SrcString,long Start,long End/*,char *ReturnStr*/)
{
	//�ڴ����ʹ�öѷ���API

	//����洢����ַ���ָ��
	char *LeftString = (char *)HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,Start+1);

	//����洢����ַ���ָ��
	char *ReturnStr = (char *)HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,100);

	//����洢�ұ��ַ���ָ��
	char *RightString = (char *)HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,strlen(SrcString)-End+1);
	
	//�����0����ʼ�滻���������κζ���
	if(Start ==0){
		strncpy(LeftString,SrcString,Start);
	}else{
		//������벿�ֵ��ַ���
		StringCopy(LeftString,SrcString,0,Start-1,0);
	}

	if(End == -1){
		strcpy(RightString,"");
	}else
	//�����Ұ벿�ֵ��ַ���
	StringCopy(RightString,SrcString,End+1,-1,0);

	//������ߵ��ַ���
	strcpy(ReturnStr,LeftString);
	//׷���м���ַ���
	strcat(ReturnStr,DesString);
	//׷���ұߵ��ַ���
	strcat(ReturnStr,RightString);
	//׷�ӽ�����
	strcat(ReturnStr,"\0");
	
	//�ͷ��ڴ�
	HeapFree(GetProcessHeap(),0,LeftString);
	HeapFree(GetProcessHeap(),0,RightString);
 
	//�����ַ������� 
	//return strlen(ReturnStr);
	if(Start ==0)
	{
		memset(SrcString,0,strlen(SrcString));
		strcpy(SrcString,ReturnStr);
	}
	strcpy(SrcString,ReturnStr);
	return ReturnStr;
}

//������ת�����ַ���
char *SumToString(int Time)
{
	//�ߵ����ַ���
	//char *String=(char *)malloc(10);
	char *String = (char *)HeapAlloc(GetProcessHeap(),0,10);
	int i;
	//����
	//memset(String,0,sizeof(char));
	ZeroMemory(String,sizeof(String));
	if(Time==0)
	{
		sprintf(String,"0\0");
		return String;
	}
	//����ȡ�������ߵ�����
	for(i=0;Time>0;i++)
	{
		*(String+i)=((Time%10)+48);
		Time/=10;
	}
	//�ַ�����β
	*(String+i)='\0';

	//��һ�������˽�β��
	i--;

	//��ʱ������ַ�
	char TempString;

	//���������ַ��ߵ�
	for(int j=0;j<=i/2;j++)
	{
		TempString=*(String+j);
		*(String+j)=*(String+i-j);
		*(String+i-j)=TempString;
	}

	//�����ַ���
	return String;
}
//���ļ��в����ַ���
int InsertStringToFile(char *FileName,HANDLE Handle,int Point,char *WriteString,int MemAllocLength)
{

	int Flag=0;

	if(Handle==0)
	{
		Flag=1;
	}
	//���ļ�
	Handle=CreScertOpenFile(FileName,Handle,MemAllocLength); 

	HGLOBAL hMem;

	if(MemAllocLength==0)
	{			
		//�����ڴ�
		hMem=GlobalAlloc(GMEM_MOVEABLE,Length);
	}
	else 	
	{
		hMem=GlobalAlloc(GMEM_MOVEABLE,MemAllocLength);
	}
	if(hMem==NULL)
	{
		MessageBox(NULL,"�ڴ�ռ����ʧ��","ERROR",MB_OK);
		return -1;
	}

	//�Ȼ�ȡ�ַ����Ĵ�С
	int StringLength=strlen(WriteString);

	//�����ڴ棬���õ�ָ���ַ
	char *String=(char *)GlobalLock(hMem);

	memset(String,0,Length);
	if(String==NULL)
	{
		MessageBox(NULL,"�ڴ�����ʧ��","ERROR",MB_OK);
		return -1;
	}

	//����ָ��λ���ڲ����ַ�����λ��
	SetFilePointer(Handle,Point,0,FILE_BEGIN);

	//��ȡ����������ַ���λ�õĺ��浽�ļ���β�����ݲ�����
	ReadFile(Handle,String,Length-Point,&Return,0);

	//�ٰ�ָ��λ�õ�����
	SetFilePointer(Handle,Point,0,FILE_BEGIN);


	//д�������ַ���
	WriteFile(Handle,WriteString,StringLength,&Return,0);

	//д����ǰ����Ĳ����ַ�������β���ַ���
	//���п��ܳ�������
	//WriteFile(Handle,String,Length-Point,&Return,0);
	WriteFile(Handle,String,strlen(String),&Return,0);
	//	FlushFileBuffers(Handle);

	//�����ڴ�����
	GlobalUnlock(hMem);
	GlobalFree(hMem);


	//�ر��ļ����
	if(Flag==1)
	{	
		CloseHandle(Handle);
	}

	return 0;
}

//ɾ���ļ����ַ�
int DeleteStringToFile(char *FileName,HANDLE Handle,int Point,int DeleteLength,int MemAllocLength)
{
	int Flag=0;

	if(Handle==0)
	{
		Flag=1;
	}
	//���ļ�
	Handle=CreScertOpenFile(FileName,Handle,MemAllocLength); 
	HGLOBAL hMem;

	if(MemAllocLength==0)
	{			
		//�����ڴ�
		hMem=GlobalAlloc(GMEM_MOVEABLE,Length);
	}
	else 	
	{
		hMem=GlobalAlloc(GMEM_MOVEABLE,MemAllocLength);
	}
	if(hMem==NULL)
	{
		MessageBox(NULL,"�ڴ�ռ����ʧ��","ERROR",MB_OK);
		return 0;
	}

	//�����ڴ棬���õ�ָ���ַ
	char *String=(char *)GlobalLock(hMem);

	if(String==NULL)
	{
		MessageBox(NULL,"�ڴ�����ʧ��","ERROR",MB_OK);
		return -1;
	}

	//����ָ��λ���ڽ�Ҫɾ�����ַ�����λ��
	SetFilePointer(Handle,Point+DeleteLength,0,FILE_BEGIN);


	//��ȡ����ɾ�����ַ���λ�õĺ��浽�ļ���β�����ݲ�����
	ReadFile(Handle,String,Length-Point-DeleteLength,&Return,0);


	//�ٰ�ָ��λ�õ�����
	SetFilePointer(Handle,Point,0,FILE_BEGIN);

	//д����ǰ�����ɾ���ַ�������β���ַ���
	WriteFile(Handle,String,Length-Point-DeleteLength,&Return,0);

	SetEndOfFile(Handle);

	//�����ڴ�����
	GlobalUnlock(hMem);
	GlobalFree(hMem);

	//�ر��ļ����
	if(Flag==1)
	{	
		CloseHandle(Handle);
	}

	return 0;
}

//��ȡIni�ַ���
char  *GetIniString(char *IniFilePath,HANDLE Handle,char *Section,char *Key,int *Pointer,int FilePoint,int FilePointEnd,BOOL Repeat,int RepeatSum)
{


	int Point=FilePoint;//�ļ�ָ�룬�β���ָ���ʾ������÷���

	int Equal=0;//����ȡ�ļ������ַ����Ĵ���ʱ�õ�
	char *SectionTempString=(char *)malloc(100);//�������ַ���
	char *String=(char*)calloc(MAX_LINE,sizeof(char ));//��ȡ��ÿһ�е�����
	int RepeatSum_Now=0;
	BOOL IsCloseHandle=FALSE;
	if((int)Handle==0)
	{
		Handle=CreScertOpenFile(IniFilePath,0,0);
		IsCloseHandle=TRUE;
	}


	if(FilePointEnd==-1)
	{

		FilePointEnd=SetFilePointer(Handle,0,0,FILE_END);
		if(FilePointEnd==-1)
		{
			char *ErrorCode=(char *)malloc(25);

			sprintf(ErrorCode,"���ȴ��� ����:%d",GetLastError());

			MessageBox(NULL,ErrorCode,"ERROR",MB_OK);
		}
	}
	SetFilePointer(Handle,0,0,FILE_BEGIN);
	while(1){


		//��ȡÿ�е�����,�õ��õ����ַ����ĳ���
		int LineLength = GetLineString(IniFilePath,Handle,Point,0 ,String);



		//�����ļ���ĩβ
		if(String==0)
		{	
			if(IsCloseHandle) CloseHandle(Handle);
			free(String);
			String=NULL;

			free(SectionTempString);

			SectionTempString=NULL;

			return 0;
		}




		//�޶��й����е��ַ�����Ļ��з�
		if(*(String+LineLength-1)==13)
			*(String+LineLength-1)='\0';

		//�ַ���Ϊ ����
		if(*(String+0)=='['&&*(String+LineLength-2)==']')
		{


			//�����жϽ����Ƿ�һ�£��������ڵ��ַ���ֱ�Ӹ�ֵ�ж�
			//��ȡ������һ�е��������ݣ���������
			strcpy(SectionTempString,String);
			if(LineLength-2>strlen(SectionTempString)){
				system("pause");
			}
			//ȥ��β����
			*(SectionTempString+LineLength-2)='\0';
			//ȥ��ͷ����
			strcpy(SectionTempString,SectionTempString+1);


			//�Ƚ��Ƿ��ǽ���һ�£���һ�������ѭ����һֱ�����ж�
			if(!strcmp(Section,SectionTempString))
			{

				//����Ϊ�㣬��ʾֻ�ǻ�ȡ������λ��
				if(Key==0)
				{
					//�Ȱ��ļ���ָ�����ӵ���һ��
					Point+=strlen(String)+1;

					//������λ��
					*(Pointer)=Point;

					free(String);
					String=NULL;

					free(SectionTempString);
					SectionTempString=NULL;
					if(IsCloseHandle) CloseHandle(Handle);
					return Section;
				}
				//�ڽ�����������������ַ�
				while(1)
				{

					//�Ȱ��ļ���ָ�����ӵ���һ��
					Point+=strlen(String)+1;
					//�����һ�е��ַ������ж��ַ����ĳ��ȣ���������ַ��������һ���ַ��Ƿ�Ϊ"]"
					LineLength+=GetLineString(IniFilePath,Handle,Point,0,String);

					//�����ļ���ĩβ
					if(String==0)
					{
						free(String);
						String=NULL;

						free(SectionTempString);
						SectionTempString=NULL;
						if(IsCloseHandle) CloseHandle(Handle);
						return 0;
					}
					//�ж��ַ����ĳ��ȣ���������ַ��������һ���ַ��Ƿ�Ϊ"]"
					//LineLength=strlen(String);

					//�Լ����ƣ�Ϊʲô�������žͺ���
					//��⵽�µĽ�������ͷ����

					if(*(String+0)=='[') 				
					{
						if(LineLength-1>strlen(String))
							break;
						if(*(String+LineLength-1)==']')
							break;
					}
					//������⣬�ض�"="ǰ�ַ���
					Equal=StringContain(String,"=",1);

					char *Value=(char*)malloc(200);
					if(Value==NULL)
					{
						if(IsCloseHandle) CloseHandle(Handle);
						MessageBox(NULL,"�����ڴ����",0,MB_OK);
					}
					//����δ�ضϵļ�����
					strcpy(Value,String);

					if(Equal>=0)
						//�ضϼ����У���ȡ����
						*(String+Equal)='\0';

					//�Ƚϼ����Ƿ�����Ҫ�ļ���
					if(!strcmp(String,Key))
					{
						if(Repeat) RepeatSum_Now++;
						else	RepeatSum_Now=RepeatSum;
						if(RepeatSum==RepeatSum_Now)
						{
							//�ضϼ������ַ�������ȡ��ֵ
							Value+=Equal+1;

							if(*(Value+strlen(Value)-1)==13)
								*(Value+strlen(Value)-1)='\0';

							if(Pointer!=0)
								*(Pointer)=Point+Equal;

							//��β������������free�ͷţ����ؼ�ֵ
							free(String);
							String=NULL;

							free(SectionTempString);
							SectionTempString=NULL;

							if(IsCloseHandle) CloseHandle(Handle);
							return Value;
						}
					}
					free(Value);
					Value=NULL;
					if(FilePointEnd<Point)
					{
						break;
					}
				}
			}
			else
			{ 
				//�Ȱ��ļ���ָ�����ӵ���һ��
				Point+=strlen(String)+1;
				continue;
			}

		}
		if(FilePointEnd<Point)
		{
			break;
		}
		if(String!=0)
			//ָ��λ�ü�һ
			Point+=strlen(String)+1;

	}
	if(IsCloseHandle) CloseHandle(Handle);
	free(SectionTempString);
	SectionTempString=NULL;
	free(String);
	String=NULL;

	return 0;
}
//���Iniֵ
//IniFilePath����Ϊ�ļ���λ��
//SecTionΪ����
//KeyΪ����
//����ֵΪ��ֵ
char *GetIniValue(char *IniFilePath,char *Section,char *Key)
{

	return GetIniString(IniFilePath,0,Section,Key,0,0,-1,FALSE,0);

}
char *GetIniValueEx(char *IniFilePath,HANDLE Handle,char *Section,char *Key,int Pointer,int PointerEnd,BOOL Repeat,int RepeatSum)
{
	return GetIniString(IniFilePath,Handle,Section,Key,0,Pointer,PointerEnd,Repeat,RepeatSum);
}
char *SetIniValue(char *IniFilePath,char *Section,char *Key,char *NewValue)
{
	return SetIniValueEx(IniFilePath,0,Section,Key,NewValue,0,-1,FALSE,0);
}
//�޸�iniֵ
//NewValueΪ��Ҫ�޸ĵ��µ�ֵ
char *SetIniValueEx(char *IniFilePath,HANDLE Handle,char *Section,char *Key,char *NewValue,int PointerStart,int PointerEnd,BOOL Repeat,int RepeatSum)
{
	BOOL Flag=FALSE;
	if((int)Handle==0)
	{
		//���ļ�
		Handle=CreScertOpenFile(IniFilePath,0,0); 
		Flag=TRUE;
	}

	int Pointer;

	//��ȡIniֵ
	char *Value=GetIniString(IniFilePath,Handle,Section,Key,&Pointer,PointerStart,PointerEnd,Repeat,RepeatSum);

	//�����µļ�ֵ����
	int Length=strlen(NewValue);



	//�����µ��ļ�ָ��λ�ڼ�ֵ��ǰ��
	SetFilePointer(Handle,Pointer+1,0,FILE_BEGIN);


	//ɾ��ָ���ļ�ֵ��ò��ɾ�����ַ���δ�������з���
	DeleteStringToFile(IniFilePath,Handle,Pointer+1,strlen(Value),0);

	//�ٴ������µ��ļ�ָ��λ�ã�����д���ֵ
	SetFilePointer(Handle,Pointer+1,0,FILE_BEGIN);

	//д���ֵ
	InsertStringToFile(IniFilePath,Handle,Pointer+1,NewValue,0);

	//�ر��ļ�����˳�
	if(Flag)
		CloseHandle(Handle);

	return 0;
}
//��ȡ��һ�е�λ��
int GetNextLinePointer(char *IniFilePath,HANDLE Handle,int Pointer)
{
	int CloseBool=FALSE;

	//���Ϊ�㣬�򿪾��
	if(Handle==0)
	{
		Handle=CreScertOpenFile(IniFilePath,0,0);
		CloseBool=TRUE;
	}

	SetFilePointer(Handle,Pointer,0,FILE_BEGIN);
	char * LineString = (char *)calloc(MAX_LINE,sizeof(char));
	int StringLineLength=GetLineString(IniFilePath,Handle,Pointer,0,LineString);

	if(CloseBool)
	{
		CloseHandle(Handle);
	}
	Pointer+=StringLineLength;

	free(LineString);
	LineString=NULL;
	return Pointer+1;
} 

int AddIniValue(char *IniFilePath,char *Section,char *NewKey,char *NewValue)
{
	return AddIniValueEx(IniFilePath,0,Section,NewKey,NewValue,0,-1,FALSE,0);
}
//����Iniֵ
int AddIniValueEx(char *IniFilePath,HANDLE Handle,char *Section,char *NewKey,char *NewValue,int IniPointer,int IniPointerEnd,BOOL Repeat,int RepeatSum)
{
	BOOL IsCloseHandle=FALSE;
	if(Handle==0)
	{
		Handle=CreScertOpenFile(IniFilePath,0,0);
		IsCloseHandle=TRUE;
	}
	int Pointer;
	int NewIniPointer=IniPointer;
	//�Ȳ��ҽ����Ƿ���ڣ������ڴ�������
	if(0==GetIniString(IniFilePath,Handle,Section,0,&Pointer,IniPointer,IniPointerEnd,Repeat,RepeatSum)/*||IniPointer!=0*/)
	{

		if(IniPointer==0)
		{
			//����д��λ��
			int FileLength=SetFilePointer(Handle,0,0,FILE_END);

			//������ȡǰ�����ַ����ж��Ƿ��ǻ��з���
			char *TempReadEntry=(char *)malloc(3);

			//����
			ZeroMemory(TempReadEntry,3);
			//��ǰ�ƽ������ַ�
			SetFilePointer(Handle,FileLength-2,0,FILE_BEGIN);

			//��ȡ
			ReadFile(Handle,TempReadEntry,2,&Return,0);

			//���β����ʶ��
			*(TempReadEntry+2)='\0';

			//û�л��з��ţ�����ϻ��з���Ȼ����һ��
			if(strcmp(TempReadEntry,"\r\n"))
			{
				WriteFile(Handle,"\r\n",2,&Return,0);
			}

			if(TempReadEntry!=NULL)
			{
				free(TempReadEntry);
				TempReadEntry=NULL;
			}

			//д��[..]��ʽ���������ϻ��з���
			WriteFile(Handle,"[",1,&Return,0);
			WriteFile(Handle,Section,strlen(Section),&Return,0);
			WriteFile(Handle,"]\r\n",3,&Return,0);

		}
		else
		{
			NewIniPointer=GetNextLinePointer(IniFilePath,Handle,NewIniPointer);
			//����д��λ��
			SetFilePointer(Handle,NewIniPointer,0,FILE_BEGIN);
			InsertStringToFile(IniFilePath,Handle,NewIniPointer++,"[",0);
			InsertStringToFile(IniFilePath,Handle,NewIniPointer,Section,0);
			NewIniPointer+=strlen(Section);
			InsertStringToFile(IniFilePath,Handle,NewIniPointer++,"]",0);
			InsertStringToFile(IniFilePath,Handle,NewIniPointer,"\r\n",0);

		}

	}

	//�������Ϊ0����ʾֻ����ӽ��������˳�
	if(NewKey==0)
	{	if(Handle)CloseHandle(Handle);
	return 0;
	}

	//���ʵ��Ϊ��ʱ����������ˣ�����Ϊ�˷�ֹ��һ���⡣
	if(NewValue==0)
	{
		NewValue=(char *)malloc(2);
		strcpy(NewValue,"0\0");
	}
	//�Ȼ�ȡ�Ƿ�������ڣ�������ڣ����ü�ֵ���˳�����������ڣ��½������ֵ
	char *Value=GetIniString(IniFilePath,Handle,Section,NewKey,&Pointer,IniPointer,IniPointerEnd,Repeat,RepeatSum);

	//�������ڣ����ü�ֵ
	if(Value!=0)
	{

		SetIniValueEx(IniFilePath,Handle,Section,NewKey,NewValue,IniPointer,IniPointerEnd,Repeat,RepeatSum);
		if(IsCloseHandle)
			CloseHandle(Handle);
		return 0;
	}

	//�Ȼ�ȡ������λ�ã�ֱ����������Ӷ���
	GetIniString(IniFilePath,Handle,Section,0,&Pointer,IniPointer,IniPointerEnd,Repeat,RepeatSum);

	//����д��λ��
	SetFilePointer(Handle,Pointer+1,0,FILE_BEGIN);

	//�½�д���ַ���
	char *m_NewValue=(char *)calloc(strlen(NewKey)+strlen(NewValue)+20,sizeof(char));

	//д����������ںš���ֵ�����з�
	/*strcpy(m_NewValue,NewKey);
	strcat(m_NewValue,"=");
	strcat(m_NewValue,NewValue);
	strcat(m_NewValue,"\r\n");
	*/
	sprintf(m_NewValue,"%s=%s\r\n",NewKey,NewValue);
	//�����ַ���
	InsertStringToFile(0,Handle,Pointer+1,m_NewValue,0);

	m_NewValue=NULL;

	if(IsCloseHandle)
		CloseHandle(Handle);

	return 0;
}

//ɾ��Iniֵ
int DeleteIniValue(char *IniFilePath,char *Section,char *Key)
{
	return DeleteIniValueEx(IniFilePath,0,Section,Key,0,-1,FALSE,0);
}

int DeleteIniValueEx(char *IniFilePath,HANDLE Handle,char *Section,char *Key,int IniPointer,int IniPointerEnd,BOOL Repeat,int RepeatSum)
{
	BOOL IsCloseHandle=FALSE;
	if(Handle==0)
	{
		Handle=CreScertOpenFile(IniFilePath,0,0);
		IsCloseHandle=TRUE;
	}

	int Pointer;

	//key�����㣬��ζɾ�������Լ�֮�����еļ�ֵ
	if(Key==0)
	{
		char *DelAllKey=(char *)malloc(50);

		GetIniString(IniFilePath,Handle,Section,0,&Pointer,IniPointer,IniPointerEnd,Repeat,RepeatSum);

		//û���ҵ�ָ���Ľ���
		if(0==Pointer)
		{	
			if((int)Handle!=0)
				CloseHandle(Handle);
			return -1;
		}

		//�ҵ�ָ���Ľ�������ʱPointerΪ����֮���λ�ã���ʱ��Ҫɾ����֮ǰ��λ��
		Pointer-=strlen(Section);

		//���ϻ��з��ŵ�λ��
		Pointer-=2;

		//ɾ��[ ]�ַ�
		Pointer-=2;

		//�õ���һ��
		DelAllKey=GetFirstIniValueEx(IniFilePath,Handle,Section,&Pointer,0,-1);


		while(DelAllKey!=0)
		{	
			Pointer-=DeleteIniValueEx(IniFilePath,Handle,Section,DelAllKey,IniPointer,IniPointerEnd,FALSE,0);

			DelAllKey=GetNextIniValue(IniFilePath,Handle,&Pointer);
		}


		GetIniString(IniFilePath,Handle,Section,0,&Pointer,IniPointer,IniPointerEnd,FALSE,0);

		//�ҵ�ָ���Ľ�������ʱPointerΪ����֮���λ�ã���ʱ��Ҫɾ����֮ǰ��λ��
		Pointer-=strlen(Section);

		//���ϻ��з��ŵ�λ��
		Pointer--;

		//ɾ��[ ]�ַ�
		Pointer-=2;

		DeleteStringToFile(IniFilePath,Handle,Pointer,strlen(Section)+2+2,0);

		free(DelAllKey);
		DelAllKey=NULL;

		if(IsCloseHandle)
			CloseHandle(Handle);
		return 0;


	}
	//��ȡ������������������˳�������ɾ��
	char *Value=GetIniString(IniFilePath,Handle,Section,Key,&Pointer,IniPointer,IniPointerEnd,Repeat,RepeatSum);

	//���������ڣ��˳�
	if(Value==0)
	{		
		if(IsCloseHandle)
			CloseHandle(Handle);
		Value=NULL;
		return 0;
	}

	//��ȡ��ֵ���ȣ��������Ļ��з�
	int ValueLength=strlen(Value)+2;

	//ָ��ɾ���ַ����ĳ��Ȳ���
	int StringLength=0;

	//��ȡ��������
	StringLength+=strlen(Key);
	//���õȺų���
	StringLength++;


	//���ļ�ָ���˺󵽼�����һ�п�ͷ
	Pointer-=StringLength;
	SetFilePointer(Handle,Pointer+1,0,FILE_BEGIN);

	//ָ��ɾ���ַ����ĳ��ȣ���Ҫ���ϼ�ֵ���ַ�������
	StringLength+=ValueLength;

	//ɾ����ֵ��
	DeleteStringToFile(IniFilePath,Handle,Pointer+1,StringLength,0);

	//�رվ���˳�
	if(IsCloseHandle)
		CloseHandle(Handle);

	return StringLength;
}

//���ص�һ������������˼���ǽ���������������ֵ�ȷ���һ���ṹ���У�������û����Ҫ�ͷ����ˡ�
//IniFilePath:�ļ���
//SecTion:����
//Point:λ��
char *GetFirstIniValueEx(char *IniFilePath,HANDLE Handle,char *Section,int *Point,int IniPointer,int IniPointerEnd)
{
	//�ҵ�������λ��
	GetIniString(IniFilePath,Handle,Section,0,Point,IniPointer,IniPointerEnd,FALSE,0);

	return 	GetNextIniValue(IniFilePath,Handle,Point);
}
char *GetFirstIniValue(char *IniFilePath,char *Section,int *Point)
{
	//�ҵ�������λ��
	GetIniString(IniFilePath,0,Section,0,Point,0,-1,FALSE,0);

	return 	GetNextIniValue(IniFilePath,0,Point);
}

//��ȡ��һ������
char *GetNextIniValue(char *IniFilePath,HANDLE Handle,int *Point)
{
	BOOL IsCloseHandle=FALSE;
	if(Handle==0)
	{
		Handle=CreScertOpenFile(IniFilePath,0,0);
		IsCloseHandle=TRUE;
	}
	//ÿһ�е�����
	char *LineString=(char *)malloc(50);

	int Equal=0;

	do
	{
		//��ȡ�������õ�λ�ã�//�߹�����
		*(Point)+=GetLineString(IniFilePath,Handle,*Point,0,LineString)+1;


		//�����ļ���ĩβ���˳�
		if(LineString==0) 
		{
			if(IsCloseHandle)
				CloseHandle(Handle);
			return 0;
		}

		//������µĽ������˳�ѭ��
		if(-1!=StringContain(LineString,"[",1)&&-1!=StringContain(LineString,"]",1))
		{	
			if(IsCloseHandle)///////���ã��ҵ�����������һ��˲���ʱ�䣬��ǰд����Handle��û�뵽����������⣬���Ե������������ʱ��ֱ�������ˣ�����
				CloseHandle(Handle);
			free(LineString);
			LineString=NULL;
			return 0;
		}




		//�鿴�Ƿ��е��ںţ�û��
		Equal=StringContain(LineString,"=",1);

		//�ҵ����ں����˳�ѭ��
		if(-1!=Equal)
			break;

		//ֱ���ļ���ĩβ
	}while(LineString!=0);

	if(IsCloseHandle)CloseHandle(Handle);
	//�����ļ���ĩβ���˳�
	if(LineString==0) return 0;

	//�ضϵ��ں��Լ���ֵ
	*(LineString+Equal)='\0';

	return LineString;
}

char *GetFileExe(char *FilePath)
{
	char *AllFilePath=FilePath;

	int Length=strlen(AllFilePath);
	for(;*(AllFilePath+Length)!='\\';Length--);
	AllFilePath+=++Length;


	return AllFilePath;
}

char *GetNextSection(char *IniFilePath,HANDLE Handle,int *Point)
{
	BOOL IsCloseHandle=FALSE;
	if(Handle==0)
	{
		Handle=CreScertOpenFile(IniFilePath,0,0);
		IsCloseHandle=TRUE;
	}
	//ÿһ�е�����
	char *LineString=(char *)malloc(50);

	int Equal=0;

	do
	{
		//��ȡ�������õ�λ��
		//�߹�����
		*(Point)+=GetLineString(IniFilePath,Handle,*Point,0,LineString)+1;


		//�����ļ���ĩβ���˳�
		if(LineString==0) 
		{
			if(IsCloseHandle)
				CloseHandle(Handle);
			return 0;
		}

		//������µĽ������˳�ѭ��
		if(-1!=StringContain(LineString,"[",1)&&-1!=StringContain(LineString,"]",1))
			break;




		//ֱ���ļ���ĩβ
	}while(*(Point)>Length);

	if(IsCloseHandle)CloseHandle(Handle);
	//�����ļ���ĩβ���˳�
	if(LineString==0) return 0;

	return LineString;
}

//������
int GetInternetStatusFlag(int StatusArrayFlag[],int Flag)
{
	int FlagTemp=Flag;//һֱ�޸ĵ�Flag

	int MoveLeft=0;//���ƶ��˶��٣���������ƶ�

	int TotalSum=0;//�����м�¼��ǵ�����

	int Sum=1;//��ǵ��±�
	//��¼����,ÿ����ɺ��������¼��㣬�õ���һ�εı��
	for(;Flag>0;)
	{
		//������������
		MoveLeft=0;
		//ÿ�μ�����
		for(FlagTemp=Flag;FlagTemp>0;)
		{
			//ÿ�������ƶ�һ����ֱ��Ϊ1
			FlagTemp>>=1;
			//�����ƶ��������ۼ�
			MoveLeft++;
			//�����ǵ���1���˳�ѭ����������һ�����
			if(FlagTemp<=1)
			{

				//�����ƶ������ƶ����������õ�����2��n�η���
				FlagTemp<<=MoveLeft;

				//���Ϊ��ֱ���˳�ѭ��
				if(FlagTemp==0) break;

				//������
				StatusArrayFlag[Sum++]=FlagTemp;

				//��Ǽ�¼�ۼӲ�����
				StatusArrayFlag[0]=++TotalSum;
				//����õ���һ��ѭ���ı��
				Flag-=FlagTemp;
				break;

			}

		}
		//��ǵ���1���˳�ѭ��
		if(Flag==1)
		{
			//��Ǽ�¼�ۼӲ�����
			StatusArrayFlag[Sum]=1;
			StatusArrayFlag[0]++;
			break;
		}

	}

	return 0;
}
//������Ļͼ��bmp
HBITMAP CopyScreenToBitmap(LPRECT lpRect) 
{
	HDC hScrDC, hMemDC;             // ��Ļ���ڴ��豸������  
	HBITMAP hBitmap, hOldBitmap;    // λͼ���  
	int nX, nY, nX2, nY2;           // ѡ����������  
	int nWidth, nHeight;            // λͼ��Ⱥ͸߶�  
	int xScrn, yScrn;               // ��Ļ�ֱ���  

	if (IsRectEmpty(lpRect))  
		return NULL;  

	hScrDC = CreateDC("DISPLAY", NULL, NULL, NULL);     // Ϊ��Ļ�����豸������  

	hMemDC = CreateCompatibleDC(hScrDC);                // Ϊ��Ļ�豸�����������ݵ��ڴ��豸������  

	nX = lpRect->left;  
	nY = lpRect->top;  
	nX2 = lpRect->right;  
	nY2 = lpRect->bottom;  

	xScrn = GetDeviceCaps(hScrDC, HORZRES); // �����Ļˮƽ�ֱ���  
	yScrn = GetDeviceCaps(hScrDC, VERTRES);  

	if (nX < 0)  
		nX = 0;  
	if (nY < 0)  
		nY = 0;  
	if (nX2 > xScrn)  
		nX2 = xScrn;  
	if (nY2 > yScrn)  
		nY2 = yScrn;  
	nWidth = nX2 - nX;  
	nHeight = nY2 - nY;  

	hBitmap = CreateCompatibleBitmap(hScrDC, nWidth, nHeight);      // ����һ������Ļ�豸��������ݵ�λͼ  
	hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);            // ����λͼѡ���ڴ��豸��������  
	BitBlt(hMemDC, 0, 0, nWidth, nHeight, hScrDC, nX, nY, SRCCOPY); // ����Ļ�豸�����������ڴ��豸��������  
	hBitmap = (HBITMAP)SelectObject(hMemDC, hOldBitmap);            // �õ���Ļλͼ�ľ��  

	DeleteDC(hScrDC);  
	DeleteDC(hMemDC);  

	return hBitmap;  
}
//����BMP�ļ�
BOOL SaveBitmapToFile(HBITMAP hBitmap, LPSTR lpFileName)  
{
	HDC hDC;                        // �豸������  

	int iBits;                      // ��ǰ��ʾ�ֱ�����ÿ��������ռ�ֽ���  
	WORD wBitCount;                 // λͼ��ÿ��������ռ�ֽ���  
	DWORD dwPaletteSize = 0, dwBmBitsSize, dwDIBSize, dwWritten;    // ��ɫ���С��λͼ���ݴ�С��λͼ�ļ���С��д���ļ��ֽ���  
	BITMAP Bitmap;                  //λͼ���Խṹ  
	BITMAPFILEHEADER bmfHdr;        // λͼ�ļ�ͷ  
	BITMAPINFOHEADER bi;            // λͼ��Ϣͷ  
	LPBITMAPINFOHEADER lpbi;        // ָ��λͼ��Ϣͷ�ṹ  

	HANDLE fh, hDib;                // �����ļ��������ڴ���  
	HPALETTE hPal, hOldPal=NULL;    // ��ɫ����  

	// ����λͼ�ļ�ÿ��������ռ�ֽ���  
	hDC = CreateDC("DISPLAY", NULL, NULL, NULL);  
	iBits = GetDeviceCaps(hDC, BITSPIXEL) * GetDeviceCaps(hDC, PLANES);  
	DeleteDC(hDC);  
	if (iBits <= 1)  
		wBitCount = 1;  
	else if (iBits <= 4)  
		wBitCount = 4;  
	else if (iBits <= 8)  
		wBitCount = 8;  
	else if (iBits <= 24)  
		wBitCount = 24;  
	else  
		wBitCount = 32;  
	if (wBitCount <= 8)  
		dwPaletteSize = (1 << wBitCount) * sizeof(RGBQUAD);       // �����ɫ���С  

	// ����λͼ��Ϣͷ�ṹ  
	GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&Bitmap);  
	bi.biSize = sizeof(BITMAPINFOHEADER);  
	bi.biWidth = Bitmap.bmWidth;  
	bi.biHeight = Bitmap.bmHeight;  
	bi.biPlanes = 1;  
	bi.biBitCount = wBitCount;  
	bi.biCompression = BI_RGB;  
	bi.biSizeImage = 0;  
	bi.biXPelsPerMeter = 0;  
	bi.biYPelsPerMeter = 0;  
	bi.biClrUsed = 0;  
	bi.biClrImportant = 0;  
	dwBmBitsSize = ((Bitmap.bmWidth * wBitCount + 31) / 32) * 4 * Bitmap.bmHeight;  

	hDib = GlobalAlloc(GHND, dwBmBitsSize + dwPaletteSize + sizeof(BITMAPINFOHEADER));  // Ϊλͼ���ݷ����ڴ�  
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);  
	*lpbi = bi;  
	// �����ɫ��  
	hPal = (HPALETTE)GetStockObject(DEFAULT_PALETTE);  
	if (hPal)  
	{  
		hDC = GetDC(NULL);  
		hOldPal = SelectPalette(hDC, hPal, FALSE);  
		RealizePalette(hDC);  
	}  
	// ��ȡ�õ�ɫ�����µ�����ֵ  
	GetDIBits(hDC, hBitmap, 0, (UINT)Bitmap.bmHeight, (LPSTR)lpbi + sizeof(BITMAPINFOHEADER) + dwPaletteSize, (BITMAPINFO*)lpbi, DIB_RGB_COLORS);  

	if (hOldPal)                // �ָ���ɫ��  
	{  
		SelectPalette(hDC, hOldPal, TRUE);  
		RealizePalette(hDC);  
		ReleaseDC(NULL, hDC);  
	}  
	// ����λͼ�ļ�   
	fh = CreateFile(lpFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);  
	if (fh == INVALID_HANDLE_VALUE)  
		return FALSE;  

	// ����λͼ�ļ�ͷ  
	bmfHdr.bfType = 0x4D42;     // �ļ�����: "BM"  
	dwDIBSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize;   
	bmfHdr.bfSize = dwDIBSize;  // λͼ�ļ���С  
	bmfHdr.bfReserved1 = 0;  
	bmfHdr.bfReserved2 = 0;  
	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + dwPaletteSize;  

	WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);  // д��λͼ�ļ�ͷ  
	WriteFile(fh, (LPSTR)lpbi, dwDIBSize, &dwWritten, NULL);                    // д��λͼ�ļ���������  

	GlobalUnlock(hDib);  
	GlobalFree(hDib);  
	CloseHandle(fh);  

	return TRUE;  
}