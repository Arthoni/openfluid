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
  @file AddObserverDialog.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#include <QPushButton>

#include <openfluid/ui/config.hpp>
#include <openfluid/machine/ObserverSignatureRegistry.hpp>
#include <openfluid/machine/ObserverInstance.hpp>

#include "ui_AddWareDialog.h"
#include "AddObserverDialog.hpp"
#include "SignatureWidget.hpp"


AddObserverDialog::AddObserverDialog(QWidget* Parent) :
  AddWareDialog(Parent)
{
  updateDefaultMessage(tr("Add observer"));

  openfluid::machine::ObserverSignatureRegistry* Reg =
    openfluid::machine::ObserverSignatureRegistry::instance();

  std::vector<openfluid::machine::ObserverSignatureInstance*> ObsSigns =
    Reg->getAvailableSignatures();


  for (std::vector<openfluid::machine::ObserverSignatureInstance*>::iterator it =
      ObsSigns.begin(); it != ObsSigns.end(); ++it)
  {
    QListWidgetItem* Item = new QListWidgetItem();

    QStringList WareInfos = {
      QString::fromStdString((*it)->Signature->ID),
      QString::fromStdString((*it)->Signature->Name)
    };

    Item->setData(Qt::UserRole,WareInfos);

    Item->setIcon(QIcon(":/builder/images/ware-obs-plugged.png"));

    ui->WaresListWidget->addItem(Item);
  }

  labelizeItems(ui->UseNameCheckBox->isChecked());

  connect(ui->WaresListWidget,SIGNAL(itemSelectionChanged()),this,SLOT(updateSignature()));
  connect(ui->UseNameCheckBox,SIGNAL(toggled(bool)),this,SLOT(labelizeItems(bool)));

  setMessage(tr("Select observer to add"));
}


// =====================================================================
// =====================================================================


AddObserverDialog::~AddObserverDialog()
{

}


// =====================================================================
// =====================================================================


void AddObserverDialog::updateSignature()
{
  openfluid::machine::ObserverSignatureRegistry* Reg =
    openfluid::machine::ObserverSignatureRegistry::instance();

  const openfluid::machine::ObserverSignatureInstance* Sign =
      Reg->signature(ui->WaresListWidget->currentItem()->data(Qt::UserRole).toStringList().at(0).toStdString());

  ui->WareSignatureWidget->update(Sign);

  setMessage();
}

