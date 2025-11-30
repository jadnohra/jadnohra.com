#include "..\_INCLUDES.h"
#include "Fraction.h"


VOID Fraction::Set(int num)
{
	Dividend=num;
	Divider=1;
}


VOID Fraction::Add(int dividend,int divider)
{
			
	if(Divider==divider)
	{
		Dividend+=dividend;
		if(Dividend==0)
			Divider=1;

		return;
	}

	if(Divider>divider)
	{
		if( (FLOAT) ((FLOAT) Divider / (FLOAT) divider) == (int) ((FLOAT) Divider / (FLOAT) divider) )
		{
			Dividend+=dividend*((int) ((FLOAT) Divider / (FLOAT) divider));

			if(Dividend==0)
				Divider=1;

			return;
		}

	}
	else
	{
		if( (FLOAT) ((FLOAT) divider / (FLOAT) Divider) == (int) ((FLOAT) divider / (FLOAT) Divider) )
		{
			Dividend=dividend+( Dividend*((int) ((FLOAT) divider / (FLOAT) Divider)) );
			Divider=divider;

			if(Dividend==0)
				Divider=1;

			return;
		}	
	}



	Dividend=Dividend*divider+dividend*Divider;
	Divider=divider*Divider;



	if( Dividend && ((FLOAT) ((FLOAT) Divider / (FLOAT) Dividend) == (int) ((FLOAT) Divider / (FLOAT) Dividend) ))
	{
		Dividend/=Divider;
		Divider=1;
	}
	else
	{

		if(Dividend==0)
			Divider=1;
	}
}
