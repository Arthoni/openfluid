/**
  @file %%ROOTCPPFILENAME%%
*/


/*
<sim2doc>

</sim2doc>
*/


#include <openfluid/ware/PluggableSimulator.hpp>


// =====================================================================
// =====================================================================


BEGIN_SIMULATOR_SIGNATURE("%%WAREID%%")

  DECLARE_NAME("")
  DECLARE_DESCRIPTION("")

  DECLARE_VERSION("")
  DECLARE_STATUS(openfluid::ware::EXPERIMENTAL)

  DECLARE_DOMAIN("")
  DECLARE_PROCESS("")
  DECLARE_METHOD("")
  DECLARE_AUTHOR("","")

END_SIMULATOR_SIGNATURE


// =====================================================================
// =====================================================================


/**

*/
class %%CLASSNAME%% : public openfluid::ware::PluggableSimulator
{
  private:

  
  public:

  
    %%CLASSNAME%%(): PluggableSimulator()
    {
  
  
    }
  
  
    // =====================================================================
    // =====================================================================
  
  
    ~%%CLASSNAME%%()
    {
  
  
    }
  
  
    // =====================================================================
    // =====================================================================
  
  
    void initParams(const openfluid::ware::WareParams_t& /*Params*/)
    {


    }


    // =====================================================================
    // =====================================================================
  
  
    void prepareData()
    {
  
  
    }
  
  
    // =====================================================================
    // =====================================================================
  
  
    void checkConsistency()
    {
  
  
    }
  
  
    // =====================================================================
    // =====================================================================
  
  
    openfluid::base::SchedulingRequest initializeRun()
    {  
    
      
      return DefaultDeltaT();
    }


    // =====================================================================
    // =====================================================================
  
  
    openfluid::base::SchedulingRequest runStep()
    {


      return DefaultDeltaT();
    }


    // =====================================================================
    // =====================================================================
  
  
    void finalizeRun()
    {
  
  
    }

};


// =====================================================================
// =====================================================================


DEFINE_SIMULATOR_CLASS(%%CLASSNAME%%);


// =====================================================================
// =====================================================================


#ifdef OPENFLUID_PARAMSUI_ENABLED

%%PARAMSUICOMMENT%%#include "%%PARAMSUIROOTHPPFILENAME%%"

%%PARAMSUICOMMENT%%DEFINE_PARAMETERIZATIONWIDGET_CLASS(%%PARAMSUICLASSNAME%%)

#endif

