/*

  This file is part of OpenFLUID software
  Copyright(c) 2007, INRA - Montpellier SupAgro


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
  along with OpenFLUID. If not, see <http://www.gnu.org/licenses/>.


 == Other Usage ==

  Other Usage means a use of OpenFLUID that is inconsistent with the GPL
  license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID may use this file in accordance
  with the terms contained in the written agreement between You and INRA.
  
*/


/**
  @file MatrixValue.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#include <openfluid/core/MatrixValue.hpp>


namespace openfluid { namespace core {


MatrixValue& MatrixValue::operator=(const Value& Other)
{
  if (this != &Other)
  {
    const MatrixValue* CastedValue = dynamic_cast<const MatrixValue*>(&Other);

    if (CastedValue)
    {
      Matrix<double>::operator=(static_cast<const Matrix<double>&>(Other.asMatrixValue()));
    }
  }

  return *this;
}


// =====================================================================
// =====================================================================


MatrixValue& MatrixValue::operator=(Value&& Other)
{
  if (this != &Other)
  {
    MatrixValue* CastedValue = dynamic_cast<MatrixValue*>(&Other);

    if (CastedValue)
    {
      Matrix<double>::operator=(static_cast<Matrix<double>&&>(Other.asMatrixValue()));
    }
  }

  return *this;
}


// =====================================================================
// =====================================================================


MatrixValue& MatrixValue::operator=(const MatrixValue& Other)
{
  if (this != &Other)
  {
    Matrix<double>::operator=(static_cast<const Matrix<double>&>(Other));
  }

  return *this;
}


// =====================================================================
// =====================================================================


MatrixValue& MatrixValue::operator=(MatrixValue&& Other)
{
  if (this != &Other)
  {
    Matrix<double>::operator=(static_cast<Matrix<double>&&>(Other));
  }

  return *this;
}


// =====================================================================
// =====================================================================


void MatrixValue::writeToStream(std::ostream& OutStm) const
{
  const unsigned long s = getSize();

  if (s == 0)
  {
    OutStm << "[[]]";
  }
  else
  {
    const unsigned long ColsNbr = getColsNbr();
    const unsigned long RowsNbr = getRowsNbr();

    OutStm << "[";
    for (unsigned int i = 0; i<RowsNbr; i++)
    {
      OutStm << "[";
      for (unsigned int j = 0; j<ColsNbr; j++)
      {
        OutStm << get(j,i);
        if (j != ColsNbr-1)
        {
          OutStm << ",";
        }
      }
      OutStm << "]";

      if (i != RowsNbr-1)
      {
        OutStm << ",";
      }
    }
    OutStm << "]";
  }
}


} }  // namespaces

