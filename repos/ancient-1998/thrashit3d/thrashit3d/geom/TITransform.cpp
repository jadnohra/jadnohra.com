#include "TITransform.h"
#include "TIGeometry.h"


VOID TIInlineTranslate(TIBasicVertex& vertex,TIBasicVertex& trans)
{
	vertex.x+=trans.x;
	vertex.y+=trans.y;
	vertex.z+=trans.z;
}

VOID TIInlineTranslate(TIBasicVertex& vertex,TIBasicVertex& trans,TIBasicVertex& result)
{
	result.x=vertex.x+trans.x;
	result.y=vertex.y+trans.y;
	result.z=vertex.z+trans.z;
}

VOID TIInlineNegativeTranslate(TIBasicVertex& vertex,TIBasicVertex& trans)
{
	vertex.x-=trans.x;
	vertex.y-=trans.y;
	vertex.z-=trans.z;
}

VOID TIInlineNegativeTranslate(TIBasicVertex& vertex,TIBasicVertex& trans,TIBasicVertex& result)
{
	result.x=vertex.x-trans.x;
	result.y=vertex.y-trans.y;
	result.z=vertex.z-trans.z;
}



VOID TIInlineRotate(TIBasicVertex vertex,TIBasicVertex& trans,TIBasicVertex& resultVertex)
{
		
	// X axis rotation 
	resultVertex.y = (FLOAT)((vertex.y*TIGEOMETRY_COS(trans.x)) - (vertex.z*TIGEOMETRY_SIN(trans.x)));  
	resultVertex.z = (FLOAT)((vertex.z*TIGEOMETRY_COS(trans.x)) + (vertex.y*TIGEOMETRY_SIN(trans.x)));

	//(Copy NewY and NewZ into OldY and OldZ)
	vertex.y=resultVertex.y;
	vertex.z=resultVertex.z;
	//Y axis rotation 

	resultVertex.z = (FLOAT)((vertex.z*TIGEOMETRY_COS(trans.y)) - (vertex.x*TIGEOMETRY_SIN(trans.y)));  
	resultVertex.x = (FLOAT)((vertex.x*TIGEOMETRY_COS(trans.y)) + (vertex.z*TIGEOMETRY_SIN(trans.y)));

	//(Copy NewZ and NewX into OldZ and OldX)
	vertex.x=resultVertex.x;
	vertex.z=resultVertex.z;
	
	// Z axis rotation 
	resultVertex.x = (FLOAT)((vertex.x*TIGEOMETRY_COS(trans.z)) - (vertex.y*TIGEOMETRY_SIN(trans.z))); 
	resultVertex.y = (FLOAT)((vertex.y*TIGEOMETRY_COS(trans.z)) + (vertex.x*TIGEOMETRY_SIN(trans.z)));

}


VOID TIInlineAcceleratedRotate(TIBasicVertex vertex,TIAcceleratedRotation& trans,TIBasicVertex& resultVertex)
{
		
	// X axis rotation 
	resultVertex.y = (FLOAT)((vertex.y*(trans.x.Cosine)) - (vertex.z*(trans.x.Sine)));  
	resultVertex.z = (FLOAT)((vertex.z*(trans.x.Cosine)) + (vertex.y*(trans.x.Sine)));

	//(Copy NewY and NewZ into OldY and OldZ)
	vertex.y=resultVertex.y;
	vertex.z=resultVertex.z;
	//Y axis rotation 

	resultVertex.z = (FLOAT)((vertex.z*(trans.y.Cosine)) - (vertex.x*(trans.y.Sine)));  
	resultVertex.x = (FLOAT)((vertex.x*(trans.y.Cosine)) + (vertex.z*(trans.y.Sine)));

	//(Copy NewZ and NewX into OldZ and OldX)
	vertex.x=resultVertex.x;
	vertex.z=resultVertex.z;
	
	// Z axis rotation 
	resultVertex.x = (FLOAT)((vertex.x*(trans.z.Cosine)) - (vertex.y*(trans.z.Sine))); 
	resultVertex.y = (FLOAT)((vertex.y*(trans.z.Cosine)) + (vertex.x*(trans.z.Sine)));

}



VOID TIInlineRotate(TIBasicVertex& vertex,TIBasicVertex& trans)
{
	TIBasicVertex temp=vertex;
	
	// X axis rotation 
	vertex.y = (FLOAT)((temp.y*TIGEOMETRY_COS(trans.x)) - (temp.z*TIGEOMETRY_SIN(trans.x)));  
	vertex.z = (FLOAT)((temp.z*TIGEOMETRY_COS(trans.x)) + (temp.y*TIGEOMETRY_SIN(trans.x)));

	//(Copy NewY and NewZ into OldY and OldZ)
	temp.y=vertex.y;
	temp.z=vertex.z;
	//Y axis rotation 

	vertex.z = (FLOAT)((temp.z*TIGEOMETRY_COS(trans.y)) - (temp.x*TIGEOMETRY_SIN(trans.y)));  
	vertex.x = (FLOAT)((temp.x*TIGEOMETRY_COS(trans.y)) + (temp.z*TIGEOMETRY_SIN(trans.y)));

	//(Copy NewZ and NewX into OldZ and OldX)
	temp.x=vertex.x;
	temp.z=vertex.z;
	
	// Z axis rotation 
	vertex.x = (FLOAT)((temp.x*TIGEOMETRY_COS(trans.z)) - (temp.y*TIGEOMETRY_SIN(trans.z))); 
	vertex.y = (FLOAT)((temp.y*TIGEOMETRY_COS(trans.z)) + (temp.x*TIGEOMETRY_SIN(trans.z)));

}



VOID TIInlineNegativeRotate(TIBasicVertex vertex,TIBasicVertex& trans,TIBasicVertex& resultVertex)
{
		
	// X axis rotation 
	resultVertex.y = (FLOAT)((vertex.y*TIGEOMETRY_COS(-trans.x)) - (vertex.z*TIGEOMETRY_SIN(-trans.x)));  
	resultVertex.z = (FLOAT)((vertex.z*TIGEOMETRY_COS(-trans.x)) + (vertex.y*TIGEOMETRY_SIN(-trans.x)));

	//(Copy NewY and NewZ into OldY and OldZ)
	vertex.y=resultVertex.y;
	vertex.z=resultVertex.z;
	//Y axis rotation 

	resultVertex.z = (FLOAT)((vertex.z*TIGEOMETRY_COS(-trans.y)) - (vertex.x*TIGEOMETRY_SIN(-trans.y)));  
	resultVertex.x = (FLOAT)((vertex.x*TIGEOMETRY_COS(-trans.y)) + (vertex.z*TIGEOMETRY_SIN(-trans.y)));

	//(Copy NewZ and NewX into OldZ and OldX)
	vertex.x=resultVertex.x;
	vertex.z=resultVertex.z;
	
	// Z axis rotation 
	resultVertex.x = (FLOAT)((vertex.x*TIGEOMETRY_COS(-trans.z)) - (vertex.y*TIGEOMETRY_SIN(-trans.z))); 
	resultVertex.y = (FLOAT)((vertex.y*TIGEOMETRY_COS(-trans.z)) + (vertex.x*TIGEOMETRY_SIN(-trans.z)));

}


VOID TIInlineNegativeRotate(TIBasicVertex& vertex,TIBasicVertex& trans)
{
	TIBasicVertex temp=vertex;
	
	// X axis rotation 
	vertex.y = (FLOAT)((temp.y*TIGEOMETRY_COS(-trans.x)) - (temp.z*TIGEOMETRY_SIN(-trans.x)));  
	vertex.z = (FLOAT)((temp.z*TIGEOMETRY_COS(-trans.x)) + (temp.y*TIGEOMETRY_SIN(-trans.x)));

	//(Copy NewY and NewZ into OldY and OldZ)
	temp.y=vertex.y;
	temp.z=vertex.z;
	//Y axis rotation 

	vertex.z = (FLOAT)((temp.z*TIGEOMETRY_COS(-trans.y)) - (temp.x*TIGEOMETRY_SIN(-trans.y)));  
	vertex.x = (FLOAT)((temp.x*TIGEOMETRY_COS(-trans.y)) + (temp.z*TIGEOMETRY_SIN(-trans.y)));

	//(Copy NewZ and NewX into OldZ and OldX)
	temp.x=vertex.x;
	temp.z=vertex.z;
	
	// Z axis rotation 
	vertex.x = (FLOAT)((temp.x*TIGEOMETRY_COS(-trans.z)) - (temp.y*TIGEOMETRY_SIN(-trans.z))); 
	vertex.y = (FLOAT)((temp.y*TIGEOMETRY_COS(-trans.z)) + (temp.x*TIGEOMETRY_SIN(-trans.z)));

}





VOID TIInlineProject(int& Focal,short& ViewCenterX,short& ViewCenterY,TIBasicVertex& vertex,D2Vector<float>& result)
{
	if(vertex.z)
	{
		result.x = (Focal * vertex.x / vertex.z + ViewCenterX);
		result.y = (Focal * -vertex.y / vertex.z + ViewCenterY);
	}
	else
	{
		result.x = (Focal * vertex.x + ViewCenterX);
		result.y = (Focal * -vertex.y + ViewCenterY);
	}


}

