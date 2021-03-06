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
  @file WFSSourceAddDialog.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#ifndef __WFSSOURCEADDDIALOG_HPP__
#define __WFSSOURCEADDDIALOG_HPP__


#include "SourceAddDialog.hpp"


class WFSSourceAddDialog : public SourceAddDialog
{
  Q_OBJECT;

  private slots:

    void connectToWFS();

    bool prepareToImport();


  private:

    void generateXMLConfigFile(const QString& URL);

    QString getXMLConfigFilePath();

    QString m_TempDir;

    QString m_CurrentUsername;

    QString m_CurrentPassword;

    QString m_RealSourceURI;


  protected:

    void updateAfterOpen();


  public:

    WFSSourceAddDialog(const QString& TempDir, QWidget* Parent);

    ~WFSSourceAddDialog();

};

#endif /* __WFSSOURCEADDDIALOG_HPP__ */
