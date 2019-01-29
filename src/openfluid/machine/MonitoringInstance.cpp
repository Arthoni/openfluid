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
  @file MonitoringInstance.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
 */


#include <openfluid/base/RunContextManager.hpp>
#include <openfluid/machine/MonitoringInstance.hpp>
#include <openfluid/machine/SimulationBlob.hpp>
#include <openfluid/machine/ObserverPluginsManager.hpp>
#include <openfluid/machine/ObserverInstance.hpp>


namespace openfluid { namespace machine {


MonitoringInstance::MonitoringInstance(openfluid::machine::SimulationBlob& SimulationBlob):
    m_SimulationBlob(SimulationBlob), m_Initialized(false)
{

}


// =====================================================================
// =====================================================================


MonitoringInstance::~MonitoringInstance()
{
  if (m_Initialized)
  {
    finalize();
  }
}


// =====================================================================
// =====================================================================


void MonitoringInstance::appendObserver(std::shared_ptr<ObserverInstance> ObsInstance)
{
  if (m_Initialized)
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "Trying to append observer after observers list initialization");
  }

  m_Observers.push_back(std::move(ObsInstance));
}


// =====================================================================
// =====================================================================


void MonitoringInstance::initialize(openfluid::base::SimulationLogger* SimLogger)
{
  openfluid::machine::ObserverPluginsManager* OPlugsMgr = openfluid::machine::ObserverPluginsManager::instance();

  std::list<std::shared_ptr<ObserverInstance>>::const_iterator ObsIter; 
  ObserverInstance* CurrentObserver;

  ObsIter = m_Observers.begin();
  while (ObsIter != m_Observers.end())
  {
    CurrentObserver = (*ObsIter).get();
    OPlugsMgr->completeSignatureWithWareBody(CurrentObserver);

    CurrentObserver->Body->linkToSimulationLogger(SimLogger);
    CurrentObserver->Body->linkToSimulation(&(m_SimulationBlob.simulationStatus()));
    CurrentObserver->Body->linkToRunEnvironment(&openfluid::base::RunContextManager::instance()->getWaresEnvironment());
    CurrentObserver->Body->linkToSpatialGraph(&(m_SimulationBlob.spatialGraph()));
    CurrentObserver->Body->linkToDatastore(&(m_SimulationBlob.datastore()));
    CurrentObserver->Body->initializeWare(CurrentObserver->Signature->ID);

    ++ObsIter;
  }

  m_Initialized = true;
}


// =====================================================================
// =====================================================================


void MonitoringInstance::finalize()
{
  if (!m_Initialized)
  {
    throw openfluid::base::FrameworkException(OPENFLUID_CODE_LOCATION,
                                              "Trying to finalize an uninitialized observers list");
  }


  // call of finalizeWare method on each observer
  for (auto&& CurrentObserver : m_Observers)
  {
    CurrentObserver->Body->finalizeWare();
  }


  // destroy of each observer
  for (auto&& CurrentObserver : m_Observers)
  {
    CurrentObserver->Body.reset();
    // TODO reset Signature?
  }

  m_Initialized = false;
}


// =====================================================================
// =====================================================================


void MonitoringInstance::call_initParams() const
{
  // call of initParams method on each observer
  for (auto&& CurrentObserver : m_Observers)
  {
      CurrentObserver->Body->initParams(CurrentObserver->Params);
  }
}


// =====================================================================
// =====================================================================


void MonitoringInstance::call_onPrepared() const
{
  // call of onPrepared method on each observer
  for (auto&& CurrentObserver : m_Observers)
  {
    CurrentObserver->Body->onPrepared();
  }
}


// =====================================================================
// =====================================================================


void MonitoringInstance::call_onInitializedRun() const
{
  // call of onInitializedRun method on each observer
  for (auto&& CurrentObserver : m_Observers)
  {
    CurrentObserver->Body->onInitializedRun();
  }
}


// =====================================================================
// =====================================================================


void MonitoringInstance::call_onStepCompleted(const openfluid::core::TimeIndex_t& TimeIndex) const
{
  // call of onStepCompleted and setPreviousTimeIndex method on each observer
  for (auto&& CurrentObserver : m_Observers)
  {
    CurrentObserver->Body->onStepCompleted();
    CurrentObserver->Body->setPreviousTimeIndex(TimeIndex);
  }
}


// =====================================================================
// =====================================================================


void MonitoringInstance::call_onFinalizedRun() const
{
  // call of onFinalizedRun method on each observer
  for (auto&& CurrentObserver : m_Observers)
  {
    CurrentObserver->Body->onFinalizedRun();
  }
}


} }  // namespaces
