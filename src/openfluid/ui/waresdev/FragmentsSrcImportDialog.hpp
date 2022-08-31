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
  @file FragmentsSrcImportDialog.hpp

  @author Armel THÖNI <armel.thoni@inra.fr>
 */


#ifndef __OPENFLUID_UIWARESDEV_FRAGMENTSSRCIMPORTDIALOG_HPP__
#define __OPENFLUID_UIWARESDEV_FRAGMENTSSRCIMPORTDIALOG_HPP__


#include <QListWidget>
#include <QAbstractButton>
#include <QButtonGroup>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

#include <openfluid/dllexport.hpp>
#include <openfluid/ware/TypeDefs.hpp>
#include <openfluid/config.hpp>
#include <openfluid/ui/common/MessageDialog.hpp>
#include <openfluid/ui/waresdev/GenericSrcImportDialog.hpp>
#include <openfluid/ui/waresdev/WaresImportFilterWidget.hpp>


namespace Ui {
class FragmentsSrcImportDialog;
}

namespace openfluid { namespace waresdev {
class WaresDevImportPackage;
class FluidHubWaresImportWorker;
} }


namespace openfluid { namespace ui { namespace waresdev {


class FluidHubWaresImportWorker;


class OPENFLUID_API FragmentsSrcImportDialog: public GenericSrcImportDialog
{
  Q_OBJECT


  private slots:

    bool check();


  private:

    Ui::FragmentsSrcImportDialog* ui;

    QString m_WarePath;
    bool m_HasWareVersionControl;

    QButtonGroup m_SourceBtGroup;
    QButtonGroup m_StrategyBtGroup;

    QList<QWidget*> m_HubConnectWidgets;

    QList<QWidget*> m_GitLoginWidgets;

    bool isFragmentInWare(std::string FragmentName);


  protected slots:

    void onImportAsked();

  
  protected:

    QLineEdit* hubUrlLineEdit();

    QLineEdit* usernameLineEdit();
  
    QLineEdit* passwordLineEdit();

    QPushButton* hubConnectButton();

    QPushButton* hubLoginButton();
    
    QLabel* hubUsernameLabel();

    void customHideWidget(QWidget* Widget);
    
    void postConnectSetup();

    bool wareItemDisplay(const openfluid::ware::WareType Type, const QString WareId, QListWidgetItem* Item);


  public:

    explicit FragmentsSrcImportDialog(QWidget* Parent, QString WarePath, bool HasWareVersionControl);

    ~FragmentsSrcImportDialog();

};


} } } //namespaces


#endif /* __OPENFLUID_UIWARESDEV_FRAGMENTSSRCIMPORTDIALOG_HPP__ */
