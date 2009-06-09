/**
  \file PrimitivesProdFunc.h
  \brief Header of ...
*/

#ifndef __MESSAGESFUNC_H__
#define __MESSAGESFUNC_H__

#include "openfluid-base.h"
#include "openfluid-core.h"


// =====================================================================
// =====================================================================


DECLARE_PLUGIN_HOOKS


// =====================================================================
// =====================================================================


/**

*/
class MessagesFunction : public openfluid::base::PluggableFunction
{
  private:

    int m_RepeatMessages;

  public:
    /**
      Constructor
    */
    MessagesFunction();

    /**
      Destructor
    */
    ~MessagesFunction();

    bool initParams(openfluid::core::FuncParamsMap_t Params);

    bool prepareData();

    bool checkConsistency();

    bool initializeRun(const openfluid::base::SimulationInfo* SimInfo);

    bool runStep(const openfluid::base::SimulationStatus* SimStatus);

    bool finalizeRun(const openfluid::base::SimulationInfo* SimInfo);

};


#endif  // __PRIMITIVESPRODFUNC_H__
