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
  @file FluidHubWaresImportWorker.cpp

  @author Aline LIBRES <aline.libres@gmail.com>
  @author Armel THONI <armel.thoni@inrae.fr>
 */

#include <fstream>
#include <filesystem>

#include <QCoreApplication>
#include <QFileInfo>
#include <QtWidgets/QDialog>
#include <QtWidgets/QInputDialog> 

#include <openfluid/tools/DataHelpers.hpp>
#include <openfluid/waresdev/FluidHubWaresImportWorker.hpp>
#include <openfluid/base/WorkspaceManager.hpp>
#include <openfluid/utils/GitProxy.hpp>
#include <openfluid/tools/Filesystem.hpp>


namespace openfluid { namespace waresdev {


FluidHubWaresImportWorker::FluidHubWaresImportWorker(const QString& WareshubUrl, bool SslNoVerify) :
    mp_HubClient(new openfluid::utils::FluidHubAPIClient()), m_HubUrl(WareshubUrl), m_SslNoVerify(SslNoVerify)
{

}


// =====================================================================
// =====================================================================


FluidHubWaresImportWorker::~FluidHubWaresImportWorker()
{
  delete mp_HubClient;
}


// =====================================================================
// =====================================================================


bool FluidHubWaresImportWorker::isConnected() const
{
  return mp_HubClient->isConnected();
}


// =====================================================================
// =====================================================================


bool FluidHubWaresImportWorker::isLoggedIn() const
{
  return !(m_Username.isEmpty() || m_Username.isNull());
}


// =====================================================================
// =====================================================================


bool FluidHubWaresImportWorker::isV0ofAPI() const
{
  return mp_HubClient->isV0ofAPI();
}


// =====================================================================
// =====================================================================


QString FluidHubWaresImportWorker::getUsername() const
{
  return m_Username;
}


// =====================================================================
// =====================================================================


openfluid::utils::FluidHubAPIClient::WaresDetailsByID_t FluidHubWaresImportWorker::getAvailableWaresWithDetails(
  openfluid::ware::WareType Type) const
  {
  return m_AvailableWaresDetailsByIDByType.value(Type, openfluid::utils::FluidHubAPIClient::WaresDetailsByID_t());
}


// =====================================================================
// =====================================================================


void FluidHubWaresImportWorker::setSelectedWaresUrl(
  const std::map<openfluid::ware::WareType, QStringList>& SelectedWaresUrlByType)
{
  m_SelectedWaresUrlByType = SelectedWaresUrlByType;
}


// =====================================================================
// =====================================================================


bool FluidHubWaresImportWorker::connect()
{
  m_AvailableWaresDetailsByIDByType.clear();

  openfluid::utils::RESTClient::SSLConfiguration SSLConfig;
  if (m_SslNoVerify)
  {
    SSLConfig.setCertificateVerifyMode(QSslSocket::VerifyNone);
  }

  bool Ok = mp_HubClient->connect(m_HubUrl, SSLConfig);

  for (const auto& ByType : mp_HubClient->getAllAvailableWares())
  {
    m_AvailableWaresDetailsByIDByType[ByType.first] = mp_HubClient->getAvailableWaresWithDetails(ByType.first, 
                                                                                                      m_Username);  
  }

  if (!Ok)
  {
    emit finished(false, tr("Fetching information failed"));
  }
  else
  {
    emit finished(true, tr("Fetching information completed"));
  }

  if (qApp && qApp->thread() != thread())
  {
    moveToThread(qApp->thread());
  }

  return Ok;
}


// =====================================================================
// =====================================================================


void FluidHubWaresImportWorker::disconnect()
{
  mp_HubClient->disconnect();
}


// =====================================================================
// =====================================================================


bool FluidHubWaresImportWorker::login(const QString& Username, const QString& Password)
{
  m_Password = Password;
  
  if (isV0ofAPI())
  {
    m_Username = Username;
  }
  else
  {
    //fetch unixname from email via request /account/, field unixname
    m_Username = QString::fromStdString(mp_HubClient->getUserUnixname(Username.toStdString(), 
                                                                            m_Password.toStdString()));
  }

  return !m_Username.isEmpty(); // returns true when username found
}


// =====================================================================
// =====================================================================


void FluidHubWaresImportWorker::logout()
{
  m_Username = "";
  m_Password = "";
  mp_HubClient->logout();
}


// =====================================================================
// =====================================================================


bool FluidHubWaresImportWorker::clone()
{
  if (!isConnected())
  {
    return false;
  }

  auto WksMgr = openfluid::base::WorkspaceManager::instance();

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
  bool Ok = true;

  for (const auto& Pair : m_SelectedWaresUrlByType)
  {
    QString WareTypePath = QString::fromStdString(WksMgr->getWaresPath(Pair.first));

    for (const auto& GitUrl : Pair.second)
    {
      QString DestPath = QString("%1/%2").arg(WareTypePath).arg(QFileInfo(GitUrl).fileName());
      
      openfluid::utils::GitProxy Git;
      QObject::connect(&Git, SIGNAL(info(const QString&)), this, SIGNAL(info(const QString&)));
      QObject::connect(&Git, SIGNAL(error(const QString&)), this, SIGNAL(error(const QString&)));

      if (!Git.clone(GitUrl, DestPath, m_Username, m_Password, m_SslNoVerify))
      {
        Ok = false;
        break;
      }

      // TODO add submodule check here (to be moved in a more appropriate place, share with import from package?)
      // 1) check if .submodule file found
      //    std::cout << DestPath.toStdString() << std::endl;
      //    - conditions for a fragment: existing in submodule + (TODO) in fragments/
      std::vector<std::pair<std::string, std::string>> SubmodulesData;
      std::string ModuleFile = DestPath.toStdString() + "/.gitmodules";
      if (openfluid::tools::Filesystem::isFile(ModuleFile))
      {
        //std::cout << "Is submodule!" << std::endl;
        std::ifstream FileContent(ModuleFile.c_str());
        std::string StrLine;
        // check if file is "openable"
        if (!FileContent)
        {
          std::cout << "No content in gitmodules file" << std::endl;
          //return false;
        }
        bool InSubsection = false;
        std::string CurrentPath;
        std::string CurrentUrl;
        while(std::getline(FileContent, StrLine))
        {
          if (StrLine.find("[submodule") != std::string::npos) {
            InSubsection = true;
          }
          else if (InSubsection && StrLine.find("path =") != std::string::npos)
          {
            CurrentPath = openfluid::tools::splitString(StrLine, " = ")[1];
            if (CurrentPath.length() > 0 && CurrentUrl.length() > 0)
            {
              SubmodulesData.push_back(std::make_pair(CurrentPath, CurrentUrl));
              CurrentPath = "";
              CurrentUrl = "";
              InSubsection = false;
            }
          }
          else if (InSubsection && StrLine.find("url =") != std::string::npos)
          {
            CurrentUrl = openfluid::tools::splitString(StrLine, " = ")[1];
            if (CurrentPath.length() > 0 && CurrentUrl.length() > 0)
            {
              SubmodulesData.push_back(std::make_pair(CurrentPath, CurrentUrl));
              CurrentPath = "";
              CurrentUrl = "";
              InSubsection = false;
            }
          }
        }
      }
      else
      {
        std::cout << "No submodule :(" << std::endl;
      }
      // TODO check if fragment found in existing layers (current workspace + .openfluid)
      
      // 2) ask location to be set or follow default setting
      // TODO request level of addition
      int WantedLevel = 1; // 0 = local, 1 = workspace, 2 = global
      bool ok;
      QStringList items;
      std::vector<std::pair<std::string, int>> levels = {{"locally in ware", 0}, 
                                           {"shared in current workspace", 1}, 
                                           {"shared in .openfluid", 2}};
      for (const auto& level : levels)
      {
        items << QString::fromStdString(level.first);
      }

      QString text = QInputDialog::getItem(0, tr("Fragment import"),
                                         tr("Import level:"), items, 0, false, &ok);
      if (ok && !text.isEmpty())
      {
        for (const auto& level : levels)
        {
          if (level.first == text.toStdString())
          {
            WantedLevel = level.second;
          }
        }
      }
      std::cout << "WANTED LEVEL: " << WantedLevel << std::endl;


      // 3) move submodule and link correctly accordingly
      // cf https://stackoverflow.com/questions/27379818/git-possible-to-use-same-submodule-working-copy-by-multiple-projects/27384763#27384763
      if (WantedLevel > 0)
      {
        for (const auto& sm : SubmodulesData)
        {
          std::cout << "DETECTED: " << sm.first << " | " << sm.second << std::endl;
          if (WantedLevel == 1)
          {
            // Move to workspace level
            // a- clone isolated in workspace/fragments
            QString WareTypePathSub = Mgr->getWareTypePath(openfluid::ware::WareType::FRAGMENT);
            QString DestPathSub = QString("%1/%2").arg(WareTypePathSub).arg(QFileInfo(QString::fromStdString(sm.second)).fileName());

            if (openfluid::tools::Filesystem::isDirectory(DestPathSub.toStdString()))
            {
              std::cout << "Sub already there: " << DestPathSub.toStdString() << std::endl;
            }
            else
            {
              std::cout << "Cloning sub in " << DestPathSub.toStdString() << std::endl;
              // TODO BE CAREFUL: can be on a wrong commit (not the wanted targetted by the current commit)
              // can be done with "git submodule status" to get the commit number and do a checkout in the fragment folder (if new, otherwise ask, but issue with which one is the most recent)
              openfluid::utils::GitProxy Git;
              QObject::connect(&Git, SIGNAL(info(const QString&)), this, SIGNAL(info(const QString&)));
              QObject::connect(&Git, SIGNAL(error(const QString&)), this, SIGNAL(error(const QString&)));
              if (!Git.clone(QString::fromStdString(sm.second), DestPathSub, m_Username, m_Password, m_SslNoVerify))
              {
                Ok = false;
                break;
              }
            }

            // b- set up parent projet to use the .git of the isolated submodule
            //location of the local submodule to be adjusted: sm.first 
            //std::filesystem::remove_all(DestPath.toStdString() + "/" + sm.first); // DANGEROUS, check that was not there before submodule work
            std::string dir_path = DestPath.toStdString() + "/" + sm.first;
            std::cout << "DELETING ALL? " << dir_path << std::endl;
            for (const auto& entry : std::filesystem::directory_iterator(dir_path)) 
            {
              std::filesystem::remove_all(entry.path());
            }
            // replacing .git
            std::ofstream myfile;
            myfile.open(dir_path + "/.git");
            myfile << "gitdir: " << DestPathSub.toStdString() << "/.git";
            myfile.close();
            // ADD file with explicit name like "DELOCALIZED_SUBMODULE.txt" and remember the effective location?)
            myfile.open(dir_path + "/DELOCALIZED_SUBMODULE");
            myfile << "Fragment is linked with: " << DestPathSub.toStdString();
            myfile.close();
            
            //TODO WAY TO CHANGE THE LEVEL OF FRAGMENT FROM DS ? (user defined it as workspace-level but now wants to have it at local level)

            // TODO ADD TAB FOR FRAGMENTS IN WARE MANAGER?
          }
          else if (WantedLevel == 2)
          {
            // Move to global level
            // TODO
          }
          // TODO
        }
      }

      Progress += ProgressRatio;
      emit progressed(Progress);
    }
  }

  if (Ok)
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

  return Ok;
}


} } // namespaces
