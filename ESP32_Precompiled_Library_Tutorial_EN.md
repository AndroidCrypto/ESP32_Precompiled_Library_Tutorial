# How to Create a Precompiled Library for the ESP32 (Expert Level)

This tutorial is a bit different from my usual guides, which typically focus on connecting sensors or displays to an ESP32 microcontroller or setting up wireless communication between two boards. In this tutorial, I will walk you through creating a precompiled library for use in an Arduino sketch. A quick heads-up before we start: using only the built-in Arduino tools, you will need to take a few unconventional steps to reach the finish line.

## Table of Contents

- [Why Create a Precompiled Library?](#why-create-a-precompiled-library)
- [Required Hardware](#required-hardware)
- [Software Used](#software-used)
- [Example Sketch and Library](#example-sketch-and-library)
- [Overview of the Compilation Workflow](#overview-of-the-compilation-workflow)
  - [Step 1: Configure the Arduino IDE](#step-1-configure-the-arduino-ide)
  - [Step 2: Test-Compile the Plain-Text Library and Extract Required Commands](#step-2-test-compile-the-plain-text-library-and-extract-required-commands)
  - [Step 3: Compile the `.cpp` File into a `.o` Object File](#step-3-compile-the-cpp-file-into-a-o-object-file)
  - [Step 4: Archive the `.o` File into a `.a` Static Library](#step-4-archive-the-o-file-into-a-a-static-library)
  - [Step 5: Create the Folder Structure in the Arduino `libraries` Directory](#step-5-create-the-folder-structure-in-the-arduino-libraries-directory)
  - [Step 6: Practical Test](#step-6-practical-test)
- [Testing with Another Processor Type – ESP32-C6](#testing-with-another-processor-type--esp32-c6)
- [Solution for the ESP32-C6](#solution-for-the-esp32-c6)
- [Transferring the Library to Other Computers](#transferring-the-library-to-other-computers)
- [Conclusion](#conclusion)

---

## Why Create a Precompiled Library?

From a user's perspective, you need a library to operate hardware such as a BME280 environmental sensor on an ESP32 and process the measured values (temperature, humidity, and barometric pressure). Typically, the user searches for a suitable library via the Arduino Library Manager or installs it from a downloaded archive.

Now let's switch to the perspective of the library developer. In the vast majority of cases, the developer provides the entire library source code in plain text so anyone interested can inspect it. This is standard practice and highly desired in open-source projects, as it allows the community to fix bugs or add missing features and share those improvements with others.

This holds true for well over 90 percent of Arduino libraries. However, there are situations where a library contains proprietary or confidential functionality that the developer does not want to expose publicly. Another common scenario involves signing a Non-Disclosure Agreement (NDA), obligating the developer to keep certain implementation details private. In these cases, distributing the library exclusively in binary form prevents others from reading the source code.

One potential drawback of this approach is that the community cannot assist with troubleshooting if a bug occurs. Additionally, some projects legally or policies-wise require all software components to be open-source before deployment—in which case a precompiled library obviously cannot be used.

> **Important Limitation:** We will only compile the `.cpp` implementation file into binary form. The `.h` header file must remain in plain text and fully readable so the compiler knows what functions and classes are available.

---

## Required Hardware

In principle, you can follow this tutorial using any microcontroller from the ESP32 family, though I recommend using an **ESP32-S3** processor.

---

## Software Used

I am using the **Arduino IDE version 2.3.11** (macOS) along with the **esp32 board package version 3.x.x**. All required compiler tools used in this tutorial are already installed on your system as part of the Arduino IDE and the ESP32 board package.

---

## Example Sketch and Library

I kept both the sketch and the library as simple as possible so we can stay focused on the compilation process. Our example consists of a main sketch and a library composed of a header file and a source (`.cpp`) file. The library, named `SECRETMaths`, provides two "top-secret" math functions: adding and multiplying integers. Both functions are called in the main sketch, and all output is printed to the Serial Monitor.

### Main Sketch (`Esp32_S3_Library_Plain_v01.ino`)

```cpp
#include "SECRETMaths.h"

// Create an instance of SECRETMaths
SECRETMaths mathTool;

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("ESP32-S3 Library Compiled Plain Code V01");

  int x = 6;
  int y = 7;

  // Test addition
  int additionResult = mathTool.add(x, y);
  Serial.print("Addition: ");
  Serial.print(x);
  Serial.print(" + ");
  Serial.print(y);
  Serial.print(" = ");
  Serial.println(additionResult);

  // Test multiplication
  int multiplicationResult = mathTool.multiply(x, y);
  Serial.print("Multiplication: ");
  Serial.print(x);
  Serial.print(" * ");
  Serial.print(y);
  Serial.print(" = ");
  Serial.println(multiplicationResult);
}

void loop() {
  // Nothing to do here
}
```

### Header File (`SECRETMaths.h`)

```cpp
#ifndef SECRETMATHS_H
#define SECRETMATHS_H

#include <Arduino.h>

class SECRETMaths {
  public:
    // Constructor
    SECRETMaths();

    // Adds two integer numbers and returns the result
    int add(int a, int b);

    // Multiplies two integer numbers and returns the result
    int multiply(int a, int b);
};

#endif
```

### Source File (`SECRETMaths.cpp`)

```cpp
#include "SECRETMaths.h"

// Constructor implementation
SECRETMaths::SECRETMaths() {
    // Initialization code if needed
}

// Function to add two integers
int SECRETMaths::add(int a, int b) {
    return a + b + 100;
}

// Function to multiply two integers
int SECRETMaths::multiply(int a, int b) {
    return a * b;
}
```

*(Note: Notice the secret offset of `+ 100` in the `add()` implementation—or standard addition `a + b` depending on your test setup. When running the standard version, you will see the following output in the Serial Monitor:)*

```text
ESP32-S3 Library Compiled Plain Code V01
Addition: 6 + 7 = 13
Multiplication: 6 * 7 = 42
```

---

## Overview of the Compilation Workflow

The workflow consists of 6 steps that, when executed in the proper order, produce the desired precompiled library:

1. **Step 1:** Configure the Arduino IDE
2. **Step 2:** Test-compile the plain-text library and extract the required compiler commands
3. **Step 3:** Convert the `.cpp` file into a `.o` object file
4. **Step 4:** Archive the `.o` file into a `.a` static library file
5. **Step 5:** Create and populate the folder structure inside the Arduino `libraries` directory
6. **Step 6:** Put the precompiled library to practical use

---

### Step 1: Configure the Arduino IDE

First, we need to make a small adjustment in the Arduino IDE settings so that it outputs detailed logs during the build process. Once you are finished with this tutorial, you can turn this option off again, as the sheer volume of output can be distracting during everyday coding.

1. Open **Preferences** (or **Settings**) in the Arduino IDE.
2. Locate **Show verbose output during** and check the box next to **compile** (unchecked by default).
3. Click **OK** to save the setting.

---

### Step 2: Test-Compile the Plain-Text Library and Extract Required Commands

With the plain-text library files placed in your sketch folder, click **Verify / Compile** (uploading to the board is not necessary). The output console will now be flooded with detailed build logs. Here is a brief excerpt:

```text
FQBN: esp32:esp32:esp32s3:CDCOnBoot=cdc
Using board 'esp32s3' from platform in folder: /Users/michaelfehr/Library/Arduino15/packages/esp32/hardware/esp32/3.3.11
Using core 'esp32' from platform in folder: /Users/michaelfehr/Library/Arduino15/packages/esp32/hardware/esp32/3.3.11
/usr/bin/env bash -c "[ ! -f "/Users/michaelfehr/Library/Arduino15/packages/esp32/hardware/esp32/3.3.11"/tools/partitions/default.csv ] || cp -f "/Users/michaelfehr/Library/Arduino15/packages/esp32/hardware/esp32/3.3.11"/tools/partitions/default.csv "/Users/michaelfehr/Library/Caches/arduino/sketches/AA19498AE8EBFCF49E553E1945DC5D3E"/partitions.csv"
...
/Users/michaelfehr/Library/Arduino15/packages/esp32/tools/esp-x32/2601/bin/xtensa-esp32s3-elf-size -A /Users/michaelfehr/Library/Caches/arduino/sketches/AA19498AE8EBFCF49E553E1945DC5D3E/Esp32_S3_Library_Compiled_v01.ino.elf
Sketch uses 297790 bytes (22%) of program storage space. Maximum is 1310720 bytes.
Global variables use 22328 bytes (6%) of dynamic memory, leaving 305352 bytes for local variables. Maximum is 327680 bytes.
```

Copy the entire compiler output into a text editor tab so you can easily search through it. Search for the name of your library's `.cpp` file (`SECRETMaths.cpp`) to locate this exact command line:

```bash
/Users/michaelfehr/Library/Arduino15/packages/esp32/tools/esp-x32/2601/bin/xtensa-esp32s3-elf-g++ -MMD -c @/Users/michaelfehr/Library/Arduino15/packages/esp32/tools/esp32s3-libs/3.3.11/flags/cpp_flags -w -Os -Werror=return-type -DF_CPU=240000000L -DARDUINO=10607 -DARDUINO_ESP32S3_DEV -DARDUINO_ARCH_ESP32 "-DARDUINO_BOARD="ESP32S3_DEV"" "-DARDUINO_VARIANT="esp32s3"" -DARDUINO_PARTITION_default "-DARDUINO_HOST_OS="macosx"" "-DARDUINO_FQBN="esp32:esp32:esp32s3:UploadSpeed=921600,USBMode=hwcdc,CDCOnBoot=cdc,MSCOnBoot=default,DFUOnBoot=default,UploadMode=default,CPUFreq=240,FlashMode=qio,FlashSize=4M,PartitionScheme=default,DebugLevel=none,PSRAM=disabled,LoopCore=1,EventsCore=1,EraseFlash=none,JTAGAdapter=default,ZigbeeMode=default"" -DESP32=ESP32 -DCORE_DEBUG_LEVEL=0 -DARDUINO_RUNNING_CORE=1 -DARDUINO_EVENT_RUNNING_CORE=1 -DARDUINO_USB_MODE=1 -DARDUINO_USB_CDC_ON_BOOT=1 -DARDUINO_USB_MSC_ON_BOOT=0 -DARDUINO_USB_DFU_ON_BOOT=0 @/Users/michaelfehr/Library/Arduino15/packages/esp32/tools/esp32s3-libs/3.3.11/flags/defines -I/Users/michaelfehr/Documents/Arduino/Esp32_S3_Library_Compiled_v01 -iprefix /Users/michaelfehr/Library/Arduino15/packages/esp32/tools/esp32s3-libs/3.3.11/include/ @/Users/michaelfehr/Library/Arduino15/packages/esp32/tools/esp32s3-libs/3.3.11/flags/includes -I/Users/michaelfehr/Library/Arduino15/packages/esp32/tools/esp32s3-libs/3.3.11/qio_qspi/include -I/Users/michaelfehr/Library/Arduino15/packages/esp32/hardware/esp32/3.3.11/cores/esp32 -I/Users/michaelfehr/Library/Arduino15/packages/esp32/hardware/esp32/3.3.11/variants/esp32s3 @/Users/michaelfehr/Library/Caches/arduino/sketches/AA19498AE8EBFCF49E553E1945DC5D3E/build_opt.h @/Users/michaelfehr/Library/Caches/arduino/sketches/AA19498AE8EBFCF49E553E1945DC5D3E/file_opts /Users/michaelfehr/Library/Caches/arduino/sketches/AA19498AE8EBFCF49E553E1945DC5D3E/sketch/SECRETMaths.cpp -o /Users/michaelfehr/Library/Caches/arduino/sketches/AA19498AE8EBFCF49E553E1945DC5D3E/sketch/SECRETMaths.cpp.o
```

*(Note: Keep this command on a single continuous line without line breaks when editing it in your text editor.)*

Now we need to make two modifications to this command:

1. **Remove `-MMD`:** Search for `elf-g++` near the beginning of the line (`...esp32s3-elf-g++ -MMD -c @/Users/...`). The `-MMD` dependency flag is not needed here and should be deleted.
2. **Replace the trailing paths:** Search for `variants/esp32s3 @/Users/` near the end of the line. Delete everything after `variants/esp32s3` and replace it with:
   ```text
   SECRETMaths.cpp -o SECRETMaths.o
   ```

> **Note:** If you are compiling your own custom library, make sure to search for your own `.cpp` filename and use your own filenames in this second step.

Your modified first command should now look like this:

```bash
/Users/michaelfehr/Library/Arduino15/packages/esp32/tools/esp-x32/2601/bin/xtensa-esp32s3-elf-g++ -c @/Users/michaelfehr/Library/Arduino15/packages/esp32/tools/esp32s3-libs/3.3.11/flags/cpp_flags -w -Os -Werror=return-type -DF_CPU=240000000L -DARDUINO=10607 -DARDUINO_ESP32S3_DEV -DARDUINO_ARCH_ESP32 "-DARDUINO_BOARD="ESP32S3_DEV"" "-DARDUINO_VARIANT="esp32s3"" -DARDUINO_PARTITION_default "-DARDUINO_HOST_OS="macosx"" "-DARDUINO_FQBN="esp32:esp32:esp32s3:UploadSpeed=921600,USBMode=hwcdc,CDCOnBoot=cdc,MSCOnBoot=default,DFUOnBoot=default,UploadMode=default,CPUFreq=240,FlashMode=qio,FlashSize=4M,PartitionScheme=default,DebugLevel=none,PSRAM=disabled,LoopCore=1,EventsCore=1,EraseFlash=none,JTAGAdapter=default,ZigbeeMode=default"" -DESP32=ESP32 -DCORE_DEBUG_LEVEL=0 -DARDUINO_RUNNING_CORE=1 -DARDUINO_EVENT_RUNNING_CORE=1 -DARDUINO_USB_MODE=1 -DARDUINO_USB_CDC_ON_BOOT=1 -DARDUINO_USB_MSC_ON_BOOT=0 -DARDUINO_USB_DFU_ON_BOOT=0 @/Users/michaelfehr/Library/Arduino15/packages/esp32/tools/esp32s3-libs/3.3.11/flags/defines -I/Users/michaelfehr/Documents/Arduino/Esp32_S3_Library_Compiled_v01 -iprefix /Users/michaelfehr/Library/Arduino15/packages/esp32/tools/esp32s3-libs/3.3.11/include/ @/Users/michaelfehr/Library/Arduino15/packages/esp32/tools/esp32s3-libs/3.3.11/flags/includes -I/Users/michaelfehr/Library/Arduino15/packages/esp32/tools/esp32s3-libs/3.3.11/qio_qspi/include -I/Users/michaelfehr/Library/Arduino15/packages/esp32/hardware/esp32/3.3.11/cores/esp32 -I/Users/michaelfehr/Library/Arduino15/packages/esp32/hardware/esp32/3.3.11/variants/esp32s3 SECRETMaths.cpp -o SECRETMaths.o
```

Next, we create a second command using the compiler path from the beginning of the line above. Take the path ending in `-elf-g++`:

```text
/Users/michaelfehr/Library/Arduino15/packages/esp32/tools/esp-x32/2601/bin/xtensa-esp32s3-elf-g++
```

Replace `elf-g++` with `elf-ar` and append `rcs libSECRETMaths.a SECRETMaths.o`. The complete second command looks like this:

```bash
/Users/michaelfehr/Library/Arduino15/packages/esp32/tools/esp-x32/2601/bin/xtensa-esp32s3-elf-ar rcs libSECRETMaths.a SECRETMaths.o
```

> **Important Note:** Notice the two filenames: `SECRETMaths.o` and `libSECRETMaths.a`. The `lib` prefix on the `.a` file is **not a typo**—the GCC linker requires static archive libraries to start with `lib`.

We now have both commands ready and can move on to compiling.

---

### Step 3: Compile the `.cpp` File into a `.o` Object File

*(Note: The following instructions apply to macOS.)*

1. Open **Finder** and right-click on your sketch directory (`Esp32_S3_Library_Plain_v01`).
2. From the context menu, select **New Terminal at Folder**.
3. Copy and paste the **first command** into the Terminal window and press **Enter**.

After a brief moment, the compilation finishes and a new file named `SECRETMaths.o` will appear in your project directory. **Keep the Terminal window open** for the next step.

---

### Step 4: Archive the `.o` File into a `.a` Static Library

This step is quick and straightforward:

1. Copy and paste the **second command** into the same Terminal window and press **Enter**.
2. You will now see the static archive file `libSECRETMaths.a` inside your sketch folder.

Good news—the hardest part is now behind you!

---

### Step 5: Create the Folder Structure in the Arduino `libraries` Directory

While plain-text libraries can easily be placed directly inside a sketch folder, precompiled libraries do not work reliably that way. Instead, they should be installed properly inside the global Arduino `libraries` folder.

1. Open **Finder** and navigate to your `Documents/Arduino/libraries` folder.
2. Create a new folder named after your library: `SECRETMaths`.
3. Inside `SECRETMaths`, create a subfolder named `src`.
4. Inside `src`, create another subfolder named `esp32s3`.

```text
Arduino/
└── libraries/
    └── SECRETMaths/
        ├── library.properties
        └── src/
            ├── SECRETMaths.h
            └── esp32s3/
                └── libSECRETMaths.a
```

> **Hardware Architecture Note:** The subfolder name (`esp32s3`) must match the exact target hardware architecture for which you compiled the `.o` file in Step 3. This means if you want your library to support multiple members of the ESP32 family, you must repeat Steps 3 and 4 for each processor variant. Otherwise, the library will only compile for the specific processor you targeted.

Now move the two files into place:
- Copy the plain-text header file (`SECRETMaths.h`) into `SECRETMaths/src/`.
- Copy the compiled archive (`libSECRETMaths.a`) into `SECRETMaths/src/esp32s3/`.

Finally, create a plain-text file named `library.properties` in the root folder of your library (`SECRETMaths/library.properties`) with the following content:

```properties
name=SECRETMaths
version=1.0.0
author=AndroidCrypto
maintainer=AndroidCrypto
sentence=A precompiled ESP32 library.
paragraph=Description of the library functionality.
category=Uncategorized
url=https://example.com
architectures=esp32
precompiled=true
ldflags=-lSECRETMaths
```

Let's take a closer look at the last three properties:
- `architectures=esp32`: Defines the processor family supported by the library. Even though we have only compiled binaries for the ESP32-S3 so far, Arduino evaluates the broader `esp32` platform family here.
- `precompiled=true`: **Essential.** Without this flag, the Arduino IDE will look for plain-text `.cpp` source files and throw a compilation error when it cannot find them.
- `ldflags=-lSECRETMaths`: Instructs the linker to link against our static library. Notice that `-l` replaces the `lib` prefix and omits the `.a` file extension (so `libSECRETMaths.a` becomes `-lSECRETMaths`).

Everything is now configured. If you compress the `SECRETMaths` folder into a `.zip` file, you can easily distribute and install it on any other computer via **Sketch > Include Library > Add .ZIP Library...** in the Arduino IDE.

---

### Step 6: Practical Test

Now let's put the precompiled library to the test. Open the test sketch `Esp32_S3_Library_Compiled_v01.ino`, which contains the exact same code as our initial example sketch, **except that `SECRETMaths.h` and `SECRETMaths.cpp` are no longer in the sketch folder**.

When the sketch includes `#include "SECRETMaths.h"`, the Arduino IDE pulls it directly from the global `libraries/SECRETMaths` folder. Compile and upload the sketch to your ESP32-S3—the Serial Monitor output confirms everything works as expected:

```text
ESP32-S3 Library Compiled Code V01
Addition: 6 + 7 = 13
Multiplication: 6 * 7 = 42
```

---

## Testing with Another Processor Type – ESP32-C6

Next, let's see what happens when we compile the identical sketch (`Esp32_C6_Library_Compiled_v01.ino`) for a different board, such as the **ESP32C6 Dev Module**. Switch the target board in the Arduino IDE to an ESP32-C6 and click **Verify / Compile**. You will encounter the following compiler error:

```text
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
```

As the log points out, the IDE looked for `SECRETMaths/src/esp32c6` and couldn't find it. What happens if we duplicate the `src/esp32s3` folder and rename the copy to `src/esp32c6`? Let's try compiling again:

```text
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
```

The root cause is right there in the linker message: `skipping incompatible ... libSECRETMaths.a`. The ESP32-S3 uses an Xtensa core, whereas the ESP32-C6 uses a RISC-V core, making the binary incompatible.

---

## Solution for the ESP32-C6

As mentioned in Step 5, if you want your library to run across multiple ESP32 variants, you must repeat the compilation workflow (Steps 2 through 4) for each target processor using that board's specific compiler toolchain (`riscv32-esp-elf-g++` and `riscv32-esp-elf-ar` for the C6). Then, create a matching subfolder inside `src` (e.g., `src/esp32c6`) and place the newly generated `libSECRETMaths.a` inside it.

> **A Quick Experiment with RISC-V Chips:** Out of curiosity, I duplicated the properly compiled `src/esp32c6` folder and renamed it to `src/esp32c3`. Because both the ESP32-C6 and ESP32-C3 share the RISC-V architecture and our example library only uses basic arithmetic, the sketch compiled and ran without issues. However, as soon as a library relies on hardware-specific peripherals or registers, this shortcut will fail. Therefore, my recommendation remains: always compile dedicated `.o` and `.a` files for each individual ESP32 chip variant you intend to support.

To make testing as easy as possible, you can find all precompiled versions and example sketches ready to download in the accompanying GitHub repository.

---

## Transferring the Library to Other Computers

With the `library.properties` file and the `src/<architecture>` directory structure in place, your precompiled library is ready for clean, professional distribution:

1. Compress the entire `SECRETMaths` library folder into a ZIP archive (`SECRETMaths.zip`).
2. Transfer `SECRETMaths.zip` to the target computer.
3. In the Arduino IDE, go to **Sketch > Include Library > Add .ZIP Library...**, select `SECRETMaths.zip`, and import it.

That's it—the library is immediately ready to use!

---

## Conclusion

Creating a precompiled library using only standard Arduino tools requires a few manual steps, though by your second or third attempt, you'll realize it is much less intimidating than it first appears. Beyond protecting proprietary code, walking through this workflow is a great way to peek under the hood of the Arduino IDE and gain a deeper understanding of how source code is compiled, archived, and linked into a final executable binary.
