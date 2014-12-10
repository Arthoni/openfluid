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
 \file WareSrcFileEditor.cpp
 \brief Implements ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#include <openfluid/ui/waresdev/WareSrcFileEditor.hpp>

#include <QDir>

#include <openfluid/base/FrameworkException.hpp>
#include <openfluid/ui/waresdev/WareSrcSyntaxHighlighter.hpp>


namespace openfluid { namespace ui { namespace waresdev {


// =====================================================================
// =====================================================================


WareSrcFileEditor::WareSrcFileEditor(const QString& FilePath, QWidget* Parent) :
    QPlainTextEdit(Parent)
{
  QFile File(FilePath);
  if (!File.open(QIODevice::ReadOnly | QIODevice::Text))
    throw openfluid::base::FrameworkException(
        "WareSrcFileEditor constructor",
        QString("Cannot open file %1").arg(FilePath).toStdString());

  QMap<QString, QString> FileTypes;
  FileTypes["cpp"] =
      "*.c++;*.cxx;*.cpp;*.cc;*.C;*.h;*.hh;*.H;*.h++;*.hxx;*.hpp;*.hcc;*.moc";
  FileTypes["fortran"] =
      "*.f;*.F;*.for;*.FOR;*.f90;*.F90;*.fpp;*.FPP;*.f95;*.F95";
  FileTypes["cmake"] = "CMakeLists.txt;CMake.in.config;*.cmake";
  FileTypes["latex"] = "*.tex;*.ltx;*.dtx;*.sty;*.cls;*.bbx;*.cbx;*.lbx;*.bib";

  QString FileName = QFileInfo(File).fileName();
  for (QMap<QString, QString>::iterator it = FileTypes.begin();
      it != FileTypes.end(); ++it)
  {
    if (QDir::match(it.value(), FileName))
    {
      new WareSrcSyntaxHighlighter(document(), it.key());
      break;
    }
  }

  // TODO get defaults from conf file
  // setStyleSheet("fFont: 11pt \"Courier\";");
  QFont Font;
  Font.setFamily("Courier");
  Font.setFixedPitch(true);
  Font.setPointSize(11);
  setFont(Font);


  setPlainText(File.readAll());
}


// =====================================================================
// =====================================================================


WareSrcFileEditor::~WareSrcFileEditor()
{

}


// =====================================================================
// =====================================================================


} } }  // namespaces
