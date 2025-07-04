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
  @file ReportTasks.cpp

  @author Jean-Christophe FABRE <jean-christophe.fabre@inrae.fr>
  @author Dorian GERARDIN <dorian.gerardin@inrae.fr>
*/


#include <openfluid/machine/SimulatorRegistry.hpp>
#include <openfluid/machine/ObserverRegistry.hpp>
#include <openfluid/machine/WareRegistrySerializer.hpp>
#include <openfluid/base/Environment.hpp>

#include "ReportTasks.hpp"


int ReportTasks::process() const
{

    std::string Waretype;

    if (!m_Cmd.isOptionActive("ware-type"))
    {
      if (m_ExtraArgs.empty() || (m_ExtraArgs.at(0) != "simulators" && m_ExtraArgs.at(0) != "observers"))
      {
        return error("type of wares is missing for reporting");
      }
      else
      {
        Waretype = m_ExtraArgs.at(0);
      }
    }
    else if(m_Cmd.getOptionValue("ware-type") != "simulators" && m_Cmd.getOptionValue("ware-type") != "observers")
    {
      return error("type of wares is not valid for reporting");
    }
    else 
    {
      Waretype = m_Cmd.getOptionValue("ware-type");
    }

    bool Detailed = !m_Cmd.isOptionActive("list");
    bool WithErrors = m_Cmd.isOptionActive("with-errors");
    std::string Format = "text";

    if (m_Cmd.isOptionActive("format"))
    {
      Format = m_Cmd.getOptionValue("format");
    }

    if (Format != "text" && Format != "json")
    {
      return error("unknown format");
    }

    if (Waretype == "simulators")
    {
      if (m_Cmd.isOptionActive("simulators-paths"))
      {
        openfluid::base::Environment::addExtraSimulatorsDirs(m_Cmd.getOptionValue("simulators-paths"));
      }

      auto Reg = openfluid::machine::SimulatorRegistry::instance();
      Reg->clear();
      Reg->discoverWares(false);

      const openfluid::machine::WareRegistrySerializer<openfluid::ware::SimulatorSignature> RegSzr(Reg);
      RegSzr.writeToStream(std::cout,Format,Detailed,WithErrors);
      std::cout.flush();

      return 0;
    }
    else if (Waretype == "observers")
    {
      if (m_Cmd.isOptionActive("observers-paths"))
      {
        openfluid::base::Environment::addExtraObserversDirs(m_Cmd.getOptionValue("observers-paths"));
      }

      auto Reg = openfluid::machine::ObserverRegistry::instance();
      Reg->clearWares();
      Reg->discoverWares();

      const openfluid::machine::WareRegistrySerializer<openfluid::ware::ObserverSignature> RegSzr(Reg);
      RegSzr.writeToStream(std::cout,Format,Detailed,WithErrors);
      std::cout.flush();

      return 0;
    }

    return error();
}


// =====================================================================
// =====================================================================


int StatusTasks::process() const
{

    std::string Format = "text";

    if (m_Cmd.isOptionActive("format"))
    {
      Format = m_Cmd.getOptionValue("format");
    }

    // args: 
    // - scope: *, project, ware
    // - element-type: *, simulators/observers/builderexts/fragments
    // - source: *, workspace, ware
    // - format: json/txt

    // objectif : donner des informations sur le status des dépôts de wares disponibles, filtrable d'après un IN pour ne garder que les wares qui nous intéressent
    // Renvoit des informations sur le versionnement, l'état des submodules utilisés
    // cf rapport construit par David (ci-après). Différence principale : méthode de David fournit un instantané de l'état au moment de la compilation du ware alors que status indique l'état actuel
    
    

    return 0;

    //return error();
}


/*

[Info][source=framework] Date: Wed Jun  4 20:44:20 2025

[Info][source=framework] Computer: lisah-dsoul
[Info][source=framework] User: crevoisi
[Info][source=framework] Input directory: /home/data/simulations/OpenFLUID/RIPPviti/projects/cible-sol/RIPP_CibleSol/BUGS/virtual-bv-raster-2x2/IN
[Info][source=framework] Output directory: /home/data/simulations/OpenFLUID/RIPPviti/projects/cible-sol/RIPP_CibleSol/BUGS/virtual-bv-raster-2x2/OUT
[Info][source=ware,stage=INITPARAMS,wareid=water-pop.atm-surf.state-change-scheduler.mipp,waretype=simulator] 
 - hostname : lisah-dsoul
 - user     : crevoisi
 - arch     : x86_64
 - OS       : Ubuntu 20.04.6 LTS
 - compilation date  : lun. 24 févr. 2025 23:35:08 CET
 - OpenFLUID version : 2.1.11
 - Current path      : /home/crevoisi/Developpements/OpenFLUID/MIPP/wares-dev/simulators/water-pop.atm-surf.state-change-scheduler.mipp/_build-release-2.1
 - git branch   : openfluid-2.1
 - git tag      : no tag
 - git last log : 37f73b6 Integrate informations in ware binary
 - git status   : Sur la branche openfluid-2.1
Votre branche est a jour avec 'origin/openfluid-2.1'.

rien a valider, la copie de travail est propre
 - git diff     : 
  ==========

[Info][source=ware,stage=INITPARAMS,wareid=meteo.atm-surf.vars-generator,waretype=simulator] 
 - hostname : lisah-dsoul
 - user     : crevoisi
 - arch     : x86_64
 - OS       : Ubuntu 20.04.6 LTS
 - compilation date  : lun. 24 févr. 2025 23:35:14 CET
 - OpenFLUID version : 2.1.11
 - Current path      : /home/crevoisi/Developpements/OpenFLUID/MIPP/wares-dev/simulators/meteo.atm-surf.vars-generator/_build-release-2.1
 - git branch   : openfluid-2.1
 - git tag      : no tag
 - git last log : 758cb2b Integrate informations in ware binary
 - git status   : Sur la branche openfluid-2.1
Votre branche est a jour avec 'origin/openfluid-2.1'.

rien a valider, la copie de travail est propre
 - git diff     : 
  ==========

[Info][source=ware,stage=INITPARAMS,wareid=meteo.atm-surf.vars-generator,waretype=simulator] Meteorological variable ET0 is computed from other variables.
[Info][source=ware,stage=INITPARAMS,wareid=meteo.atm-surf.vars-generator,waretype=simulator] Meteorological variable Pvap is computed from other variables.
[Info][source=ware,stage=INITPARAMS,wareid=pop.plant-surf.application.mipp,waretype=simulator] 
 - hostname : lisah-dsoul
 - user     : crevoisi
 - arch     : x86_64
 - OS       : Ubuntu 20.04.6 LTS
 - compilation date  : lun. 24 févr. 2025 23:35:27 CET
 - OpenFLUID version : 2.1.11
 - Current path      : /home/crevoisi/Developpements/OpenFLUID/MIPP/wares-dev/simulators/pop.plant-surf.application.mipp/_build-release-2.1
 - git branch   : openfluid-2.1
 - git tag      : no tag
 - git last log : 40b31f6 Integrate informations in ware binary
 - git status   : Sur la branche openfluid-2.1
Votre branche est a jour avec 'origin/openfluid-2.1'.

rien a valider, la copie de travail est propre
 - git diff     : 
  ==========

[Info][source=ware,stage=INITPARAMS,wareid=plant.surf.vine-growth,waretype=simulator] 
 - hostname : lisah-dsoul
 - user     : crevoisi
 - arch     : x86_64
 - OS       : Ubuntu 20.04.6 LTS
 - compilation date  : lun. 24 févr. 2025 23:35:22 CET
 - OpenFLUID version : 2.1.11
 - Current path      : /home/crevoisi/Developpements/OpenFLUID/MIPP/wares-dev/simulators/plant.surf.vine-growth/_build-release-2.1
 - git branch   : openfluid-2.1
 - git tag      : no tag
 - git last log : 1ce38bb Integrate informations in ware binary
 - git status   : Sur la branche openfluid-2.1
Votre branche est a jour avec 'origin/openfluid-2.1'.

rien a valider, la copie de travail est propre
 - git diff     : 
  ==========

[Info][source=ware,stage=INITPARAMS,wareid=energy.surf.rainfall-kinetic.zanchi-torri,waretype=simulator] 
 - hostname : lisah-dsoul
 - user     : crevoisi
 - arch     : x86_64
 - OS       : Ubuntu 20.04.6 LTS
 - compilation date  : lun. 24 févr. 2025 23:35:32 CET
 - OpenFLUID version : 2.1.11
 - Current path      : /home/crevoisi/Developpements/OpenFLUID/MHYDAS/wares-dev/simulators/energy.surf.rainfall-kinetic.zanchi-torri/_build-release-2.1
 - git branch   : openfluid-2.1
 - git tag      : no tag
 - git last log : 5c5201e Integrate informations in ware binary
 - git status   : Sur la branche openfluid-2.1
Votre branche est a jour avec 'origin/openfluid-2.1'.

rien a valider, la copie de travail est propre
[Info][source=ware,stage=INITPARAMS,wareid=energy.surf.vine-grassing-soil-balance.montes,waretype=simulator] 
 - hostname : lisah-dsoul
 - user     : crevoisi
 - arch     : x86_64
 - OS       : Ubuntu 20.04.6 LTS
 - compilation date  : lun. 24 févr. 2025 23:35:35 CET
 - OpenFLUID version : 2.1.11
 - Current path      : /home/crevoisi/Developpements/OpenFLUID/MIPP/wares-dev/simulators/energy.surf.vine-grassing-soil-balance.montes/_build-release-2.1
 - git branch   : openfluid-2.1
 - git tag      : no tag
 - git last log : 193d479 Miscellaneous improvements
 - git status   : Sur la branche openfluid-2.1
Votre branche est a jour avec 'origin/openfluid-2.1'.

rien a valider, la copie de travail est propre
 - git diff     : 
  ==========

[Info][source=ware,stage=INITPARAMS,wareid=water-pop-energy.atm-surf-uz.pesticide-fate.vsoil-interface,waretype=simulator] 
 - hostname : lisah-dsoul
 - user     : crevoisi
 - arch     : x86_64
 - OS       : Ubuntu 20.04.6 LTS
 - compilation date  : mar. 25 févr. 2025 12:00:36 CET
 - OpenFLUID version : 2.1.11
 - Current path      : /home/crevoisi/Developpements/OpenFLUID/MIPP/wares-dev/simulators/water-pop-energy.atm-surf-uz.pesticide-fate.vsoil-interface/_build-release-2.1
 - git branch   : openfluid-2.1
 - git tag      : no tag
 - git last log : 19f096f Refactor log info about replacing patern in vsoil xml input file
 - git status   : Sur la branche openfluid-2.1
Votre branche est a jour avec 'origin/openfluid-2.1'.

rien a valider, la copie de travail est propre
 - git diff     : 
  ==========

[Info][source=ware,stage=INITPARAMS,wareid=plant.surf.vegetal-growth,waretype=simulator] 
 - hostname : lisah-dsoul
 - user     : crevoisi
 - arch     : x86_64
 - OS       : Ubuntu 20.04.6 LTS
 - compilation date  : mar. 25 févr. 2025 14:36:29 CET
 - OpenFLUID version : 2.1.11
 - Current path      : /home/crevoisi/Developpements/OpenFLUID/MIPP/wares-dev/simulators/modelGrowthVeg/OF_Simulator/_build-release-2.1
 - git branch   : develop
 - git tag      : no tag
 - git last log : 74510e2 [OFsim] Update add-binary-info tools to specificities of the ware
 - git status   : Sur la branche develop
Votre branche est a jour avec 'origin/develop'.

rien a valider, la copie de travail est propre
 - git diff     : 
  ==========

[Info][source=ware,stage=INITPARAMS,wareid=soil.surf.SSC-evolution.logistic-regression-mipp,waretype=simulator] 
 - hostname : lisah-dsoul
 - user     : crevoisi
 - arch     : x86_64
 - OS       : Ubuntu 20.04.6 LTS
 - compilation date  : lun. 24 févr. 2025 23:36:12 CET
 - OpenFLUID version : 2.1.11
 - Current path      : /home/crevoisi/Developpements/OpenFLUID/MIPP/wares-dev/simulators/soil.surf.SSC-evolution.logistic-regression-mipp/_build-release-2.1
 - git branch   : openfluid-2.1
 - git tag      : no tag
 - git last log : 77f4e57 Remove display at screen output
 - git status   : Sur la branche openfluid-2.1
Votre branche est a jour avec 'origin/openfluid-2.1'.

rien a valider, la copie de travail est propre
 - git diff     : 
  ==========

[Info][source=ware,stage=INITPARAMS,wareid=soil.surf.SSC-evolution.logistic-regression-mipp,waretype=simulator] WITH WEED CONDUCTANCE
[Info][source=ware,stage=INITPARAMS,wareid=view.progress.bar,waretype=observer] 
 - hostname : lisah-dsoul
 - user     : crevoisi
 - arch     : x86_64
 - OS       : Ubuntu 20.04.6 LTS
 - compilation date  : lun. 24 févr. 2025 23:36:26 CET
 - OpenFLUID version : 2.1.11
 - Current path      : /home/crevoisi/Developpements/OpenFLUID/common/wares-dev/observers/view.progress.bar/_build-release-2.1
 - git branch   : openfluid-2.1
 - git tag      : no tag
 - git last log : 1d787c6 Integrate informations in ware binary
 - git status   : Sur la branche openfluid-2.1
Votre branche est a jour avec 'origin/openfluid-2.1'.

rien a valider, la copie de travail est propre
[Info][source=ware,stage=INITPARAMS,wareid=export.vars.mipp-massbalance.csv,waretype=observer] 
 - hostname : lisah-dsoul
 - user     : crevoisi
 - arch     : x86_64
 - OS       : Ubuntu 20.04.6 LTS
 - compilation date  : lun. 24 févr. 2025 23:36:29 CET
 - OpenFLUID version : 2.1.11
 - Current path      : /home/crevoisi/Developpements/OpenFLUID/MIPP/wares-dev/observers/export.vars.mipp-massbalance.csv/_build-release-2.1
 - git branch   : openfluid-2.1
 - git tag      : no tag
 - git last log : 212a06c Integrate informations in ware binary
 - git status   : Sur la branche openfluid-2.1
Votre branche est a jour avec 'origin/openfluid-2.1'.

rien a valider, la copie de travail est propre
[Info][source=ware,stage=INITPARAMS,wareid=export.vars.mipp-massbalance.csv,waretype=observer] Unit to write : SPU101 SU1 
[Info][source=ware,stage=INITPARAMS,wareid=export.vars.dygraphs.html,waretype=observer] 
 - hostname : lisah-dsoul
 - user     : crevoisi
 - arch     : x86_64
 - OS       : Ubuntu 20.04.6 LTS
 - compilation date  : lun. 24 févr. 2025 23:36:18 CET
 - OpenFLUID version : 2.1.11
 - Current path      : /home/crevoisi/Developpements/OpenFLUID/common/wares-dev/observers/export.vars.dygraphs.html/_build-release-2.1
 - git branch   : openfluid-2.1
 - git tag      : no tag
 - git last log : fcc2184 Integrate informations in ware binary
 - git status   : Sur la branche openfluid-2.1
Votre branche est a jour avec 'origin/openfluid-2.1'.

rien a valider, la copie de travail est propre
[Warning][source=ware,stage=INITPARAMS,wareid=export.vars.dygraphs.html,waretype=observer] 
[Warning][source=ware,stage=INITPARAMS,wareid=export.vars.dygraphs.html,waretype=observer] 
[Warning][source=ware,stage=INITPARAMS,wareid=export.vars.dygraphs.html,waretype=observer] 
[Warning][source=ware,stage=INITPARAMS,wareid=export.vars.dygraphs.html,waretype=observer] 
[Warning][source=ware,stage=INITPARAMS,wareid=export.vars.dygraphs.html,waretype=observer] 
[Warning][source=ware,stage=INITPARAMS,wareid=export.vars.dygraphs.html,waretype=observer] 
[Warning][source=ware,stage=INITPARAMS,wareid=export.vars.dygraphs.html,waretype=observer] 
[Warning][source=ware,stage=INITPARAMS,wareid=export.vars.dygraphs.html,waretype=observer] 
[Warning][source=ware,stage=INITPARAMS,wareid=export.vars.dygraphs.html,waretype=observer] 
[Warning][source=ware,stage=INITPARAMS,wareid=export.vars.dygraphs.html,waretype=observer] 
[Warning][source=ware,stage=INITPARAMS,wareid=export.vars.dygraphs.html,waretype=observer] 
[Warning][source=ware,stage=INITPARAMS,wareid=export.vars.dygraphs.html,waretype=observer] 
[Warning][source=ware,stage=INITPARAMS,wareid=export.vars.dygraphs.html,waretype=observer] 
[Warning][source=ware,stage=INITPARAMS,wareid=export.vars.dygraphs.html,waretype=observer] 
[Warning][source=ware,stage=INITPARAMS,wareid=export.vars.dygraphs.html,waretype=observer] 
[Warning][source=ware,stage=INITPARAMS,wareid=export.vars.dygraphs.html,waretype=observer] 
[Warning][source=ware,stage=INITPARAMS,wareid=export.vars.dygraphs.html,waretype=observer] 
[Warning][source=ware,stage=INITPARAMS,wareid=export.vars.dygraphs.html,waretype=observer] 
[Warning][source=ware,stage=INITPARAMS,wareid=export.vars.dygraphs.html,waretype=observer] 
[Warning][source=ware,stage=INITPARAMS,wareid=export.vars.dygraphs.html,waretype=observer] 
[Warning][source=ware,stage=INITPARAMS,wareid=export.vars.dygraphs.html,waretype=observer] 
[Warning][source=ware,stage=INITPARAMS,wareid=export.vars.dygraphs.html,waretype=observer] 
[Info][source=ware,stage=PREPAREDATA,wareid=water-pop.atm-surf.state-change-scheduler.mipp,waretype=simulator] Number of rain events after the reading of the files                   : 11
[Info][source=ware,stage=PREPAREDATA,wareid=water-pop.atm-surf.state-change-scheduler.mipp,waretype=simulator] Cumulated rain on the whole watershed after the reading of the files   : 0.0757104
[Info][source=ware,stage=PREPAREDATA,wareid=water-pop.atm-surf.state-change-scheduler.mipp,waretype=simulator] Number of rain events  after the fusion of close events                : 11
[Info][source=ware,stage=PREPAREDATA,wareid=water-pop.atm-surf.state-change-scheduler.mipp,waretype=simulator] Cumulated rain on the whole watershed after the fusion of close events : 0.0757104
[Info][source=ware,stage=PREPAREDATA,wareid=water-pop.atm-surf.state-change-scheduler.mipp,waretype=simulator] Cumulated rain on the whole watershed added in the events              : 0.0757104
[Info][source=ware,stage=PREPAREDATA,wareid=water-pop-energy.atm-surf-uz.pesticide-fate.vsoil-interface,waretype=simulator] 
Replace pattern "%%PEST_NAME%%" by its value in /home/data/simulations/OpenFLUID/RIPPviti/projects/cible-sol/RIPP_CibleSol/BUGS/virtual-bv-raster-2x2/IN/data/vsoil-xml/SoilUnit_M3_3m_RIPP.tpl.xml
Replace pattern "%%MOLAR_MASS%%" by its value in /home/data/simulations/OpenFLUID/RIPPviti/projects/cible-sol/RIPP_CibleSol/BUGS/virtual-bv-raster-2x2/IN/data/vsoil-xml/SoilUnit_M3_3m_RIPP.tpl.xml
Replace pattern "%%PSAT_REF%%" by its value in /home/data/simulations/OpenFLUID/RIPPviti/projects/cible-sol/RIPP_CibleSol/BUGS/virtual-bv-raster-2x2/IN/data/vsoil-xml/SoilUnit_M3_3m_RIPP.tpl.xml
Replace pattern "%%ENTHALPY_VAP%%" by its value in /home/data/simulations/OpenFLUID/RIPPviti/projects/cible-sol/RIPP_CibleSol/BUGS/virtual-bv-raster-2x2/IN/data/vsoil-xml/SoilUnit_M3_3m_RIPP.tpl.xml
Replace pattern "%%ENTHALPY_DISSOL%%" by its value in /home/data/simulations/OpenFLUID/RIPPviti/projects/cible-sol/RIPP_CibleSol/BUGS/virtual-bv-raster-2x2/IN/data/vsoil-xml/SoilUnit_M3_3m_RIPP.tpl.xml
Replace pattern "%%MU_DEGRAD%%" by its value in /home/data/simulations/OpenFLUID/RIPPviti/projects/cible-sol/RIPP_CibleSol/BUGS/virtual-bv-raster-2x2/IN/data/vsoil-xml/SoilUnit_M3_3m_RIPP.tpl.xml
Replace pattern "%%KOC%%" by its value in /home/data/simulations/OpenFLUID/RIPPviti/projects/cible-sol/RIPP_CibleSol/BUGS/virtual-bv-raster-2x2/IN/data/vsoil-xml/SoilUnit_M3_3m_RIPP.tpl.xml
Replace pattern "%%SOLUBILITY%%" by its value in /home/data/simulations/OpenFLUID/RIPPviti/projects/cible-sol/RIPP_CibleSol/BUGS/virtual-bv-raster-2x2/IN/data/vsoil-xml/SoilUnit_M3_3m_RIPP.tpl.xml
Replace pattern "%%LOGKOW%%" by its value in /home/data/simulations/OpenFLUID/RIPPviti/projects/cible-sol/RIPP_CibleSol/BUGS/virtual-bv-raster-2x2/IN/data/vsoil-xml/SoilUnit_M3_3m_RIPP.tpl.xml
Replace pattern "%%DIFFPLANT%%" by its value in /home/data/simulations/OpenFLUID/RIPPviti/projects/cible-sol/RIPP_CibleSol/BUGS/virtual-bv-raster-2x2/IN/data/vsoil-xml/SoilUnit_M3_3m_RIPP.tpl.xml
Replace pattern "%%DIFFW%%" by its value in /home/data/simulations/OpenFLUID/RIPPviti/projects/cible-sol/RIPP_CibleSol/BUGS/virtual-bv-raster-2x2/IN/data/vsoil-xml/SoilUnit_M3_3m_RIPP.tpl.xml
Replace pattern "%%DIFFG%%" by its value in /home/data/simulations/OpenFLUID/RIPPviti/projects/cible-sol/RIPP_CibleSol/BUGS/virtual-bv-raster-2x2/IN/data/vsoil-xml/SoilUnit_M3_3m_RIPP.tpl.xml
Replace pattern "%%KHENRY%%" by its value in /home/data/simulations/OpenFLUID/RIPPviti/projects/cible-sol/RIPP_CibleSol/BUGS/virtual-bv-raster-2x2/IN/data/vsoil-xml/SoilUnit_M3_3m_RIPP.tpl.xml
Replace pattern "%%LAMBDA_CANOPY%%" by its value in /home/data/simulations/OpenFLUID/RIPPviti/projects/cible-sol/RIPP_CibleSol/BUGS/virtual-bv-raster-2x2/IN/data/vsoil-xml/SoilUnit_M3_3m_RIPP.tpl.xml
Replace pattern "%%KWASHOFF%%" by its value in /home/data/simulations/OpenFLUID/RIPPviti/projects/cible-sol/RIPP_CibleSol/BUGS/virtual-bv-raster-2x2/IN/data/vsoil-xml/SoilUnit_M3_3m_RIPP.tpl.xml
Replace pattern "%%KDEGREF%%" by its value in /home/data/simulations/OpenFLUID/RIPPviti/projects/cible-sol/RIPP_CibleSol/BUGS/virtual-bv-raster-2x2/IN/data/vsoil-xml/SoilUnit_M3_3m_RIPP.tpl.xml
Replace pattern "%%KDISSIPTISSUE%%" by its value in /home/data/simulations/OpenFLUID/RIPPviti/projects/cible-sol/RIPP_CibleSol/BUGS/virtual-bv-raster-2x2/IN/data/vsoil-xml/SoilUnit_M3_3m_RIPP.tpl.xml
Replace pattern "%%MAX_DEPTH%%" by its value in /home/data/simulations/OpenFLUID/RIPPviti/projects/cible-sol/RIPP_CibleSol/BUGS/virtual-bv-raster-2x2/OUT/vsoil_init_data_from_template.xml
Replace pattern "%%HORIZ_DEPTHS%%" by its value in /home/data/simulations/OpenFLUID/RIPPviti/projects/cible-sol/RIPP_CibleSol/BUGS/virtual-bv-raster-2x2/OUT/vsoil_init_data_from_template.xml
Replace pattern "%%NODE_DEPTHS%%" by its value in /home/data/simulations/OpenFLUID/RIPPviti/projects/cible-sol/RIPP_CibleSol/BUGS/virtual-bv-raster-2x2/OUT/vsoil_init_data_from_template.xml
[Info][source=ware,stage=PREPAREDATA,wareid=water-pop-energy.atm-surf-uz.pesticide-fate.vsoil-interface,waretype=simulator] I/O info :
 - mode       : normal
 - filename   : _<ID>.bin
 - list of ID : []
[Warning][source=ware,stage=INITIALIZERUN,timeindex=0,wareid=plant.surf.vine-growth,waretype=simulator] on SU4, initial cumulated temperature (basis 10) set to null because budburst not reached.
[Warning][source=ware,stage=INITIALIZERUN,timeindex=0,wareid=plant.surf.vine-growth,waretype=simulator] on SU3, initial cumulated temperature (basis 10) set to null because budburst not reached.
[Warning][source=ware,stage=INITIALIZERUN,timeindex=0,wareid=plant.surf.vine-growth,waretype=simulator] on SU2, initial cumulated temperature (basis 10) set to null because budburst not reached.
[Warning][source=ware,stage=INITIALIZERUN,timeindex=0,wareid=plant.surf.vine-growth,waretype=simulator] on SU1, initial cumulated temperature (basis 10) set to null because budburst not reached.
[Info][source=ware,stage=INITIALIZERUN,timeindex=0,wareid=export.vars.mipp-massbalance.csv,waretype=observer] Write soil water balance in file "/home/data/simulations/OpenFLUID/RIPPviti/projects/cible-sol/RIPP_CibleSol/BUGS/virtual-bv-raster-2x2/OUT/mass-balance-SPU101-soil-water-cum-final.csv".
[Info][source=ware,stage=INITIALIZERUN,timeindex=0,wareid=export.vars.mipp-massbalance.csv,waretype=observer] Write soil plant pest balance in file "/home/data/simulations/OpenFLUID/RIPPviti/projects/cible-sol/RIPP_CibleSol/BUGS/virtual-bv-raster-2x2/OUT/mass-balance-SPU101-soil-plant-pest-cum-final.csv".
[Info][source=ware,stage=INITIALIZERUN,timeindex=0,wareid=export.vars.mipp-massbalance.csv,waretype=observer] Write soil water balance in file "/home/data/simulations/OpenFLUID/RIPPviti/projects/cible-sol/RIPP_CibleSol/BUGS/virtual-bv-raster-2x2/OUT/mass-balance-SPU101-soil-water-cum-day.csv".
[Info][source=ware,stage=INITIALIZERUN,timeindex=0,wareid=export.vars.mipp-massbalance.csv,waretype=observer] Write soil plant pest balance in file "/home/data/simulations/OpenFLUID/RIPPviti/projects/cible-sol/RIPP_CibleSol/BUGS/virtual-bv-raster-2x2/OUT/mass-balance-SPU101-soil-plant-pest-cum-day.csv".
[Info][source=ware,stage=INITIALIZERUN,timeindex=0,wareid=export.vars.mipp-massbalance.csv,waretype=observer] Write soil water balance in file "/home/data/simulations/OpenFLUID/RIPPviti/projects/cible-sol/RIPP_CibleSol/BUGS/virtual-bv-raster-2x2/OUT/mass-balance-SPU101-soil-water-day.csv".
[Info][source=ware,stage=INITIALIZERUN,timeindex=0,wareid=export.vars.mipp-massbalance.csv,waretype=observer] Write soil/plant pest balance in file "/home/data/simulations/OpenFLUID/RIPPviti/projects/cible-sol/RIPP_CibleSol/BUGS/virtual-bv-raster-2x2/OUT/mass-balance-SPU101-soil-plant-pest-day.csv".
[Info][source=ware,stage=INITIALIZERUN,timeindex=0,wareid=export.vars.mipp-massbalance.csv,waretype=observer] Write soil water balance in file "/home/data/simulations/OpenFLUID/RIPPviti/projects/cible-sol/RIPP_CibleSol/BUGS/virtual-bv-raster-2x2/OUT/mass-balance-SPU101-soil-water-cum-hour.csv".
[Info][source=ware,stage=INITIALIZERUN,timeindex=0,wareid=export.vars.mipp-massbalance.csv,waretype=observer] Write soil plant pest balance in file "/home/data/simulations/OpenFLUID/RIPPviti/projects/cible-sol/RIPP_CibleSol/BUGS/virtual-bv-raster-2x2/OUT/mass-balance-SPU101-soil-plant-pest-cum-hour.csv".
[Info][source=ware,stage=INITIALIZERUN,timeindex=0,wareid=export.vars.mipp-massbalance.csv,waretype=observer] Write soil water balance in file "/home/data/simulations/OpenFLUID/RIPPviti/projects/cible-sol/RIPP_CibleSol/BUGS/virtual-bv-raster-2x2/OUT/mass-balance-SPU101-soil-water-hour.csv".
[Info][source=ware,stage=INITIALIZERUN,timeindex=0,wareid=export.vars.mipp-massbalance.csv,waretype=observer] Write soil/plant pest balance in file "/home/data/simulations/OpenFLUID/RIPPviti/projects/cible-sol/RIPP_CibleSol/BUGS/virtual-bv-raster-2x2/OUT/mass-balance-SPU101-soil-plant-pest-hour.csv".
[Info][source=ware,stage=INITIALIZERUN,timeindex=0,wareid=export.vars.mipp-massbalance.csv,waretype=observer] Write soil water balance in file "/home/data/simulations/OpenFLUID/RIPPviti/projects/cible-sol/RIPP_CibleSol/BUGS/virtual-bv-raster-2x2/OUT/mass-balance-SU1-soil-water-cum-final.csv".
[Info][source=ware,stage=INITIALIZERUN,timeindex=0,wareid=export.vars.mipp-massbalance.csv,waretype=observer] Write soil plant pest balance in file "/home/data/simulations/OpenFLUID/RIPPviti/projects/cible-sol/RIPP_CibleSol/BUGS/virtual-bv-raster-2x2/OUT/mass-balance-SU1-soil-plant-pest-cum-final.csv".
[Info][source=ware,stage=INITIALIZERUN,timeindex=0,wareid=export.vars.mipp-massbalance.csv,waretype=observer] Write soil water balance in file "/home/data/simulations/OpenFLUID/RIPPviti/projects/cible-sol/RIPP_CibleSol/BUGS/virtual-bv-raster-2x2/OUT/mass-balance-SU1-soil-water-cum-day.csv".
[Info][source=ware,stage=INITIALIZERUN,timeindex=0,wareid=export.vars.mipp-massbalance.csv,waretype=observer] Write soil plant pest balance in file "/home/data/simulations/OpenFLUID/RIPPviti/projects/cible-sol/RIPP_CibleSol/BUGS/virtual-bv-raster-2x2/OUT/mass-balance-SU1-soil-plant-pest-cum-day.csv".
[Info][source=ware,stage=INITIALIZERUN,timeindex=0,wareid=export.vars.mipp-massbalance.csv,waretype=observer] Write soil water balance in file "/home/data/simulations/OpenFLUID/RIPPviti/projects/cible-sol/RIPP_CibleSol/BUGS/virtual-bv-raster-2x2/OUT/mass-balance-SU1-soil-water-day.csv".
[Info][source=ware,stage=INITIALIZERUN,timeindex=0,wareid=export.vars.mipp-massbalance.csv,waretype=observer] Write soil/plant pest balance in file "/home/data/simulations/OpenFLUID/RIPPviti/projects/cible-sol/RIPP_CibleSol/BUGS/virtual-bv-raster-2x2/OUT/mass-balance-SU1-soil-plant-pest-day.csv".
[Info][source=ware,stage=INITIALIZERUN,timeindex=0,wareid=export.vars.mipp-massbalance.csv,waretype=observer] Write soil water balance in file "/home/data/simulations/OpenFLUID/RIPPviti/projects/cible-sol/RIPP_CibleSol/BUGS/virtual-bv-raster-2x2/OUT/mass-balance-SU1-soil-water-cum-hour.csv".
[Info][source=ware,stage=INITIALIZERUN,timeindex=0,wareid=export.vars.mipp-massbalance.csv,waretype=observer] Write soil plant pest balance in file "/home/data/simulations/OpenFLUID/RIPPviti/projects/cible-sol/RIPP_CibleSol/BUGS/virtual-bv-raster-2x2/OUT/mass-balance-SU1-soil-plant-pest-cum-hour.csv".
[Info][source=ware,stage=INITIALIZERUN,timeindex=0,wareid=export.vars.mipp-massbalance.csv,waretype=observer] Write soil water balance in file "/home/data/simulations/OpenFLUID/RIPPviti/projects/cible-sol/RIPP_CibleSol/BUGS/virtual-bv-raster-2x2/OUT/mass-balance-SU1-soil-water-hour.csv".
[Info][source=ware,stage=INITIALIZERUN,timeindex=0,wareid=export.vars.mipp-massbalance.csv,waretype=observer] Write soil/plant pest balance in file "/home/data/simulations/OpenFLUID/RIPPviti/projects/cible-sol/RIPP_CibleSol/BUGS/virtual-bv-raster-2x2/OUT/mass-balance-SU1-soil-plant-pest-hour.csv".
[Info][source=ware,stage=RUNSTEP,timeindex=21600,wareid=pop.surf-atm.dispersion-deposition.fides,waretype=simulator] 
Simulation parameters :
 - Vdir    = -9.0000e+01
 - Zo      = 9.4000e-02
 - D       = 4.0000e-01
 - L       = -4.2473e+02
 - U*      = 7.0589e-01
 - distMin = 1.0000e+00
 - seuil   = 1.0000e-06
 - a       = 4.9790e+00
 - p       = 2.1144e-01
 - b       = 3.8482e-01
 - n       = 9.7251e-01
 - alpha   = 1.2389e+00
 - nu      = 2.2190e-02

[Info][source=ware,stage=RUNSTEP,timeindex=21600,wareid=pop.surf-atm.dispersion-deposition.fides,waretype=simulator] 
- nb of calcInfluence evaluations 9995 / 25600 for dist higher than distMin 1 m
- concRes size is 4 x 4 only 6 / 16 elements higher than threshold 1.000000e-06 (nb non nul element is 6)

[Info][source=ware,stage=RUNSTEP,timeindex=23400,wareid=pop.surf-atm.dispersion-deposition.fides,waretype=simulator] 
Simulation parameters :
 - Vdir    = -9.0000e+01
 - Zo      = 9.4000e-02
 - D       = 4.0000e-01
 - L       = -4.2460e+02
 - U*      = 7.0589e-01
 - distMin = 1.0000e+00
 - seuil   = 1.0000e-06
 - a       = 4.9790e+00
 - p       = 2.1143e-01
 - b       = 3.8482e-01
 - n       = 9.7253e-01
 - alpha   = 1.2389e+00
 - nu      = 2.2173e-02

[Info][source=ware,stage=RUNSTEP,timeindex=23400,wareid=pop.surf-atm.dispersion-deposition.fides,waretype=simulator] 
- nb of calcInfluence evaluations 9995 / 25600 for dist higher than distMin 1 m
- concRes size is 4 x 4 only 6 / 16 elements higher than threshold 1.000000e-06 (nb non nul element is 6)

[Info][source=ware,stage=RUNSTEP,timeindex=25200,wareid=pop.surf-atm.dispersion-deposition.fides,waretype=simulator] 
Simulation parameters :
 - Vdir    = -9.0000e+01
 - Zo      = 9.4000e-02
 - D       = 4.0000e-01
 - L       = -4.1052e+02
 - U*      = 7.0140e-01
 - distMin = 1.0000e+00
 - seuil   = 1.0000e-06
 - a       = 4.9470e+00
 - p       = 2.1119e-01
 - b       = 3.8233e-01
 - n       = 9.7481e-01
 - alpha   = 1.2364e+00
 - nu      = 2.0374e-02

[Info][source=ware,stage=RUNSTEP,timeindex=25200,wareid=pop.surf-atm.dispersion-deposition.fides,waretype=simulator] 
- nb of calcInfluence evaluations 9995 / 25600 for dist higher than distMin 1 m
- concRes size is 4 x 4 only 6 / 16 elements higher than threshold 1.000000e-06 (nb non nul element is 6)

[Info][source=ware,stage=RUNSTEP,timeindex=27000,wareid=pop.surf-atm.dispersion-deposition.fides,waretype=simulator] 
Simulation parameters :
 - Vdir    = -9.0000e+01
 - Zo      = 9.4000e-02
 - D       = 4.0000e-01
 - L       = -4.1052e+02
 - U*      = 7.0140e-01
 - distMin = 1.0000e+00
 - seuil   = 1.0000e-06
 - a       = 4.9470e+00
 - p       = 2.1119e-01
 - b       = 3.8233e-01
 - n       = 9.7481e-01
 - alpha   = 1.2364e+00
 - nu      = 2.0374e-02

[Info][source=ware,stage=RUNSTEP,timeindex=27000,wareid=pop.surf-atm.dispersion-deposition.fides,waretype=simulator] 
- nb of calcInfluence evaluations 9995 / 25600 for dist higher than distMin 1 m
- concRes size is 4 x 4 only 6 / 16 elements higher than threshold 1.000000e-06 (nb non nul element is 6)

[Info][source=ware,stage=RUNSTEP,timeindex=28800,wareid=pop.surf-atm.dispersion-deposition.fides,waretype=simulator] 
Simulation parameters :
 - Vdir    = -9.0000e+01
 - Zo      = 9.4000e-02
 - D       = 4.0000e-01
 - L       = -5.8050e+02
 - U*      = 7.0014e-01
 - distMin = 1.0000e+00
 - seuil   = 1.0000e-06
 - a       = 4.9406e+00
 - p       = 2.1344e-01
 - b       = 3.8241e-01
 - n       = 9.5328e-01
 - alpha   = 1.2602e+00
 - nu      = 3.7073e-02

[Info][source=ware,stage=RUNSTEP,timeindex=28800,wareid=pop.surf-atm.dispersion-deposition.fides,waretype=simulator] 
- nb of calcInfluence evaluations 9995 / 25600 for dist higher than distMin 1 m
- concRes size is 4 x 4 only 6 / 16 elements higher than threshold 1.000000e-06 (nb non nul element is 6)

[Info][source=ware,stage=RUNSTEP,timeindex=30600,wareid=pop.surf-atm.dispersion-deposition.fides,waretype=simulator] 
Simulation parameters :
 - Vdir    = -9.0000e+01
 - Zo      = 9.4000e-02
 - D       = 4.0000e-01
 - L       = -5.8050e+02
 - U*      = 7.0014e-01
 - distMin = 1.0000e+00
 - seuil   = 1.0000e-06
 - a       = 4.9406e+00
 - p       = 2.1344e-01
 - b       = 3.8241e-01
 - n       = 9.5328e-01
 - alpha   = 1.2602e+00
 - nu      = 3.7073e-02

[Info][source=ware,stage=RUNSTEP,timeindex=30600,wareid=pop.surf-atm.dispersion-deposition.fides,waretype=simulator] 
- nb of calcInfluence evaluations 9995 / 25600 for dist higher than distMin 1 m
- concRes size is 4 x 4 only 6 / 16 elements higher than threshold 1.000000e-06 (nb non nul element is 6)

[Info][source=ware,stage=RUNSTEP,timeindex=32400,wareid=pop.surf-atm.dispersion-deposition.fides,waretype=simulator] 
Simulation parameters :
 - Vdir    = -9.0000e+01
 - Zo      = 9.4000e-02
 - D       = 4.0000e-01
 - L       = -1.4154e+03
 - U*      = 6.9371e-01
 - distMin = 1.0000e+00
 - seuil   = 1.0000e-06
 - a       = 4.8970e+00
 - p       = 2.1709e-01
 - b       = 3.8197e-01
 - n       = 9.1587e-01
 - alpha   = 1.3012e+00
 - nu      = 6.4653e-02

[Info][source=ware,stage=RUNSTEP,timeindex=32400,wareid=pop.surf-atm.dispersion-deposition.fides,waretype=simulator] 
- nb of calcInfluence evaluations 9995 / 25600 for dist higher than distMin 1 m
- concRes size is 4 x 4 only 6 / 16 elements higher than threshold 1.000000e-06 (nb non nul element is 6)

[Info][source=ware,stage=RUNSTEP,timeindex=34200,wareid=pop.surf-atm.dispersion-deposition.fides,waretype=simulator] 
Simulation parameters :
 - Vdir    = -9.0000e+01
 - Zo      = 9.4000e-02
 - D       = 4.0000e-01
 - L       = -1.4154e+03
 - U*      = 6.9371e-01
 - distMin = 1.0000e+00
 - seuil   = 1.0000e-06
 - a       = 4.8970e+00
 - p       = 2.1709e-01
 - b       = 3.8197e-01
 - n       = 9.1587e-01
 - alpha   = 1.3012e+00
 - nu      = 6.4654e-02

[Info][source=ware,stage=RUNSTEP,timeindex=34200,wareid=pop.surf-atm.dispersion-deposition.fides,waretype=simulator] 
- nb of calcInfluence evaluations 9995 / 25600 for dist higher than distMin 1 m
- concRes size is 4 x 4 only 6 / 16 elements higher than threshold 1.000000e-06 (nb non nul element is 6)

[Info][source=ware,stage=RUNSTEP,timeindex=36000,wareid=pop.surf-atm.dispersion-deposition.fides,waretype=simulator] 
Simulation parameters :
 - Vdir    = -9.0000e+01
 - Zo      = 9.4000e-02
 - D       = 4.0000e-01
 - L       = 1.1288e+03
 - U*      = 6.9595e-01
 - distMin = 1.0000e+00
 - seuil   = 1.0000e-06
 - a       = 4.9109e+00
 - p       = 2.2490e-01
 - b       = 3.9150e-01
 - n       = 8.5596e-01
 - alpha   = 1.3689e+00
 - nu      = 1.0522e-01

[Info][source=ware,stage=RUNSTEP,timeindex=36000,wareid=pop.surf-atm.dispersion-deposition.fides,waretype=simulator] 
- nb of calcInfluence evaluations 9995 / 25600 for dist higher than distMin 1 m
- concRes size is 4 x 4 only 6 / 16 elements higher than threshold 1.000000e-06 (nb non nul element is 6)

[Info][source=ware,stage=RUNSTEP,timeindex=37800,wareid=pop.surf-atm.dispersion-deposition.fides,waretype=simulator] 
Simulation parameters :
 - Vdir    = -9.0000e+01
 - Zo      = 9.4000e-02
 - D       = 4.0000e-01
 - L       = 1.1290e+03
 - U*      = 6.9595e-01
 - distMin = 1.0000e+00
 - seuil   = 1.0000e-06
 - a       = 4.9109e+00
 - p       = 2.2490e-01
 - b       = 3.9150e-01
 - n       = 8.5596e-01
 - alpha   = 1.3689e+00
 - nu      = 1.0522e-01

[Info][source=ware,stage=RUNSTEP,timeindex=37800,wareid=pop.surf-atm.dispersion-deposition.fides,waretype=simulator] 
- nb of calcInfluence evaluations 9995 / 25600 for dist higher than distMin 1 m
- concRes size is 4 x 4 only 6 / 16 elements higher than threshold 1.000000e-06 (nb non nul element is 6)

[Info][source=ware,stage=RUNSTEP,timeindex=39600,wareid=pop.surf-atm.dispersion-deposition.fides,waretype=simulator] 
Simulation parameters :
 - Vdir    = -9.0000e+01
 - Zo      = 9.4000e-02
 - D       = 4.0000e-01
 - L       = 3.6775e+02
 - U*      = 6.9181e-01
 - distMin = 1.0000e+00
 - seuil   = 1.0000e-06
 - a       = 4.8794e+00
 - p       = 2.3473e-01
 - b       = 3.9523e-01
 - n       = 8.0610e-01
 - alpha   = 1.4286e+00
 - nu      = 1.3572e-01

[Info][source=ware,stage=RUNSTEP,timeindex=39600,wareid=pop.surf-atm.dispersion-deposition.fides,waretype=simulator] 
- nb of calcInfluence evaluations 9995 / 25600 for dist higher than distMin 1 m
- concRes size is 4 x 4 only 6 / 16 elements higher than threshold 1.000000e-06 (nb non nul element is 6)

[Info][source=ware,stage=RUNSTEP,timeindex=41400,wareid=pop.surf-atm.dispersion-deposition.fides,waretype=simulator] 
Simulation parameters :
 - Vdir    = -9.0000e+01
 - Zo      = 9.4000e-02
 - D       = 4.0000e-01
 - L       = 3.6775e+02
 - U*      = 6.9181e-01
 - distMin = 1.0000e+00
 - seuil   = 1.0000e-06
 - a       = 4.8794e+00
 - p       = 2.3473e-01
 - b       = 3.9523e-01
 - n       = 8.0610e-01
 - alpha   = 1.4286e+00
 - nu      = 1.3572e-01

[Info][source=ware,stage=RUNSTEP,timeindex=41400,wareid=pop.surf-atm.dispersion-deposition.fides,waretype=simulator] 
- nb of calcInfluence evaluations 9995 / 25600 for dist higher than distMin 1 m
- concRes size is 4 x 4 only 6 / 16 elements higher than threshold 1.000000e-06 (nb non nul element is 6)

[Info][source=ware,stage=RUNSTEP,timeindex=43200,wareid=pop.surf-atm.dispersion-deposition.fides,waretype=simulator] 
Simulation parameters :
 - Vdir    = -9.0000e+01
 - Zo      = 9.4000e-02
 - D       = 4.0000e-01
 - L       = 2.1886e+02
 - U*      = 6.9222e-01
 - distMin = 1.0000e+00
 - seuil   = 1.0000e-06
 - a       = 4.8812e+00
 - p       = 2.4429e-01
 - b       = 3.9932e-01
 - n       = 7.6287e-01
 - alpha   = 1.4814e+00
 - nu      = 1.6007e-01

[Info][source=ware,stage=RUNSTEP,timeindex=43200,wareid=pop.surf-atm.dispersion-deposition.fides,waretype=simulator] 
- nb of calcInfluence evaluations 9995 / 25600 for dist higher than distMin 1 m
- concRes size is 4 x 4 only 6 / 16 elements higher than threshold 1.000000e-06 (nb non nul element is 6)

[Info][source=ware,stage=FINALIZERUN,wareid=water-pop.atm-surf.state-change-scheduler.mipp,waretype=simulator] Cumulated rain on the whole watershed produced as output from events   : 0


*/