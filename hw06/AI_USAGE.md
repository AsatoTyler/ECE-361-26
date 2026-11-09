- **Tools used:** Gemini
- **For what:** Used to generate `regress.sh` shell script, create test suite, scenarios test text files, and formatting the Makefile.
**Debugging compilation errors:** Assisted in diagnosing GCC compiler and linker errors during testing.
- **One thing a model got wrong:**
When initially generating the `Makefile` the AI correctly identified that `roundf()` should be used to round the sensor values to the nearest 0.1, but it failed to include the `-lm` flag in the GCC linking targets. This caused the compiler to throw an `undefined reference to 'roundf'` ld linker error.
Additionally, the AI initially generated the `tests/test_hw06.c` unit test without adding the `-I.` flag to the compiler. Because the unit test was located in the `tests/` subdirectory, the compiler threw a "No such file or directory" fatal error when trying to `#include "sensor.h"`. Both issues were resolved by updating the `CFLAGS` and build targets in the `Makefile`.