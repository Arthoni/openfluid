/**
  \file HydroObject.h
  \brief

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/

#ifndef __HYDROOBJECT_H__
#define __HYDROOBJECT_H__

#include <string>
#include <vector>
#include <wx/hashmap.h>


namespace mhydasdk { namespace core {


typedef int hoid_t;

/**
  Hash table for calculated variables during simulation.
  Each variable is stored in the hash table as an entry, located by its name.
  (exemple "effrain" for efficient rain calculated by the production function)
  Each variable is stored as a vector of double (one vector item = one step, vector[25] is calculated at the 25th step)
*/
WX_DECLARE_STRING_HASH_MAP(std::vector<double>, SimulatedVarsMap);



/**
  Generic definition class for Hydrological Objetcs

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/
class HydroObject
{
	private:

	protected:
		hoid_t m_ID;
		int m_ProcessOrder;

    SimulatedVarsMap m_SimVars;

	public:
		/**
		  Constructeur par d�faut
		*/
		HydroObject();

		/**
		  Constructor
		  \param[in] Geometry The objects geometry
		  \param[in] ID The object identifier
		  \param[in] Comment The associated comment
		  \param[in] ProcessOrder the process order of the objects during the model execution
		*/
		HydroObject(hoid_t ID, int ProcessOrder);

		/**
		  Virtual destructor
		*/
		virtual ~HydroObject();

    int getID() const;

    int getProcessOrder() const;

    void setProcessOrder(const int ProcessOrder);


};



} } // namespace mhydasdk::core

#endif
