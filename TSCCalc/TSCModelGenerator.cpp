#include <boost/algorithm/string/replace.hpp>
#include "stdafx.h"
#include "TSCModelGenerator.h"


///////////Model Generator/////////////////////////////////////////////////////////////////////////////

GroupedTSC::CTSCModelGenerator::CTSCModelGenerator(int _width,int _height):mWidth(_width),mHeight(_height)
{
}

GroupedTSC::CTSCModelGenerator::~CTSCModelGenerator(void)
{
}

GroupedTSC::TModelManager* GroupedTSC::CTSCModelGenerator::CreateTableManager()
{
	MathStrings data;
	data.resize(mWidth*mHeight);
	//Заменить последний элемент на 1-все остальные
	MathString strTemp, Pmax,PmaxEquiv = _T("(1");
	Pmax=boost::str(boost::format(_T("p%1%"))%(mWidth-1));
	//MathCalc::MathStrings::Format(Pmax,Pmax.size(),_T("p%d"), mWidth - 1);
	for (int i = 0; i < mWidth - 1; i++ ) 
	{
		strTemp = boost::str(boost::format(_T("-p%1%"))%i);
		//MathCalc::MathStrings::Format(strTemp,strTemp.size(), _T("-p%d"), i);
		PmaxEquiv += strTemp;
	}
	PmaxEquiv += _T(")");

	MathString P00 = _T("(padv + (1-padv)*(1-pch+pch*p0))");
	MathString Pjj = _T("(1-padv)*(1-pch+pch*pj)");
	MathString P0j = _T("(padv + (1-padv)*pch*pj)");
	MathString Pij = _T("(1-padv)*pch*pj");

	MathString strLine;
	for (int i = 0; i < mHeight; i++ )
		for (int j = 0; j < mWidth; j++ ) 
		{
			strLine = boost::str(boost::format(_T("p%1%*"))%i);
			//MathCalc::MathStrings::Format(strLine,strLine.size(),_T("p%d*"),i);
			if ( i == j ) 
			{
				if ( j == 0 )
					strLine += P00;
				else
					strLine += Pjj;
			}
			else 
			{
				if ( j == 0 )
					strLine += P0j;
				else
					strLine += Pij;
			}
			strTemp = boost::str(boost::format(_T("%d"))%j);
			//MathCalc::MathStrings::Format(strTemp,strTemp.size(),_T("%d"), j);
			boost::replace_all(strLine,_T("j"), strTemp);
			if (i==mHeight-1||j==mWidth-1)
				boost::replace_all(strLine,Pmax, PmaxEquiv);
			data[i*mWidth+j]=strLine;
		}
	std::vector<Tsc::Formula::CMathExpString<MathString> > tempdata(data.begin(),data.end());
	TModelManager *rez=new TModelManager(&tempdata.front(),mWidth,mHeight);
	return rez;
}

//**********************************************
//****   class CTSCModelGenerator_1Pos   *******
//p[i][j] values of model CTSCModel_1Pos (model A)
//**********************************************
GroupedTSC::TModelManager* GroupedTSC::CTSCModelGenerator_1Pos::CreateTableManager()
{
	MathStrings data;
	data.resize(mWidth * mHeight);
	MathString strTemp, Pmax, PmaxEquiv = _T("(1");
	Pmax = boost::str(boost::format(_T("p%1%")) % (mWidth - 1));
	for (int i = 0; i < mWidth - 1; i++)
	{
		strTemp = boost::str(boost::format(_T("-p%1%")) % i);
		PmaxEquiv += strTemp;
	}
	PmaxEquiv += _T(")");

	MathString P00 = _T("(padv + (1-padv)*(1-pch+pch*p0))");
	MathString Pjj = _T("(1-padv)*(1-pch+pch*pj)");
	MathString P0j = _T("(padv + (1-padv)*pch*pj)");
	MathString Pij = _T("(1-padv)*pch*pj");

	MathString strLine;
	for (int i = 0; i < mHeight; i++)
		for (int j = 0; j < mWidth; j++)
		{
			strLine = boost::str(boost::format(_T("p%1%*")) % i);
			if (i == j)
			{
				if (j == 0)
					strLine += P00;
				else
					strLine += Pjj;
			}
			else
			{
				if (j == 0)
					strLine += P0j;
				else
					strLine += Pij;
			}
			strTemp = boost::str(boost::format(_T("%d")) % j);
			boost::replace_all(strLine, _T("j"), strTemp);
			if (i == mHeight - 1 || j == mWidth - 1)
				boost::replace_all(strLine, Pmax, PmaxEquiv);
			data[i * mWidth + j] = strLine;
		}
	std::vector<Tsc::Formula::CMathExpString<MathString> > tempdata(data.begin(), data.end());
	TModelManager* rez = new TModelManager(&tempdata.front(), mWidth, mHeight);
	return rez;
}

//**********************************************
//****  class CTSCModelGenerator_1Pos_mine  *****
//p[i][j] values of model CTSCModel_1Pos_mine (model B)
//**********************************************
GroupedTSC::TModelManager* GroupedTSC::CTSCModelGenerator_1Pos_mine::CreateTableManager()
{
	MathStrings data;
	data.resize(mWidth * mHeight);
	MathString strTemp, Pmax, PmaxEquiv = _T("(1");
	Pmax = boost::str(boost::format(_T("p%1%")) % (mWidth - 1));
	for (int i = 0; i < mWidth - 1; i++)
	{
		strTemp = boost::str(boost::format(_T("-p%1%")) % i);
		PmaxEquiv += strTemp;
	}
	PmaxEquiv += _T(")");

	MathString P00 = _T("((1-pch)+(pch*padv)+pch*(1-padv)*p0)");
	MathString Pjj = _T("((1-pch)+pch*(1-padv)*pj)");
	MathString P0j = _T("(pch*padv+pch*(1-padv)*p0)");
	MathString Pij = _T("pch*(1-padv)*pj");

	MathString strLine;
	for (int i = 0; i < mHeight; i++)
		for (int j = 0; j < mWidth; j++)
		{
			strLine = boost::str(boost::format(_T("p%1%*")) % i);
			if (i == j)
			{
				if (j == 0)
					strLine += P00;
				else
					strLine += Pjj;
			}
			else
			{
				if (j == 0)
					strLine += P0j;
				else
					strLine += Pij;
			}
			strTemp = boost::str(boost::format(_T("%d")) % j);
			boost::replace_all(strLine, _T("j"), strTemp);
			if (i == mHeight - 1 || j == mWidth - 1)
				boost::replace_all(strLine, Pmax, PmaxEquiv);
			data[i * mWidth + j] = strLine;
		}
	std::vector<Tsc::Formula::CMathExpString<MathString> > tempdata(data.begin(), data.end());
	TModelManager* rez = new TModelManager(&tempdata.front(), mWidth, mHeight);
	return rez;
}
