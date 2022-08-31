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
  @file GenericSrcImportDialog.hpp

  @author Armel THÖNI <armel.thoni@inra.fr>
 */


#ifndef __OPENFLUID_UIWARESDEV_GENERICSRCIMPORTDIALOG_HPP__
#define __OPENFLUID_UIWARESDEV_GENERICSRCIMPORTDIALOG_HPP__


#include <QListWidget>
#include <QLineEdit>
#include <QAbstractButton>
#include <QButtonGroup>
#include <QLabel>
#include <QPushButton>

#include <openfluid/dllexport.hpp>
#include <openfluid/ware/TypeDefs.hpp>
#include <openfluid/config.hpp>
#include <openfluid/ui/common/MessageDialog.hpp>
#include <openfluid/ui/waresdev/WaresSrcIOProgressDialog.hpp>
#include <openfluid/ui/waresdev/WaresImportFilterWidget.hpp>
#include <openfluid/ui/waresdev/GitImportWorker.hpp>


namespace openfluid { namespace ui { namespace waresdev {


class FluidHubWaresImportWorker;


class OPENFLUID_API GenericSrcImportDialog: public openfluid::ui::common::MessageDialog
{
  Q_OBJECT

  protected slots :

    virtual bool check() = 0;

    virtual void onHubConnectButtonClicked();
    
    void onHubLoginButtonClicked();

    virtual void onImportAsked() = 0;


  protected:

    bool m_IsConnectedToHub = false;
    bool m_IsLoggedInHub = false;
    std::map<openfluid::ware::WareType, QListWidget*> m_ListWidgetsByWareType;

    QList<QWidget*> m_HubLoginWidgets;
    QList<QWidget*> m_HubConnectionInfoWidgets;
    QList<QWidget*> m_HubLoginWidgetsAndButton;

    QString m_HubButtonConnectLabel = tr("Connect");
    QString m_HubButtonDisconnectLabel = tr("Disconnect");
    QString m_HubButtonLoginLabel = tr("Log in");
    QString m_HubButtonLogoutLabel = tr("Log out");

    bool checkHubSection();

    QStringList getSelectedWares();

    void updateHubWaresList();

    virtual QLineEdit* hubUrlLineEdit() = 0;
  
    virtual QLineEdit* usernameLineEdit() = 0;
  
    virtual QLineEdit* passwordLineEdit() = 0;

    virtual QPushButton* hubConnectButton() = 0;

    virtual QPushButton* hubLoginButton() = 0;

    virtual QLabel* hubUsernameLabel() = 0;

    void clearListWidgets();

    virtual void customHideWidget(QWidget* Widget) = 0;

    virtual void postConnectSetup() = 0;

    virtual bool isWareDisplayed(const openfluid::ware::WareType& Type, const QString WareId, 
                                           const bool WareInWorkspace, const bool WareNotAuthorized);

    virtual bool wareItemDisplay(const openfluid::ware::WareType Type, const QString WareId, QListWidgetItem* Item) = 0;

    void handleThread(QThread* Thread, openfluid::ui::waresdev::WaresSrcIOProgressDialog* ProgressDialog);

    void importThreadWrapper(QThread* Thread, GitImportWorker* ImportWorker, 
                             openfluid::ui::waresdev::WaresSrcIOProgressDialog* ProgressDialogPtr);

    FluidHubWaresImportWorker* mp_FluidHubWaresImportWorker = nullptr;

  public:

    explicit GenericSrcImportDialog(QWidget* Parent);

    ~GenericSrcImportDialog();

};


} } } //namespaces


#endif /* __OPENFLUID_UIWARESDEV_GENERICSRCIMPORTDIALOG_HPP__ */
