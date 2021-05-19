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
  @file SignatureDataEditorWidget.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#include <QComboBox>

#include <openfluid/ui/common/UIHelpers.hpp>
#include <openfluid/core/Value.hpp>

#include "ui_SignatureDataEditorWidget.h"
#include "SignatureDataEditorWidget.hpp"


namespace openfluid { namespace ui { namespace common {


SignatureDataEditorWidget::SignatureDataEditorWidget(QWidget* Parent):
  QWidget(Parent), ui(new Ui::SignatureDataEditorWidget)
{
  ui->setupUi(this);

  ui->AddButton->setText("");
  ui->AddButton->setIcon(openfluid::ui::common::getIcon("add","/ui/common"));
  ui->AddButton->setIconSize(QSize(20,20));

  ui->RemoveButton->setText("");
  ui->RemoveButton->setIcon(openfluid::ui::common::getIcon("remove","/ui/common"));
  ui->RemoveButton->setIconSize(QSize(20,20));

  connect(ui->AddButton,SIGNAL(clicked()),this,SLOT(addDataLine()));
  connect(ui->RemoveButton,SIGNAL(clicked()),this,SLOT(removeDataLine()));
}


// =====================================================================
// =====================================================================


SignatureDataEditorWidget::~SignatureDataEditorWidget()
{
  delete ui;
}


// =====================================================================
// =====================================================================


const QTableWidget* SignatureDataEditorWidget::dataTableWidget() const
{
  return ui->DataTableWidget;
}


// =====================================================================
// =====================================================================


void SignatureDataEditorWidget::initialize(const QList<DataColumns>& Columns)
{
  m_Columns = Columns;
  int StretchedCol = Columns.size()-1;
  int i = 0;

  QStringList ColHeaders;

  for (auto& Col : m_Columns)
  {
    if (Col == DataColumns::DATACOL_DATAID)
    {
      ColHeaders << tr("Name");
    }
    else if (Col == DataColumns::DATACOL_UNITSCLASS)
    {
      ColHeaders << tr("Units class");
    }
    else if (Col == DataColumns::DATACOL_ROCONDITION || 
             Col == DataColumns::DATACOL_RWCONDITION || 
             Col == DataColumns::DATACOL_RWUCONDITION)
    {
      ColHeaders << tr("I/O condition");
    }
    else if (Col == DataColumns::DATACOL_DATATYPE)
    {
      ColHeaders << tr("Type");
    }
    else if (Col == DataColumns::DATACOL_DESCRIPTION)
    {
      ColHeaders << tr("Description");
      StretchedCol = i;
    }
    else if (Col == DataColumns::DATACOL_SIUNIT)
    {
      ColHeaders << tr("SI unit");
    }

    i++;
  }

  ui->DataTableWidget->setColumnCount(m_Columns.size());
  ui->DataTableWidget->setHorizontalHeaderLabels(ColHeaders);

  ui->DataTableWidget->horizontalHeader()->setSectionResizeMode(StretchedCol,QHeaderView::Stretch);

}


// =====================================================================
// =====================================================================


void SignatureDataEditorWidget::addDataLine(const QMap<int,QVariant>& DataLine)
{
  int RowCount = ui->DataTableWidget->rowCount();
  ui->DataTableWidget->setRowCount(RowCount+1);
  int i = 0;

  for (auto& Col : m_Columns)
  {
    if (Col == DataColumns::DATACOL_DATAID)
    {
      QString TmpStr = "name";
      if (DataLine.contains(static_cast<int>(DataColumns::DATACOL_DATAID)))
      {
        TmpStr = DataLine.value(static_cast<int>(DataColumns::DATACOL_DATAID)).toString();
      }
      ui->DataTableWidget->setItem(RowCount,i,new QTableWidgetItem(TmpStr));
    }
    else if (Col == DataColumns::DATACOL_UNITSCLASS)
    {
      QString TmpStr = "CLASS";
      if (DataLine.contains(static_cast<int>(DataColumns::DATACOL_UNITSCLASS)))
      {
        TmpStr = DataLine.value(static_cast<int>(DataColumns::DATACOL_UNITSCLASS)).toString();
      }
      ui->DataTableWidget->setItem(RowCount,i,new QTableWidgetItem(TmpStr));
    }
    else if (Col == DataColumns::DATACOL_DESCRIPTION)
    {
      QString TmpStr = "";
      if (DataLine.contains(static_cast<int>(DataColumns::DATACOL_DESCRIPTION)))
      {
        TmpStr = DataLine.value(static_cast<int>(DataColumns::DATACOL_DESCRIPTION)).toString();
      }
      ui->DataTableWidget->setItem(RowCount,i,new QTableWidgetItem(TmpStr));
    }
    else if (Col == DataColumns::DATACOL_SIUNIT)
    {
      QString TmpStr = "";
      if (DataLine.contains(static_cast<int>(DataColumns::DATACOL_SIUNIT)))
      {
        TmpStr = DataLine.value(static_cast<int>(DataColumns::DATACOL_SIUNIT)).toString();
      }
      ui->DataTableWidget->setItem(RowCount,i,new QTableWidgetItem(TmpStr));
    }
    else if (Col == DataColumns::DATACOL_ROCONDITION)
    {
      QComboBox* Combo = new QComboBox();
      Combo->addItem(tr("Used"),static_cast<int>(DataConditionsIndices::DATACOND_USED));
      Combo->addItem(tr("Required"),static_cast<int>(DataConditionsIndices::DATACOND_REQUIRED));
      if (DataLine.contains(static_cast<int>(DataColumns::DATACOL_ROCONDITION)))
      {
        Combo->setCurrentIndex(DataLine.value(static_cast<int>(DataColumns::DATACOL_ROCONDITION)).toInt());
      }
      ui->DataTableWidget->setCellWidget(RowCount,i,Combo);
    }
    else if (Col == DataColumns::DATACOL_RWCONDITION)
    {
      QComboBox* Combo = new QComboBox();
      Combo->addItem(tr("Used"),static_cast<int>(DataConditionsIndices::DATACOND_USED));
      Combo->addItem(tr("Required"),static_cast<int>(DataConditionsIndices::DATACOND_REQUIRED));
      Combo->addItem(tr("Produced"),static_cast<int>(DataConditionsIndices::DATACOND_PRODUCED));
      if (DataLine.contains(static_cast<int>(DataColumns::DATACOL_RWCONDITION)))
      {
        Combo->setCurrentIndex(DataLine.value(static_cast<int>(DataColumns::DATACOL_RWCONDITION)).toInt());
      }
      ui->DataTableWidget->setCellWidget(RowCount,i,Combo);
    }
    else if (Col == DataColumns::DATACOL_RWUCONDITION)
    {
      QComboBox* Combo = new QComboBox();
      Combo->addItem(tr("Used"),static_cast<int>(DataConditionsIndices::DATACOND_USED));
      Combo->addItem(tr("Required"),static_cast<int>(DataConditionsIndices::DATACOND_REQUIRED));
      Combo->addItem(tr("Produced"),static_cast<int>(DataConditionsIndices::DATACOND_PRODUCED));
      Combo->addItem(tr("Updated"),static_cast<int>(DataConditionsIndices::DATACOND_UPDATED));
      if (DataLine.contains(static_cast<int>(DataColumns::DATACOL_RWUCONDITION)))
      {
        Combo->setCurrentIndex(DataLine.value(static_cast<int>(DataColumns::DATACOL_RWUCONDITION)).toInt());
      }
      ui->DataTableWidget->setCellWidget(RowCount,i,Combo);
    }
    else if (Col == DataColumns::DATACOL_DATATYPE)
    {
      QComboBox* Combo = new QComboBox();
      for (int TypeInt = openfluid::core::Value::NONE; TypeInt <= openfluid::core::Value::NULLL; TypeInt++)
      {
        Combo->addItem(QString::fromStdString(
          openfluid::core::Value::getStringFromValueType(static_cast<openfluid::core::Value::Type>(TypeInt))),TypeInt);
      }
      if (DataLine.contains(static_cast<int>(DataColumns::DATACOL_DATATYPE)))
      {
        Combo->setCurrentIndex(DataLine.value(static_cast<int>(DataColumns::DATACOL_DATATYPE)).toInt());
      }
      ui->DataTableWidget->setCellWidget(RowCount,i,Combo);
    }
    i++;
  }
}


// =====================================================================
// =====================================================================


void SignatureDataEditorWidget::addDataLine()
{
  addDataLine({});

  QModelIndex Index = ui->DataTableWidget->model()->index(ui->DataTableWidget->rowCount()-1,0,QModelIndex());
  ui->DataTableWidget->edit(Index);
}


// =====================================================================
// =====================================================================


void SignatureDataEditorWidget::removeDataLine()
{
  int Row = ui->DataTableWidget->currentRow();

  if (Row >= 0)
  {
    ui->DataTableWidget->removeRow(Row);
  }
}


} } } // namespaces

