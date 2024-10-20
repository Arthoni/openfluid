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
  @file AddGeneratorDialog.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
*/


#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>

#include <openfluid/base/RunContextManager.hpp>
#include <openfluid/machine/ModelItemInstance.hpp>
#include <openfluid/ui/common/ShortcutCompleter.hpp>
#include <openfluid/ui/config.hpp>

#include "ui_AddGeneratorDialog.h"
#include "AddGeneratorDialog.hpp"
#include "ProjectCentral.hpp"


AddGeneratorDialog::AddGeneratorDialog(QWidget* Parent) :
  openfluid::ui::common::MessageDialog(Parent),ui(new Ui::AddGeneratorDialog)
{
  ui->setupUi(this);

  ui->VectorSpinBox->setMaximum(std::numeric_limits<int>::max());

  setupMessageUi(tr("Add generator"));

  connect(ui->FixedRadioButton,SIGNAL(toggled(bool)),this,SLOT(switchGeneratorOptions()));
  connect(ui->RandomRadioButton,SIGNAL(toggled(bool)),this,SLOT(switchGeneratorOptions()));
  connect(ui->InterpRadioButton,SIGNAL(toggled(bool)),this,SLOT(switchGeneratorOptions()));
  connect(ui->InjectRadioButton,SIGNAL(toggled(bool)),this,SLOT(switchGeneratorOptions()));

  connect(ui->VectorRadioButton,SIGNAL(toggled(bool)),ui->VectorSpinBox,SLOT(setEnabled(bool)));

  connect(ui->VarNameEdit,SIGNAL(textEdited(const QString&)),this,SLOT(checkGlobal()));
  connect(ui->UnitsClassEdit,SIGNAL(textEdited(const QString&)),this,SLOT(checkGlobal()));

  connect(ui->SourcesBrowseButton,SIGNAL(clicked()),this,SLOT(selectSourcesFile()));
  connect(ui->DistriBrowseButton,SIGNAL(clicked()),this,SLOT(selectDistriFile()));

  switchGeneratorOptions();

  m_Method = openfluid::fluidx::GeneratorDescriptor:: GeneratorMethod::FIXED;

  ui->FixedRadioButton->setChecked(true);
  ui->DoubleRadioButton->setChecked(true);
  ui->VectorSpinBox->setEnabled(false);

  // "required" placeholder
  QString PlaceholderStr = getPlaceholderRequired();
  ui->UnitsClassEdit->setPlaceholderText(PlaceholderStr);
  ui->VarNameEdit->setPlaceholderText(PlaceholderStr);

  openfluid::ui::common::ShortcutCompleter* Completer =
      new openfluid::ui::common::ShortcutCompleter(ProjectCentral::instance()->unitsClassesList(),this);
  Completer->linkToLineEdit(ui->UnitsClassEdit);
  connect(Completer,SIGNAL(activated(const QString&)),this,SLOT(checkGlobal()));

  ui->VarNameEdit->setFocus();

  checkGlobal();
}


// =====================================================================
// =====================================================================


AddGeneratorDialog::~AddGeneratorDialog()
{
  delete ui;
}


// =====================================================================
// =====================================================================


void AddGeneratorDialog::switchGeneratorOptions()
{
  if (ui->FixedRadioButton->isChecked())
  {
    m_Method = openfluid::fluidx::GeneratorDescriptor:: GeneratorMethod::FIXED;
    ui->OptionsWidget->setCurrentIndex(0);
  }
  else if (ui->RandomRadioButton->isChecked())
  {
    m_Method = openfluid::fluidx::GeneratorDescriptor:: GeneratorMethod::RANDOM;
    ui->OptionsWidget->setCurrentIndex(1);
  }
  else if (ui->InterpRadioButton->isChecked())
  {
    m_Method = openfluid::fluidx::GeneratorDescriptor:: GeneratorMethod::INTERP;
    ui->OptionsWidget->setCurrentIndex(2);
  }
  else if (ui->InjectRadioButton->isChecked())
  {
    m_Method = openfluid::fluidx::GeneratorDescriptor:: GeneratorMethod::INJECT;
    ui->OptionsWidget->setCurrentIndex(2);
  }
}


// =====================================================================
// =====================================================================


void AddGeneratorDialog::checkGlobal()
{
  if (ui->VarNameEdit->text().isEmpty())
  {
    setMessage(tr("Variable name cannot be empty"));
  }
  else if (ui->UnitsClassEdit->text().isEmpty())
  {
    setMessage(tr("Units class cannot be empty"));
  }
  else
  {
    setMessage();
  }
}


// =====================================================================
// =====================================================================


QString AddGeneratorDialog::getVariableName() const
{
  return ui->VarNameEdit->text();
}


// =====================================================================
// =====================================================================


QString AddGeneratorDialog::getUnitClass() const
{
  return ui->UnitsClassEdit->text();
}


// =====================================================================
// =====================================================================


unsigned int AddGeneratorDialog::getVariableSize() const
{
  if (ui->DoubleRadioButton->isChecked())
  {
    return 1;
  }
  else
  {
    return ui->VectorSpinBox->value();
  }
}


// =====================================================================
// =====================================================================


openfluid::ware::WareParams_t AddGeneratorDialog::getParams() const
{
  openfluid::ware::WareParams_t Params;

  if (m_Method == openfluid::fluidx::GeneratorDescriptor:: GeneratorMethod::FIXED)
  {
    if (!ui->ValueEdit->text().isEmpty())
    {
      Params["fixedvalue"] = ui->ValueEdit->text().toStdString();
    }
  }
  else if (m_Method == openfluid::fluidx::GeneratorDescriptor:: GeneratorMethod::RANDOM)
  {
    if (!ui->MinValueEdit->text().isEmpty())
    {
      Params["min"] = ui->MinValueEdit->text().toStdString();
    }
    if (!ui->MaxValueEdit->text().isEmpty())
    {
      Params["max"] = ui->MaxValueEdit->text().toStdString();
    }
  }
  else if (m_Method == openfluid::fluidx::GeneratorDescriptor:: GeneratorMethod::INTERP)
  {
    Params["keeptmpdir"] = "false";
    if (!ui->SourcesFileEdit->text().isEmpty())
    {
      Params["sources"] = ui->SourcesFileEdit->text().toStdString();
    }
    if (!ui->DistriFileEdit->text().isEmpty())
    {
      Params["distribution"] = ui->DistriFileEdit->text().toStdString();
    }
  }
  else if (m_Method == openfluid::fluidx::GeneratorDescriptor:: GeneratorMethod::INJECT)
  {
    if (!ui->SourcesFileEdit->text().isEmpty())
    {
      Params["sources"] = ui->SourcesFileEdit->text().toStdString();
    }
    if (!ui->DistriFileEdit->text().isEmpty())
    {
      Params["distribution"] = ui->DistriFileEdit->text().toStdString();
    }
  }

  return Params;
}


// =====================================================================
// =====================================================================


void AddGeneratorDialog::selectSourcesFile()
{
  QString CurrentDir = QString::fromStdString(openfluid::base::RunContextManager::instance()->getInputDir());

  QString FileName = QFileDialog::getOpenFileName(this,
                                                  tr("Select sources file"),
                                                  CurrentDir,
                                                  tr("All files (*.*)"));
  if (FileName.startsWith(CurrentDir))
  {
    ui->SourcesFileEdit->setText(FileName.remove(0,CurrentDir.length()+1));
  }
  else
  {
    QMessageBox::critical(QApplication::activeWindow(),"OpenFLUID-Builder",
                          tr("Choosen file is not located in the input dataset of the project"),QMessageBox::Close);
  }

}


// =====================================================================
// =====================================================================


void AddGeneratorDialog::selectDistriFile()
{
  QString CurrentDir = QString::fromStdString(openfluid::base::RunContextManager::instance()->getInputDir());

  QString FileName = QFileDialog::getOpenFileName(this,
                                                  tr("Select distribution file"),
                                                  CurrentDir,
                                                  tr("All files (*.*)"));
  if (FileName.startsWith(CurrentDir))
  {
    ui->DistriFileEdit->setText(FileName.remove(0,CurrentDir.length()+1));
  }
  else
  {
    QMessageBox::critical(QApplication::activeWindow(),"OpenFLUID-Builder",
                          tr("Choosen file is not located in the input dataset of the project"),QMessageBox::Close);
  }

}

