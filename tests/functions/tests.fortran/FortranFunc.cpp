/*
  This file is part of OpenFLUID software
  Copyright (c) 2007-2010 INRA-Montpellier SupAgro


 == GNU General Public License Usage ==

  OpenFLUID is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  OpenFLUID is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with OpenFLUID.  If not, see <http://www.gnu.org/licenses/>.

  In addition, as a special exception, INRA gives You the additional right
  to dynamically link the code of OpenFLUID with code not covered
  under the GNU General Public License ("Non-GPL Code") and to distribute
  linked combinations including the two, subject to the limitations in this
  paragraph. Non-GPL Code permitted under this exception must only link to
  the code of OpenFLUID dynamically through the OpenFLUID libraries
  interfaces, and only for building OpenFLUID plugins. The files of
  Non-GPL Code may be link to the OpenFLUID libraries without causing the
  resulting work to be covered by the GNU General Public License. You must
  obey the GNU General Public License in all respects for all of the
  OpenFLUID code and other code used in conjunction with OpenFLUID
  except the Non-GPL Code covered by this exception. If you modify
  this OpenFLUID, you may extend this exception to your version of the file,
  but you are not obligated to do so. If you do not wish to provide this
  exception without modification, you must delete this exception statement
  from your version and license this OpenFLUID solely under the GPL without
  exception.


 == Other Usage ==

  Other Usage means a use of OpenFLUID that is inconsistent with the GPL
  license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID may use this file in accordance
  with the terms contained in the written agreement between You and INRA.
*/


/**
  \file FortranFunc.cpp
  \brief Implements ...
*/


#include "FortranFunc.h"
#include <openfluid/tools.hpp>
#include <cmath>


// =====================================================================
// =====================================================================


DEFINE_FUNCTION_HOOK(FortranFunction)


// =====================================================================
// =====================================================================


BEGIN_SIGNATURE_HOOK
  DECLARE_SIGNATURE_ID(("tests.fortran"));
  DECLARE_SIGNATURE_NAME(("test function for fortran code wrapping"));
  DECLARE_SIGNATURE_DESCRIPTION((""));

  DECLARE_SIGNATURE_VERSION(("1.0"));
  DECLARE_SIGNATURE_SDKVERSION;
  DECLARE_SIGNATURE_STATUS(openfluid::base::EXPERIMENTAL);

  DECLARE_SIGNATURE_DOMAIN((""));
  DECLARE_SIGNATURE_PROCESS((""));
  DECLARE_SIGNATURE_METHOD((""));
  DECLARE_SIGNATURE_AUTHORNAME((""));
  DECLARE_SIGNATURE_AUTHOREMAIL((""));
END_SIGNATURE_HOOK

// =====================================================================
// =====================================================================


BEGIN_EXTERN_FORTRAN
  EXTERN_FSUBROUTINE(multrealvalue)(FREAL8*,FREAL8*,FREAL8*);
  EXTERN_FSUBROUTINE(multintvalue)(FINT*,FINT*,FINT*);
  EXTERN_FSUBROUTINE(catstrings)(FCHARACTER*,FCHARACTER*,FCHARACTER*);
  EXTERN_FSUBROUTINE(multrealmatrix)(FREAL8*,FINT*,FINT*,FINT*,FREAL8*);
END_EXTERN_FORTRAN


// =====================================================================
// =====================================================================


FortranFunction::FortranFunction()
                : PluggableFunction()
{


}


// =====================================================================
// =====================================================================


FortranFunction::~FortranFunction()
{


}


// =====================================================================
// =====================================================================


bool FortranFunction::initParams(openfluid::core::FuncParamsMap_t Params)
{


  return true;
}

// =====================================================================
// =====================================================================


bool FortranFunction::prepareData()
{


  return true;
}


// =====================================================================
// =====================================================================


bool FortranFunction::checkConsistency()
{


  return true;
}


// =====================================================================
// =====================================================================


bool FortranFunction::initializeRun(const openfluid::base::SimulationInfo* SimInfo)
{

  m_Precision = 0.000001;

  return true;
}

// =====================================================================
// =====================================================================


bool FortranFunction::runStep(const openfluid::base::SimulationStatus* SimStatus)
{
  int i;


  // ====== double ======

  double DValue, DMult, DResult;
  DValue = 1.5436;
  DMult = 2.5;
  DResult = 0.0;

  CALL_FSUBROUTINE(multrealvalue)(&DValue,&DMult,&DResult);

  if (std::abs(DResult - (DValue*DMult)) > m_Precision)
    OPENFLUID_RaiseError("tests.fortran","incorrect fortran call (multrealvalue)");


  // ====== int ======

  int IValue, IMult, IResult;
  IValue = 45;
  IMult = 18;
  IResult = 0;

  CALL_FSUBROUTINE(multintvalue)(&IValue,&IMult,&IResult);

  if (IResult != (IValue*IMult))
    OPENFLUID_RaiseError("tests.fortran","incorrect fortran call (multintvalue)");


  // ====== string ======

  // TODO finish this
/*  std::string SStr1, SStr2, SResult;
  char* CResult;
  SStr1 = "Hello";
  SStr2 = "from fortran";

  CALL_FSUBROUTINE(catstrings)(STD2FSTRING(SStr1),STD2FSTRING(SStr2),CResult);

  SResult = std::string(CResult);

  if (SResult != (SStr1 + " " + SStr2))
    OPENFLUID_RaiseError("tests.fortran","incorrect fortran call (catstrings)");
*/


  // ====== matrix ======

  int MMult, MDim1,MDim2;
  double *MValue;
  double *MResult;
  MMult = 3;
  MDim1 = 2;
  MDim2 = 3;


  MValue = new double[MDim1*MDim2];
  MResult = new double[MDim1*MDim2];

  for (i=0; i < MDim1*MDim2;i++) MValue[i] = 1.5;
  for (i=0; i < MDim1*MDim2;i++) MResult[i] = 0.0;

  CALL_FSUBROUTINE(multrealmatrix)(MValue,&MDim1,&MDim2,&MMult,MResult);

  for (i=0; i < MDim1*MDim2;i++)
  {
    if (std::abs(MResult[i] - (MValue[i] * MMult)) > m_Precision)
      OPENFLUID_RaiseError("tests.fortran","incorrect fortran call (multrealmatrix)");
  }

  return true;
}

// =====================================================================
// =====================================================================


bool FortranFunction::finalizeRun(const openfluid::base::SimulationInfo* SimInfo)
{


  return true;
}

