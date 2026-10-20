/*

  This file is part of OpenFLUID software
  Copyright(c) 2021-2026, INRAE


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
  @file JSON.hpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#ifndef __OPENFLUID_THIRDPARTY_JSON_HPP__
#define __OPENFLUID_THIRDPARTY_JSON_HPP__


#include <openfluid/thirdparty/nlohmannjson-3.10.2/nlohmann/json.hpp>


namespace openfluid { namespace thirdparty {

/**
  Alias to embed the nlohmann::json third party library as openfluid::thirdparty::json

  Basic examples:

  openfluid::thirdparty::json ListOfItems(openfluid::thirdparty::json::value_t::array); // create a json list

  for (unsigned int Data : {1,2,5})
  {
    openfluid::thirdparty::json Item(openfluid::thirdparty::json::value_t::object); // create a json map
    Item["name"] = "My item "+std::to_string(Data); // map value can be a string
    Item["data"] = Data;                            // map value can also be an int (or any built-in type)
    ListOfItems.push_back(Item);  // adding json object to json list
  }

  // ListOfItems is now 
  //   [{"name": "My item 1", "data": 1}, {"name": "My item 2", "data": 2}, {"name": "My item 5", "data": 5}]

  See https://json.nlohmann.me/api/ordered_json/ for full documentation
*/
using json = nlohmann::ordered_json;


} } // namespaces


#endif /* __OPENFLUID_THIRDPARTY_JSON_HPP__ */
