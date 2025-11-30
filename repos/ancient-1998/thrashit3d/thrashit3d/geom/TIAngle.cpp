#include "TIAngle.h"
#include "TIGeometry.h"


int TIInlineDegToTIAngle(float deg)
{
	if(deg < 0) {
		float multi = (-deg / THRASHIT3D_2PIDEG);
		deg += THRASHIT3D_2PIDEG * ((int) multi + 1);
	} 
	else 
	{
		if(deg >= THRASHIT3D_2PIDEG) {
			float multi = (deg / THRASHIT3D_2PIDEG);
			deg -= THRASHIT3D_2PIDEG * (int) (multi);
		}
	}
	
	return( THRASHIT3D_DEGTOTIANGLE(deg) );

}

int TIInlineRadToTIAngle(float rad)
{
	if(rad < 0) {
		float multi = (-rad / THRASHIT3D_2PIRAD);
		rad += THRASHIT3D_2PIRAD * ((int) multi + 1);
	} 
	else 
	{
		if(rad >= THRASHIT3D_2PIRAD) {
			float multi = (rad / THRASHIT3D_2PIRAD);
			rad -= THRASHIT3D_2PIRAD * (int) (multi);
		}
	}
	
	return( THRASHIT3D_RADTOTIANGLE(rad) );

}


VOID TIInlineDegToTIAngle(float& deg,TIAcceleratedAngle& angle)
{
	angle.Cosine	= TIGEOMETRY_COS(THRASHIT3D_DEGTORAD(deg));
	angle.Sine		= TIGEOMETRY_SIN(THRASHIT3D_DEGTORAD(deg));

	angle.TIAngle	= TIInlineDegToTIAngle(deg);
}


VOID TIInlineRadToTIAngle(float& rad,TIAcceleratedAngle& angle)
{
	angle.Cosine	= TIGEOMETRY_COS(rad);
	angle.Sine		= TIGEOMETRY_SIN(rad);

	angle.TIAngle	= TIInlineRadToTIAngle(rad);
}


FLOAT TIInlineTIAngleToDeg(int& angle)
{
	return( THRASHIT3D_TIANGLETODEG(angle) );	
}

FLOAT TIInlineTIAngleToRad(int& angle)
{
	return( THRASHIT3D_TIANGLETORAD(angle) );	
}


VOID TIInlineDegToTIRotation(float degX,float degY,float degZ,TIAcceleratedRotation& angle)
{
	TIInlineDegToTIAngle(degX,angle.x);
	TIInlineDegToTIAngle(degY,angle.y);
	TIInlineDegToTIAngle(degZ,angle.z);
}

VOID TIInlineRadToTIRotation(float radX,float radY,float radZ,TIAcceleratedRotation& angle)
{
	TIInlineRadToTIAngle(radX,angle.x);
	TIInlineRadToTIAngle(radY,angle.y);
	TIInlineRadToTIAngle(radZ,angle.z);
}
