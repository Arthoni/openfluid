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
  @file GenericSrcImportDialog.cpp

  @author Aline LIBRES <aline.libres@gmail.com>
  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
  @author Armel THÖNI <armel.thoni@inra.fr>
*/


#include <QFileDialog>
#include <QThread>
#include <QMessageBox>

#include <openfluid/base/PreferencesManager.hpp>
#include <openfluid/base/WorkspaceManager.hpp>
#include <openfluid/tools/FilesystemPath.hpp>
#include <openfluid/utils/GitProxy.hpp>
#include <openfluid/utils/CMakeProxy.hpp>
#include <openfluid/ui/waresdev/FluidHubWaresImportWorker.hpp>
#include <openfluid/ui/waresdev/GenericSrcImportDialog.hpp>
#include <openfluid/ui/waresdev/WaresSrcIOProgressDialog.hpp>
#include <openfluid/ui/config.hpp>
#include <openfluid/waresdev/WareSrcHelpers.hpp>


namespace openfluid { namespace ui { namespace waresdev {


GenericSrcImportDialog::GenericSrcImportDialog(QWidget* Parent) :
    openfluid::ui::common::MessageDialog(Parent)
{
}


// =====================================================================
// =====================================================================


GenericSrcImportDialog::~GenericSrcImportDialog()
{
  delete mp_FluidHubWaresImportWorker;
}


// =====================================================================
// =====================================================================


void GenericSrcImportDialog::clearListWidgets()
{
  for (const auto& WidgetPair : m_ListWidgetsByWareType)
  {
    WidgetPair.second->clear();
  }
}


// =====================================================================
// =====================================================================


bool GenericSrcImportDialog::checkHubSection()
{
  if (hubUrlLineEdit()->text().isEmpty())
  {
    setMessage(tr("No Hub URL defined"));
    hubConnectButton()->setEnabled(false);
    return false;
  }
  else
  {
    hubConnectButton()->setEnabled(true);
  }

  if (!mp_FluidHubWaresImportWorker || !mp_FluidHubWaresImportWorker->isConnected())
  {
    setMessage(tr("Not connected to a Hub"));
    return false;
  }
  setMessage("");
  return true;
}


// =====================================================================
// =====================================================================


void GenericSrcImportDialog::onHubConnectButtonClicked()
{
  if (m_IsConnectedToHub)
  {
    m_IsConnectedToHub = false;

    if (mp_FluidHubWaresImportWorker)
    {
      mp_FluidHubWaresImportWorker->disconnect();
    }

    clearListWidgets();

    hubConnectButton()->setText(m_HubButtonConnectLabel);

    for (auto& Widget : m_HubConnectionInfoWidgets)
    {
      Widget->setEnabled(true);
    }

    m_IsLoggedInHub = false;
    hubLoginButton()->setText(m_HubButtonLoginLabel);

    // Hide login fields
    hubUrlLineEdit()->setEnabled(true);
    for (auto& Widget : m_HubLoginWidgetsAndButton)
    {
      customHideWidget(Widget);
    }

    return;
  }
  else
  {
    QString HubUrl = hubUrlLineEdit()->text();

    if (HubUrl.isEmpty())
    {
      check();
      return;
    }

    if (mp_FluidHubWaresImportWorker)
    {
      delete mp_FluidHubWaresImportWorker;
      mp_FluidHubWaresImportWorker = nullptr;
    }

    QThread* Thread = new QThread();

    openfluid::ui::waresdev::WaresSrcIOProgressDialog ProgressDialog(tr("Connecting to Hub:"), true, this);

    try
    {
      bool SslNoVerify = openfluid::base::PreferencesManager::instance()->isWaresdevGitSslNoVerify();
      mp_FluidHubWaresImportWorker = new FluidHubWaresImportWorker(HubUrl, SslNoVerify);

      mp_FluidHubWaresImportWorker->moveToThread(Thread);

      connect(Thread, SIGNAL(started()), mp_FluidHubWaresImportWorker, SLOT(connect()));
      connect(Thread, SIGNAL(finished()), Thread, SLOT(deleteLater()));

      connect(mp_FluidHubWaresImportWorker, SIGNAL(finished(bool, const QString&)), Thread, SLOT(quit()));
      connect(mp_FluidHubWaresImportWorker, SIGNAL(finished(bool, const QString&)), &ProgressDialog,
              SLOT(finishAndQuit(bool, const QString&)));

      connect(mp_FluidHubWaresImportWorker, SIGNAL(info(const QString&)), &ProgressDialog,
              SLOT(writeInfo(const QString&)));
      connect(mp_FluidHubWaresImportWorker, SIGNAL(error(const QString&)), &ProgressDialog,
              SLOT(writeError(const QString&)));

      Thread->start();
    }
    catch (std::exception& e)
    {
      ProgressDialog.writeError(e.what());
    }

    if (ProgressDialog.exec())
    {
      hubConnectButton()->setText(m_HubButtonDisconnectLabel);
      m_IsConnectedToHub = true;

      hubUrlLineEdit()->setEnabled(false);
      for (auto& Widget : m_HubLoginWidgetsAndButton)
      {
        Widget->setVisible(true);
        Widget->setEnabled(true);
      }

      if (mp_FluidHubWaresImportWorker->isV0ofAPI())
      {
        hubUsernameLabel()->setText(tr("Username"));
      }
      else
      {
        hubUsernameLabel()->setText(tr("User ID"));
        hubUsernameLabel()->setToolTip("User ID for Hub can be the user email or user uid from LDAP");
        usernameLineEdit()->setPlaceholderText("Email / LDAP uid");
      }
    }
  
    updateHubWaresList();

    openfluid::base::PreferencesManager::instance()->setWaresdevImportHubUrl(hubUrlLineEdit()->text().toStdString());
    postConnectSetup();
    check();
  }
}


// =====================================================================
// =====================================================================


void GenericSrcImportDialog::onHubLoginButtonClicked()
{
  if (m_IsLoggedInHub)
  {
    m_IsLoggedInHub = false;
    hubLoginButton()->setText(m_HubButtonLoginLabel);
    for (auto& Widget : m_HubLoginWidgets)
    {
      Widget->setEnabled(true);
    }
    mp_FluidHubWaresImportWorker->logout();
    updateHubWaresList();
    return;
  }
  else
  {
    setMessage();
    if (mp_FluidHubWaresImportWorker)
    {
      if (mp_FluidHubWaresImportWorker->login(usernameLineEdit()->text(), passwordLineEdit()->text()))
      {
        m_IsLoggedInHub = true;
        hubLoginButton()->setText(m_HubButtonLogoutLabel);
        for (auto& Widget : m_HubLoginWidgets)
        {
          Widget->setEnabled(false);
        }
        updateHubWaresList();

        openfluid::base::PreferencesManager::instance()->setWaresdevImportHubUsername(
          usernameLineEdit()->text().toStdString());  // username is added to preferences when validated by login
      }
      else
      {
        QMessageBox::warning(this, tr("Login error"), tr("Unable to log in with given information"));
      }
    }
  }
}


// =====================================================================
// =====================================================================


bool GenericSrcImportDialog::isWareDisplayed(const openfluid::ware::WareType& /* Type */, const QString /* WareId */, 
                                           const bool /*WareInWorkspace */, const bool /* WareNotAuthorized */)
{
  return true;
}


// =====================================================================
// =====================================================================


void GenericSrcImportDialog::updateHubWaresList()
{
  clearListWidgets();

  if (!mp_FluidHubWaresImportWorker)
  {
    return;
  }

  auto Mgr = openfluid::base::WorkspaceManager::instance();

  QString UserName = mp_FluidHubWaresImportWorker->getUsername();
  QString ErrStr;

  for (const auto& Pair : m_ListWidgetsByWareType)
  {
    openfluid::ware::WareType Type = Pair.first;
    for (const auto& WarePair : mp_FluidHubWaresImportWorker->getAvailableWaresWithDetails(Type))
    {
      QString WareId = QString::fromStdString(WarePair.first);
      std::string WarePath = Mgr->getWarePath(Type,WareId.toStdString());
      bool WareInWorkspace = openfluid::waresdev::isWareInWorkspace(WarePath);

      bool WareNotAuthorized = !openfluid::waresdev::hasUserAccess(UserName.toStdString(), 
                                                                   WarePair.second.ROUsers, 
                                                                   WarePair.second.RWUsers); 

      if (isWareDisplayed(Type, WareId, WareInWorkspace, WareNotAuthorized))
      {
        // ware display

        QString WareUrl = QString::fromStdString(WarePair.second.GitUrl);
        QListWidgetItem* Item = new QListWidgetItem(WareId);
        Item->setData(Qt::UserRole, WareUrl);

        if (!wareItemDisplay(Type, WareId, Item))
        {
          if (mp_FluidHubWaresImportWorker->isLoggedIn())
          {
            if (WareNotAuthorized)
            {
              Item->setFlags(Item->flags() & ~Qt::ItemIsEnabled);
              Item->setToolTip(tr("You are not authorized to access to \"%1\"").arg(WareId));
              Item->setForeground(QColor("orange"));
            }
          }
          else
          {
            Item->setFlags(Item->flags() & ~Qt::ItemIsEnabled);
            Item->setToolTip(tr("You must be logged in to clone a ware"));
          }
        }

        Item->setCheckState(Qt::Unchecked);

        m_ListWidgetsByWareType[Type]->addItem(Item);
      }
    }
  }

  check();
}


// =====================================================================
// =====================================================================


QStringList GenericSrcImportDialog::getSelectedWares()
{
  QStringList Wares;
  for (auto& Pair : m_ListWidgetsByWareType)
  {
    for (const QListWidgetItem* Item : Pair.second->findItems("*", Qt::MatchWildcard))
    {
      if (Item->checkState() == Qt::Checked)
      {
        Wares << Item->data(Qt::UserRole).toString();
      }
    }
  }
  return Wares;
}


// =====================================================================
// =====================================================================


void GenericSrcImportDialog::handleThread(QThread* Thread, 
                                          openfluid::ui::waresdev::WaresSrcIOProgressDialog* ProgressDialog)
{
  try
  {
    Thread->start();
  }
  catch (std::exception& e)
  {
    ProgressDialog->writeError(e.what());
  }

  if (ProgressDialog->exec())
  {
    accept();
  }
}


// =====================================================================
// =====================================================================


void GenericSrcImportDialog::importThreadWrapper(QThread* Thread, GitImportWorker* ImportWorker, 
                                                 openfluid::ui::waresdev::WaresSrcIOProgressDialog* ProgressDialogPtr)
{
  connect(Thread, SIGNAL(finished()), Thread, SLOT(deleteLater()));

  ImportWorker->moveToThread(Thread);

  connect(Thread, SIGNAL(started()), ImportWorker, SLOT(onCloneRequest()));

  connect(ImportWorker, SIGNAL(finished(bool, const QString&)), Thread, SLOT(quit()));
  connect(ImportWorker, SIGNAL(finished(bool, const QString&)), ProgressDialogPtr,
          SLOT(finish(bool, const QString&)));

  connect(ImportWorker, SIGNAL(info(const QString&)), ProgressDialogPtr, 
          SLOT(writeInfo(const QString&)));
  connect(ImportWorker, SIGNAL(error(const QString&)), ProgressDialogPtr, 
          SLOT(writeError(const QString&)));
  connect(ImportWorker, SIGNAL(progressed(int)), ProgressDialogPtr, SLOT(progress(int)));
}


} } } //namespaces
