//Generates the Pii by p0,..,pn-2,pch,padv
//for input use vector<double>
//////////////////////////////////////////

#pragma once
#include "IGenerator.h"
#include "distribution.h"

namespace TSCGenerator
{
	class /*TSCGEN_DLLENTRY*/ CGeneratorException:public std::exception
	{
		virtual const char *what( ) const throw( ){return "Uninitialized!";};
	};
#ifdef USE_PRAGMA
#pragma warning( disable : 4231 ) //disable warnings on extern before template instantiation
#endif // USE_PRAGMA
	TSCGEN_DLLENTRYTEMPLATE template class TSCGEN_DLLENTRY std::allocator< double >;
	TSCGEN_DLLENTRYTEMPLATE template class TSCGEN_DLLENTRY std::vector< double >;
#ifdef USE_PRAGMA
#pragma warning( default : 4231 ) //disable warnings on extern before template instantiation
#endif // USE_PRAGMA

	class TSCGEN_DLLENTRY CGenerator: public IGenerator
	{
	public:
		CGenerator(const doubles& distribution_,int seed_):
			distribution(distribution_),alpha(new CUniformDist(seed_)),
			poly(0),isInit(false)
			{sample.resize((distribution.size()-1)*(distribution.size()-1));}
		~CGenerator(void){delete poly;delete alpha;}
		CGenerator(const CGenerator& gen);
		CGenerator& operator=(const CGenerator& gen);

		// Generate pij using current settings but new size
		inline void Generate(int size_){size=size_;Generate();}
		void SetSize(int _size){size=_size;}
		// Generate pij using current settings
		inline void Generate(void);

		//get output
		const doubles& GetSample(void){return sample;}
		void SaveSample(const char * fileName,bool isTtranspose=false);
		const doubles& GetDistribution(void){return distribution;}

		const doubles& GetP(void){if(isInit)return pi;throw CGeneratorException();}

	protected:
		inline virtual void InitModel(void);
		inline virtual void GenerateOne(void);
		void TransposeSample(void);

	protected:
		bool isInit;
		doubles distribution;//p0,pn-2,pch,padv as vector watch the order
		doubles sample;//square matrix of pij as one long array n^2
		doubles pi;
		double padv,pch;
		int size;//sample size
		int dim;//sample dimention;
		CUniformDist *alpha;
		CChenUFDD *poly;
	};

	//****************************************
	//******  class CGenerator_1Pos   ********
	//model A: positive persistence with adv. Kernel equals base CGenerator.
	//****************************************
	class TSCGEN_DLLENTRY CGenerator_1Pos: public CGenerator
	{
	public:
		CGenerator_1Pos(const doubles& distribution_,int seed_):CGenerator(distribution_, seed_)
		{sample.resize((distribution.size()-1)*(distribution.size()-1));}
		virtual ~CGenerator_1Pos(void){/*delete poly;delete alpha;*/}
		CGenerator_1Pos(const CGenerator_1Pos& gen);
		CGenerator_1Pos& operator=(const CGenerator_1Pos& gen);

		const doubles& GetP(void){if(isInit)return pi;throw CGeneratorException();}

	protected:
		inline virtual void InitModel(void);
		inline virtual void GenerateOne(void);
	};

	//****************************************
	//****  class CGenerator_1Pos_mine   *****
	//model B (mine, edit 11.04): P(j|i) = (1-pch)*[j==i] + pch*padv*[j==0] + pch*(1-padv)*p_j
	//****************************************
	class TSCGEN_DLLENTRY CGenerator_1Pos_mine : public CGenerator_1Pos
	{
	public:
		CGenerator_1Pos_mine(const doubles& distribution_, int seed_) :CGenerator_1Pos(distribution_, seed_) {};
		virtual ~CGenerator_1Pos_mine(void) {/*delete poly;delete alpha;*/ }
		CGenerator_1Pos_mine(const CGenerator_1Pos_mine& gen);
		CGenerator_1Pos_mine& operator=(const CGenerator_1Pos_mine& gen);

	protected:
		inline virtual void GenerateOne(void) override;
	};
}//namespace TSCGenetator
