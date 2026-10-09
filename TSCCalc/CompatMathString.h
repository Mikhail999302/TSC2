// Build shim (wired through project settings via ForcedIncludeFiles).
//
// A few member-function definitions in this tree spell their return type
// without a namespace, e.g.
//     MathString GroupedTSC::CTSCModel::SetEstimatesFormulasStrRep(int);
//     CMathParameterValues GroupedTSC::CTSCModel::SetSimpleEstimates(...);
// The original compiler found those names through the class scope
// (GroupedTSC re-exports MathModels via "using namespace MathModels;",
// and MathModels re-exports MathCalc::MathString). Conforming MSVC (v142
// here) looks the return type up at namespace scope instead, where the
// bare names are not visible, producing C2872 / implicit-int errors.
//
// Forcing this header in front of every translation unit restores global
// visibility for those names. They resolve to the very same entities, so no
// genuine ambiguity is introduced.
#pragma once
#include "stdafx.h"

using MathCalc::MathString;
using MathModels::CMathParameterValues;
