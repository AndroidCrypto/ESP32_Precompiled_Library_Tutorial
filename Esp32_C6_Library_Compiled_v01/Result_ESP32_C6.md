WARNING: library SECRETMaths claims to run on esp32s3 architecture(s) and may be incompatible with your current board which runs on esp32 architecture(s).
Library SECRETMaths has been declared precompiled:
Precompiled library in "/Users/michaelfehr/Documents/Arduino/libraries/SECRETMaths/src/esp32c6" not found
/Users/michaelfehr/Library/Arduino15/packages/esp32/tools/esp-rv32/2601/bin/../lib/gcc/riscv32-esp-elf/14.2.0/../../../../riscv32-esp-elf/bin/ld: /Users/michaelfehr/Library/Caches/arduino/sketches/1E98D5441E565B440ACD0521A0B758B0/sketch/Esp32_C6_Library_Compiled_v01.ino.cpp.o: in function `setup()':
/Users/michaelfehr/Documents/Arduino/Esp32_C6_Library_Compiled_v01/Esp32_C6_Library_Compiled_v01.ino:9:(.text._Z5setupv+0x56): undefined reference to `SECRETMaths::add(int, int)'
/Users/michaelfehr/Library/Arduino15/packages/esp32/tools/esp-rv32/2601/bin/../lib/gcc/riscv32-esp-elf/14.2.0/../../../../riscv32-esp-elf/bin/ld: /Users/michaelfehr/Documents/Arduino/Esp32_C6_Library_Compiled_v01/Esp32_C6_Library_Compiled_v01.ino:20:(.text._Z5setupv+0xba): undefined reference to `SECRETMaths::multiply(int, int)'
/Users/michaelfehr/Library/Arduino15/packages/esp32/tools/esp-rv32/2601/bin/../lib/gcc/riscv32-esp-elf/14.2.0/../../../../riscv32-esp-elf/bin/ld: /Users/michaelfehr/Library/Caches/arduino/sketches/1E98D5441E565B440ACD0521A0B758B0/sketch/Esp32_C6_Library_Compiled_v01.ino.cpp.o: in function `_GLOBAL__sub_I_mathTool':
/Users/michaelfehr/Documents/Arduino/Esp32_C6_Library_Compiled_v01/Esp32_C6_Library_Compiled_v01.ino:28:(.text.startup._GLOBAL__sub_I_mathTool+0x8): undefined reference to `SECRETMaths::SECRETMaths()'
collect2: error: ld returned 1 exit status
exit status 1

Compilation error: exit status 1

Solution 1: duplicate folder src/esp32s3 and rename duplicate to esp32c6:
WARNING: library SECRETMaths claims to run on esp32s3 architecture(s) and may be incompatible with your current board which runs on esp32 architecture(s).
Library SECRETMaths has been declared precompiled:
Using precompiled library in /Users/michaelfehr/Documents/Arduino/libraries/SECRETMaths/src/esp32c6
/Users/michaelfehr/Library/Arduino15/packages/esp32/tools/esp-rv32/2601/bin/../lib/gcc/riscv32-esp-elf/14.2.0/../../../../riscv32-esp-elf/bin/ld: skipping incompatible /Users/michaelfehr/Documents/Arduino/libraries/SECRETMaths/src/esp32c6/libSECRETMaths.a when searching for -lSECRETMaths
/Users/michaelfehr/Library/Arduino15/packages/esp32/tools/esp-rv32/2601/bin/../lib/gcc/riscv32-esp-elf/14.2.0/../../../../riscv32-esp-elf/bin/ld: cannot find -lSECRETMaths: No such file or directory
/Users/michaelfehr/Library/Arduino15/packages/esp32/tools/esp-rv32/2601/bin/../lib/gcc/riscv32-esp-elf/14.2.0/../../../../riscv32-esp-elf/bin/ld: skipping incompatible /Users/michaelfehr/Documents/Arduino/libraries/SECRETMaths/src/esp32c6/libSECRETMaths.a when searching for -lSECRETMaths
/Users/michaelfehr/Library/Arduino15/packages/esp32/tools/esp-rv32/2601/bin/../lib/gcc/riscv32-esp-elf/14.2.0/../../../../riscv32-esp-elf/bin/ld: cannot find -lSECRETMaths: No such file or directory
collect2: error: ld returned 1 exit status
exit status 1

Compilation error: exit status 1

Solution 2: edit library.properties file:

old: architectures=esp32s3
new: architectures=esp32s3,esp32c6

WARNING: library SECRETMaths claims to run on esp32s3, esp32c6 architecture(s) and may be incompatible with your current board which runs on esp32 architecture(s).
Library SECRETMaths has been declared precompiled:
Using precompiled library in /Users/michaelfehr/Documents/Arduino/libraries/SECRETMaths/src/esp32c6
/Users/michaelfehr/Library/Arduino15/packages/esp32/tools/esp-rv32/2601/bin/../lib/gcc/riscv32-esp-elf/14.2.0/../../../../riscv32-esp-elf/bin/ld: skipping incompatible /Users/michaelfehr/Documents/Arduino/libraries/SECRETMaths/src/esp32c6/libSECRETMaths.a when searching for -lSECRETMaths
/Users/michaelfehr/Library/Arduino15/packages/esp32/tools/esp-rv32/2601/bin/../lib/gcc/riscv32-esp-elf/14.2.0/../../../../riscv32-esp-elf/bin/ld: cannot find -lSECRETMaths: No such file or directory
/Users/michaelfehr/Library/Arduino15/packages/esp32/tools/esp-rv32/2601/bin/../lib/gcc/riscv32-esp-elf/14.2.0/../../../../riscv32-esp-elf/bin/ld: skipping incompatible /Users/michaelfehr/Documents/Arduino/libraries/SECRETMaths/src/esp32c6/libSECRETMaths.a when searching for -lSECRETMaths
/Users/michaelfehr/Library/Arduino15/packages/esp32/tools/esp-rv32/2601/bin/../lib/gcc/riscv32-esp-elf/14.2.0/../../../../riscv32-esp-elf/bin/ld: cannot find -lSECRETMaths: No such file or directory
collect2: error: ld returned 1 exit status
exit status 1

Compilation error: exit status 1

ONLY Solution 3 will work: run the complete creation process on the ESP32-C6 (RISC-V processor) architecture

New Result:
ESP32-C6 Library Compiled Compiled Code V01
Addition: 6 + 7 = 13
Multiplication: 6 * 7 = 42
