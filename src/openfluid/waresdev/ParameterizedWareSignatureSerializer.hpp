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
  @file WareSignatureSerializer.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inrae.fr>
  @author Armel THÖNI <armel.thoni@inrae.fr>
 */


#ifndef __OPENFLUID_WARESDEV_PARAMETERIZEDWARESIGNATURESERIALIZER_HPP__
#define __OPENFLUID_WARESDEV_PARAMETERIZEDWARESIGNATURESERIALIZER_HPP__


#include <string>
#include <vector>

#include <openfluid/dllexport.hpp>
#include <openfluid/tools/IDHelpers.hpp>
#include <openfluid/ware/TypeDefs.hpp>
#include <openfluid/ware/WareSignature.hpp>
#include <openfluid/waresdev/WareSignatureSerializer.hpp>


namespace openfluid { namespace waresdev {


std::string getCPPValueType(const openfluid::core::Value::Type ValueType)
{
  switch (ValueType)
  {
    case openfluid::core::Value::NONE:
      return "openfluid::core::Value::NONE";
    case openfluid::core::Value::DOUBLE:
      return "openfluid::core::Value::DOUBLE";
    case openfluid::core::Value::INTEGER:
      return "openfluid::core::Value::INTEGER";
    case openfluid::core::Value::BOOLEAN:
      return "openfluid::core::Value::BOOLEAN";
    case openfluid::core::Value::VECTOR:
      return "openfluid::core::Value::VECTOR";
    case openfluid::core::Value::MATRIX:
      return "openfluid::core::Value::MATRIX";
    case openfluid::core::Value::MAP:
      return "openfluid::core::Value::MAP";
    case openfluid::core::Value::TREE:
      return "openfluid::core::Value::TREE";
    case openfluid::core::Value::STRING:
      return "openfluid::core::Value::STRING";
    case openfluid::core::Value::NULLL:
      return "openfluid::core::Value::NULLL";
    default:
      return "";
  }
}


// =====================================================================
// =====================================================================



inline openfluid::ware::SignatureDataItem readDataItemFromJSON(const openfluid::thirdparty::json& Item)
{
  openfluid::ware::SignatureDataItem Data;

  Data.Name = Item.value("name","");

  if (!openfluid::tools::isValidVariableName(Data.Name))
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,"Missing or invalid data name");
  }

  Data.Description = Item.value("description","");
  Data.SIUnit = Item.value("siunit","");
  
  openfluid::core::Value::Type VT;

  if (openfluid::core::Value::getValueTypeFromString(Item.value("type",""),VT))
  {
    Data.DataType = VT;
  }

  return Data;
}


// =====================================================================
// =====================================================================


inline openfluid::thirdparty::json serializeDataItemToJSON(const openfluid::ware::SignatureDataItem& Item)
{
  openfluid::thirdparty::json Json = openfluid::thirdparty::json::object();

  Json["name"] = Item.Name;
  Json["description"] = Item.Description;
  Json["siunit"] = Item.SIUnit;
  Json["type"] = openfluid::core::Value::getStringFromValueType(Item.DataType);

  return Json;
}


// =====================================================================
// =====================================================================


inline std::vector<openfluid::ware::SignatureDataItem> 
readDataListFromJSON(const openfluid::thirdparty::json& Json)
{
  std::vector<openfluid::ware::SignatureDataItem> List;

  if (Json.is_array())
  {
    for (const auto& I : Json)
    {
      List.push_back(readDataItemFromJSON(I));
    }
  }

  return List;
}


// =====================================================================
// =====================================================================

template<class SignatureType>
class OPENFLUID_API ParameterizedWareSignatureSerializer : public WareSignatureSerializer<SignatureType>
{
  protected:

    ParameterizedWareSignatureSerializer(): WareSignatureSerializer<SignatureType>()
    { }

    void unserializeParametersFromJSON(const openfluid::thirdparty::json& Json, 
                                       SignatureType& Sign) const;

    openfluid::thirdparty::json serializeParametersToJSON(const SignatureType& Sign) const;

    std::string getCPPDataString(const std::string Member, const std::vector<openfluid::ware::SignatureDataItem>& Data);

    std::string toWareCPPParams(const SignatureType& Sign) const;

    openfluid::thirdparty::json serializeDataToJSON(const SignatureType& Sign) const;


  public:

    ~ParameterizedWareSignatureSerializer()
    { }
};

template<class SignatureType>
openfluid::thirdparty::json
ParameterizedWareSignatureSerializer<SignatureType>::serializeParametersToJSON(const SignatureType& Sign) const
{
  openfluid::thirdparty::json Json = openfluid::thirdparty::json::object();

  auto JsonReq = openfluid::thirdparty::json::array();
  for (const auto& P : Sign.HandledData.RequiredParams)
  {
    JsonReq.push_back(serializeDataItemToJSON(P));
  }
  Json["required"] = JsonReq;

  auto JsonUs = openfluid::thirdparty::json::array();
  for (const auto& P : Sign.HandledData.UsedParams)
  {
    JsonUs.push_back(serializeDataItemToJSON(P));
  }
  Json["used"] = JsonUs;

  return Json;
}


// =====================================================================
// =====================================================================


template<class SignatureType>
openfluid::thirdparty::json
ParameterizedWareSignatureSerializer<SignatureType>::serializeDataToJSON(const SignatureType& Sign) const
{
  openfluid::thirdparty::json Json = openfluid::thirdparty::json::object();

  Json["parameters"] = serializeParametersToJSON(Sign);

  return Json;
}


// =====================================================================
// =====================================================================


template<class SignatureType>
void ParameterizedWareSignatureSerializer<SignatureType>::unserializeParametersFromJSON(const openfluid::thirdparty::json& Json, 
                                                                 SignatureType& Sign) const
{
  if (Json.contains("used"))
  {
    Sign.HandledData.UsedParams = readDataListFromJSON(Json.at("used"));
  }

  if (Json.contains("required"))
  {
    Sign.HandledData.RequiredParams = readDataListFromJSON(Json.at("required"));
  }
}


// =====================================================================
// =====================================================================



template<class SignatureType>
std::string ParameterizedWareSignatureSerializer<SignatureType>::getCPPDataString(const std::string Member, 
                                                           const std::vector<openfluid::ware::SignatureDataItem>& Data)
{
  std::string Str;
  
  for (const auto& D : Data)
  {

    Str += getCPPMethod(Member,"push_back",{"{"+getQuotedString(D.Name)+","+
                                                getQuotedString(D.Description)+","+
                                                getQuotedString(D.SIUnit)+","+
                                                getCPPValueType(D.DataType)+"}"});
  }

  return Str;
}


// =====================================================================
// =====================================================================


template<class SignatureType>
std::string ParameterizedWareSignatureSerializer<SignatureType>::toWareCPPParams(const SignatureType& Sign) const
{
  std::string CPP;
  
  // Parameters
  CPP += getCPPDataString("HandledData.UsedParams",Sign.HandledData.UsedParams);
  CPP += getCPPDataString("HandledData.RequiredParams",Sign.HandledData.RequiredParams);
  
  return CPP;
}


} }  // namespaces


#endif /* __OPENFLUID_WARESDEV_PARAMETERIZEDWARESIGNATURESERIALIZER_HPP__ */