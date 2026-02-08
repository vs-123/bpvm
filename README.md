# BPVM

A dead-simple implementation of [BytePusher](https://esolangs.org/wiki/BytePusher) VM.

## FEATURES

- **OISC architecture** emulaton
- **Cycle Accurate ByeByeJump CPU** -- Executes exactly 65536 instructions per frame @ 3.93 MHZ
- **Simple I/O Mapping** -- Direct memory mapped handling for 16 key hex keypad bitmask and 8 signed mono audi bank
- **Linear Framebuffer** -- Graphics are rendered with a simple page swapping mechanism. 8 bit pixel indices are mapped to a pre calculated 216 colour palette cube (plus it's web safe).
- **Memory Safety** -- The 16 MiB RAM block has an 8 byte extra buffer in case an edge case instruction fetch occurs without the need for bounds checking.
- **CLI Arguments & Flag Support** -- Supports case-insensitive coomand line argument parsing for ROM loading & a few helpful flags including `--help`


## LICENSE

This project is licensed under the GNU Affero General Public License version 3.0 or later.

**NO WARRANTY PROVIDED**

For full terms, see `LICENSE` file or visit **https://www.gnu.org/licenses/agpl-3.0.en.html**.
