#ifndef _TITRANSFORM_H__
#define _TITRANSFORM_H__

#include "TIBasicVertex.h"
#include "TIAngle.h"
#include "..\engine\TIDefs.h"
#include "math.h"


class TITransform
{
};

inline VOID TIInlineTranslate(TIBasicVertex& vertex,TIBasicVertex& trans);
inline VOID TIInlineTranslate(TIBasicVertex& vertex,TIBasicVertex& trans,TIBasicVertex& result);
inline VOID TIInlineNegativeTranslate(TIBasicVertex& vertex,TIBasicVertex& trans);
inline VOID TIInlineNegativeTranslate(TIBasicVertex& vertex,TIBasicVertex& trans,TIBasicVertex& result);
inline VOID TIInlineRotate(TIBasicVertex vertex,TIBasicVertex& trans,TIBasicVertex& resultVertex);
inline VOID TIInlineRotate(TIBasicVertex& vertex,TIBasicVertex& trans);
inline VOID TIInlineNegativeRotate(TIBasicVertex vertex,TIBasicVertex& trans,TIBasicVertex& resultVertex);
inline VOID TIInlineNegativeRotate(TIBasicVertex& vertex,TIBasicVertex& trans);
inline VOID TIInlineProject(int& Focal,short& ViewCenterX,short& ViewCenterY,TIBasicVertex& vertex,D2Vector<float>& result);

inline VOID TIInlineAcceleratedRotate(TIBasicVertex vertex,TIAcceleratedRotation& trans,TIBasicVertex& resultVertex);

#endif