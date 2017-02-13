#include <windows.h>
#include <stdio.h>
#pragma warning(disable:4996)

#define CHAREXTERN extern "C" char _declspec (dllexport) 

#define INTEXTERN extern "C" int _declspec (dllexport) 

#define HWNDEXTERN extern "C" HWND _declspec (dllexport) 

#define LPDLGTEMPLATEEXTERN extern "C" LPDLGTEMPLATE _declspec (dllexport) 

#define LPWORDEXTERN extern "C" LPWORD _declspec (dllexport) 

#define BOOLEXTERN extern "C" BOOL _declspec (dllexport) 

#define HBITMAPEXTERN extern "C" HBITMAP _declspec (dllexport) 

#define MAX_LINE 300	//ÿ�е���󳤶�
//�ļ�ÿ�����ݻ�ȡ�������ַ�ָ��
INTEXTERN GetLineString(
			   char *FileName,
			   HANDLE Handle,
			   int Point,
			   int MemAllocLength,
			   char *LineString
			   );
INTEXTERN
GetPastLineString(
				char *FileName,
				HANDLE Handle,
				int Point,
				int MemAllocLength,
				char *LineString
);

//�����ַ������ļ���
INTEXTERN 
InsertStringToFile(
				   char *FileName,
				   HANDLE Handle,
				   int Point,
				   char *WriteString,
				   int MemAllocLength
				   );

//ɾ���ַ������ļ���
INTEXTERN 
DeleteStringToFile(
				   char *FileName,
				   HANDLE Handle,
				   int Point,
				   int DeleteLength,
				   int MemAllocLength
				   );

//�����ַ��жϣ��ɹ��ҵ��ַ����ص�һ���ַ�λ�ã�ʧ�ܷ���-1
INTEXTERN
StringContain(
			  char *ContainString,
			  char *NewString,
			  int Number
			  );
INTEXTERN
StringContainEx(char *ContainString,
			char *NewString,
			int Number,
			BOOL Sort);
//�������ݣ���start��end�����ݣ����endΪ-1��������ĩβ
//����ֵ-1����ʧ��
//DesStringΪ�ͳ��ַ�
INTEXTERN 
StringCopy(
		   
		   char *DesString,
		   char *ScrString,
		   int Start,
		   int End,
		   char Identifier
		   );

BOOLEXTERN 
StringCmpOfStart(char *Destring,
		char *ScrString,
		int Start,
		int End ,
		BOOL Sort
		);
//�ַ����ض�
CHAREXTERN
*StringCat(
			char *String,
			int Start,
			int End,
			BOOL IsCopy
			);

//�����ƶ�λ��
CHAREXTERN
* StringReplace(
	char *DesString,
	char *SrcString,
	long Start,
	long End/*,char *ReturnStr*/);

//���ַ���ת��������
INTEXTERN 
StringToSum(char *String);

//������ת�����ַ���
CHAREXTERN 
*SumToString(int Time);

INTEXTERN 
GetNextLinePointer(
		char *IniFilePath,
		HANDLE Handle,
		int Pointer
		);

INTEXTERN 
AddIniValueEx(
		char *IniFilePath,
		HANDLE Handle,
		char *Section,
		char *NewKey,
		char *NewValue,
		int IniPointer,
		int IniPointerEnd,BOOL Repeat,int RepeatSum
		);

//���Iniֵ
CHAREXTERN 
*GetIniValue(
			 char *IniFilePath,
			 char *Section,
			 char *Key
			 );
CHAREXTERN
*GetIniValueEx(
			 char *IniFilePath,
			 HANDLE Handle,
			 char *Section,
			 char *Key,
			 int Pointer,
			 int PointerEnd,BOOL Repeat,int RepeatSum
);

//����Iniֵ
CHAREXTERN 
*SetIniValue(
			 char *IniFilePath,
			 char *Section,
			 char *Key,
			 char *NewValue
			 );
//����Iniֵ
INTEXTERN 
AddIniValue(
			char *IniFilePath,
			char *Section,
			char *NewKey,
			char *NewValue
			);
//ɾ��Iniֵ
INTEXTERN 
DeleteIniValue(
			   char *IniFilePath,
			   char *Section,
			   char *Key
			   );
INTEXTERN 
DeleteIniValueEx(
				char *IniFilePath,
				HANDLE Handle,
				char *Section,
				char *Key,
				int IniPointer,
				int IniPointerEnd,
				BOOL Repeat,int RepeatSum
				);

//��ȡ��һ��ini��
CHAREXTERN 
*GetFirstIniValue(
				  char *IniFilePath,
				  char *Section,
				  int *Point
				  );
CHAREXTERN 
*GetFirstIniValueEx(
				char *IniFilePath,
				HANDLE Handle,
				char *Section,
				int *Point,
				int IniPointer,
				int IniPointerEnd
	);
//�Դ�Ѱ��֮���iniֵ
CHAREXTERN 
*GetNextIniValue(
				 char *IniFilePath,
				 HANDLE Handle,
				 int *Point
				 );
CHAREXTERN
*GetNextSection(
				char *IniFilePath,
				HANDLE Handle,
				int *Point
				);
CHAREXTERN
*SetIniValueEx(
	char *IniFilePath,
	HANDLE Handle,
	char *Section,
	char *Key,
	char *NewValue,
	int Pointer,
	int PointerEnd,
	BOOL Repeat,int RepeatSum
	);
//��ȡ�ļ�������
CHAREXTERN 
	*GetFileExe(
	char *FilePath
	);
//��ȡ������ֵ
INTEXTERN 
	GetInternetStatusFlag(
	int StatusFlagArray[],
	int Flag
	);

//��ͼ��BMP��ʽ
HBITMAPEXTERN 
	CopyScreenToBitmap(
	LPRECT lpRect
	);
//����BMP�ļ�
BOOLEXTERN
	SaveBitmapToFile(
	HBITMAP hBitmap,
	LPSTR lpFileName
	);

