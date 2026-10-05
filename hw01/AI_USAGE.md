# AI Usage

**Tools Used:** Gemini
**Purpose:** I used Gemini to help structure my C headers and source files, generate bitmask logic safely to avoid undefined behavior (specifically using `1ULL` for 32-bit shifts), and build my `Makefile` and `test_bits.c` test harness. 

**Correction/Fix:** Gemini initially suggested logic for `sign_extend` that failed the 32-bit edge case because shifting `0xFFFFFFFF` by 32 bits caused undefined behavior, resulting in a return value of `-1` instead of the expected `-2147483648`. I found this by running `make test`, and we fixed it by adding an explicit `if (width == 32)` guard at the top of the function to bypass the shift entirely.