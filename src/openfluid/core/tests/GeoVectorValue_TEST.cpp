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
  @file GeoVectorValue_TEST.cpp

  @author Aline LIBRES <libres@supagro.inra.fr>
  @author Jean-Christophe Fabre <jean-christophe.fabre@inra.fr>
 */


#define BOOST_TEST_NO_MAIN
#define BOOST_AUTO_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE unittest_geovectorvalue


#include <iostream>

#include <boost/test/unit_test.hpp>

#include <openfluid/base/FrameworkException.hpp>
#include <openfluid/core/GeoVectorValue.hpp>
#include <openfluid/utils/GDALCompatibility.hpp>

#include "tests-config.hpp"


class GeoVectorValueSub: public openfluid::core::GeoVectorValue
{
  public:

    GeoVectorValueSub(const std::string& PrefixPath, const std::string& RelativePath) :
        openfluid::core::GeoVectorValue(PrefixPath, RelativePath)
    { }

    GDALDataset_COMPAT* dataWithoutCheck()
    {
      return mp_Data;
    }

    std::string getAbsolutePath()
    {
      return computeAbsolutePath(m_FilePath, m_FileName);
    }

    void tryToOpenSource()
    {
      openfluid::core::GeoVectorValue::tryToOpenSource();
    }
};


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_construction)
{
  GeoVectorValueSub* Val = new GeoVectorValueSub(CONFIGTESTS_INPUT_MISCDATA_DIR,"GeoVectorValue");

  BOOST_CHECK_EQUAL(Val->getType(),openfluid::core::UnstructuredValue::GeoVectorValue);

  BOOST_CHECK_EQUAL(Val->getAbsolutePath(),CONFIGTESTS_INPUT_MISCDATA_DIR+"/GeoVectorValue");

  BOOST_CHECK(!Val->dataWithoutCheck());

  delete Val;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_tryOpeningSource_WrongDir)
{
  GeoVectorValueSub* Val = new GeoVectorValueSub(CONFIGTESTS_INPUT_MISCDATA_DIR,"WrongDir");

  BOOST_CHECK_THROW(Val->tryToOpenSource(), openfluid::base::FrameworkException);
  BOOST_CHECK(!Val->dataWithoutCheck());

  delete Val;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_tryOpeningSource_CorrectDir)
{
  GeoVectorValueSub* Val = new GeoVectorValueSub(CONFIGTESTS_INPUT_MISCDATA_DIR,"GeoVectorValue");

  Val->tryToOpenSource();
  BOOST_CHECK(Val->dataWithoutCheck());

  delete Val;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_tryOpeningSource_WrongFile)
{
  GeoVectorValueSub* Val = new GeoVectorValueSub(CONFIGTESTS_INPUT_MISCDATA_DIR,"GeoVectorValue/wrongfile");

  BOOST_CHECK_THROW(Val->tryToOpenSource(), openfluid::base::FrameworkException);

  BOOST_CHECK(!Val->dataWithoutCheck());

  delete Val;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_tryOpeningSource_WrongFile_NoExtension)
{
  if (!GDALGetDriverByName_COMPAT("ESRI Shapefile"))
  {
    std::cout << "GDAL driver not found : ESRI Shapefile -> test not run" << std::endl;
    return;
  }

  GeoVectorValueSub* Val = new GeoVectorValueSub(CONFIGTESTS_INPUT_MISCDATA_DIR,"GeoVectorValue/SU");

  BOOST_CHECK_THROW(Val->tryToOpenSource(), openfluid::base::FrameworkException);

  BOOST_CHECK(!Val->dataWithoutCheck());

  delete Val;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_tryOpeningSource_WrongFileExtension)
{
  GeoVectorValueSub* Val = new GeoVectorValueSub(CONFIGTESTS_INPUT_MISCDATA_DIR,"GeoVectorValue/SU.aaa");

  BOOST_CHECK_THROW(Val->tryToOpenSource(), openfluid::base::FrameworkException);

  BOOST_CHECK(!Val->dataWithoutCheck());

  delete Val;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_tryOpeningSource_CorrectFile_Shp)
{
  if (!GDALGetDriverByName_COMPAT("ESRI Shapefile"))
  {
    std::cout << "GDAL driver not found : ESRI Shapefile -> test not run" << std::endl;
    return;
  }

  GeoVectorValueSub* Val = new GeoVectorValueSub(CONFIGTESTS_INPUT_MISCDATA_DIR,"GeoVectorValue/SU.shp");

  Val->tryToOpenSource();

  BOOST_CHECK(Val->dataWithoutCheck());

  delete Val;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_tryOpeningSource_CorrectFile_Dbf)
{
  if (!GDALGetDriverByName_COMPAT("ESRI Shapefile"))
  {
    std::cout << "GDAL driver not found : ESRI Shapefile -> test not run" << std::endl;
    return;
  }

  GeoVectorValueSub* Val = new GeoVectorValueSub(CONFIGTESTS_INPUT_MISCDATA_DIR,"GeoVectorValue/SU.dbf");

  Val->tryToOpenSource();

  BOOST_CHECK(Val->dataWithoutCheck());

  delete Val;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_tryOpeningSource_CorrectFile_Shx)
{
  if (!GDALGetDriverByName_COMPAT("ESRI Shapefile"))
  {
    std::cout << "GDAL driver not found : ESRI Shapefile -> test not run" << std::endl;
    return;
  }

  GeoVectorValueSub* Val = new GeoVectorValueSub(CONFIGTESTS_INPUT_MISCDATA_DIR,"GeoVectorValue/SU.shx");

  Val->tryToOpenSource();

  BOOST_CHECK(Val->dataWithoutCheck());

  delete Val;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_get_CorrectDir)
{
  GeoVectorValueSub* Val = new GeoVectorValueSub(CONFIGTESTS_INPUT_MISCDATA_DIR,"GeoVectorValue");

  BOOST_CHECK(Val->data());

  delete Val;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_get_WrongDir)
{
  GeoVectorValueSub* Val = new GeoVectorValueSub(CONFIGTESTS_INPUT_MISCDATA_DIR,"WrongDir");

  BOOST_CHECK_THROW(Val->data(), openfluid::base::FrameworkException);

  BOOST_CHECK(!Val->dataWithoutCheck());

  delete Val;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_Properties)
{
  if (!GDALGetDriverByName_COMPAT("ESRI Shapefile"))
  {
    std::cout << "GDAL driver not found : ESRI Shapefile -> test not run" << std::endl;
    return;
  }

  GeoVectorValueSub* Val = new GeoVectorValueSub(CONFIGTESTS_INPUT_MISCDATA_DIR, "GeoVectorValue/SU.shp");

  BOOST_CHECK(!Val->isLineType());
  BOOST_CHECK(Val->isPolygonType());

  BOOST_CHECK(Val->containsField("OFLD_ID"));
  BOOST_CHECK(!Val->containsField("wrongField"));

  BOOST_CHECK_EQUAL(Val->getFieldIndex("OFLD_ID"), 9);

  delete Val;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_tryOpeningSource_CorrectFile_GeoJSON)
{
  if (!GDALGetDriverByName_COMPAT("GeoJSON"))
  {
    std::cout << "GDAL driver not found : GeoJSON -> test not run" << std::endl;
    return;
  }

  GeoVectorValueSub* Val = new GeoVectorValueSub(CONFIGTESTS_INPUT_MISCDATA_DIR,"GeoVectorValue/SU.geojson");
  Val->tryToOpenSource();

  BOOST_CHECK(Val->dataWithoutCheck());
  BOOST_CHECK_EQUAL(Val->isPolygonType(),true);
  BOOST_CHECK_EQUAL(Val->containsField("OFLD_ID"),true);
#if (GDAL_VERSION_MAJOR >= 2)
  BOOST_CHECK_EQUAL(Val->data()->GetDriver()->GetDescription(),"GeoJSON");
#else
  BOOST_CHECK_EQUAL(Val->data()->GetDriver()->GetName(),"GeoJSON");
#endif

  GeoVectorValueSub* Val2 = new GeoVectorValueSub(CONFIGTESTS_INPUT_MISCDATA_DIR,"GeoVectorValue/RS.geojson");
  Val2->tryToOpenSource();

  BOOST_CHECK(Val2->dataWithoutCheck());
  BOOST_CHECK_EQUAL(Val2->isLineType(),true);
  BOOST_CHECK_EQUAL(Val2->containsField("OFLD_ID"),true);
#if (GDAL_VERSION_MAJOR >= 2)
  BOOST_CHECK_EQUAL(Val2->data()->GetDriver()->GetDescription(),"GeoJSON");
#else
  BOOST_CHECK_EQUAL(Val2->data()->GetDriver()->GetName(),"GeoJSON");
#endif


  delete Val2;
  delete Val;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_tryOpeningSource_CorrectFile_GML)
{
  if (!GDALGetDriverByName_COMPAT("Geography Markup Language (GML)"))
  {
    std::cout << "GDAL driver not found : GML -> test not run" << std::endl;
    return;
  }

  GeoVectorValueSub* Val = new GeoVectorValueSub(CONFIGTESTS_INPUT_MISCDATA_DIR,"GeoVectorValue/SU.gml");
  Val->tryToOpenSource();

  BOOST_CHECK(Val->dataWithoutCheck());
  BOOST_CHECK_EQUAL(Val->isPolygonType(),true);
  BOOST_CHECK_EQUAL(Val->containsField("OFLD_ID"),true);
#if (GDAL_VERSION_MAJOR >= 2)
  BOOST_CHECK_EQUAL(Val->data()->GetDriver()->GetDescription(),"GML");
#else
  BOOST_CHECK_EQUAL(Val->data()->GetDriver()->GetName(),"GML");
#endif

  GeoVectorValueSub* Val2 = new GeoVectorValueSub(CONFIGTESTS_INPUT_MISCDATA_DIR,"GeoVectorValue/RS.gml");
  Val2->tryToOpenSource();

  BOOST_CHECK(Val2->dataWithoutCheck());
  BOOST_CHECK_EQUAL(Val2->isLineType(),true);
  BOOST_CHECK_EQUAL(Val2->containsField("OFLD_ID"),true);
#if (GDAL_VERSION_MAJOR >= 2)
  BOOST_CHECK_EQUAL(Val2->data()->GetDriver()->GetDescription(),"GML");
#else
  BOOST_CHECK_EQUAL(Val2->data()->GetDriver()->GetName(),"GML");
#endif

  delete Val2;
  delete Val;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_Polygon_Geometry)
{
  if (!GDALGetDriverByName_COMPAT("ESRI Shapefile"))
  {
    std::cout << "GDAL driver not found : ESRI Shapefile -> test not run" << std::endl;
    return;
  }
  
  GeoVectorValueSub* Val = new GeoVectorValueSub(CONFIGTESTS_INPUT_MISCDATA_DIR, "GeoVectorValue/SU.shp");

  BOOST_CHECK_EQUAL(Val->getGeometryType(),wkbPolygon);

  BOOST_CHECK(Val->isPolygonType());

  BOOST_CHECK(!Val->isMultiPolygonType());
  BOOST_CHECK(!Val->isLineType());
  BOOST_CHECK(!Val->isMultiLineType());
  BOOST_CHECK(!Val->isPointType());
  BOOST_CHECK(!Val->isMultiPointType());

  delete Val;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_MultiPolygon_Geometry)
{
  if (!GDALGetDriverByName_COMPAT("GeoJSON"))
  {
    std::cout << "GDAL driver not found : GeoJSON -> test not run" << std::endl;
    return;
  }

  GeoVectorValueSub* Val = new GeoVectorValueSub(CONFIGTESTS_INPUT_MISCDATA_DIR, "GeoVectorValue/MultiSU.geojson");

  BOOST_CHECK_EQUAL(Val->getGeometryType(),wkbMultiPolygon);

  BOOST_CHECK(Val->isMultiPolygonType());

  BOOST_CHECK(!Val->isPolygonType());
  BOOST_CHECK(!Val->isLineType());
  BOOST_CHECK(!Val->isMultiLineType());
  BOOST_CHECK(!Val->isPointType());
  BOOST_CHECK(!Val->isMultiPointType());

  delete Val;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_Line_Geometry)
{
  if (!GDALGetDriverByName_COMPAT("ESRI Shapefile"))
  {
    std::cout << "GDAL driver not found : ESRI Shapefile -> test not run" << std::endl;
    return;
  }

  GeoVectorValueSub* Val = new GeoVectorValueSub(CONFIGTESTS_INPUT_MISCDATA_DIR, "GeoVectorValue/RS.shp");

  BOOST_CHECK_EQUAL(Val->getGeometryType(),wkbLineString);

  BOOST_CHECK(Val->isLineType());

  BOOST_CHECK(!Val->isMultiPolygonType());
  BOOST_CHECK(!Val->isPolygonType());
  BOOST_CHECK(!Val->isMultiLineType());
  BOOST_CHECK(!Val->isPointType());
  BOOST_CHECK(!Val->isMultiPointType());

  delete Val;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_MultiLine_Geometry)
{
  if (!GDALGetDriverByName_COMPAT("GeoJSON"))
  {
    std::cout << "GDAL driver not found : GeoJSON -> test not run" << std::endl;
    return;
  }

  GeoVectorValueSub* Val = new GeoVectorValueSub(CONFIGTESTS_INPUT_MISCDATA_DIR, "GeoVectorValue/MultiRS.geojson");

  BOOST_CHECK_EQUAL(Val->getGeometryType(),wkbMultiLineString);

  BOOST_CHECK(Val->isMultiLineType());

  BOOST_CHECK(!Val->isMultiPolygonType());
  BOOST_CHECK(!Val->isPolygonType());
  BOOST_CHECK(!Val->isLineType());
  BOOST_CHECK(!Val->isPointType());
  BOOST_CHECK(!Val->isMultiPointType());

  delete Val;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_Point_Geometry)
{
  if (!GDALGetDriverByName_COMPAT("ESRI Shapefile"))
  {
    std::cout << "GDAL driver not found : ESRI Shapefile -> test not run" << std::endl;
    return;
  }

  GeoVectorValueSub* Val = new GeoVectorValueSub(CONFIGTESTS_INPUT_MISCDATA_DIR, "GeoVectorValue/PU.shp");

  BOOST_CHECK_EQUAL(Val->getGeometryType(),wkbPoint);

  BOOST_CHECK(Val->isPointType());

  BOOST_CHECK(!Val->isMultiPolygonType());
  BOOST_CHECK(!Val->isPolygonType());
  BOOST_CHECK(!Val->isLineType());
  BOOST_CHECK(!Val->isMultiLineType());
  BOOST_CHECK(!Val->isMultiPointType());

  delete Val;
}


// =====================================================================
// =====================================================================


BOOST_AUTO_TEST_CASE(check_MultiPoint_Geometry)
{
  if (!GDALGetDriverByName_COMPAT("GeoJSON"))
  {
    std::cout << "GDAL driver not found : GeoJSON -> test not run" << std::endl;
    return;
  }

  GeoVectorValueSub* Val = new GeoVectorValueSub(CONFIGTESTS_INPUT_MISCDATA_DIR, "GeoVectorValue/MultiPU.geojson");

  BOOST_CHECK_EQUAL(Val->getGeometryType(),wkbMultiPoint);

  BOOST_CHECK(Val->isMultiPointType());

  BOOST_CHECK(!Val->isMultiPolygonType());
  BOOST_CHECK(!Val->isPolygonType());
  BOOST_CHECK(!Val->isLineType());
  BOOST_CHECK(!Val->isMultiLineType());
  BOOST_CHECK(!Val->isPointType());

  delete Val;
}


// =====================================================================
// =====================================================================


int main(int argc, char *argv[])
{
  GDALAllRegister_COMPAT();

  return ::boost::unit_test::unit_test_main(&init_unit_test, argc, argv);
}
