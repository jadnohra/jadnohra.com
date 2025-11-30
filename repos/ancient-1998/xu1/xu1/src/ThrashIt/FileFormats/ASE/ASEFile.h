#ifndef _ASEFILE_H__
#define _ASEFILE_H__


#include "ASEFileComponentsBasic.h"



class ASEFile : public ASEFileComponent
{
protected:
	ASEFileComponentManager Components;

public:

	ASEFileComponent_3DSMAX_EXPORT	_3DSMAX_EXPORT;
	ASEFileComponent_COMMENT		_COMMENT;
	ASEFileComponent_SCENE			_SCENE;
	ASEFileComponent_MATERIAL_LIST	_MATERIAL_LIST;
	ASEFileComponent_GEOMOBJECT		_GEOMOBJECT ;

		
	ASEFile();
	virtual ~ASEFile();

	BOOL			Process(LPCHAR FileName);
	virtual BOOL	Process(AsciiFileParser* lpFile);
	virtual BOOL	GetKey(String& str);
};

#endif