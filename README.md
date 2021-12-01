# AER Mod Development Kit (MDK)


*(Mod authors should change this README.)*

The **AER MDK** is a starter template for creating new modifications (mods) for the Linux version of the game [Hyper Light Drifter (HLD)](https://en.wikipedia.org/wiki/Hyper_Light_Drifter) using the [Action-Event-Response (AER)](https://github.com/users/Foxbud/projects/1) C modding framework.

## Features
- [Visual Studio Code (VS Code)](https://code.visualstudio.com/) support.
- [CMake](https://cmake.org/) build system with pre-configured targets.
- VS Code tasks that wrap the CMake targets.
- VS Code snippets for common modding idioms.
- Quickly run and test mod without having to install it.
- Line-by-line mod debugging.
- Debug-modpack for configuring mod and testing how it interacts with other mods.

## Requirements
- Either the [Steam](https://duckduckgo.com/?t=ffab&q=hyper+light+drifter&ia=web) or [GOG](https://www.gog.com/game/hyper_light_drifter) version of Hyper Light Drifter.
- `AER_GAMEDIR` environment variable that points to the root of the HLD installation.
- Recent versions of the AER [executable patch](https://github.com/Foxbud/aerpatch/releases/latest) and [mod runtime environment](https://github.com/Foxbud/libaermre/releases/latest) (use [aerman](https://github.com/Foxbud/aerman/releases/latest) to install them).
- A recent version of either [GCC](https://gcc.gnu.org/) or [Clang](https://clang.llvm.org/) with 32-bit support.
- [CMake](https://cmake.org/) v3.12 or greater.
- A recent version of the [GNU Project Debugger (gdb)](https://www.gnu.org/software/gdb/).
- [Visual Studio Code](https://code.visualstudio.com/) (either open-source or proprietary).

## Quick Start
Download and extract the [latest version of the MDK](https://github.com/Foxbud/aermdk/releases/latest), then open it in VS Code:

```
wget https://github.com/Foxbud/aermdk/archive/refs/tags/v20210326.tar.gz
tar -xf ./v20210326.tar.gz
code ./aermdk-20210326/
```

Install the recommended VS Code extensions specified in the file `.vscode/extensions.json`. If you use the open-source version of VS Code, you may need to manually download some of them from the [marketplace](https://marketplace.visualstudio.com/VSCode).

Next, enter your mod's information in the file `CMakeLists.txt`:

```cmake
# Define project.
project(         "my_mod"
    VERSION      "0.1.0"
    DESCRIPTION  "Mod that logs a message to the console."
    HOMEPAGE_URL "http://example.com"
    LANGUAGES C
)
set(PROJECT_MINIMUM_MRE_VERSION
    "1.0.0"
)
set(PROJECT_AUTHORS
    "Drifter"
    "Alt Drifter"
)
```

Note that your mod's name **must** consist **only** of lowercase ASCII alphabetic letters, numbers, and underscores. Using any other characters will likely result in issues that are difficult to debug.

Then enter the same name into `.vscode/settings.json`:

```json
"mdk": {
    "modName": "my_mod",
    "extraModNames": []
},
```

After that, press the keyboard shortcut `Ctrl + Shift + B` to build the empty mod library in debug mode. You should see output similar to:

```
> Executing task: cmake -S . -B build/debug -DCMAKE_BUILD_TYPE=Debug <

-- The C compiler identification is GNU 10.2.0
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working C compiler: /usr/bin/gcc - skipped
-- Detecting C compile features
-- Detecting C compile features - done
-- Performing Test COMPILER_HAS_HIDDEN_VISIBILITY
-- Performing Test COMPILER_HAS_HIDDEN_VISIBILITY - Success
-- Performing Test COMPILER_HAS_HIDDEN_INLINE_VISIBILITY
-- Performing Test COMPILER_HAS_HIDDEN_INLINE_VISIBILITY - Failed
-- Performing Test COMPILER_HAS_DEPRECATED_ATTR
-- Performing Test COMPILER_HAS_DEPRECATED_ATTR - Success
-- Configuring done
-- Generating done
-- Build files have been written to: /home/gfairburn/Documents/development/c/aermdk/build/debug

Terminal will be reused by tasks, press any key to close it.

> Executing task: cmake --build build/debug <

Scanning dependencies of target my_mod
[ 50%] Building C object CMakeFiles/my_mod.dir/src/moddef.c.o
[100%] Linking C shared library libmy_mod.so
[100%] Built target my_mod

Terminal will be reused by tasks, press any key to close it.
```

If you don't see output like that, please [submit an issue](https://github.com/Foxbud/aermdk/issues/new) or reach out for help on the [HLD Discord Server](https://discord.gg/XaBrHDHTGe).

Note that you may need to restart VS Code for Intellisense to kick in.

Now edit `src/moddef.c` to look like:

```c
/* src/moddef.c */
#include "aer/log.h"

#include "moddef.h"
#include "export.h"

/* ----- PRIVATE FUNCTIONS ----- */

static void RoomChangeListener(int32_t newRoomIdx, int32_t prevRoomIdx) {
    AERLogInfo("Switched from room %i to room %i.", prevRoomIdx, newRoomIdx);
}

/* ----- PUBLIC FUNCTIONS ----- */

MOD_EXPORT void DefineMod(AERModDef* def) {
    def->roomChangeListener = RoomChangeListener;

    return;
}
```

Finally, press the `F5` key to build and run your mod. You should see the HLD game window open up, but turn your attention towards the "TERMINAL" tab of the console window at the bottom of the screen. Periodically, messages similar to the following should appear:

```
[18:23:14][aer][my_mod] (INFO) Switched from room 2 to room 3.
```