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

#define MAX_LINE 300	//每行的最大长度
//文件每行数据获取，返回字符指针
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

//插入字符串于文件中
INTEXTERN 
InsertStringToFile(
				   char *FileName,
				   HANDLE Handle,
				   int Point,
				   char *WriteString,
				   int MemAllocLength
				   );

//删除字符串于文件中
INTEXTERN 
DeleteStringToFile(
				   char *FileName,
				   HANDLE Handle,
				   int Point,
				   int DeleteLength,
				   int MemAllocLength
				   );

//包含字符判断，成功找到字符返回第一个字符位置，失败返回-1
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
//拷贝数据，从start到end的数据，如果end为-1代表拷贝到末尾
//返回值-1代表失败
//DesString为送出字符
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
//字符串截断
CHAREXTERN
*StringCat(
			char *String,
			int Start,
			int End,
			BOOL IsCopy
			);

//代替制定位置
CHAREXTERN
* StringReplace(
	char *DesString,
	char *SrcString,
	long Start,
	long End/*,char *ReturnStr*/);

//将字符串转换成数字
INTEXTERN 
StringToSum(char *String);

//将数字转换成字符串
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

//获得Ini值
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

//设置Ini值
CHAREXTERN 
*SetIniValue(
			 char *IniFilePath,
			 char *Section,
			 char *Key,
			 char *NewValue
			 );
//增加Ini值
INTEXTERN 
AddIniValue(
			char *IniFilePath,
			char *Section,
			char *NewKey,
			char *NewValue
			);
//删除Ini值
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

//获取第一个ini键
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
//以此寻找之后的ini值
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
//获取文件的名称
CHAREXTERN 
	*GetFileExe(
	char *FilePath
	);
//获取网络标记值
INTEXTERN 
	GetInternetStatusFlag(
	int StatusFlagArray[],
	int Flag
	);

//截图到BMP格式
HBITMAPEXTERN 
	CopyScreenToBitmap(
	LPRECT lpRect
	);
//保存BMP文件
BOOLEXTERN
	SaveBitmapToFile(
	HBITMAP hBitmap,
	LPSTR lpFileName
	);

