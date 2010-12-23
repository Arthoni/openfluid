/*
  This file is part of OpenFLUID software
  Copyright (c) 2007-2010 INRA-Montpellier SupAgro


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
  along with OpenFLUID.  If not, see <http://www.gnu.org/licenses/>.

  In addition, as a special exception, INRA gives You the additional right
  to dynamically link the code of OpenFLUID with code not covered
  under the GNU General Public License ("Non-GPL Code") and to distribute
  linked combinations including the two, subject to the limitations in this
  paragraph. Non-GPL Code permitted under this exception must only link to
  the code of OpenFLUID dynamically through the OpenFLUID libraries
  interfaces, and only for building OpenFLUID plugins. The files of
  Non-GPL Code may be link to the OpenFLUID libraries without causing the
  resulting work to be covered by the GNU General Public License. You must
  obey the GNU General Public License in all respects for all of the
  OpenFLUID code and other code used in conjunction with OpenFLUID
  except the Non-GPL Code covered by this exception. If you modify
  this OpenFLUID, you may extend this exception to your version of the file,
  but you are not obligated to do so. If you do not wish to provide this
  exception without modification, you must delete this exception statement
  from your version and license this OpenFLUID solely under the GPL without
  exception.


 == Other Usage ==

  Other Usage means a use of OpenFLUID that is inconsistent with the GPL
  license, and requires a written agreement between You and INRA.
  Licensees for Other Usage of OpenFLUID may use this file in accordance
  with the terms contained in the written agreement between You and INRA.
*/


/**
  \file MarketPackage_TEST.cpp
  \brief Implements ...

  \author Jean-Christophe FABRE <fabrejc@supagro.inra.fr>
*/


#define BOOST_TEST_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_marketpackage
#include <boost/test/unit_test.hpp>
#include <boost/test/auto_unit_test.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/convenience.hpp>

#include <openfluid/market.hpp>

#include <tests-config.hpp>

// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction)
{
  std::string TmpDir = boost::filesystem::path(CONFIGTESTS_OUTPUT_DATA_DIR+"/market/packages/temp").string();;
  std::string MarketBagDir = boost::filesystem::path(CONFIGTESTS_OUTPUT_DATA_DIR+"/market/packages/market").string();;

  openfluid::market::MarketPackage::initialize();

  openfluid::market::MarketPackage::setWorksDirs(TmpDir,MarketBagDir);

  boost::filesystem::remove_all(boost::filesystem::path(TmpDir));
  boost::filesystem::remove_all(boost::filesystem::path(MarketBagDir));

  BOOST_REQUIRE(boost::filesystem::create_directories(boost::filesystem::path(openfluid::market::MarketPackage::getMarketBagDir())));
  BOOST_REQUIRE(boost::filesystem::create_directories(boost::filesystem::path(openfluid::market::MarketPackage::getTempBuildsDir())));
  BOOST_REQUIRE(boost::filesystem::create_directories(boost::filesystem::path(openfluid::market::MarketPackage::getTempDownloadsDir())));


  openfluid::market::MarketSrcPackage SPack("dummy.id","file:://path/to/file");

  openfluid::market::MarketBinPackage BPack("dummy.id","file:://path/to/file");
}

// =====================================================================
// =====================================================================

BOOST_AUTO_TEST_CASE(check_operations)
{

  std::string TmpDir = boost::filesystem::path(CONFIGTESTS_OUTPUT_DATA_DIR+"/market/packages/temp").string();;
  std::string MarketBagDir = boost::filesystem::path(CONFIGTESTS_OUTPUT_DATA_DIR+"/market/packages/market").string();;

  openfluid::market::MarketPackage::initialize();

  openfluid::market::MarketPackage::setWorksDirs(TmpDir,MarketBagDir);


  openfluid::market::MarketBinPackage BPack("tests.market.bin.dummy","file://"+boost::filesystem::path(CONFIGTESTS_INPUT_DATASETS_DIR+"/market/packages/tests.market.bin.dummy.ofpk").string());

  BPack.download();
  BOOST_REQUIRE(boost::filesystem::exists(boost::filesystem::path(CONFIGTESTS_OUTPUT_DATA_DIR+"/market/packages/temp/downloads/tests.market.bin.dummy.ofpk")));

  BPack.process();
  BOOST_REQUIRE(boost::filesystem::exists(boost::filesystem::path(CONFIGTESTS_OUTPUT_DATA_DIR+"/market/packages/market/tests.market.bin.dummy.pdf.txt")));
  BOOST_REQUIRE(boost::filesystem::exists(boost::filesystem::path(CONFIGTESTS_OUTPUT_DATA_DIR+"/market/packages/market/tests.market.bin.dummy.xxmpi.txt")));



  std::cout << "CMake Options : " << CONFIGTESTS_OPTIONS_FOR_CMAKE << std::endl;

  openfluid::market::MarketSrcPackage SPack("tests.market.src.use","file://"+boost::filesystem::path(CONFIGTESTS_INPUT_DATASETS_DIR+"/market/packages/tests.market.src.use.ofpk").string());
  SPack.setAdditionalBuildConfigOptions(CONFIGTESTS_OPTIONS_FOR_CMAKE);

  SPack.download();
  BOOST_REQUIRE(boost::filesystem::exists(boost::filesystem::path(CONFIGTESTS_OUTPUT_DATA_DIR+"/market/packages/temp/downloads/tests.market.src.use.ofpk")));

  SPack.process();
  BOOST_REQUIRE(boost::filesystem::exists(boost::filesystem::path(CONFIGTESTS_OUTPUT_DATA_DIR+"/market/packages/temp/builds/tests.market.src.use/tests.market.src.use"+CONFIGTESTS_PLUGINS_EXT)));
  BOOST_REQUIRE(boost::filesystem::exists(boost::filesystem::path(CONFIGTESTS_OUTPUT_DATA_DIR+"/market/packages/market/tests.market.src.use"+CONFIGTESTS_PLUGINS_EXT)));


}



