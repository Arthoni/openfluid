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
 \file WareSrcSyntaxManager_TEST.cpp
 \brief Implements ...

 \author Aline LIBRES <aline.libres@gmail.com>
 */

#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_WareSrcSyntaxManager
#include <boost/test/unit_test.hpp>

#include <openfluid/ui/waresdev/WareSrcSyntaxManager.hpp>

#include <QRegExp>


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(parseSyntaxFile)
{
  openfluid::ui::waresdev::WareSrcSyntaxManager* Manager =
      openfluid::ui::waresdev::WareSrcSyntaxManager::getInstance();

  BOOST_CHECK(!Manager->parseSyntaxFile("wrongtype").isEmpty());

  BOOST_CHECK_EQUAL(qPrintable(Manager->parseSyntaxFile("cpp")), "");

  BOOST_CHECK(!Manager->getHighlightingRules("cpp").isEmpty());
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(checkRegExp)
{
  BOOST_CHECK(QRegExp("\\bint\\b").indexIn("bla int bla") != -1);
  BOOST_CHECK(QRegExp("\\bit\\b").indexIn("bla int bla") == -1);

  QRegExp preproc("^\\s*#\\s*\\w+");
  BOOST_CHECK(preproc.indexIn("#include bla") != -1);
  BOOST_CHECK(preproc.indexIn(" #include bla") != -1);
  BOOST_CHECK(preproc.indexIn("# include bla") != -1);

  BOOST_CHECK(
      QRegExp("\\bOPENFLUID_\\w+\\b").indexIn(" OPENFLUID_GET(bla) ") != -1);

  QRegExp simpleComment("//.*$");
  BOOST_CHECK(simpleComment.indexIn("// comment") != -1);
  BOOST_CHECK(simpleComment.indexIn("bla //comment ") != -1);
  BOOST_CHECK(simpleComment.indexIn("bla / bla") == -1);
  BOOST_CHECK(simpleComment.indexIn("bla/bla") == -1);

  QRegExp multiComment("(/\\*).*(\\*/)");
  multiComment.indexIn("/* comment */");
  BOOST_CHECK(multiComment.matchedLength() != -1);
  multiComment.indexIn("/* comment\n next line */");
  BOOST_CHECK(multiComment.matchedLength() != -1);
  multiComment.indexIn("// comment ");
  BOOST_CHECK(multiComment.matchedLength() == -1);
  multiComment.indexIn("/ bla ");
  BOOST_CHECK(multiComment.matchedLength() == -1);

  BOOST_CHECK(QRegExp("\".*\"").indexIn(" bla \"text text \"") != -1);
  BOOST_CHECK(QRegExp("\'.*\'").indexIn(" bla 'text text '") != -1);

  QRegExp function("\\w+\\s*(?=\\()");
  BOOST_CHECK(function.indexIn("func()") != -1);
  BOOST_CHECK(function.indexIn("func(param)") != -1);
  BOOST_CHECK(function.indexIn("func ( param)") != -1);
  BOOST_CHECK(function.indexIn("2*(3+5)") == -1);
}

