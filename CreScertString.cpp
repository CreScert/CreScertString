//字符串的处理统一：所有方法不能返回字符串，在形参中传递。
//读取到文件末尾时不能以字符串为NULL作判断，以返回为-1做判断。
#include "CreScertString.h"

int Length;
DWORD Return;


//打开文件,如果handle为0，那么filename就打开文件，否则handle不为0，不用打开文件
HANDLE CreScertOpenFile(char *FileName,HANDLE Handle,int MemAllocLength)
{

	//需要打开文件
	if(Handle==0) 
	{ 
		Handle=CreateFile(FileName,GENERIC_WRITE|GENERIC_READ,0,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);	

		if(Handle==INVALID_HANDLE_VALUE)
		{	
			MessageBox(NULL,"Create Faile","Tip",MB_OK);
			return 0;
		}

	}
	//移动指针到最后获取文件大小
	Length=SetFilePointer(Handle,0,0,FILE_END);

	if(Length==-1||Length>=20000)
	{
		char *ErrorCode=(char *)malloc(25);

		sprintf(ErrorCode,"长度错误 代码:%d",GetLastError());

		MessageBox(NULL,ErrorCode,"ERROR",MB_OK);
	}
	//恢复指针在初始位置
	SetFilePointer(Handle,0,0,FILE_BEGIN);


	return Handle;
}

//获得整行函数，外部调用
//返回的是读取每行的长度
int GetLineString(char *FileName,HANDLE Handle,int Point,int MemAllocLength,char *LineString)
{

	int i;
	//用来标记Handle是否等于0，需不需要关闭文件句柄
	int Flag=0;

	/******************************************************************************/
	//特别注意  ！！！！！！出错 ！！！指针申请错误。。
	//char *LineString=(char *)malloc(300);
	ZeroMemory(LineString,100);

	//handle为0，则需要关闭文件句柄，flag置一
	if((int)Handle==0)
		Flag=1;
	//返回值仍为handle类型
	Handle=CreScertOpenFile(FileName,Handle,MemAllocLength);

	HGLOBAL hMem;

 
	if(MemAllocLength==0)
	{			
		//分配内存
		hMem=GlobalAlloc(GMEM_MOVEABLE,Length);
	}
	else 	
	{
		hMem=GlobalAlloc(GMEM_MOVEABLE,MemAllocLength);
	}

	if(hMem==NULL)
	{
		char *ErrorCode=(char *)malloc(25);

		sprintf(ErrorCode,"内存空间分配失败 代码:%d",GetLastError());

		MessageBox(NULL,ErrorCode,"ERROR",MB_OK);
		return 0;
	}


	//锁定内存，并得到指针地址
	char *String=(char *)GlobalLock(hMem);

	//置零，大小是申请的内存的大小
	memset(String,0,GlobalSize(hMem));

	if(String==NULL)
	{	//解锁内存区域

		MessageBox(NULL,"内存锁定失败","ERROR",MB_OK);

		return 0;
	}

	//读取文件，将文件保存到指针
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

	//解锁内存区域
	GlobalUnlock(hMem);
	GlobalFree(hMem);


	//关闭文件句柄
	if(Flag==1)
	{	
		CloseHandle(Handle);
	}

	if(Point>Length) 
		return -1;

	else 
		return strlen(LineString);

}

//获得上一行字符串的函数，外部调用
//返回读取每行的长度
int GetPastLineString(char *FileName,HANDLE Handle,int Point,int MemAllocLength,char * LineString)
{
	if(LineString==NULL)return -1;
	//用来标记Handle是否等于0，需不需要关闭文件句柄
	int Flag=0;

	//	char *LineString=(char *)malloc(100);
	//memset(LineString,0,100);
	ZeroMemory(LineString,sizeof(100));

	//handle为0，则需要关闭文件句柄，flag置一
	if((int)Handle==0)
		Flag=1;

	//返回值仍为handle类型
	Handle=CreScertOpenFile(FileName,Handle,MemAllocLength);

	HGLOBAL hMem;

	if(MemAllocLength==0)
	{			
		//分配内存
		hMem=GlobalAlloc(GMEM_MOVEABLE,Length);
	}
	else 	
	{
		hMem=GlobalAlloc(GMEM_MOVEABLE,MemAllocLength);
	}


	if(hMem==NULL)
	{
		MessageBox(NULL,"内存空间分配失败","ERROR",MB_OK);
		return 0;
	}

	//锁定内存，并得到指针地址
	char *String=(char *)GlobalLock(hMem);

	if(String==NULL)
	{	
		MessageBox(NULL,"内存锁定失败","ERROR",MB_OK);
		return 0;
	}

	//读取文件，将文件保存到指针
	ReadFile(Handle,String,Length,&Return,NULL);
	//*(String+Length)='\0';

	//向前依次寻找换行符号
	for(;(*(String+Point)!='\r')&&Point>=0;)
	{
		if(((*(String+Point-1)=='\r')&&(*(String+Point-2)=='\r'))||((*(String+Point-1)=='\r')&&(*(String+Point-3)=='\r')))
			Point--;
		if(((*(String+Point-1)=='\n')&&(*(String+Point-2)=='\n'))||((*(String+Point-1)=='\n')&&(*(String+Point-3)=='\n')))
			Point--;
		Point--;
	}

	//跳过换行符
	Point--;

	//再次向前寻找换行符号，找到后+2跳过换行符号开始得到这一行的数据
	for(;(*(String+Point)!='\r')&&Point>=0;)	
		Point--;

	//如果文件位置为负号，置零
	//否则跳过换行符号
	if(Point<=0)
		Point=0;
	else
		Point+=2;		//跳过换行符号

	//解锁内存区域
	GlobalUnlock(hMem);
	GlobalFree(hMem);

	//获取这一行的数据，如果遇到一行只有换行符号则获取下一行符号

	//LineString=GetLineString(FileName,Handle,Point,0);

	//Point+=strlen(LineString);

	Point+=GetLineString(FileName,Handle,Point,0,LineString);
	//添加换行符号
	*(LineString+strlen(LineString))='\0';

	//关闭文件句柄
	if(Flag==1)
	{	
		CloseHandle(Handle);
	}

	return strlen(LineString);

}
//实现倒序查询
int StringContainEx(char *ContainString,char *NewString,int Number,BOOL Sort)
{
	//先计算两个字符串的长度，长度要减一
	int Length=strlen(ContainString)-1;
	int NewLength=strlen(NewString)-1;

	//进行字符串的另外拷贝，这样做的目的是因为在实参中输入的不是字符串参数而是字符串，后者是const的，不能修改字符串内容
	char *tempContainString=(char *)malloc(Length+1);
	char *tempNewString=(char *)malloc(NewLength+1);

	strcpy(tempContainString,ContainString);
	strcpy(tempNewString,NewString);

	//颠倒字符串，为真或者非0为倒序
	if(!Sort)
	{
		//临时保存的字符
		char TempString;

		//两个两个字符颠倒
		//被包含的字符串
		for(int j=0;j<=Length/2;j++)
		{
			TempString=*(tempContainString+j);
			*(tempContainString+j)=*(tempContainString+Length-j);
			*(tempContainString+Length-j)=TempString;
		}
		//要检测的包含的字符串
		for(int j=0;j<=NewLength/2;j++)
		{
			TempString=*(tempNewString+j);
			*(tempNewString+j)=*(tempNewString+NewLength-j);
			*(tempNewString+NewLength-j)=TempString;
		}	
	}

	//倒序查找
	int OrderSortSum=StringContain(tempContainString,tempNewString,Number);

	//如果是FALSE,也就是倒序，进行数值的加减才能正确
	if(!Sort)
	{
		if(OrderSortSum==-1)return -1;
		OrderSortSum=Length-OrderSortSum-NewLength;

	}

	return OrderSortSum;

}
//包含字符判断,参数Number是第几个符合判断的字符
//包含字符		NewString
//要包含字符	ContainString
int StringContain(char *ContainString,char *NewString,int Number)
{

	//要包含字符下标
	int Sub=0;
	//包含字符
	int NewSub=0;

	//相等次数
	int EquNum=0;

	//保存的下标
	int SaveSub=0;

	//先获取包含的字符大小，如果大于要包含的大小则失败返回-1
	int nLength=strlen(NewString);
	int nContainStringLength=strlen(ContainString);

	if(nLength>nContainStringLength)
		return -1;


	//第一层判断是对Number的判断
	for(int m_Number=0;m_Number<=Number&&Sub<=nContainStringLength;)
	{
		//判断字符是否一样
		for(;Sub<=nContainStringLength;)
		{
			//如果两个字符相等
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
			//如果相等次数等于包含字符的大小，则代表完全相等
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

//不全比较，比较指定的字符串是否是在指定位置，如果匹配返回TRUE,不匹配返回FALSE
BOOL StringCmpOfStart(char *ContainString,char *NewString,int Start,int End ,BOOL Sort)
{
	int StartNow=Start;
	//匹配的开始位置符合
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
//拷贝数据，从start到end的数据，如果end为-1代表拷贝到末尾,Identifier为标识符，意思就是遇到什么字符，比如遇到空格，换行结束
//返回值-1代表失败
//DesString为送出字符
int StringCopy(char *DesString,char *ScrString,int Start,int End,char Identifier)
{

	if(ScrString==NULL)
	{
		ZeroMemory(DesString,End-Start);
		return 0;
	}
	//如果End为-1，代表拷贝到结尾
	if(-1==End)
	{
		End=strlen(ScrString);
	}

	//开始拷贝数据
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
//截断字符串
char *StringCat(char *String,int Start,int End,BOOL IsCopy)
{
	char *TempString=(char *)malloc(strlen(String));

	End++;
	//复制字符串，先保存住原来的字符串
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

//将字符串转换成数字
int StringToSum(char *String)
{
	if(String==0)
		return 0;
	//获取字符串的大小
	int Length=strlen(String);
	//保存的数字
	int Sum=0;

	//先将第一个字符放入Sum
	Sum=(*(String+0)-48);

	//从字符串的左边开始乘以10以此向右增加
	//1253
	//((1*10+2)*10+5)*10+3=1253
	for(int i=0;i<Length-1;i++)
	{
		Sum*=10;
		Sum+=(*(String+i+1)-48);
	}

	//返回计算好的数字
	return Sum;
}

//代替制定位置
char * StringReplace(char *DesString,char *SrcString,long Start,long End/*,char *ReturnStr*/)
{
	//内存分配使用堆分配API

	//定义存储左边字符串指针
	char *LeftString = (char *)HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,Start+1);

	//定义存储左边字符串指针
	char *ReturnStr = (char *)HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,100);

	//定义存储右边字符串指针
	char *RightString = (char *)HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,strlen(SrcString)-End+1);
	
	//如果从0处开始替换，不拷贝任何东西
	if(Start ==0){
		strncpy(LeftString,SrcString,Start);
	}else{
		//拷贝左半部分的字符串
		StringCopy(LeftString,SrcString,0,Start-1,0);
	}

	if(End == -1){
		strcpy(RightString,"");
	}else
	//拷贝右半部分的字符串
	StringCopy(RightString,SrcString,End+1,-1,0);

	//拷贝左边的字符串
	strcpy(ReturnStr,LeftString);
	//追加中间的字符串
	strcat(ReturnStr,DesString);
	//追加右边的字符串
	strcat(ReturnStr,RightString);
	//追加结束符
	strcat(ReturnStr,"\0");
	
	//释放内存
	HeapFree(GetProcessHeap(),0,LeftString);
	HeapFree(GetProcessHeap(),0,RightString);
 
	//返回字符串长度 
	//return strlen(ReturnStr);
	if(Start ==0)
	{
		memset(SrcString,0,strlen(SrcString));
		strcpy(SrcString,ReturnStr);
	}
	strcpy(SrcString,ReturnStr);
	return ReturnStr;
}

//将数字转换成字符串
char *SumToString(int Time)
{
	//颠倒的字符串
	//char *String=(char *)malloc(10);
	char *String = (char *)HeapAlloc(GetProcessHeap(),0,10);
	int i;
	//置零
	//memset(String,0,sizeof(char));
	ZeroMemory(String,sizeof(String));
	if(Time==0)
	{
		sprintf(String,"0\0");
		return String;
	}
	//依次取余数，颠倒计算
	for(i=0;Time>0;i++)
	{
		*(String+i)=((Time%10)+48);
		Time/=10;
	}
	//字符串结尾
	*(String+i)='\0';

	//减一，减过了结尾符
	i--;

	//临时保存的字符
	char TempString;

	//两个两个字符颠倒
	for(int j=0;j<=i/2;j++)
	{
		TempString=*(String+j);
		*(String+j)=*(String+i-j);
		*(String+i-j)=TempString;
	}

	//返回字符串
	return String;
}
//在文件中插入字符串
int InsertStringToFile(char *FileName,HANDLE Handle,int Point,char *WriteString,int MemAllocLength)
{

	int Flag=0;

	if(Handle==0)
	{
		Flag=1;
	}
	//打开文件
	Handle=CreScertOpenFile(FileName,Handle,MemAllocLength); 

	HGLOBAL hMem;

	if(MemAllocLength==0)
	{			
		//分配内存
		hMem=GlobalAlloc(GMEM_MOVEABLE,Length);
	}
	else 	
	{
		hMem=GlobalAlloc(GMEM_MOVEABLE,MemAllocLength);
	}
	if(hMem==NULL)
	{
		MessageBox(NULL,"内存空间分配失败","ERROR",MB_OK);
		return -1;
	}

	//先获取字符串的大小
	int StringLength=strlen(WriteString);

	//锁定内存，并得到指针地址
	char *String=(char *)GlobalLock(hMem);

	memset(String,0,Length);
	if(String==NULL)
	{
		MessageBox(NULL,"内存锁定失败","ERROR",MB_OK);
		return -1;
	}

	//这是指针位置在插入字符串的位置
	SetFilePointer(Handle,Point,0,FILE_BEGIN);

	//读取即将插入的字符串位置的后面到文件结尾的内容并保存
	ReadFile(Handle,String,Length-Point,&Return,0);

	//再把指针位置调回来
	SetFilePointer(Handle,Point,0,FILE_BEGIN);


	//写入插入的字符串
	WriteFile(Handle,WriteString,StringLength,&Return,0);

	//写入先前保存的插入字符串到结尾的字符串
	//此行可能出现问题
	//WriteFile(Handle,String,Length-Point,&Return,0);
	WriteFile(Handle,String,strlen(String),&Return,0);
	//	FlushFileBuffers(Handle);

	//解锁内存区域
	GlobalUnlock(hMem);
	GlobalFree(hMem);


	//关闭文件句柄
	if(Flag==1)
	{	
		CloseHandle(Handle);
	}

	return 0;
}

//删除文件的字符
int DeleteStringToFile(char *FileName,HANDLE Handle,int Point,int DeleteLength,int MemAllocLength)
{
	int Flag=0;

	if(Handle==0)
	{
		Flag=1;
	}
	//打开文件
	Handle=CreScertOpenFile(FileName,Handle,MemAllocLength); 
	HGLOBAL hMem;

	if(MemAllocLength==0)
	{			
		//分配内存
		hMem=GlobalAlloc(GMEM_MOVEABLE,Length);
	}
	else 	
	{
		hMem=GlobalAlloc(GMEM_MOVEABLE,MemAllocLength);
	}
	if(hMem==NULL)
	{
		MessageBox(NULL,"内存空间分配失败","ERROR",MB_OK);
		return 0;
	}

	//锁定内存，并得到指针地址
	char *String=(char *)GlobalLock(hMem);

	if(String==NULL)
	{
		MessageBox(NULL,"内存锁定失败","ERROR",MB_OK);
		return -1;
	}

	//这是指针位置在将要删除的字符串的位置
	SetFilePointer(Handle,Point+DeleteLength,0,FILE_BEGIN);


	//读取即将删除的字符串位置的后面到文件结尾的内容并保存
	ReadFile(Handle,String,Length-Point-DeleteLength,&Return,0);


	//再把指针位置调回来
	SetFilePointer(Handle,Point,0,FILE_BEGIN);

	//写入先前保存的删除字符串到结尾的字符串
	WriteFile(Handle,String,Length-Point-DeleteLength,&Return,0);

	SetEndOfFile(Handle);

	//解锁内存区域
	GlobalUnlock(hMem);
	GlobalFree(hMem);

	//关闭文件句柄
	if(Flag==1)
	{	
		CloseHandle(Handle);
	}

	return 0;
}

//获取Ini字符串
char  *GetIniString(char *IniFilePath,HANDLE Handle,char *Section,char *Key,int *Pointer,int FilePoint,int FilePointEnd,BOOL Repeat,int RepeatSum)
{


	int Point=FilePoint;//文件指针，形参用指针表示，最后不用返回

	int Equal=0;//最后获取的键名的字符串的处理时用的
	char *SectionTempString=(char *)malloc(100);//节名的字符串
	char *String=(char*)calloc(MAX_LINE,sizeof(char ));//获取的每一行的数据
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

			sprintf(ErrorCode,"长度错误 代码:%d",GetLastError());

			MessageBox(NULL,ErrorCode,"ERROR",MB_OK);
		}
	}
	SetFilePointer(Handle,0,0,FILE_BEGIN);
	while(1){


		//获取每行的数据,得到得到的字符串的长度
		int LineLength = GetLineString(IniFilePath,Handle,Point,0 ,String);



		//到了文件的末尾
		if(String==0)
		{	
			if(IsCloseHandle) CloseHandle(Handle);
			free(String);
			String=NULL;

			free(SectionTempString);

			SectionTempString=NULL;

			return 0;
		}




		//修订有关于有的字符后面的换行符
		if(*(String+LineLength-1)==13)
			*(String+LineLength-1)='\0';

		//字符串为 节名
		if(*(String+0)=='['&&*(String+LineLength-2)==']')
		{


			//用来判断节名是否一致，对括号内的字符串直接赋值判断
			//获取节名这一行的所有数据，包括括号
			strcpy(SectionTempString,String);
			if(LineLength-2>strlen(SectionTempString)){
				system("pause");
			}
			//去掉尾括号
			*(SectionTempString+LineLength-2)='\0';
			//去掉头括号
			strcpy(SectionTempString,SectionTempString+1);


			//比较是否是节名一致，不一致则继续循环，一直进入判断
			if(!strcmp(Section,SectionTempString))
			{

				//键名为零，表示只是获取节名的位置
				if(Key==0)
				{
					//先把文件的指针增加到下一行
					Point+=strlen(String)+1;

					//节名的位置
					*(Pointer)=Point;

					free(String);
					String=NULL;

					free(SectionTempString);
					SectionTempString=NULL;
					if(IsCloseHandle) CloseHandle(Handle);
					return Section;
				}
				//在节名段下逐次搜索键字符
				while(1)
				{

					//先把文件的指针增加到下一行
					Point+=strlen(String)+1;
					//获得下一行的字符串，判断字符串的长度，用来检测字符串的最后一个字符是否为"]"
					LineLength+=GetLineString(IniFilePath,Handle,Point,0,String);

					//到了文件的末尾
					if(String==0)
					{
						free(String);
						String=NULL;

						free(SectionTempString);
						SectionTempString=NULL;
						if(IsCloseHandle) CloseHandle(Handle);
						return 0;
					}
					//判断字符串的长度，用来检测字符串的最后一个字符是否为"]"
					//LineLength=strlen(String);

					//自己纳闷，为什么加上括号就好了
					//检测到新的节名，从头再来

					if(*(String+0)=='[') 				
					{
						if(LineLength-1>strlen(String))
							break;
						if(*(String+LineLength-1)==']')
							break;
					}
					//键名检测，截断"="前字符串
					Equal=StringContain(String,"=",1);

					char *Value=(char*)malloc(200);
					if(Value==NULL)
					{
						if(IsCloseHandle) CloseHandle(Handle);
						MessageBox(NULL,"申请内存出错",0,MB_OK);
					}
					//保存未截断的键名行
					strcpy(Value,String);

					if(Equal>=0)
						//截断键名行，获取键名
						*(String+Equal)='\0';

					//比较键名是否是需要的键名
					if(!strcmp(String,Key))
					{
						if(Repeat) RepeatSum_Now++;
						else	RepeatSum_Now=RepeatSum;
						if(RepeatSum==RepeatSum_Now)
						{
							//截断键名行字符串，获取键值
							Value+=Equal+1;

							if(*(Value+strlen(Value)-1)==13)
								*(Value+strlen(Value)-1)='\0';

							if(Pointer!=0)
								*(Pointer)=Point+Equal;

							//收尾工作，不能用free释放，返回键值
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
				//先把文件的指针增加到下一行
				Point+=strlen(String)+1;
				continue;
			}

		}
		if(FilePointEnd<Point)
		{
			break;
		}
		if(String!=0)
			//指针位置加一
			Point+=strlen(String)+1;

	}
	if(IsCloseHandle) CloseHandle(Handle);
	free(SectionTempString);
	SectionTempString=NULL;
	free(String);
	String=NULL;

	return 0;
}
//获得Ini值
//IniFilePath参数为文件的位置
//SecTion为节名
//Key为键名
//返回值为键值
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
//修改ini值
//NewValue为将要修改的新的值
char *SetIniValueEx(char *IniFilePath,HANDLE Handle,char *Section,char *Key,char *NewValue,int PointerStart,int PointerEnd,BOOL Repeat,int RepeatSum)
{
	BOOL Flag=FALSE;
	if((int)Handle==0)
	{
		//打开文件
		Handle=CreScertOpenFile(IniFilePath,0,0); 
		Flag=TRUE;
	}

	int Pointer;

	//获取Ini值
	char *Value=GetIniString(IniFilePath,Handle,Section,Key,&Pointer,PointerStart,PointerEnd,Repeat,RepeatSum);

	//计算新的键值长度
	int Length=strlen(NewValue);



	//设置新的文件指针位于键值的前面
	SetFilePointer(Handle,Pointer+1,0,FILE_BEGIN);


	//删除指定的键值，貌似删除的字符中未包含换行符号
	DeleteStringToFile(IniFilePath,Handle,Pointer+1,strlen(Value),0);

	//再次设置新的文件指针位置，用来写入键值
	SetFilePointer(Handle,Pointer+1,0,FILE_BEGIN);

	//写入键值
	InsertStringToFile(IniFilePath,Handle,Pointer+1,NewValue,0);

	//关闭文件句柄退出
	if(Flag)
		CloseHandle(Handle);

	return 0;
}
//获取下一行的位置
int GetNextLinePointer(char *IniFilePath,HANDLE Handle,int Pointer)
{
	int CloseBool=FALSE;

	//句柄为零，打开句柄
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
//增加Ini值
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
	//先查找节名是否存在，不存在创建节名
	if(0==GetIniString(IniFilePath,Handle,Section,0,&Pointer,IniPointer,IniPointerEnd,Repeat,RepeatSum)/*||IniPointer!=0*/)
	{

		if(IniPointer==0)
		{
			//设置写入位置
			int FileLength=SetFilePointer(Handle,0,0,FILE_END);

			//用来读取前两个字符，判断是否是换行符号
			char *TempReadEntry=(char *)malloc(3);

			//置零
			ZeroMemory(TempReadEntry,3);
			//向前推进两个字符
			SetFilePointer(Handle,FileLength-2,0,FILE_BEGIN);

			//读取
			ReadFile(Handle,TempReadEntry,2,&Return,0);

			//添加尾部标识符
			*(TempReadEntry+2)='\0';

			//没有换行符号，添加上换行符号然后下一行
			if(strcmp(TempReadEntry,"\r\n"))
			{
				WriteFile(Handle,"\r\n",2,&Return,0);
			}

			if(TempReadEntry!=NULL)
			{
				free(TempReadEntry);
				TempReadEntry=NULL;
			}

			//写入[..]格式，最后添加上换行符号
			WriteFile(Handle,"[",1,&Return,0);
			WriteFile(Handle,Section,strlen(Section),&Return,0);
			WriteFile(Handle,"]\r\n",3,&Return,0);

		}
		else
		{
			NewIniPointer=GetNextLinePointer(IniFilePath,Handle,NewIniPointer);
			//设置写入位置
			SetFilePointer(Handle,NewIniPointer,0,FILE_BEGIN);
			InsertStringToFile(IniFilePath,Handle,NewIniPointer++,"[",0);
			InsertStringToFile(IniFilePath,Handle,NewIniPointer,Section,0);
			NewIniPointer+=strlen(Section);
			InsertStringToFile(IniFilePath,Handle,NewIniPointer++,"]",0);
			InsertStringToFile(IniFilePath,Handle,NewIniPointer,"\r\n",0);

		}

	}

	//如果键名为0，表示只是添加节名，则退出
	if(NewKey==0)
	{	if(Handle)CloseHandle(Handle);
	return 0;
	}

	//如果实参为零时，可能输错了，所以为了防止这一意外。
	if(NewValue==0)
	{
		NewValue=(char *)malloc(2);
		strcpy(NewValue,"0\0");
	}
	//先获取是否键名存在，如果存在，设置键值并退出，如果不存在，新建键与键值
	char *Value=GetIniString(IniFilePath,Handle,Section,NewKey,&Pointer,IniPointer,IniPointerEnd,Repeat,RepeatSum);

	//键名存在，设置键值
	if(Value!=0)
	{

		SetIniValueEx(IniFilePath,Handle,Section,NewKey,NewValue,IniPointer,IniPointerEnd,Repeat,RepeatSum);
		if(IsCloseHandle)
			CloseHandle(Handle);
		return 0;
	}

	//先获取节名的位置，直接在下面添加段名
	GetIniString(IniFilePath,Handle,Section,0,&Pointer,IniPointer,IniPointerEnd,Repeat,RepeatSum);

	//设置写入位置
	SetFilePointer(Handle,Pointer+1,0,FILE_BEGIN);

	//新建写入字符串
	char *m_NewValue=(char *)calloc(strlen(NewKey)+strlen(NewValue)+20,sizeof(char));

	//写入键名、等于号、键值、换行符
	/*strcpy(m_NewValue,NewKey);
	strcat(m_NewValue,"=");
	strcat(m_NewValue,NewValue);
	strcat(m_NewValue,"\r\n");
	*/
	sprintf(m_NewValue,"%s=%s\r\n",NewKey,NewValue);
	//插入字符串
	InsertStringToFile(0,Handle,Pointer+1,m_NewValue,0);

	m_NewValue=NULL;

	if(IsCloseHandle)
		CloseHandle(Handle);

	return 0;
}

//删除Ini值
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

	//key等于零，意味删除节名以及之下所有的键值
	if(Key==0)
	{
		char *DelAllKey=(char *)malloc(50);

		GetIniString(IniFilePath,Handle,Section,0,&Pointer,IniPointer,IniPointerEnd,Repeat,RepeatSum);

		//没有找到指定的节名
		if(0==Pointer)
		{	
			if((int)Handle!=0)
				CloseHandle(Handle);
			return -1;
		}

		//找到指定的节名，此时Pointer为节名之后的位置，此时需要删除到之前的位置
		Pointer-=strlen(Section);

		//加上换行符号的位置
		Pointer-=2;

		//删除[ ]字符
		Pointer-=2;

		//得到第一个
		DelAllKey=GetFirstIniValueEx(IniFilePath,Handle,Section,&Pointer,0,-1);


		while(DelAllKey!=0)
		{	
			Pointer-=DeleteIniValueEx(IniFilePath,Handle,Section,DelAllKey,IniPointer,IniPointerEnd,FALSE,0);

			DelAllKey=GetNextIniValue(IniFilePath,Handle,&Pointer);
		}


		GetIniString(IniFilePath,Handle,Section,0,&Pointer,IniPointer,IniPointerEnd,FALSE,0);

		//找到指定的节名，此时Pointer为节名之后的位置，此时需要删除到之前的位置
		Pointer-=strlen(Section);

		//加上换行符号的位置
		Pointer--;

		//删除[ ]字符
		Pointer-=2;

		DeleteStringToFile(IniFilePath,Handle,Pointer,strlen(Section)+2+2,0);

		free(DelAllKey);
		DelAllKey=NULL;

		if(IsCloseHandle)
			CloseHandle(Handle);
		return 0;


	}
	//获取键名，如果不存在则退出，存在删除
	char *Value=GetIniString(IniFilePath,Handle,Section,Key,&Pointer,IniPointer,IniPointerEnd,Repeat,RepeatSum);

	//键名不存在，退出
	if(Value==0)
	{		
		if(IsCloseHandle)
			CloseHandle(Handle);
		Value=NULL;
		return 0;
	}

	//获取键值长度，加上最后的换行符
	int ValueLength=strlen(Value)+2;

	//指定删除字符串的长度参数
	int StringLength=0;

	//获取键名长度
	StringLength+=strlen(Key);
	//设置等号长度
	StringLength++;


	//将文件指针退后到键名这一行开头
	Pointer-=StringLength;
	SetFilePointer(Handle,Pointer+1,0,FILE_BEGIN);

	//指定删除字符串的长度，需要加上键值的字符串长度
	StringLength+=ValueLength;

	//删除键值，
	DeleteStringToFile(IniFilePath,Handle,Pointer+1,StringLength,0);

	//关闭句柄退出
	if(IsCloseHandle)
		CloseHandle(Handle);

	return StringLength;
}

//返回第一个键名，本来思考是将节名、键名、键值等放在一个结构体中，后来想没多大必要就放弃了。
//IniFilePath:文件名
//SecTion:节名
//Point:位置
char *GetFirstIniValueEx(char *IniFilePath,HANDLE Handle,char *Section,int *Point,int IniPointer,int IniPointerEnd)
{
	//找到节名的位置
	GetIniString(IniFilePath,Handle,Section,0,Point,IniPointer,IniPointerEnd,FALSE,0);

	return 	GetNextIniValue(IniFilePath,Handle,Point);
}
char *GetFirstIniValue(char *IniFilePath,char *Section,int *Point)
{
	//找到节名的位置
	GetIniString(IniFilePath,0,Section,0,Point,0,-1,FALSE,0);

	return 	GetNextIniValue(IniFilePath,0,Point);
}

//获取下一个键名
char *GetNextIniValue(char *IniFilePath,HANDLE Handle,int *Point)
{
	BOOL IsCloseHandle=FALSE;
	if(Handle==0)
	{
		Handle=CreScertOpenFile(IniFilePath,0,0);
		IsCloseHandle=TRUE;
	}
	//每一行的数据
	char *LineString=(char *)malloc(50);

	int Equal=0;

	do
	{
		//获取节名，得到位置，//走过换行
		*(Point)+=GetLineString(IniFilePath,Handle,*Point,0,LineString)+1;


		//到了文件的末尾，退出
		if(LineString==0) 
		{
			if(IsCloseHandle)
				CloseHandle(Handle);
			return 0;
		}

		//如果是新的节名，退出循环
		if(-1!=StringContain(LineString,"[",1)&&-1!=StringContain(LineString,"]",1))
		{	
			if(IsCloseHandle)///////你妹，找到这个问题让我花了不少时间，先前写成了Handle。没想到这里出了问题，调试到调用这个函数时我直接跳过了！！！
				CloseHandle(Handle);
			free(LineString);
			LineString=NULL;
			return 0;
		}




		//查看是否有等于号，没有
		Equal=StringContain(LineString,"=",1);

		//找到等于号则退出循环
		if(-1!=Equal)
			break;

		//直到文件的末尾
	}while(LineString!=0);

	if(IsCloseHandle)CloseHandle(Handle);
	//到了文件的末尾，退出
	if(LineString==0) return 0;

	//截断等于号以及键值
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
	//每一行的数据
	char *LineString=(char *)malloc(50);

	int Equal=0;

	do
	{
		//获取节名，得到位置
		//走过换行
		*(Point)+=GetLineString(IniFilePath,Handle,*Point,0,LineString)+1;


		//到了文件的末尾，退出
		if(LineString==0) 
		{
			if(IsCloseHandle)
				CloseHandle(Handle);
			return 0;
		}

		//如果是新的节名，退出循环
		if(-1!=StringContain(LineString,"[",1)&&-1!=StringContain(LineString,"]",1))
			break;




		//直到文件的末尾
	}while(*(Point)>Length);

	if(IsCloseHandle)CloseHandle(Handle);
	//到了文件的末尾，退出
	if(LineString==0) return 0;

	return LineString;
}

//计算标记
int GetInternetStatusFlag(int StatusArrayFlag[],int Flag)
{
	int FlagTemp=Flag;//一直修改的Flag

	int MoveLeft=0;//右移动了多少，最后向左移动

	int TotalSum=0;//数组中记录标记的数量

	int Sum=1;//标记的下标
	//记录数组,每次完成后置零重新计算，得到下一次的标记
	for(;Flag>0;)
	{
		//右移数量置零
		MoveLeft=0;
		//每次计算标记
		for(FlagTemp=Flag;FlagTemp>0;)
		{
			//每次向右移动一个，直到为1
			FlagTemp>>=1;
			//向右移动的数量累加
			MoveLeft++;
			//如果标记等于1，退出循环，计算下一个标记
			if(FlagTemp<=1)
			{

				//向左移动向右移动的数量，得到的是2的n次方幂
				FlagTemp<<=MoveLeft;

				//如果为零直接退出循环
				if(FlagTemp==0) break;

				//保存标记
				StatusArrayFlag[Sum++]=FlagTemp;

				//标记记录累加并保存
				StatusArrayFlag[0]=++TotalSum;
				//相减得到下一次循环的标记
				Flag-=FlagTemp;
				break;

			}

		}
		//标记等于1，退出循环
		if(Flag==1)
		{
			//标记记录累加并保存
			StatusArrayFlag[Sum]=1;
			StatusArrayFlag[0]++;
			break;
		}

	}

	return 0;
}
//复制屏幕图像到bmp
HBITMAP CopyScreenToBitmap(LPRECT lpRect) 
{
	HDC hScrDC, hMemDC;             // 屏幕和内存设备描述表  
	HBITMAP hBitmap, hOldBitmap;    // 位图句柄  
	int nX, nY, nX2, nY2;           // 选定区域坐标  
	int nWidth, nHeight;            // 位图宽度和高度  
	int xScrn, yScrn;               // 屏幕分辨率  

	if (IsRectEmpty(lpRect))  
		return NULL;  

	hScrDC = CreateDC("DISPLAY", NULL, NULL, NULL);     // 为屏幕创建设备描述表  

	hMemDC = CreateCompatibleDC(hScrDC);                // 为屏幕设备描述表创建兼容的内存设备描述表  

	nX = lpRect->left;  
	nY = lpRect->top;  
	nX2 = lpRect->right;  
	nY2 = lpRect->bottom;  

	xScrn = GetDeviceCaps(hScrDC, HORZRES); // 获得屏幕水平分辨率  
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

	hBitmap = CreateCompatibleBitmap(hScrDC, nWidth, nHeight);      // 创建一个与屏幕设备描述表兼容的位图  
	hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);            // 把新位图选到内存设备描述表中  
	BitBlt(hMemDC, 0, 0, nWidth, nHeight, hScrDC, nX, nY, SRCCOPY); // 把屏幕设备描述表拷贝到内存设备描述表中  
	hBitmap = (HBITMAP)SelectObject(hMemDC, hOldBitmap);            // 得到屏幕位图的句柄  

	DeleteDC(hScrDC);  
	DeleteDC(hMemDC);  

	return hBitmap;  
}
//保存BMP文件
BOOL SaveBitmapToFile(HBITMAP hBitmap, LPSTR lpFileName)  
{
	HDC hDC;                        // 设备描述表  

	int iBits;                      // 当前显示分辨率下每个像素所占字节数  
	WORD wBitCount;                 // 位图中每个像素所占字节数  
	DWORD dwPaletteSize = 0, dwBmBitsSize, dwDIBSize, dwWritten;    // 调色板大小，位图数据大小，位图文件大小，写入文件字节数  
	BITMAP Bitmap;                  //位图属性结构  
	BITMAPFILEHEADER bmfHdr;        // 位图文件头  
	BITMAPINFOHEADER bi;            // 位图信息头  
	LPBITMAPINFOHEADER lpbi;        // 指向位图信息头结构  

	HANDLE fh, hDib;                // 定义文件，分配内存句柄  
	HPALETTE hPal, hOldPal=NULL;    // 调色板句柄  

	// 计算位图文件每个像素所占字节数  
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
		dwPaletteSize = (1 << wBitCount) * sizeof(RGBQUAD);       // 计算调色板大小  

	// 设置位图信息头结构  
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

	hDib = GlobalAlloc(GHND, dwBmBitsSize + dwPaletteSize + sizeof(BITMAPINFOHEADER));  // 为位图内容分配内存  
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);  
	*lpbi = bi;  
	// 处理调色板  
	hPal = (HPALETTE)GetStockObject(DEFAULT_PALETTE);  
	if (hPal)  
	{  
		hDC = GetDC(NULL);  
		hOldPal = SelectPalette(hDC, hPal, FALSE);  
		RealizePalette(hDC);  
	}  
	// 获取该调色板下新的像素值  
	GetDIBits(hDC, hBitmap, 0, (UINT)Bitmap.bmHeight, (LPSTR)lpbi + sizeof(BITMAPINFOHEADER) + dwPaletteSize, (BITMAPINFO*)lpbi, DIB_RGB_COLORS);  

	if (hOldPal)                // 恢复调色板  
	{  
		SelectPalette(hDC, hOldPal, TRUE);  
		RealizePalette(hDC);  
		ReleaseDC(NULL, hDC);  
	}  
	// 创建位图文件   
	fh = CreateFile(lpFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);  
	if (fh == INVALID_HANDLE_VALUE)  
		return FALSE;  

	// 设置位图文件头  
	bmfHdr.bfType = 0x4D42;     // 文件类型: "BM"  
	dwDIBSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize;   
	bmfHdr.bfSize = dwDIBSize;  // 位图文件大小  
	bmfHdr.bfReserved1 = 0;  
	bmfHdr.bfReserved2 = 0;  
	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + dwPaletteSize;  

	WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);  // 写入位图文件头  
	WriteFile(fh, (LPSTR)lpbi, dwDIBSize, &dwWritten, NULL);                    // 写入位图文件其余内容  

	GlobalUnlock(hDib);  
	GlobalFree(hDib);  
	CloseHandle(fh);  

	return TRUE;  
}