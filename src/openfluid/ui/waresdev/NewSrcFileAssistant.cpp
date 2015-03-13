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
 @file NewSrcFileAssistant.cpp
 @brief Implements ...

 @author Aline LIBRES <aline.libres@gmail.com>
 */

#include "NewSrcFileAssistant.hpp"
#include "ui_NewSrcFileAssistant.h"
#include "ui_CppPage.h"
#include "ui_CMakeConfigPage.h"

#include <QRadioButton>
#include <QCloseEvent>
#include <QFile>
#include <QMessageBox>

#include <openfluid/ui/waresdev/WareExplorerDialog.hpp>
#include <openfluid/builderext/BuilderExtensionSignature.hpp>
#include <openfluid/config.hpp>
#include <openfluid/ui/config.hpp>


namespace openfluid { namespace ui { namespace waresdev {


// =====================================================================
// =====================================================================


CppPage::CppPage(const QDir& WareDir, const QStringList& BextType_Text, const QStringList& BextCategory_Text,
                 QWidget* Parent) :
    QWizardPage(Parent), ui(new Ui::CppPage), m_WareDir(WareDir), m_IsHpp(false)
{
  ui->setupUi(this);

  ui->BextType_comboBox->addItems(BextType_Text);
  ui->BextCategory_comboBox->addItems(BextCategory_Text);

  ui->ClassName_lineEdit->setValidator(
      new QRegExpValidator(openfluid::waresdev::WareSrcFactory::getClassnameRegExp(), this));

  registerField("SourceFilename", ui->SourceFilename_lineEdit);
  registerField("Classname", ui->ClassName_lineEdit);
  registerField("BextType", ui->BextType_comboBox);
  registerField("BextCategory", ui->BextCategory_comboBox);
  registerField("BextMenutext", ui->BextMenutext_lineEdit);

  connect(ui->SourceFilename_lineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(onInformationChanged()));
  connect(ui->ClassName_lineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(onInformationChanged()));
}


// =====================================================================
// =====================================================================


CppPage::~CppPage()
{
  delete ui;
}


// =====================================================================
// =====================================================================


void CppPage::initialize(bool IsHpp, bool IsUi, openfluid::waresdev::WareSrcManager::WareType Type)
{
  m_IsHpp = IsHpp;

  ui->SourceFilename_lineEdit->setValidator(
      new QRegExpValidator(openfluid::waresdev::WareSrcFactory::getCppFilenameRegExp(IsHpp), this));

  QString ClassName;
  QString SourceFilename;
  QString SourceFilename_label;

  if (IsUi)
  {
    ClassName = "ParamsUiWidget";
    SourceFilename = "ParamsUiWidget";

    if (IsHpp)  // UiHpp
    {
      m_DefaultMsg = tr("Create the ui-parameterization header file (.hpp)");
      SourceFilename_label = tr("Widget header file name (.hpp)");
    }
    else  // UiCpp
    {
      m_DefaultMsg = tr("Create the ui-parameterization source file (.cpp)");
      SourceFilename_label = tr("Widget source file name (.cpp)");
    }
  }
  else
  {
    switch (Type)
    {
      case openfluid::waresdev::WareSrcManager::SIMULATOR:
        ClassName = "MySimulator";
        SourceFilename = "MySim";
        break;
      case openfluid::waresdev::WareSrcManager::OBSERVER:
        ClassName = "MyObserver";
        SourceFilename = "MyObs";
        break;
      case openfluid::waresdev::WareSrcManager::BUILDEREXT:
        ClassName = "MyExtension";
        SourceFilename = "MyExt";
        break;
      default:
        break;
    }

    if (IsHpp)  // MainHpp
    {
      m_DefaultMsg = tr("Create the main C++ header file (.hpp)");
      SourceFilename_label = tr("Header file name (.hpp)");
    }
    else  // MainCpp
    {
      m_DefaultMsg = tr("Create the main C++ source file (.cpp)");
      SourceFilename_label = tr("Source file name (.cpp)");
    }
  }

  SourceFilename.append(IsHpp ? ".hpp" : ".cpp");

  ui->SourceFilename_label->setText(SourceFilename_label);
  ui->SourceFilename_lineEdit->setText(SourceFilename);
  ui->ClassName_lineEdit->setText(ClassName);

  bool IsBuilderExt = (Type == openfluid::waresdev::WareSrcManager::BUILDEREXT);
  ui->BuilderExt_line->setVisible(IsBuilderExt);
  ui->BextType_label->setVisible(IsBuilderExt);
  ui->BextType_comboBox->setVisible(IsBuilderExt);
  ui->BuilderExt_groupBox->setVisible(IsBuilderExt && !IsHpp);
}


// =====================================================================
// =====================================================================


void CppPage::onInformationChanged()
{
  QString WarningMsg = "";

  if (!ui->SourceFilename_lineEdit->hasAcceptableInput())
    WarningMsg = QString("File name must be of the form \"xxx.%1\"").arg(m_IsHpp ? "hpp" : "cpp");
  else if (m_WareDir.exists(ui->SourceFilename_lineEdit->text()))
    WarningMsg = "You must enter the name of a file that doesn't already exist";
  else if (!ui->ClassName_lineEdit->hasAcceptableInput())
    WarningMsg = "You must enter a class name";

  NewSrcFileAssistant::setStatus(m_DefaultMsg, WarningMsg, ui->MessageLabel, ui->MessageFrame);

  emit QWizardPage::completeChanged();
}


// =====================================================================
// =====================================================================


bool CppPage::isComplete() const
{
  return (ui->SourceFilename_lineEdit->hasAcceptableInput() && !m_WareDir.exists(ui->SourceFilename_lineEdit->text())
          && ui->ClassName_lineEdit->hasAcceptableInput());
}


// =====================================================================
// =====================================================================

// =====================================================================
// =====================================================================


CMakeConfigPage::CMakeConfigPage(openfluid::waresdev::WareSrcManager::WareType Type, const QStringList& Sim2doc_Text,
                                 QWidget* Parent) :
    QWizardPage(Parent), ui(new Ui::CMakeConfigPage)
{
  ui->setupUi(this);

  bool IsSim = false;
  QString RootFilename;

  switch (Type)
  {
    case openfluid::waresdev::WareSrcManager::SIMULATOR:
      IsSim = true;
      RootFilename = "MySim.cpp";
      break;
    case openfluid::waresdev::WareSrcManager::OBSERVER:
      RootFilename = "MyObs.cpp";
      break;
    case openfluid::waresdev::WareSrcManager::BUILDEREXT:
      RootFilename = "MyExt.cpp";
      break;
    default:
      break;
  }

  ui->RootFilename_lineEdit->setText(RootFilename);

  ui->Sim2doc_comboBox->addItems(Sim2doc_Text);
  ui->Sim2doc_groupBox->setVisible(IsSim);
  ui->Sim2doc_line->setVisible(IsSim);

  ui->RootFilename_lineEdit->setValidator(
      new QRegExpValidator(openfluid::waresdev::WareSrcFactory::getCppFilenameRegExp(), this));

  m_DefaultMsg = "Create the \"CMake.in.config\" file";
  NewSrcFileAssistant::setStatus(m_DefaultMsg, "", ui->MessageLabel, ui->MessageFrame);

  registerField("RootFilename", ui->RootFilename_lineEdit);
  registerField("Sim2docMode", ui->Sim2doc_comboBox);
  registerField("Sim2docInstall", ui->Sim2doc_checkBox);

  connect(ui->RootFilename_lineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(onRootfilenameTextChanged()));
}


// =====================================================================
// =====================================================================


CMakeConfigPage::~CMakeConfigPage()
{
  delete ui;
}


// =====================================================================
// =====================================================================


bool CMakeConfigPage::isComplete() const
{
  return ui->RootFilename_lineEdit->hasAcceptableInput();
}


// =====================================================================
// =====================================================================

void CMakeConfigPage::onRootfilenameTextChanged()
{
  NewSrcFileAssistant::setStatus(
      m_DefaultMsg, ui->RootFilename_lineEdit->hasAcceptableInput() ? "" : "File name must be of the form \"xxx.cpp\"",
      ui->MessageLabel, ui->MessageFrame);

  emit QWizardPage::completeChanged();
}


// =====================================================================
// =====================================================================

// =====================================================================
// =====================================================================


NewSrcFileAssistant::NewSrcFileAssistant(const openfluid::waresdev::WareSrcContainer& Container, QWidget* Parent) :
    QWizard(Parent), ui(new Ui::NewSrcFileAssistant), mref_Container(Container)
{
  ui->setupUi(this);

  QString BgColorCss = QString("background-color: %1;").arg(openfluid::ui::config::DIALOGBANNER_BGCOLOR);
  ui->MessageFrame->setStyleSheet(BgColorCss);
  ui->MessageFrame_2->setStyleSheet(BgColorCss);

  ui->buttonGroup->setId(ui->Wareshub_radioButton, -1);
  ui->buttonGroup->setId(ui->CMakeLists_radioButton, -1);
  ui->buttonGroup->setId(ui->Empty_radioButton, EMPTY_PAGE);
  ui->buttonGroup->setId(ui->CMakeConfig_radioButton, CMAKECONFIG_PAGE);
  ui->buttonGroup->setId(ui->Cpp_radioButton, CPP_PAGE);
  ui->buttonGroup->setId(ui->Hpp_radioButton, HPP_PAGE);
  ui->buttonGroup->setId(ui->CppUi_radioButton, CPP_UI_PAGE);
  ui->buttonGroup->setId(ui->HppUi_radioButton, HPP_UI_PAGE);

  QString MainCpp = mref_Container.getMainCppPath();
  QString UiParamCpp = mref_Container.getUiParamCppPath();

  ui->Wareshub_radioButton->setEnabled(mref_Container.getJsonPath().isEmpty());
  ui->CMakeLists_radioButton->setEnabled(mref_Container.getCMakeListsPath().isEmpty());
  ui->CMakeConfig_radioButton->setEnabled(mref_Container.getCMakeConfigPath().isEmpty());
  ui->Cpp_radioButton->setEnabled(MainCpp.isEmpty());
  ui->Hpp_radioButton->setEnabled(
      !QFile::exists(openfluid::waresdev::WareSrcFactory::Replacements::getHppFilename(MainCpp)));
  ui->CppUi_radioButton->setEnabled(UiParamCpp.isEmpty());
  ui->HppUi_radioButton->setEnabled(
      !QFile::exists(openfluid::waresdev::WareSrcFactory::Replacements::getHppFilename(UiParamCpp)));

  foreach(QRadioButton* Bt,findChildren<QRadioButton*>()){
  if(!Bt->isEnabled())Bt->setToolTip(tr("This file already exists"));
}

  openfluid::waresdev::WareSrcManager::WareType Type = mref_Container.getType();

  if (Type == openfluid::waresdev::WareSrcManager::BUILDEREXT)
  {
    ui->CppUi_radioButton->setVisible(false);
    ui->HppUi_radioButton->setVisible(false);
    ui->Ui_line->setVisible(false);
  }
  else
  {
    ui->Hpp_radioButton->setVisible(false);
  }

  m_BextType_Data << openfluid::builderext::TYPE_MODAL << openfluid::builderext::TYPE_MODELESS
                  << openfluid::builderext::TYPE_WORKSPACE;
  QStringList BextType_Text;
  BextType_Text << tr("Modal") << tr("Modeless") << tr("Workspace");

  m_BextCategory_Data << "openfluid::builderext::CAT_SPATIAL" << "openfluid::builderext::CAT_MODEL"
                      << "openfluid::builderext::CAT_RESULTS" << "openfluid::builderext::CAT_OTHER";
  QStringList BextCategory_Text;
  BextCategory_Text << tr("Spatial domain") << tr("Model") << tr("Results") << tr("Other");

  m_Sim2doc_Data << "ON" << "AUTO" << "OFF";
  QStringList Sim2doc_Text;
  Sim2doc_Text << tr("On - sim2doc must be run manually") << tr("Auto - sim2doc is automatically run")
               << tr("Off - sim2doc is disabled");

// existing pages : IntroPage (index 0), EmptyPage (index 1)
  addPage(new CMakeConfigPage(Type, Sim2doc_Text, this));  // index 2
  addPage(new CppPage(QDir(mref_Container.getAbsolutePath()), BextType_Text, BextCategory_Text, this));  // index 3


  mp_Factory = new openfluid::waresdev::WareSrcFactory(Type);
  mp_Factory->setWareId(Container.getName());


  connect(ui->buttonGroup, SIGNAL(buttonClicked(int)), this, SLOT(onFileTypeButtonClicked(int)));
  connect(ui->toolButton, SIGNAL(clicked()), this, SLOT(onEmptyBrowseToolButtonClicked()));
}


// =====================================================================
// =====================================================================


NewSrcFileAssistant::~NewSrcFileAssistant()
{
  delete ui;
  delete mp_Factory;
}


// =====================================================================
// =====================================================================


int NewSrcFileAssistant::nextId() const
{
  int CheckedId = ui->buttonGroup->checkedId();

  if (currentId() != INTRO_PAGE || CheckedId < 0)
    return -1;

  if (CheckedId >= CPP_PAGE)
    return CPP_PAGE;

  return CheckedId;
}


// =====================================================================
// =====================================================================


void NewSrcFileAssistant::initializePage(int Id)
{
  if (Id == CPP_PAGE)
  {
    int CheckedId = ui->buttonGroup->checkedId();

    bool IsHpp = (CheckedId == HPP_PAGE || CheckedId == HPP_UI_PAGE);
    bool IsUi = (CheckedId == CPP_UI_PAGE || CheckedId == HPP_UI_PAGE);

    qobject_cast<CppPage*>(page(CPP_PAGE))->initialize(IsHpp, IsUi, mref_Container.getType());
  }
}


// =====================================================================
// =====================================================================


void NewSrcFileAssistant::onFileTypeButtonClicked(int Id)
{
  currentPage()->setFinalPage(Id < 0);
}


// =====================================================================
// =====================================================================


void NewSrcFileAssistant::onEmptyBrowseToolButtonClicked()
{
  QString WarePath = mref_Container.getAbsolutePath();

  QString NewPath = WareExplorerDialog::getCreateFilePath(this, WarePath, "");

  if (NewPath.isEmpty())
    return;

  ui->EmptyFile_label->setText(QDir(WarePath).relativeFilePath(NewPath));

  m_NewFilePath = NewPath;
}


// =====================================================================
// =====================================================================


void NewSrcFileAssistant::accept()
{
  QString NewFilePath, ErrMsg;
  bool Ok;

  switch (currentId())
  {
    case INTRO_PAGE:
      if (ui->Wareshub_radioButton->isChecked())
        Ok = mp_Factory->createJsonFile(NewFilePath, ErrMsg);
      else if (ui->CMakeLists_radioButton->isChecked())
        Ok = mp_Factory->createCMakeListsFile(NewFilePath, ErrMsg);
      break;
    case EMPTY_PAGE:
    {
      QFile File(m_NewFilePath);
      Ok = File.open(QIODevice::ReadWrite | QIODevice::Text);
      break;
    }
    case CPP_PAGE:
    {
      openfluid::waresdev::WareSrcFactory::Replacements R;

      bool IsBuilderExt = (mref_Container.getType() == openfluid::waresdev::WareSrcManager::BUILDEREXT);

      switch (ui->buttonGroup->checkedId())
      {
        case CPP_PAGE:
        {
          R.ClassName = field("Classname").toString();
          R.RootCppFilename = field("SourceFilename").toString();
          if (IsBuilderExt)
          {
            R.RootHppFilename = openfluid::waresdev::WareSrcFactory::Replacements::getHppFilename(R.RootCppFilename);
            R.BuilderExtType = m_BextType_Data.value(field("BextType").toInt(), openfluid::builderext::TYPE_UNKNOWN);
            R.BuilderExtCategory = m_BextCategory_Data.value(field("BextCategory").toInt(), "");
            R.BuilderExtMenuText = field("BextMenutext").toString();
          }

          Ok = mp_Factory->createCppFile(R, NewFilePath, ErrMsg);
          break;
        }
        case HPP_PAGE:
        {
          R.ClassName = field("Classname").toString();
          R.RootHppFilename = field("SourceFilename").toString();
          if (IsBuilderExt)
          {
            R.BuilderExtType = m_BextType_Data.value(field("BextType").toInt(), openfluid::builderext::TYPE_UNKNOWN);
            R.HppHeaderGuard = openfluid::waresdev::WareSrcFactory::Replacements::getHeaderGuard(R.RootHppFilename);
          }

          Ok = mp_Factory->createHppFile(R, NewFilePath, ErrMsg);
          break;
        }
        case CPP_UI_PAGE:
          R.ParamsUiClassname = field("Classname").toString();
          R.ParamsUiRootCppFilename = field("SourceFilename").toString();
          R.ParamsUiRootHppFilename = openfluid::waresdev::WareSrcFactory::Replacements::getHppFilename(
              R.ParamsUiRootCppFilename);

          Ok = mp_Factory->createParamUiCppFile(R, NewFilePath, ErrMsg);
          break;
        case HPP_UI_PAGE:
          R.ParamsUiClassname = field("Classname").toString();
          R.ParamsUiRootHppFilename = field("SourceFilename").toString();
          R.ParamsUiHeaderGuard = openfluid::waresdev::WareSrcFactory::Replacements::getHeaderGuard(
              R.ParamsUiRootHppFilename);

          Ok = mp_Factory->createParamUiHppFile(R, NewFilePath, ErrMsg);

          break;
        default:
          break;
      }
      break;
    }
    case CMAKECONFIG_PAGE:
    {
      openfluid::waresdev::WareSrcFactory::Replacements R;
      R.RootCppFilename = field("RootFilename").toString();
      R.Sim2docMode = m_Sim2doc_Data.value(field("Sim2docMode").toInt(), "");
      R.Sim2docInstall = field("Sim2docInstall").toBool();

      Ok = mp_Factory->createCmakeConfigFile(R, NewFilePath, ErrMsg);
      break;
    }
    default:
      break;
  }

  if (Ok)
  {
    m_NewFilePath = NewFilePath;
    QDialog::accept();
  }
  else
    QMessageBox::warning(this, tr("Error"), tr("Unable to create the file : %1").arg(ErrMsg));
}


// =====================================================================
// =====================================================================


QString NewSrcFileAssistant::getNewFilePath()
{
  return m_NewFilePath;
}


// =====================================================================
// =====================================================================


void NewSrcFileAssistant::setStatus(const QString DefaultMsg, const QString WarningMsg, QLabel* MsgLabel,
                                    QFrame* MsgFrame)
{
  bool Ok = WarningMsg.isEmpty();

  MsgLabel->setText(Ok ? DefaultMsg : WarningMsg);

  MsgFrame->setStyleSheet(
      QString("background-color: %1;").arg(
          Ok ? openfluid::ui::config::DIALOGBANNER_BGCOLOR : openfluid::ui::config::DIALOGBANNER_WARNBGCOLOR));
}


// =====================================================================
// =====================================================================


} } }  // namespaces
