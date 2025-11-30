#ifndef _FRACTION_H
#define _FRACTION_H

struct Fraction
{
	WORD Dividend;
	WORD Divider;

	VOID Set(int num);
	VOID Add(int dividend,int divider);
	

};	

#endif