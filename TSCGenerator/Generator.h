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
		inline void InitModel(void);
		inline void GenerateOne(void);
		void TransposeSample(void);

	private:
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

}//namespace TSCGenetator
