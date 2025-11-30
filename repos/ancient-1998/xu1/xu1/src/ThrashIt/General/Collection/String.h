// String.h: interface for the String class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _STRING_H__
#define _STRING_H__


#include "..\_INCLUDES.h"
#include "CArray.h"

#define LPSTRING String*
static CHAR NULLCHAR = 0;


/*
the data is stored with ending null
*/

class String : public CClassArrayElement
{

protected:
	inline ULONG String::_Find(LPCHAR lpcChars,CHAR &cChar);

	
public:
	LPCHAR	startAddress;	
	LPCHAR	endAddress;		//points to the null char
	ULONG	lSize;			//total num of chars (null is a char)

	
	String();
	String(LPCHAR lpString);
	String(String &sString);
	String(LONG lConvert);
	String(FLOAT fConvert);
	String(double dConvert,UINT uiPrecision);
	String(DWORD dwConvert);
	String(CHAR cChar);


	virtual ~String();

	VOID FC Set(LPCHAR lpString,int charCount);
	VOID FC Set(LPCHAR lpString);
	VOID FC Set(String &sString);
	VOID FC Set(CHAR cChar);
	VOID FC Set(LONG lConvert);
	VOID FC Set(FLOAT fConvert);
	VOID FC Set(double dConvert,UINT uiPrecision);
	VOID FC Set(DWORD dwConvert);
	VOID FC SetCurrDate();
	VOID FC SetCurrTime();
	
	
	DWORD FC SetChar(ULONG ulCharIndex,CHAR cChar);

	
	VOID FC Get(LPCHAR lpString);		//with null ending char
	VOID FC Get(LPSTRING lpsString);
	VOID FC GetNoNull(LPCHAR lpString);//without the ending null
	
	DWORD FC Get(LPCHAR lpString,ULONG ulStartIndex);		//with null ending char
	DWORD FC Get(LPSTRING lpsString,ULONG ulStartIndex);
	DWORD FC GetNoNull(LPCHAR lpString,ULONG ulStartIndex);//without the ending null
	
	DWORD FC Get(LPCHAR lpString,ULONG ulStartIndex,ULONG ulEndIndex);		//with null ending char
	DWORD FC Get(LPSTRING lpsString,ULONG ulStartIndex,ULONG ulEndIndex);
	DWORD FC GetNoNull(LPCHAR lpString,ULONG ulStartIndex,ULONG ulEndIndex);//without the ending null
	
	DWORD CreateHashKey();
	
	CHAR FC GetChar(ULONG ulCharIndex);

	LPCHAR FC GetRawString();	//returns a pointer to the characters we hold (no null) -> modifying them modifies the string
	LPCHAR FC GetRawChar(ULONG ulCharIndex);
	
	
	VOID FC Append(LPCHAR lpString);
	VOID FC Append(String &sString);
	VOID FC Append(CHAR cChar);
	VOID FC Append(LONG lConvert);
	VOID FC Append(FLOAT fConvert);
	VOID FC Append(double dConvert,UINT uiPrecision);
	VOID FC Append(DWORD dwConvert);
	VOID FC AppendCurrDate();
	VOID FC AppendCurrTime();

	
	
	DWORD FC Insert(ULONG ulInsertIndex,LPCHAR lpString);
	DWORD FC Insert(ULONG ulInsertIndex,String &sString);
	DWORD FC Insert(ULONG ulInsertIndex,CHAR cChar);
	DWORD FC Insert(ULONG ulInsertIndex,LONG lConvert);
	DWORD FC Insert(ULONG ulInsertIndex,FLOAT fConvert);
	DWORD FC Insert(ULONG ulInsertIndex,double dConvert,UINT uiPrecision);
	DWORD FC Insert(ULONG ulInsertIndex,DWORD dwConvert);
	
	
	DWORD FC Strip(ULONG ulStripIndex);
	DWORD FC StripChars(ULONG ulCharCount);
	DWORD FC Delete(ULONG ulDelIndex,ULONG ulDelSize);
	
	
	DWORD FC Find(CHAR cChar,ULONG &ulSearchResult);
	DWORD FC FindResume(CHAR cChar,ULONG &ulSearchResult);//starts search at ulSeachResult
	DWORD FC FindBackward(CHAR cChar,ULONG &ulSearchResult);
	DWORD FC FindBackwardResume(CHAR cChar,ULONG &ulSearchResult);//starts search at ulSeachResult

	
	BOOL FC Equals(String &sCompare);
	BOOL FC Equals(LPCHAR lpString);

	

	BOOL FC toHex(DWORD& dwHex);
	BOOL FC toUlong(ULONG& ulULong);
	BOOL FC toLong(LONG& lLong);
	BOOL FC toFloat(FLOAT& fFLoat);
	LPCHAR toNewLPCHAR();
	
	BOOL FC MakeUpperCase();
	BOOL FC MakeLowerCase();
	
	ULONG FC Size();

	VOID FC Free();
	virtual VOID Reset();	//CClassArrayElement support

	CClassArray<String>* FC Tokenize(CHAR cSeparator,ULONG expectedTokenCount);
	
	/*
	LPCHAR operatorLPCHAR()
	operator=
	operator+=
	operator==
	operator!=
	*/

};


#endif	// !defined _STRING_H__