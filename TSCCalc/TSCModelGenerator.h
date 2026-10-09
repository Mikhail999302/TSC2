//Generation of symbolic representation of a TSC model for current sizes.
#pragma once
#include "stdafx.h"
#include "Typedefs.h"
#include <TableManager\Entry.h>

namespace GroupedTSC
{
class TSCCALC_DLLENTRY CTSCModelGenerator :
	public Tsc::TableManager::IDataLoader<Tsc::Formula::CMathExpString<MathString> >
{
	int mWidth;
	int mHeight;
public:
	CTSCModelGenerator(int _width, int _height);
	virtual ~CTSCModelGenerator(void);
	virtual TModelManager* CreateTableManager();
	virtual IDataLoader<Tsc::Formula::CMathExpString<MathString> >* Clone()
	{
		return new CTSCModelGenerator(mWidth,mHeight);
	}
};
}//namespace GroupedTSC