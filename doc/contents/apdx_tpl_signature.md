# Parametric signature {#apdx_tpl_signature}

[TOC]

### Main use

Even if compiled simulators require an explicit definition of variable and unit class, the underlying code and signature can be templated. Before json signature, such change was possible through precompilation blocks around signature lines. 

Since we store signature as json file we provide a new workflow to handle parametric information both in code and documentation.


### In openfluid-wareinfo.json
Template variables (in id, unit definition...) should be surrounded by '@'

_Example:_
 `water.surf-@foo@`

### In cmake
Affect a value to your variable in the main `CMakeLists.txt`

_Example:_
`SET(foo "SU")`

Uncomment `CONFIGURED_SIGNATURE ON` in the `CMakeLists.txt` of `src`


### In sources
(as before the parametric signature change)

Variable can be transmitted to cpp as precompilation variable by adding a definition in the CMakeLists.txt

_Example:_
 `ADD_DEFINITIONS(-DISFOO=1)`

Afterwards the precompilation variable `ISFOO` will be found in cpp code

### In doc (optional)
Patterns in README and README.md in root dir of the ware will also be converted

_Example:_
```md
# water.runoff.@foo@


Effect on @foo@ will...
```

## Advanced use

### Signature migration from 2.1 to 2.2
If a ware contains some kind of parametric definition in signature block in OpenFLUID 2.1, we will need to remove temporarily all precompilation command and temporary placeholder used to replace the variables, otherwise the migration tool will fail. The variable can be reset afterwards in the same way than usual 2.2 template signature described above.

Two known cases can not yet be converted fully to this 2.2 syntax:
- Conditional blocks
- precompilation macro replacing one or several lines to avoid replication
