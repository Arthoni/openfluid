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
  @file GitImportWorker.cpp

  @author Aline LIBRES <aline.libres@gmail.com>
  @author Armel THÖNI <armel.thoni@inrae.fr>
 */


#include <QCoreApplication>
#include <QFileInfo>

#include <openfluid/ui/waresdev/GitImportWorker.hpp>
#include <openfluid/base/WorkspaceManager.hpp>
#include <openfluid/utils/GitProxy.hpp>


namespace openfluid { namespace ui { namespace waresdev {


GitImportWorker::GitImportWorker(bool SslNoVerify) :
    m_SslNoVerify(SslNoVerify)
{

}


// =====================================================================
// =====================================================================


GitImportWorker::~GitImportWorker()
{

}


// =====================================================================
// =====================================================================


QString GitImportWorker::getUsername() const
{
  return m_Username;
}


// =====================================================================
// =====================================================================


void GitImportWorker::setSelectedWaresUrl(
  const std::map<openfluid::ware::WareType, QStringList>& SelectedWaresUrlByType)
{
  m_SelectedWaresUrlByType = SelectedWaresUrlByType;
}


// =====================================================================
// =====================================================================


bool GitImportWorker::onCloneRequest()
{

  double ProgressRatio = 100;
  int SelectedWarePathsNb = 0;
  for (const auto& Pair : m_SelectedWaresUrlByType)
  {
    SelectedWarePathsNb += Pair.second.size();
  }
  if (SelectedWarePathsNb)
  {
    ProgressRatio /= SelectedWarePathsNb;
  }

  int Progress = 0;
  bool OK = true;

  for (const auto& Pair : m_SelectedWaresUrlByType)
  {
    for (const auto& GitUrl : Pair.second)
    {
      if (!importWorkflow(GitUrl, Pair.first))
      {
        OK = false;
        break;
      }

      Progress += ProgressRatio;
      emit progressed(Progress);
    }
  }

  if (OK)
  {
    emit finished(true,tr("Import completed"));
  }
  else
  {
    emit finished(false, tr("Import failed"));
  }

  if (qApp && qApp->thread() != thread())
  {
    moveToThread(qApp->thread());
  }

  return OK;
}


} } } // namespaces
