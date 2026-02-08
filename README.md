# BPVM

A dead-simple implementation of [BytePusher](https://esolangs.org/wiki/BytePusher) VM.

## FEATURES

- **OISC architecture** -- Replicates accurate ByteByteJump with emulation
- **Cycle-Accurate Emulation** -- Executes exactly 65536 instructions per frame @ 3.93 MHz
- **Dead-Simple I/O Mapping** -- Direct memory-mapped handling for 16-key hex keypad bitmask and 8-bit signed mono audio bank
- **Linear Framebuffer** -- Graphics are rendered with a simple page swapping mechanism. 8-bit pixel indices are mapped to a pre-calculated 216 colour palette cube (plus it's web safe)
- **Memory Safe** -- The 16 MiB RAM block features an extra 8-byte buffer in case an edge-case instruction fetch occurs without the need for bounds checking
- **CLI Arguments & Flag Support** -- Supports case-insensitive command-line argument-parsing for ROM-loading & a few helpful flags including `--help`
- **<300 SLOC** -- This project was written in less than 300 lines of code including blank newlines


## LICENSE

This project is licensed under the GNU Affero General Public License version 3.0 or later.

**NO WARRANTY PROVIDED**

For full terms, see `LICENSE` file or visit **https://www.gnu.org/licenses/agpl-3.0.en.html**.
