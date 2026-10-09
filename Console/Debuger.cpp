// Debuger.cpp : Defines the entry point for the console application.
//


//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>
#include "HelperFunctions.h"

int main()
{
	//TestMoments();
	//TestEcalc();
	//TestPLevel();
	
	initDumper();
	std::ofstream out("result.log");
	MathModels::TLDotCacher ldotCacher;
	MathModels::TInfoCacher infoCacher;
	//TestInfoMatrix();
	//TestWideGenerator();

	TSCGenerator::doubles distrib;
	setDistribution(distrib);
	TSCGenerator::doubles pbefore;
	double pch;
	TSCGenerator::doubles pafter;
	setWideDistribution(pbefore,pch,pafter);
	int minNumOfInd=0;
	int maxNumOfInd=1;
	int stepNumOfInd=5;
	int fixNumofInd=155;
	int numberOfIter=8000;
	long startTime = std::clock();
	long lastStamp=startTime;
	for (int numOfInd = minNumOfInd; numOfInd < maxNumOfInd; numOfInd += stepNumOfInd)
	{
		//Generator of the sample. Takes vector of doubles for parameters of p0..p-2, pch, padv and random start seed
		auto_ptr<TSCGenerator::CGenerator>generator(new TSCGenerator::CGenerator(distrib,std::clock()));
		//auto_ptr<TSCGenerator::CWideGenerator>generator(new TSCGenerator::CWideGenerator(pbefore,pch,pafter,std::clock()));
		//Set size of generated sample
		generator->SetSize(fixNumofInd);//*/numOfInd);
		
		//parameter sets number of iteration, sends to boudnedSample to reserve enough space
		
		//Sample that could have invalid values with none groupping algorithm
		auto_ptr<Distributions::CBoundedSample> noneSample(new Distributions::CBoundedSample(numberOfIter));
		//Sample that could have invalid values with standard groupping algorithm
		auto_ptr<Distributions::CBoundedSample> standardSample(new Distributions::CBoundedSample(numberOfIter));
		//Sample that could have invalid values with shrinked none groupping algorithm
		auto_ptr<Distributions::CBoundedSample> shrinkSample(new Distributions::CBoundedSample(numberOfIter));

		//Validate that values only between (0,1)
		noneSample->Validator(new Distributions::CProbabilitySampleValidator());
		standardSample->Validator(new Distributions::CProbabilitySampleValidator());
		shrinkSample->Validator(new Distributions::CProbabilitySampleValidator());
		
		vector<PBoundedSample > initEstimates(distrib.size());
		for (size_t i=0; i<initEstimates.size(); ++i)
		{
			initEstimates[i].reset(new Distributions::CBoundedSample());
			initEstimates[i]->Validator(new Distributions::CProbabilitySampleValidator());
		}

		vector<PBoundedSample > noneEstimates(distrib.size());
		for (size_t i=0; i<noneEstimates.size(); ++i)
		{
			noneEstimates[i].reset(new Distributions::CBoundedSample());
			noneEstimates[i]->Validator(new Distributions::CProbabilitySampleValidator());
		}
		vector<PBoundedSample > standardEstimates((int)distrib.size());
		for (size_t i=0; i<standardEstimates.size(); ++i)
		{
			standardEstimates[i].reset(new Distributions::CBoundedSample());
			standardEstimates[i]->Validator(new Distributions::CProbabilitySampleValidator());
		}
		vector<PBoundedSample > shrinkEstimates(4,PBoundedSample(new Distributions::CBoundedSample()));
		for (size_t i=0; i<shrinkEstimates.size(); ++i)
		{
			shrinkEstimates[i].reset(new Distributions::CBoundedSample());
			shrinkEstimates[i]->Validator(new Distributions::CProbabilitySampleValidator());
		}
		vector<PBoundedSample > shrinkInitialEstimates(4,PBoundedSample(new Distributions::CBoundedSample()));
		for (size_t i=0; i<shrinkInitialEstimates.size(); ++i)
		{
			shrinkInitialEstimates[i].reset(new Distributions::CBoundedSample());
			shrinkInitialEstimates[i]->Validator(new Distributions::CProbabilitySampleValidator());
		}

		//auto_ptr<GroupedTSC::IModelRep> altModelRep(new GroupedTSC::CTSCModel());

		for (int i=0;i<numberOfIter;++i)
		{
			long curStamp=clock();
			if (curStamp-lastStamp>60000)
			{
				lastStamp=curStamp;
				cout<<i<<endl;
			}
			//Create none group model
			Tsc::TableManager::CTableManager<double> *sampleManager = Tsc::TableManager::TSampleManagerFactory::CreateFromGenerator(generator.get());//::CreateFromFile("D:\\TSC\\Result\\exceptional.txt");//
			Tsc::TableManager::CTableManager<double> *sampleManagerCopy = new Tsc::TableManager::CTableManager<double>(*sampleManager);
			try
			{
				auto_ptr<GroupedTSC::CModel> model(GroupedTSC::CModel::CreateCModel(sampleManager->GetWidth(),
														sampleManager,
														false,
														new GroupedTSC::CTSCModel(sampleManager->GetWidth(),ETSCModel,ldotCacher,infoCacher)//*/GroupedTSC::CTSCTrunkModel()
														));
				//setSimleEstimation(model.get(),initEstimates);
				setEstimation(model.get(),noneEstimates,noneSample.get());
				try
				{
					//Create standard subgroup for same sample
					auto_ptr<GroupedTSC::CModel> groupStrd(model->CreateSubModel(Tsc::Grouping::StandardGroup<unsigned int,double>(ETSCStdGroupModel)));
					setEstimation(groupStrd.get(),standardEstimates,standardSample.get());
				}
				catch (exception& )
				{
					standardSample->InsertInvalid();
				}
			}
			catch (exception& )
			{
				for (int i=0;i<(int)generator->GetSample().size(); ++i)
				{
					cout<<generator->GetSample()[i]<<" ";
				}
				cout<<endl;
				noneSample->InsertInvalid();
				standardSample->InsertInvalid();
			}
			////Shrink model could work without NoneModel
			//try
			//{
			//	Tsc::Grouping::ShrinkGroup<unsigned int, double> grouping;
			//	grouping.setSize(0);
			//	grouping.setStates(vector<double>(sampleManagerCopy->GetWidth()*sampleManagerCopy->GetHeight()));
			//	grouping.setLevel(0);
			//	grouping.setMinSize(0);
			//	PLevel::uints tempGroup = grouping();
			//	vector<int> group(tempGroup.begin(),tempGroup.end()); 
			//	Tsc::TableManager::CTableManager<double> *shrinkSampleManager = sampleManagerCopy->CreateJoinCrosses(&group.front());
			//	auto_ptr<GroupedTSC::CModel> groupShrink(
			//		GroupedTSC::CModel::CreateCModel(shrinkSampleManager->GetWidth(),
			//		shrinkSampleManager, 
			//		false, 
			//		new GroupedTSC::CTSCModel(shrinkSampleManager->GetWidth(),ETSCSrnkGroupModel,ldotCacher,infoCacher)//*/GroupedTSC::CTSCTrunkModel(),
			//		));

			//	setSimleEstimation(groupShrink.get(),shrinkInitialEstimates);
			//	groupShrink->ProcessInitialCalculation(false);
			//	auto_ptr<GroupedTSC::CModel> groupShrinkStrd(groupShrink->CreateSubModel(Tsc::Grouping::StandardGroup<unsigned int,double>(ETSCSrnkStdGroupModel)));
			//	setEstimation(groupShrinkStrd.get(),shrinkEstimates,shrinkSample.get());
			//}
			//catch (exception&)
			//{
			//	for (int i=0;i<(int)generator->GetSample().size(); ++i)
			//	{
			//		cout<<generator->GetSample()[i]<<" ";
			//	}
			//	cout<<endl;
			//}
		}
		if(numberOfIter>0)
		{
			cout<<numOfInd<<" ";
			out<<numOfInd<<" ";
			//printEstimates(distrib, out, initEstimates, noneEstimates, standardEstimates, shrinkInitialEstimates, shrinkEstimates);

			printPLevelResults(noneSample.get(),"Simple",out);
			printPLevelResults(standardSample.get(),"Standard",out);
			//printPLevelResults(shrinkSample.get(),"Shrink",out);
			cout<<endl;
			out<<endl;
		}
	}
	return 0;
}

