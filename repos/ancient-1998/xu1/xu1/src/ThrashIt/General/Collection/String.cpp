// String.cpp: implementation of the C1DArray class.
//
//////////////////////////////////////////////////////////////////////

#include "stdio.h"
#include "stdlib.h"
#include "time.h"


#include "String.h"



#define MAXLONGCHARS	10
#define MAXFLOATCHARS	10
#define MAXDOUBLECHARS	20



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////



String::String()
{
	endAddress=startAddress=NULL;
	lSize=0;
}


String::String(LPCHAR lpString)
{
	startAddress=NULL;
	Set(lpString);
}


String::String(String &sString)
{
	startAddress=NULL;
	Set(sString);	
}


String::String(CHAR cChar)
{
	startAddress=NULL;
	Set(cChar);
}


String::String(LONG lConvert)
{
	startAddress=NULL;
	Set(lConvert);	
}

String::String(FLOAT fConvert)
{
	startAddress=NULL;
	Set(fConvert);
}

String::String(double dConvert,UINT uiPrecision)
{
	startAddress=NULL;
	Set(dConvert,uiPrecision);
}

String::String(DWORD dwConvert)
{
	startAddress=NULL;
	Set(dwConvert);
}


VOID FC String::Set(LPCHAR lpString)
{
	Free();
	ULONG ulSize;
	ulSize=_Find(lpString,NULLCHAR);

	endAddress=startAddress=new CHAR[ulSize];
	lSize=ulSize;
	endAddress+=lSize-1;

	memcpy((LPVOID) startAddress,(LPVOID) lpString,lSize);
}

VOID FC String::Set(LPCHAR lpString,int charCount)
{
	ULONG ulSize;
	ulSize= charCount + 1;

	endAddress=startAddress=new CHAR[ulSize];
	lSize=ulSize;
	endAddress+=lSize-1;

	memcpy((LPVOID) startAddress,(LPVOID) lpString,lSize - 1);
	*endAddress = NULL;
}

VOID FC String::Set(String &sString)
{
	Set(sString.startAddress);	
}



VOID FC String::Set(CHAR cChar)
{
	Free();
	endAddress=startAddress=new CHAR[2];
	endAddress+=1;
	lSize=2;
	startAddress[0]=cChar;
	startAddress[1]=NULLCHAR;
}



VOID FC String::Set(LONG lConvert)
{
	LPCHAR lpcTemp=new char[MAXLONGCHARS];
	ltoa(lConvert,lpcTemp,10);
	
	Set(lpcTemp);
	delete[] lpcTemp;
}


VOID FC String::Set(FLOAT fConvert)
{
	LPCHAR lpcTemp=new char[MAXFLOATCHARS];
	sprintf(lpcTemp,"%g",fConvert);

	Set(lpcTemp);
	delete[] lpcTemp;
}

VOID FC String::Set(double dConvert,UINT uiPrecision)
{
	LPCHAR lpcTemp=new char[MAXDOUBLECHARS];
	sprintf(lpcTemp,"%f",dConvert);

	Set(lpcTemp);
	delete[] lpcTemp;
}

VOID FC String::Set(DWORD dwConvert)
{
	LPCHAR lpcTemp=new char[(2*DWORDSIZEOF)+1];
	sprintf(lpcTemp,"%p",(LPVOID) dwConvert);


	Set(lpcTemp);
	delete[] lpcTemp;
}


DWORD FC String::SetChar(ULONG ulCharIndex,CHAR cChar)
{
	if(ulCharIndex>=lSize-1)
		return(S_FAILED);

	startAddress[ulCharIndex]=cChar;
	
	return(S_SUCCESS);
}

VOID FC String::SetCurrDate()
{
	char dbuffer [9];
	_strdate( dbuffer );
	Set(dbuffer);	
}

VOID FC String::SetCurrTime()
{
	char tbuffer [9];
	_strtime( tbuffer );
	Set(tbuffer);	
}



String::~String()
{
	Free();
}



VOID FC String::Get(LPCHAR lpString)
{
	memcpy(lpString,startAddress,lSize);
}


VOID FC String::Get(LPSTRING lpsString)
{
	lpsString->Set(*this);
}

VOID FC String::GetNoNull(LPCHAR lpString)
{
	memcpy(lpString,startAddress,lSize-1);
}


DWORD FC String::Get(LPCHAR lpString,ULONG ulStartIndex)
{
	if(ulStartIndex>=lSize-1)
		return(S_FAILED);

	memcpy(lpString,startAddress+ulStartIndex,(lSize-ulStartIndex));
	lpString[(lSize-ulStartIndex)-1]=NULLCHAR;
	
	return(S_SUCCESS);
}
	

DWORD FC String::Get(LPSTRING lpsString,ULONG ulStartIndex)
{
	if(ulStartIndex>=lSize-1)
		return(S_FAILED);

	lpsString->Set(startAddress+ulStartIndex);
	return(S_SUCCESS);
}


DWORD FC String::GetNoNull(LPCHAR lpString,ULONG ulStartIndex)
{
	if(ulStartIndex>=lSize-1)
		return(S_FAILED);

	memcpy(lpString,startAddress+ulStartIndex,lSize-ulStartIndex);
	return(S_SUCCESS);
}


DWORD FC String::Get(LPCHAR lpString,ULONG ulStartIndex,ULONG ulEndIndex)
{
	if(ulStartIndex>=lSize-1 || ulEndIndex>lSize || ulEndIndex<ulStartIndex)
		return(S_FAILED);

	memcpy(lpString,startAddress+ulStartIndex,(ulEndIndex-ulStartIndex)+1);
	lpString[(ulEndIndex-ulStartIndex)+1]=NULLCHAR;
	return(S_SUCCESS);
}
	

DWORD FC String::Get(LPSTRING lpsString,ULONG ulStartIndex,ULONG ulEndIndex)
{
	if(ulStartIndex>=lSize-1 || ulEndIndex>lSize || ulEndIndex<ulStartIndex)
		return(S_FAILED);

	LPCHAR lpcTemp=new CHAR[lSize];
	memset((LPVOID) lpcTemp,'\x0',lSize);
	Get(lpcTemp,ulStartIndex,ulEndIndex);

	lpsString->Set(lpcTemp);

	//delete[] lpcTemp;
	
	return(S_SUCCESS);
}

DWORD FC String::GetNoNull(LPCHAR lpString,ULONG ulStartIndex,ULONG ulEndIndex)
{
	if(ulStartIndex>=lSize-1 || ulEndIndex>lSize || ulEndIndex<ulStartIndex)
		return(S_FAILED);

	memcpy(lpString,startAddress+ulStartIndex,(ulEndIndex-ulStartIndex)+1);
	return(S_SUCCESS);
}

CHAR FC String::GetChar(ULONG ulCharIndex)
{
	if(ulCharIndex>=lSize-1)
		return(NULLCHAR);

	return(startAddress[ulCharIndex]);
}


LPCHAR FC String::GetRawChar(ULONG ulCharIndex)
{
	if(ulCharIndex>=lSize-1)
		return(NULL);

	return(startAddress+ulCharIndex);
}



LPCHAR FC String::GetRawString()
{
	return(startAddress);
}
	


VOID FC String::Append(LPCHAR lpString)
{
	if(!startAddress)
	{
		Set(lpString);
		return;
	}
	

	LPCHAR	newStart;
	LPCHAR	temp;
	LONG	appSize=_Find(lpString,NULLCHAR)-1;
		
	newStart=new CHAR[appSize+lSize];
	
	
	memcpy((LPVOID) newStart,(LPVOID) startAddress,lSize-1);
	temp=(newStart+lSize)-1;
	memcpy((LPVOID) temp,(LPVOID) lpString,appSize+1);

	delete [] startAddress;

	endAddress=startAddress=newStart;
	lSize+=appSize;
	endAddress+=lSize-1;

}

VOID FC String::Append(String &sString)
{
	Append(sString.startAddress);	
}


VOID FC String::Append(CHAR cChar)
{
	CHAR temp[2]={cChar,NULLCHAR};
	Append(temp);
}


VOID FC String::Append(LONG lConvert)
{
	LPCHAR lpcTemp=new char[MAXLONGCHARS];
	ULONG ulSize;
	ltoa(lConvert,lpcTemp,10);
	ulSize=_Find(lpcTemp,NULLCHAR);

	Append(lpcTemp);
	delete[] lpcTemp;
}

VOID FC String::Append(FLOAT fConvert)
{
	LPCHAR lpcTemp=new char[MAXFLOATCHARS];
	ULONG ulSize;
	sprintf(lpcTemp,"%g",fConvert);
	ulSize=_Find(lpcTemp,NULLCHAR);

	Append(lpcTemp);
	delete[] lpcTemp;
}

VOID FC String::Append(double dConvert,UINT uiPrecision)
{
	LPCHAR lpcTemp=new char[MAXDOUBLECHARS];
	ULONG ulSize;
	sprintf(lpcTemp,"%f",dConvert);
	ulSize=_Find(lpcTemp,NULLCHAR);

	Append(lpcTemp);
	delete[] lpcTemp;
}

VOID FC String::Append(DWORD dwConvert)
{
	LPCHAR lpcTemp=new char[(2*DWORDSIZEOF)+1];
	ULONG ulSize;
	sprintf(lpcTemp,"%p",(LPVOID) dwConvert);
	ulSize=_Find(lpcTemp,NULLCHAR);

	Append(lpcTemp);
	delete[] lpcTemp;
}


VOID FC String::AppendCurrDate()
{
	char dbuffer [9];
	_strdate( dbuffer );
	Append(dbuffer);	
}


VOID FC String::AppendCurrTime()
{
	char tbuffer [9];
	_strtime( tbuffer );
	Append(tbuffer);	
}


DWORD FC String::Insert(ULONG ulInsertIndex,LPCHAR lpString)
{
	
	if(lSize)
	{
		if(ulInsertIndex>=lSize-1)
			return(S_FAILED);
	
		ULONG ulSize;
		ulSize=_Find(lpString,NULLCHAR);

		LPCHAR newStart=new CHAR[lSize+ulSize];
		memcpy((LPVOID)newStart,(LPVOID)startAddress,ulInsertIndex);
		memcpy((LPVOID)(newStart+ulInsertIndex),lpString,ulSize-1);
		memcpy((LPVOID)(newStart+ulInsertIndex+ulSize-1),(LPVOID) (startAddress+ulInsertIndex),lSize-ulInsertIndex);
		
		lSize+=ulSize-1;
		endAddress=newStart+lSize-1;

		delete[] startAddress;
		startAddress=newStart;
				
		return(S_SUCCESS);
	}
	else
	{
		Set(lpString);
		return(S_SUCCESS);
	}
	
	return(S_FAILED);
	
}


DWORD FC String::Insert(ULONG ulInsertIndex,String &sString)
{
	return(Insert(ulInsertIndex,sString.startAddress));
}



DWORD FC String::Insert(ULONG ulInsertIndex,CHAR cChar)
{
	LPCHAR lpcTemp=new char[2];
	DWORD dwRet;
	lpcTemp[0]=cChar;
	lpcTemp[1]=NULLCHAR;
	dwRet=Insert(ulInsertIndex,lpcTemp);
	return(dwRet);
}

DWORD FC String::Insert(ULONG ulInsertIndex,LONG lConvert)
{
	LPCHAR lpcTemp=new char[MAXLONGCHARS];
	DWORD dwRet;
	ltoa(lConvert,lpcTemp,10);
	dwRet=Insert(ulInsertIndex,lpcTemp);
	return(dwRet);
}

DWORD FC String::Insert(ULONG ulInsertIndex,FLOAT fConvert)
{
	LPCHAR lpcTemp=new char[MAXFLOATCHARS];
	DWORD dwRet;
	sprintf(lpcTemp,"%g",fConvert);
	dwRet=Insert(ulInsertIndex,lpcTemp);
	delete[]lpcTemp;
	return(dwRet);
	
}

DWORD FC String::Insert(ULONG ulInsertIndex,double dConvert,UINT uiPrecision)
{
	LPCHAR lpcTemp=new char[MAXFLOATCHARS];
	DWORD dwRet;
	sprintf(lpcTemp,"%f",dConvert);
	dwRet=Insert(ulInsertIndex,lpcTemp);
	delete[] lpcTemp;
	return(dwRet);
}

DWORD FC String::Insert(ULONG ulInsertIndex,DWORD dwConvert)
{
	LPCHAR lpcTemp=new char[(2*DWORDSIZEOF)+1];
	DWORD dwRet;
	sprintf(lpcTemp,"%p",dwConvert);
	
	dwRet=Insert(ulInsertIndex,lpcTemp);
	delete[] lpcTemp;
	return(dwRet);
}

	

DWORD FC String::Strip(ULONG ulStripIndex)
{
	if(ulStripIndex>=lSize-1)
			return(S_FAILED);
	
	lSize=ulStripIndex+1;
	endAddress=(startAddress+lSize)-1;
	*endAddress=NULLCHAR;

	return(S_SUCCESS);
}


DWORD FC String::StripChars(ULONG ulCharCount)
{
	if(ulCharCount>lSize-1)
		return(S_FAILED);
	
	lSize-=ulCharCount;
	endAddress=(startAddress+lSize)-1;
	*endAddress=NULLCHAR;
	
	return(S_SUCCESS);
}


DWORD FC String::Delete(ULONG ulDelIndex,ULONG ulDelSize)
{
	if(ulDelIndex>=lSize-1)
		return(S_FAILED);

	memcpy((LPVOID) (startAddress+ulDelIndex),(LPVOID) (startAddress+ulDelIndex+ulDelSize),lSize-(ulDelIndex+ulDelSize));
	lSize-=ulDelSize-1;
	endAddress=startAddress+lSize-1;
	*endAddress=NULLCHAR;

	return(S_FAILED);
}

	


DWORD FC String::Find(CHAR cChar,ULONG &ulSearchResult)
{
	ulSearchResult=0;	

	while(ulSearchResult<lSize-1)
	{
		if(startAddress[ulSearchResult++]==cChar)
		{
			ulSearchResult--;
			return(S_SUCCESS);
		}
	}

	
	return(S_FAILED);	
}

DWORD FC String::FindResume(CHAR cChar,ULONG &ulSearchResult)
{
		
	while(ulSearchResult<lSize)
	{
		if(startAddress[ulSearchResult++]==cChar)
		{
			ulSearchResult--;
			return(S_SUCCESS);
		}
	}

	
	return(S_FAILED);	
}


DWORD FC String::FindBackward(CHAR cChar,ULONG &ulSearchResult)
{
	
	ulSearchResult=lSize-1;	
	

	while(ulSearchResult>=0)
	{
		if(startAddress[ulSearchResult--]==cChar)
		{
			ulSearchResult++;
			return(S_SUCCESS);
		}
		if(ulSearchResult==0)
		{
			if(*startAddress==cChar)
				return(S_SUCCESS);
			break;
		}

	
	}
	

	return(S_FAILED);	

}

DWORD FC String::FindBackwardResume(CHAR cChar,ULONG &ulSearchResult)
{
		
	
	while(ulSearchResult>=0)
	{
		if(startAddress[ulSearchResult--]==cChar)
		{
			ulSearchResult++;
			return(S_SUCCESS);
		}
	}

	return(S_FAILED);	
}


BOOL FC String::toHex(DWORD& dwHex)
{
	LPCHAR lpcChars=new char[MAXDOUBLECHARS];
	Get((LPCHAR) lpcChars);
	int success=sscanf(lpcChars,"%x",&dwHex);
	delete[] lpcChars;
	return(success!=0);
}


BOOL FC String::toUlong(ULONG& ulULong)
{
	LPCHAR lpcChars=new char[MAXDOUBLECHARS];
	Get(lpcChars);
	int success=sscanf(lpcChars,"%u",&ulULong);
	delete[] lpcChars;
	return(success!=0);
}

BOOL FC String::toLong(LONG& lLong)
{
	LPCHAR lpcChars=new char[MAXDOUBLECHARS];
	Get(lpcChars);
	int success=sscanf(lpcChars,"%d",&lLong);
	delete[] lpcChars;
	return(success!=0);
}

BOOL FC String::toFloat(FLOAT& fFloat)
{
	LPCHAR lpcChars=new char[MAXDOUBLECHARS];
	Get(lpcChars);
	int success=sscanf(lpcChars,"%g",&fFloat);
	delete[] lpcChars;
	return(success!=0);
}



BOOL FC String::Equals(String &sCompare)
{
	if(sCompare.lSize!=lSize)
		return(FALSE);
	
	return(memcmp(startAddress,sCompare.startAddress,lSize)==0);

}


BOOL FC String::Equals(LPCHAR lpString)
{
	ULONG ulSize;
	ulSize=_Find(lpString,NULLCHAR);
		
	if(lSize!=lSize)
		return(FALSE);
	
	return( memcmp(startAddress,(LPCHAR) lpString,ulSize)==0);
}


CClassArray<String>* FC String::Tokenize(CHAR cSeparator,ULONG expectedTokenCount)
{
	CClassArray<String>* lpTokens=new CClassArray<String>();

	lpTokens->Allocate((WORD)expectedTokenCount);
	
	
	ULONG nextSeparatorPosition=0;
	ULONG prevSeparatorPosition=0;
	ULONG ulDummy=0;
	String* lpToken=NULL;

	while(FindResume(cSeparator,nextSeparatorPosition)!=S_FAILED)
	{
		lpToken=lpTokens->NewAutoSize();
		Get(lpToken,prevSeparatorPosition,nextSeparatorPosition-1);
		prevSeparatorPosition=++nextSeparatorPosition;

	}

	//put in the last token 
	lpToken=lpTokens->NewAutoSize();
	Get(lpToken,prevSeparatorPosition,Size());
	
	return(lpTokens);
}



ULONG FC String::Size()
{
	return(lSize);
}


VOID FC String::Free()
{
	if(startAddress)
	{
		delete []startAddress;
		startAddress=endAddress=NULL;
		lSize=0;
	}
}

BOOL FC String::MakeUpperCase()
{
	if(startAddress==NULL)
		return(FALSE);

	for(int i=lSize-1;i>=0;i--)
	{
		startAddress[i]=toupper(startAddress[i]);
	}

	return(TRUE);

}

BOOL FC String::MakeLowerCase()
{
	if(startAddress==NULL)
		return(FALSE);

	for(int i=lSize-1;i>=0;i--)
	{
		startAddress[i]=tolower(startAddress[i]);
	}

	return(TRUE);

}


VOID String::Reset()
{
	Free();
}


DWORD String::CreateHashKey()
{
	DWORD ret = 0;
	
	if(startAddress==NULL)
		return(ret);

	for(int i=lSize-1;i>=0;i--)
	{
		ret += startAddress[i] + (i * 255);
	}

	return(ret);

}

LPCHAR String::toNewLPCHAR()
{
	LPCHAR ret = NULL;

	if(lSize <= 0)
	{
		return(NULL);
	}

	ret = new CHAR[lSize];
	memcpy(ret,startAddress,lSize);

	return(ret);
}


//////////////////////////////////////////////////////////////////////
// Private Functions
//////////////////////////////////////////////////////////////////////


ULONG String::_Find(LPCHAR lpcChars,CHAR &cChar)
{
	ULONG ulIndex=0;
	
	while(lpcChars[ulIndex++]!=cChar);
	
	return(ulIndex);
}

