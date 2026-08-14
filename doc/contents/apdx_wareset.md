# Wareset operations {#apdx_wareset}

In OpenFLUID since version 2.2.2, a ware set is a group of simulators and observers for which we do coordinate operations. The most frequent use case is the rebuild of setup to run a given dataset.

## How to rerun a simulation based on dataset information

Since most information required to rebuild a simulation environment is contained in the fluidx files (enabled simulators and obsevers), we can use it to rebuild from scratch all these wares.

@warning For data safety reasons, we strongly advise against calling the wsetup-wareset command in your .openfluid or other work userdata folder. Folder move should be done by hand if a transfer into a working folder is wanted (see dedicated paragraph in "advanced use" section).

A simple use case: we want to rerun Firespread example dataset in an isolated environment. To do so we can call:
`openfluid setup-wareset -w https://hub.openfluid-project.org/api/ -s ~/.openfluid/examples/projects/Firespread/IN/ -p /tmp/NEW_LOCATION -r`.

### Arguments

- `-w`: ware source, can be a remote (git or hub) or local *primary* source containing source code for listed wares
- `-s`: set source, can be a remote or local dataset or a wareset-lock file
- `-p`: path to new location to build the new userdata path (isolated workspace and wares folders)
- `-r`: to run simulation after build

For information about other arguments, call `openfluid setup-wareset -h`.

### Customization of setup
A `wareset-setup.json` file can be added in the IN to provide complementary information about:
- ware source URL (will overwrite the `-w` value)
- ware version
- pre-configure commands
- configure-options

To be connected to the fluidx information, each ware require both information: `id` and `type`.

For information identical to all wares, a group with `"id": "*"` can be added, which will be propagated to all effective wares.

Example:
```
{
  "name": "Custom wareset wares",
  "description": "For direct git call",
  "version": "0.1",
  "wares-setup": [
    {
      "id": "*",
      "version": "openfluid-2.0",
    },
    {
      "type": "simulator",
      "git-url": "https://dev.openfluid-project.org/testing/git/standard",
      "id": "water.surf-uz.runoff-infiltration.mseytoux",
      "version": "for-test",
      "pre-configure-commands": 
        [
          {
            "program": "%%OF%%",
            "args": ["--version"]
          },
          {
            "program": "/usr/bin/ls",
            "args": ["%%OPENFLUID_TEMP_PATH%%"]
          }
        ],
      "configure-options": {"foo": "bar"}
    },
    {
      "type": "simulator",
      "git-url": "https://dev.openfluid-project.org/testing/git/standard",
      "id": "water.surf-uz.runoff-infiltration.mseythree",
      "configure-options": {"foo": "cut"}
    }
  ]
}
```



### Workflow of setup-wareset

- Identify the wares to build from set information

For each ware:

- **Get source**, from remote (via git) or local folder (via copy)
- **Checkout** indicated version (optional, can be a branch, a tag or a commit hash)
- Custom operations provided in wareset-setup.json
- **Configure** ware
- **Build** ware with install in isolated `wares` folder

Global:

- Write `wareset-lock` file
- Run simulation from given dataset in the new environment (optional)
- Display **summary** table for each step and each ware


## Advanced use

### Placeholder variables
As shown in the example, several placeholders can be used inside the `wareset-setup.json` file:

On program side:

- `%%OF%%`, will be replaced by the openfluid binary location

On arguments side:

- `%%BuildParent%%` will be replaced by either workspace path or ware path, useful for file manipulation in build context
- `%%WarePath%%` will be replaced by the absolute ware path
- `%%WorkspacePath%%` will be replaced by the absolute isolated workspace path
- `%%FOO%%`: the command will try to convert any variable between `%%`, here `FOO`, if it exists as environment variable in the command context


### Project CMakeLists
Option `-m` allows for shared configuration and compilation of all wares, leveraging cmake capabilities to optimize compute resources (option `-j <N>` to define the number of parallel jobs to be run).

**Pro:** can be way faster to operate and run steps than sequential build

**Cons:** if a step fails, it will be harder to know which ware had an issue

If this option is active, several files will be added in `wares-dev` folder to provide a "model-level" CMake structure:

- Central `CMakeLists.txt` with necessary options for multi-ware build
- Adjacent `CMake.wareset.in.config` listing folders of each ware
- A global `_build` folder will be created in which the operations will be done.

@warning Since all wares are compiled in the same context, it may happen that a variable set for a ware is used by a subsequent ware. To avoid this risk, you should skip this `-m` option, making each ware compilation isolated.

This strategy will allow you to do common actions on all wares when in the shared `_build` folder: reconfigure all wares, build them, apply any common target like `make docalyze` to generate documentation of each one or `ctest` to run all tests in these wares (problematic if several wares have a test with the same name)

### Individual ware build after setup
Once the wareset setup is done, operations can be done at global level or at ware level and still use the CMake customization that is stored in a `CMake.contextual.in.config` file.

If you create a `_build` folder at ware level, by default a compilation will ighore the contextual CMake file to get the default environment. You can activate the contextual CMake by adding the option `CUSTOM_VARS` during cmake call:
`cmake .. -DCUSTOM_VARS=ON`.

### Wareset lock file
Any time the command `setup-wareset` is called, a `wareset-lock.json` file is generated, containing precise information about each ware (when available):

- Full URL of git remote repository
- Commit hash of git version
- All complementary ware information from `wareset-setup.json` file (both direct and from common block)

This file can be used instead of a dataset as input for `-s` option, to get a snapshot of version for the environment as a form of reproductibility, even if not absolute.

### Injection into `.openfluid` folder
Or any folder following `.openfluid` structure.

For memory, here is what contains a `.openfluid` folder:
```
.openfluid
    wares/
    workspace/
        projects/
        wares-dev/
    ...
```

The isolated folder generated by `setup-wareset` command will have the same structure.

The main resources you may want to move to your .openfluid are ware soure code and binaries of compiled wares. OpenFLUID does not provide an automatic method to move them but you can use them or copy them manually:

- For a fully isolated work you can adjust the current userdata path, eg by launching in a command line interface: `OPENFLUID_USERDATA_PATH=/path/to/isolated/folder openfluid-builder`

- If you want to work with your previous .openfluid folder:
  - In DevStudio, you can define a custom workspace directly from the one ine the new path location to work with these ware sources and build them
  - If you copy content of generated *wares* into `.openfluid/wares` you will be able to use them with OpenFLUID (not advised since it will decorrelate the workspace content and the wares binaries and may erase valuable binaries)