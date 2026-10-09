#pragma once 
#include <Cache/Entry.h>
#include "ECalcMisc.h"

namespace MathModels
{
class ECALC_API CCacherComparingStruct
{
public:
	int mDim;
	int mType;
	CCacherComparingStruct(int _dim, int _type):mDim(_dim),mType(_type){};
	friend bool ECALC_API operator<(const CCacherComparingStruct& _lhs, const CCacherComparingStruct& _rhs);
};
bool ECALC_API operator<(const CCacherComparingStruct& _lhs, const CCacherComparingStruct& _rhs);
}//namespace MathModels
