#include ".\generator.h"

void TSCGenerator::CGenerator::Generate(void)
{
	InitModel();
	for(int i=size;i-->0;)GenerateOne();
	//TransposeSample();
}

void TSCGenerator::CGenerator::InitModel(void)
{
	pi=distribution;
	padv=pi.back();
	pi.pop_back();
	pch=pi.back();
	pi.pop_back();
	double plast=1;
	for(doubles::iterator p=pi.begin();p!=pi.end();++p)plast-=*p;
	pi.push_back(plast);
	delete poly;
	poly=new CChenUFDD(alpha->GetSeed(),pi,(int)pi.size());
	for(doubles::iterator p=sample.begin();p!=sample.end();++p)*p=0;
	dim=(int)sqrt((double)sample.size());
	isInit=true;
}

void TSCGenerator::CGenerator::GenerateOne(void)
{
	if(!isInit)InitModel();
	int first=(int)poly->Next();
	if(alpha->Next()<padv)sample[first*dim]++;
	else
	{
		if(alpha->Next()<pch)sample[(unsigned int)(first*dim+poly->Next())]++;
		else sample[first*dim+first]++;
	}
}

void TSCGenerator::CGenerator::TransposeSample()
{
	int q,r;
	dim=(int)sqrt((double)sample.size());
	for(int i=(int)sample.size();i-->0;)
	{
		if((q=i/dim)<(r=i%dim))std::swap(sample[q*dim+r],sample[r*dim+q]);
	}
}

TSCGenerator::CGenerator::CGenerator(const CGenerator&gen)
{
	distribution=gen.distribution;
	sample=gen.sample;
	size=gen.size;
	alpha=new CUniformDist(gen.alpha->GetSeed());
	isInit=gen.isInit;
	poly=0;
	if(isInit)
	{
		pi=gen.pi;
		padv=gen.padv;
		pch=gen.pch;
		dim=gen.size;
		poly=new CChenUFDD(gen.poly->GetSeed(),gen.pi,gen.poly->GetParam());
	}
}

TSCGenerator::CGenerator& TSCGenerator::CGenerator::operator=(const CGenerator &gen)
{
	if(this==&gen)
		return *this;
	delete alpha;
	distribution=gen.distribution;
	sample=gen.sample;
	size=gen.size>0?gen.size:0;
	alpha=new CUniformDist(gen.alpha->GetSeed());
	delete poly;
	poly=0;
	isInit=gen.isInit;
	if(isInit)
	{
		pi=gen.pi;
		padv=gen.padv;
		pch=gen.pch;
		dim=gen.size;
		poly=new CChenUFDD(gen.poly->GetSeed(),gen.pi,gen.poly->GetParam());
	}
	return *this;
}

void TSCGenerator::CGenerator::SaveSample(const char * fileName,bool isTranspose/*=false*/)
{
	if(isInit)
	{
	std::ofstream fout(fileName);
	int i=0;
	if(isTranspose)TransposeSample();
	dim=(int)sqrt((double)sample.size());
	for(doubles::iterator p=sample.begin();p!=sample.end();p++)
	{
		fout<<(*p)<<" ";
		if(!((++i)%dim))fout<<std::endl;
	}
	}
}