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
  @file FragmentImportWorker.hpp

  @author Armel THÖNI <armel.thoni@inrae.fr>
 */


#ifndef __OPENFLUID_UIWARESDEV_FRAGMENTIMPORTWORKER_HPP__
#define __OPENFLUID_UIWARESDEV_FRAGMENTIMPORTWORKER_HPP__


#include <QObject>
#include <QMap>
#include <QStringList>

#include <openfluid/dllexport.hpp>
#include <openfluid/utils/FluidHubAPIClient.hpp>
#include <openfluid/ui/waresdev/GitImportWorker.hpp>


namespace openfluid { namespace ui { namespace waresdev {


class OPENFLUID_API FragmentImportWorker: public GitImportWorker
{
  Q_OBJECT

  private:

    bool m_AsSubmodule;

    QString m_RootPath;


  protected:

    bool importWorkflow(QString GitUrl, openfluid::ware::WareType Type);


  public:

    FragmentImportWorker(bool SslNoVerify = false, bool AsSubmodule = true);

    ~FragmentImportWorker();

    void setup(const QString& Username, 
               const QString& Password,
               const QString& RootPath);
};


} } } // namespaces


#endif /* __OPENFLUID_UIWARESDEV_FRAGMENTIMPORTWORKER_HPP__ */
