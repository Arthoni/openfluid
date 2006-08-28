/**
  \file SpatialRepository.h
  \brief header de la classe de gestion des donn�es spatiales

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/

#ifndef __SPATIALREPOSITORY_H__
#define __SPATIALREPOSITORY_H__


#include <vector>
#include <wx/hashmap.h>

#include "SurfaceUnit.h"
#include "ReachSegment.h"
#include "GroundwaterUnit.h"




namespace mhydasdk { namespace core {



WX_DECLARE_HASH_MAP(int, SurfaceUnit*, wxIntegerHash, wxIntegerEqual, SUMap);
WX_DECLARE_HASH_MAP(int, GroundwaterUnit*, wxIntegerHash, wxIntegerEqual, GUMap);
WX_DECLARE_HASH_MAP(int, ReachSegment*, wxIntegerHash, wxIntegerEqual, RSMap);


/**
  Classe de gestion de l'ensemble des donn�es spatiales

  \author Jean-Christophe FABRE <fabrejc@ensam.inra.fr>
*/
class SpatialRepository
{
	private:
		SUMap m_SUsCollection;
		RSMap m_RSsCollection;
		GUMap m_GUsCollection;

    vector<vector<SurfaceUnit*>*> m_SUsProcessOrders;
    vector<vector<ReachSegment*>*> m_RSsProcessOrders;
    vector<vector<GroundwaterUnit*>*> m_GUsProcessOrders;


	public:

		/**
		  Constructeur
		*/
		SpatialRepository();

		/**
		  Destructeur
		*/
		~SpatialRepository();

		/**
		  Ajoute une SHU � l'ensemble des SHUs
		  \param SHU l'unit� � ajouter
		*/
		bool addSU(SurfaceUnit *SU);

		/**
		  Retourne la SHU correspondant � l'ID pass� en param�tre
		  \param ID l'ID recherch�
		  \return la SHU recherch�e, NULL si non trouv�e
		*/
		SurfaceUnit* getSUByID(int ID);

		/**
		  Renvoie la liste des d�finitions de SHUs sous la forme d'un vecteur de SHUs
		  \return le vecteur de SHUs
		*/
		SUMap getSUsCollection();


		/**
		  Ajoute un bief � l'ensemble des bief constituant le r�seau hydro
		  \param ReachUnit le bief � ajouter
		*/
		bool addRS(ReachSegment* RS);


		/**
		  Retourne le bief correspondant � l'ID pass� en param�tre
		  \param ID l'ID recherch�
		  \return le bief recherch�, NULL si non trouv�
		*/
		ReachSegment* getRSByID(int ID);

		/**
		  Renvoie la liste des d�finitions de biefs sous la forme d'un vecteur de biefs
		  \return le vecteur de biefs
		*/
		RSMap getRSsCollection();


		/**
		  Ajoute une AHU bief � l'ensemble des AHUs
		  \param AHU l' AHU � ajouter
		*/
		bool addGU(GroundwaterUnit* GU);

		/**
		  Retourne l'AHU correspondante � l'ID pass� en param�tre
		  \param ID l'ID recherch�
		  \return l'AHU recherch�, NULL si non trouv�
		*/
		GroundwaterUnit* getGUByID(int ID);

		/**
		  Renvoie la liste des d�finitions de AHUs sous la forme d'un vecteur de AHUs
		  \return le vecteur de AHUs
		*/
		GUMap getGUsCollection();


    bool buildObjectLinkedTopologyFromIDs();


		/**
      Regroupe les objets hydrologiques d�finissant l'espace �tudi� par classe d'ordre de traitement.
      Cette m�thode cr�e 3 vecteurs (un pour chaque type d'objet).
      Chacun de ces vecteurs contient un ensemble de vecteurs index�s par le num�ro d'ordre de traitement [0,ordre-1].
      Ainsi l'ensemble des objets hydrologiques index�s [2] ont un ordre de traitement de 3.
		  \return true si la constitution par classe a bien fonctionn�
		*/
    bool buildProcessOrders();


};


} } // namespace mhydasdk::core



#endif

