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
  @file WareSrcExplorer.cpp

  @author Aline LIBRES <aline.libres@gmail.com>
  @author Jean-Christophe Fabre <jean-christophe.fabre@inra.fr>
*/


#include <QAction>
#include <QApplication>
#include <QMenu>
#include <QMessageBox>
#include <QGuiApplication>
#include <QClipboard>
#include <QFileDialog>
#include <QInputDialog>

#include <openfluid/base/PreferencesManager.hpp>
#include <openfluid/tools/FilesystemPath.hpp>
#include <openfluid/ui/waresdev/WareSrcExplorer.hpp>
#include <openfluid/ui/waresdev/WareSrcExplorerModel.hpp>
#include <openfluid/ui/waresdev/WareExplorerDialog.hpp>
#include <openfluid/ui/waresdev/WareGitDialog.hpp>
#include <openfluid/ui/common/DefaultAction.hpp>
#include <openfluid/ui/common/UIHelpers.hpp>
#include <openfluid/ui/waresdev/FragmentsSrcImportDialog.hpp>


namespace openfluid { namespace ui { namespace waresdev {


WareSrcExplorer::WareSrcExplorer(QWidget* Parent) :
    QTreeView(Parent), mp_Model(0)
{
  connect(this, SIGNAL(activated(const QModelIndex&)), this, SLOT(onDoubleClicked(const QModelIndex&)));

  connect(this, SIGNAL(clicked(const QModelIndex&)), this, SLOT(onClicked(const QModelIndex&)));
}


// =====================================================================
// =====================================================================


WareSrcExplorer::~WareSrcExplorer()
{

}


// =====================================================================
// =====================================================================


void WareSrcExplorer::configure(const QString& TopDirectoryPath, bool WithContextMenu)
{
  m_TopDirectoryPath = TopDirectoryPath;

  mp_Model = new WareSrcExplorerModel(m_TopDirectoryPath);

  setModel(mp_Model);

  hideColumn(1);
  hideColumn(2);
  hideColumn(3);

  setRootIndex(mp_Model->index(m_TopDirectoryPath));

  connect(selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)), this,
          SLOT(onCurrentChanged(const QModelIndex&)));
  connect(mp_Model, SIGNAL(directoryLoaded(const QString&)), this, SLOT(scrollToCurrent()));

  if (WithContextMenu)
  {
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint&)),
            SLOT(onCustomContextMenuRequested(const QPoint&)));
  }
}


// =====================================================================
// =====================================================================


void WareSrcExplorer::onCustomContextMenuRequested(const QPoint& Point)
{
  QMenu Menu;

  Menu.addAction(tr("New file"), this, SLOT(onNewFileAsked()));
  Menu.addAction(tr("New folder"), this, SLOT(onNewFolderAsked()));
  Menu.addAction(tr("Add fragment"), this, SLOT(onNewFragmentAsked()));

  Menu.addSeparator();

  Menu.addAction(tr("Delete ware"), this, SIGNAL(deleteWareAsked()));
  QAction* DeleteFileAction = Menu.addAction(tr("Delete file"), this, SLOT(onDeleteFileAsked()));
  if (currentIndex().isValid() && mp_Model->isDir(currentIndex()))
  {
    DeleteFileAction->setEnabled(false);
  }

  Menu.addSeparator();

  Menu.addAction(tr("Open a terminal"), this, SLOT(onOpenTerminalAsked()));
  Menu.addAction(tr("Open a file explorer"), this, SLOT(onOpenExplorerAsked()));

  // Custom tools menu
  QMenu ExtToolMenu;
  ExtToolMenu.setTitle("Open in external tool");
  ExtToolMenu.setEnabled(false);

  openfluid::base::PreferencesManager* PrefMgr = openfluid::base::PreferencesManager::instance();
  std::list<openfluid::base::PreferencesManager::ExternalTool_t> ExternalTools; 
  openfluid::base::PreferencesManager::ExternalToolContext ExternalToolsCtxt;

  if (currentIndex().isValid() && mp_Model->isDir(currentIndex()))
  {
    ExternalToolsCtxt = openfluid::base::PreferencesManager::ExternalToolContext::WARE;
  }
  else
  {
    ExternalToolsCtxt = openfluid::base::PreferencesManager::ExternalToolContext::FILE;
  }
  
  ExternalTools = PrefMgr->getWaresdevExternalToolsInContext(ExternalToolsCtxt);

  QMap<QString, QAction*> ExternalToolsActions;
  for (auto const& Tool : ExternalTools)
  {
    QString ToolName = QString::fromStdString(Tool.Name);
    ExtToolMenu.setEnabled(true);
    ExternalToolsActions[ToolName] = new openfluid::ui::common::DefaultAction(ToolName, this);
    ExternalToolsActions[ToolName]->setData(QString::fromStdString(Tool.getCommand(ExternalToolsCtxt)));
    ExtToolMenu.addAction(ExternalToolsActions[ToolName]);
    connect(ExternalToolsActions[ToolName], SIGNAL(triggered()), this, SLOT(onOpenExternalToolAsked()));
  }
  Menu.addMenu(&ExtToolMenu);

  Menu.addSeparator();

  Menu.addAction(tr("Copy full path"), this, SLOT(onCopyFullPathAsked()));
  Menu.addAction(tr("Copy relative path"), this, SLOT(onCopyRelativePathAsked()));

  Menu.addSeparator();

  QMenu GitMenu;
  GitMenu.setTitle("Git");

  QAction* InitAction = new QAction("Init", this);
  QAction* StatusAction = new QAction("Status", this);
  QAction* LogAction = new QAction("Log", this);
  GitMenu.addAction(InitAction);
  GitMenu.addAction(StatusAction);
  GitMenu.addAction(LogAction);
  connect(InitAction, SIGNAL(triggered()), this, SLOT(onGitInitAsked()));
  connect(StatusAction, SIGNAL(triggered()), this, SLOT(onGitStatusAsked()));
  connect(LogAction, SIGNAL(triggered()), this, SLOT(onGitLogAsked()));
  
  Menu.addMenu(&GitMenu);

  if (currentIndex().data(Qt::DisplayRole).toString().contains("[") && openfluid::utils::GitProxy::isAvailable())
  {
    InitAction->setEnabled(false);
  }
  else
  {
    StatusAction->setEnabled(false);
    LogAction->setEnabled(false);
  }

  Menu.exec(viewport()->mapToGlobal(Point));
}


// =====================================================================
// =====================================================================


void WareSrcExplorer::onDoubleClicked(const QModelIndex& Index)
{
  emit doubleClicked(mp_Model->filePath(Index));
}


// =====================================================================
// =====================================================================


void WareSrcExplorer::onClicked(const QModelIndex& Index)
{
  emit clicked(mp_Model->filePath(Index));
}


// =====================================================================
// =====================================================================


void WareSrcExplorer::onCurrentChanged(const QModelIndex& Current)
{
  emit currentChanged(mp_Model->filePath(Current));
}


// =====================================================================
// =====================================================================


void WareSrcExplorer::onOpenExplorerAsked()
{
  emit openExplorerAsked(getCurrentDir());
}


// =====================================================================
// =====================================================================


void WareSrcExplorer::onOpenTerminalAsked()
{
  emit openTerminalAsked(getCurrentDir());
}


// =====================================================================
// =====================================================================


void WareSrcExplorer::onOpenExternalToolAsked()
{
  QAction* Sender = (QAction*)(QObject::sender());
  if (Sender != nullptr)
  {
    emit openExternalToolAsked(Sender->data().toString(), getCurrentPath());
  }
  
}


// =====================================================================
// =====================================================================


void WareSrcExplorer::onCopyFullPathAsked()
{
  QClipboard *Clip = QGuiApplication::clipboard();
  Clip->setText(getCurrentPath());
}


// =====================================================================
// =====================================================================


void WareSrcExplorer::onCopyRelativePathAsked()
{  
  QClipboard *Clip = QGuiApplication::clipboard();
  Clip->setText(QDir(m_TopDirectoryPath).relativeFilePath(getCurrentPath()));
}


// =====================================================================
// =====================================================================


QString WareSrcExplorer::getCurrentDir()
{
  if (currentIndex().isValid())
  {
    if (mp_Model->isDir(currentIndex()))
    {
      return mp_Model->filePath(currentIndex());
    }

    return mp_Model->fileInfo(currentIndex()).absolutePath();
  }

  return "";
}


// =====================================================================
// =====================================================================


QString WareSrcExplorer::getCurrentPath()
{
  if (currentIndex().isValid())
  {
    return mp_Model->filePath(currentIndex());
  }

  return "";
}


// =====================================================================
// =====================================================================


bool WareSrcExplorer::setCurrentPath(const QString& Path)
{
  if (Path.contains(m_TopDirectoryPath))
  {
    setCurrentIndex(mp_Model->index(Path));
    return true;
  }

  return false;
}


// =====================================================================
// =====================================================================


void WareSrcExplorer::scrollToCurrent()
{
  scrollTo(currentIndex());
}


// =====================================================================
// =====================================================================


void WareSrcExplorer::onNewFileAsked()
{
  if (!currentIndex().isValid())
  {
    return;
  }

  auto PInfo = 
    openfluid::waresdev::WareSrcEnquirer::getWareInfoFromPath(mp_Model->filePath(currentIndex()).toStdString());

  openfluid::ui::common::createNewFile(this,QString::fromStdString(PInfo.AbsoluteWarePath));
}


// =====================================================================
// =====================================================================


void WareSrcExplorer::onNewFolderAsked()
{
  if (!currentIndex().isValid())
  {
    return;
  }

  QString CurrentPath = mp_Model->filePath(currentIndex());

  QString WarePath = 
    QString::fromStdString(
      openfluid::waresdev::WareSrcEnquirer::getWareInfoFromPath(CurrentPath.toStdString()).AbsoluteWarePath
    );

  QString NewPath = WareExplorerDialog::getCreateFolderPath(this, WarePath, CurrentPath);
  std::cout << "NEW PATH FO" << NewPath.toStdString() << std::endl;
  if (!NewPath.isEmpty())
  {
    QDir(WarePath).mkdir(NewPath);
    setCurrentPath(NewPath);
  }
}


// =====================================================================
// =====================================================================


void WareSrcExplorer::onNewFragmentAsked()
{
  if (!openfluid::utils::GitProxy::isAvailable())
  {
    QMessageBox::warning(this, tr("Import not available"), tr("Neither CMake program nor Git program can be found."));
    return;
  }
  if (!currentIndex().isValid())
  {
    return;
  }

  QString CurrentPath = mp_Model->filePath(currentIndex());

  QString WarePath = 
    QString::fromStdString(
      openfluid::waresdev::WareSrcEnquirer::getWareInfoFromPath(CurrentPath.toStdString()).AbsoluteWarePath
    );

  openfluid::utils::GitProxy Git;
  bool HasWareVersionControl = Git.status(WarePath).m_IsGitTracked;

  openfluid::ui::waresdev::FragmentsSrcImportDialog Dialog(this, WarePath, HasWareVersionControl);
  Dialog.exec();
}


// =====================================================================
// =====================================================================


void WareSrcExplorer::onDeleteFileAsked()
{
  if (!currentIndex().isValid())
  {
    return;
  }

  QString CurrentPath = mp_Model->filePath(currentIndex());

  if (QMessageBox::warning(this, tr("Delete file"), tr("Are you sure you want to delete \"%1\"?").arg(CurrentPath),
                           QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Cancel) == QMessageBox::Cancel)
  {
    return;
  }

  if (QDir().remove(CurrentPath))
  {
    emit fileDeleted(CurrentPath);
  }
  else
  {
    QMessageBox::critical(0, tr("Error"), tr("Unable to remove the file \"%1\"").arg(CurrentPath));
  }
}


// =====================================================================
// =====================================================================


void WareSrcExplorer::onGitInitAsked()
{
  openfluid::utils::GitProxy Git;

  QString CurrentPath = mp_Model->filePath(currentIndex());

  QString WarePath = 
    QString::fromStdString(
      openfluid::waresdev::WareSrcEnquirer::getWareInfoFromPath(CurrentPath.toStdString()).AbsoluteWarePath
    );

  openfluid::ui::waresdev::WareGitDialog Dialog;
  Dialog.setWindowTitle("git init");

  Dialog.setContent(Git.init(WarePath));
  Dialog.exec();

  mp_Model->updateGitStatusInfo(CurrentPath);
}


// =====================================================================
// =====================================================================


void WareSrcExplorer::onGitStatusAsked()
{
  openfluid::utils::GitProxy Git;

  openfluid::ui::waresdev::WareGitDialog Dialog;
  Dialog.setWindowTitle("git status");
  Dialog.setContent(Git.statusHtml(mp_Model->filePath(currentIndex()), true));
  Dialog.exec();
}


// =====================================================================
// =====================================================================


void WareSrcExplorer::onGitLogAsked()
{
  openfluid::utils::GitProxy Git;

  openfluid::ui::waresdev::WareGitDialog Dialog;
  Dialog.setWindowTitle("git log");
  Dialog.setContent(Git.logHtml(mp_Model->filePath(currentIndex()), true));
  Dialog.exec();
}


// =====================================================================
// =====================================================================


void WareSrcExplorer::updateExplorerModel(const QString& Path)
{
  mp_Model->updateGitStatusInfo(Path);
}


// =====================================================================
// =====================================================================


} } } // namespaces
