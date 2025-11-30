#ifndef _TIANGLE_H__
#define _TIANGLE_H__

#include "..\..\DirectX\DXDraw\DXDraw.h"
	
struct TIAcceleratedAngle
{
	int		TIAngle;
	float	Sine;
	float	Cosine;

};


struct TIAcceleratedRotation : public D3Vector<TIAcceleratedAngle>
{
};


inline int TIInlineDegToTIAngle(float deg);	//if u know ur angle is 0<= a < 360 u can use the macro
inline int TIInlineRadToTIAngle(float rad);	//if u know ur angle is 0<= a < THRAHIT3D_2PI u can use the macro


inline VOID TIInlineDegToTIAngle(float& deg,TIAcceleratedAngle& angle);
inline VOID TIInlineRadToTIAngle(float& rad,TIAcceleratedAngle& angle);

inline FLOAT TIInlineTIAngleToDeg(int& angle);
inline FLOAT TIInlineTIAngleToRad(int& angle);

inline VOID TIInlineDegToTIRotation(float degX,float degY,float degZ,TIAcceleratedRotation& angle);
inline VOID TIInlineRadToTIRotation(float radX,float radY,float radZ,TIAcceleratedRotation& angle);


#endif