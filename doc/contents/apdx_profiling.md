# Simulation profiling {#apdx_profiling}

Profiling can be useful to see simulator order call of a simulation and duration of each step of each simulator. 

## Activation

OpenFLUID provides a tool to get this kind of information, it can be activated with `-k` in CLI and through a checkbox in the "simulation configuration" tab.

## Output

This option will add 3 files to the output folder. Below is an example of such files with a model made of a generator and a simulator:

```
==> .../OUT/openfluid-profile-cumulative.log <==
ID;INITPARAMS;PREPAREDATA;CHECKCONSISTENCY;INITIALIZERUN;RUNSTEP;FINALIZERUN
var3.unitsA.gendouble;0.000022528;0.000000532;0.000000447;0.000005736;0.005723256;0.000000684
examples.primitives.unitsA.prod;0.000000475;0.000000504;0.000000353;0.000016522;0.008818488;0.000000378

==> .../OUT/openfluid-profile-schedule.log <==
TIMEINDEX;<simulators call sequence>
0;var3.unitsA.gendouble;examples.primitives.unitsA.prod
3600;var3.unitsA.gendouble;examples.primitives.unitsA.prod
7200;var3.unitsA.gendouble;examples.primitives.unitsA.prod
10800;var3.unitsA.gendouble;examples.primitives.unitsA.prod
14400;var3.unitsA.gendouble;examples.primitives.unitsA.prod
18000;var3.unitsA.gendouble;examples.primitives.unitsA.prod
21600;var3.unitsA.gendouble;examples.primitives.unitsA.prod
25200;var3.unitsA.gendouble;examples.primitives.unitsA.prod
28800;var3.unitsA.gendouble;examples.primitives.unitsA.prod

==> .../OUT/openfluid-profile-timeindex.log <==
TIMEINDEX;var3.unitsA.gendouble;examples.primitives.unitsA.prod
0;0.000005736;0.000016522
3600;0.000003592;0.000023136
7200;0.000002119;0.000002995
10800;0.000001690;0.000002390
14400;0.000001672;0.000001982
18000;0.000001261;0.000001905
21600;0.000001129;0.000002074
25200;0.000001332;0.000002052
28800;0.000003123;0.000002813
```