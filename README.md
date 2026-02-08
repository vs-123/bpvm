# BPVM

A dead-simple implementation of [BytePusher](https://esolangs.org/wiki/BytePusher) VM.

## SCREENSHOTS

![Colour Palette](https://github.com/vs-123/bpvm/blob/a9710ad99e8c53a5a71d8364e651d78817ffcfd2/screenshots/colour_palette_bpvm_vs123.png)
![Invert Loop Sine](https://github.com/vs-123/bpvm/blob/a9710ad99e8c53a5a71d8364e651d78817ffcfd2/screenshots/invert_loop_sine_bpvm_vs123.png)
![Keboard Test](https://github.com/vs-123/bpvm/blob/a9710ad99e8c53a5a71d8364e651d78817ffcfd2/screenshots/kb_test_bpvm_vs123.png)
![Nyan Cat](https://github.com/vs-123/bpvm/blob/a9710ad99e8c53a5a71d8364e651d78817ffcfd2/screenshots/nyan_cat_bpvm_vs123.png)
![Scrolling Logo](https://github.com/vs-123/bpvm/blob/a9710ad99e8c53a5a71d8364e651d78817ffcfd2/screenshots/scrolling_logo_bpvm_vs123.png)
![Sine Scroller](https://github.com/vs-123/bpvm/blob/a9710ad99e8c53a5a71d8364e651d78817ffcfd2/screenshots/sine_scroller_bpvm_vs123.png)

**[NOTE] The BytePusher programs shown in the screenshots were obtained from https://esolangs.org/wiki/BytePusher#Programs** 

## FEATURES

- **OISC architecture** -- Replicates accurate ByteByteJump with emulation
- **Cycle-Accurate Emulation** -- Executes exactly 65536 instructions per frame @ 3.93 MHz
- **Dead-Simple I/O Mapping** -- Direct memory-mapped handling for 16-key hex keypad bitmask and 8-bit signed mono audio bank
- **Linear Framebuffer** -- Graphics are rendered with a simple page swapping mechanism. 8-bit pixel indices are mapped to a pre-calculated 216 colour palette cube (plus it's web safe)
- **Memory Safe** -- The 16 MiB RAM block features an extra 8-byte buffer in case an edge-case instruction fetch occurs without the need for bounds checking
- **CLI Arguments & Flag Support** -- Supports case-insensitive command-line argument-parsing for ROM-loading & a few helpful flags including `--help`
- **<300 SLOC** -- This project was written in less than 300 lines of code including blank newlines

## BUILD INSTRUCTIONS

Prerequisites:
   - C99 compiler
   - CMake
   - A build-system supported by CMake such as GNU Make

Clone this repository and `cd` into it:
```
   %  git clone https://github.com/vs-123/bpvm.git
   %  cd bpvm
```

Create a `build/` directory and then use `cmake` to build:
```
   %  mkdir build/
   %  cd build/
   %  cmake ..
   %  cmake --build .
```

Run `bpvm`:
```
   %  ./bpvm --help
   [USAGE]
      *  ./bpvm <FILE>              --  EMULATE <FILE> AS A BYTEPUSHER FILE
      *  ./bpvm [--INFO | -I]       --  VIEW INFORMATION ABOUT THIS PROGRAM
      *  ./bpvm [--HELP | -H | -?]  --  PRINT THIS HELP MESSAGE AND EXIT
   
   [NOTE] FLAGS ARE CASE-INSENSITIVE

   %  ./bpvm test.bp   #  assuming you've acquired a BytePusher program
```

## LICENSE

This project is licensed under the GNU Affero General Public License version 3.0 or later.

**NO WARRANTY PROVIDED**

For full terms, see `LICENSE` file or visit **https://www.gnu.org/licenses/agpl-3.0.en.html**.
