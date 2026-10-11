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
protected:
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
class TSCCALC_DLLENTRY CTSCModelGenerator_1Pos: public CTSCModelGenerator
{
public:
	CTSCModelGenerator_1Pos(int _width, int _height): CTSCModelGenerator(_width, _height){};
	virtual ~CTSCModelGenerator_1Pos(void){};

	virtual TModelManager* CreateTableManager();
};

//'mine' (edit 11.04): p[i][j] values of model B (CTSCModel_1Pos_mine)
class TSCCALC_DLLENTRY CTSCModelGenerator_1Pos_mine : public CTSCModelGenerator_1Pos
{
public:
	CTSCModelGenerator_1Pos_mine(int _width, int _height) : CTSCModelGenerator_1Pos(_width, _height) {};
	virtual ~CTSCModelGenerator_1Pos_mine(void) {};

	virtual TModelManager* CreateTableManager() override;
};
}//namespace GroupedTSC