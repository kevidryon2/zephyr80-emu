# Zephyr80-Emu

## Memory Map

| Name 	| Banking 	| Size 	| Description 	| Note 	|
|---	|---	|---	|---	|---	|
| BIOS ROM 	| - 	| 8K 	| The ROM of the BIOS. 	|  	|
| Work RAM 	| WRAM 0 	| 16K 	| General RAM 	|  	|
| Work RAM 	| WRAM 0-255 	| 16K 	| Banked RAM 	| Acts as a mirror to WRAM 0 if using WRAM bank 0 	|
| FLASH A	| FLASH 0-255 	| 8K 	| Banked Flash memory for permanent storage* |  	|
| FLASH B	| FLASH 0-255 	| 8K 	| Banked Flash memory for permanent storage* |  	|
| Unused 	| - 	| 7.75K 	|  	|  	|
| I/O 	| - 	| 256B 	| I/O registers. 	|  	|

*: The 2 Flash memory locations mapped on the CPU's address bus point to the same Flash memory, but at different addresses.

### OAM Map
| Name 	| Banking 	| Size 	| Description 	| Note 	|
|---	|---	|---	|---	|---	|
| Sprite #1 X Position 	| - 	| 1B 	|  	|  	|
| Sprite #1 Y Position 	| - 	| 1B 	|  	|  	|
| Sprite #1 Tile 	| - 	| 1B 	|  	|  	|
| Sprite #1 Palette 	| - 	| 1B 	|  	|  	|
| Sprite #1 Flags 	|  	| 1B 	| Bit 0: Horizontal Flip<br>Bit 1: Vertical Flip<br>Bit 2: X Position's 9th bit<br>Bit 3: Y Position's 9th bit 	|  	|
| Sprite #1 Tileset bank 	| - 	| 1B 	|  	|  	|
| Unused 	|  	| 2B 	|  	|  	|
| ... 	|  	|  	|  	|  	|
| Sprite #2048 X Position 	| - 	| 1B 	|  	|  	|
| Sprite #2048 Y Position 	| - 	| 1B 	|  	|  	|
| Sprite #2048 Tile 	| - 	| 1B 	|  	|  	|
| Sprite #2048 Palette 	| - 	| 1B 	|  	|  	|
| Sprite #2048 Flags 	|  	| 1B 	|  	|  	|
| Sprite #2048 Tileset bank 	| - 	| 1B 	|  	|  	|
| Unused 	|  	| 2B 	|  	|  	|

### VRAM Map
| Name 	| Banking 	| Size 	| Description 	| Note 	|
|---	|---	|---	|---	|---	|
| Palettes 	| - 	| 8K 	| The Zephyr80 uses 16-bit colors and 256 palettes, with 16 colors each. 	|  	|
| Blending 	| - 	| 1B 	| The first 4 bits specify the background layers to blended, while the last ones specify the blending modes activated. 	|  	|
| Tileset #0 	| Tileset 0-15 	| 8K 	| In the Zephyr80 each tileset contains 256 4bpp 8x8 tiles, each being 32 bytes. 	|  	|
| Tilemap Tiles 	| Tilemap Tiles 0-15 	| 4K 	| A 64x64 grid of tile indexes. 	|  	|
| Tilemap Palettes 	| Tilemap Palettes 0-15 	| 2K 	| A 64x64 grid of palette indexes. 	|  	|
| Unused 	| - 	| 2K 	|  	|  	|

#### Blending Modes
- Mode 0: Add Red and Green channels
- Mode 1: Add Blue channels
- Mode 2: Subtract Green and Blue channels
- Mode 3: Subtract Red channels

### I/O Map
| Name 	| Banking 	| Size 	| Description 	| Note 	|
|---	|---	|---	|---	|---	|
| Video RAM 	| - 	| 24K 	| The memory used by the GPU to draw the frame 	|  	|
| Audio RAM 	| - 	| 16K 	| The memory used by the APU to play audio 	| ARAM is currently unused becuase i have no idea how to implement audio. 	|
| OAM | OAM 0-7 | 16K | The memory that contains objects on the screen | |
| Unused 	| - 	| 8K 	|  	|  	|

#### Banking

##### $FF00 (RAMBANK)
Controls the memory bank used by WRAM.

##### $FF01 (FLASHBANKA)
Controls the memory bank used by the FLASH memory region A.

##### $FF02 (FLASHBANKB)
Controls the memory bank used by the FLASH memory region B.

##### $FF03 (VTILESBANK)
Controls the memory bank used by the Tileset in VRAM.

##### $FF04 (VMAPBANK)
Controls the memory bank used by the Tilemap Tiles (first 4 bits) and Tilemap Palettes (last 4 bits) in VRAM.

##### $FF05 (OAMBANK)
Controls the memory bank used by OAM.

#### DMA

DMA starts at $FF06.

| Name 	| Offset 	| Size 	|
|---	|---	|---	|
| DMA Channel #1 Source Addr. 	| 0 	| 2 	|
| DMA Channel #1 Destination Addr.	| 2 	| 2 	|
| DMA Channel #1 Data 	| 4 	| 1 	|
| DMA Channel #1 Flags 	| 5 	| 1 	|
| DMA Channel #2 Source Addr.	| 6 	| 2 	|
| DMA Channel #2 Destination Addr.	| 8 	| 2 	|
| DMA Channel #2 Data 	| 10 	| 1 	|
| DMA Channel #2 Flags 	| 11 	| 1 	|
| ... 	|  	|  	|
| DMA Channel #16 Source Addr.	| 90 	| 2 	|
| DMA Channel #16 Destination Addr.	| 92 	| 2 	|
| DMA Channel #16 Data 	| 94 	| 1 	|
| DMA Channel #16 Flags 	| 95 	| 1 	|

##### DMA Flags

- Bit 0: Auto-increment Source address
- Bit 1: Auto-increment Destination address
- Bit 2: Auto DMA (When this bit is set, the CPU is temporainly paused to perform DMA).
- Bit 3: Copy from Source Address (LOW) / Copy from Data (HIGH)

#### Other

##### $FF66 (VMODE)
The graphics mode used. I originally wanted to implement this but it turned out that it was too complicated so i left it out. This byte is currently unused.

##### $FF67-$FF68 (VPOSX)
The X position of the "scanning beam" on the screen.

##### $FF69-$FF6A (VPOSY)
The Y position of the "scanning beam" on the screen.

#### Scanline interrupts

A scanline interrupt interrupts the CPU when the "scanning beam" reaches the desired scanline. The highest bit of each scanline interrupt determines wheteaver it is activated or not. When it is triggered, it resets to 0 if the second highest bit is set.

##### $FF6B-$FF6C (VINT0)
The 1st scanline interrupt. It jumps to $40.

##### $FF6D-$FF6E (VINT1)
The 2nd scanline interrupt. It jumps to $48.

##### $FF6F-$FF70 (VINT2)
The 3rd scanline interrupt. It jumps to $50.

##### $FF71-$FF72 (VINT3)
The 4th scanline interrupt. It jumps to $58.

##### $FF73-$FF74 (VINT4)
The 5th scanline interrupt. It jumps to $60.

##### $FF75-$FF76 (VINT5)
The 6th scanline interrupt. It jumps to $68.

##### $FF77-$FF78 (VINT6)
The 7th scanline interrupt. It jumps to $70.

##### $FF79-$FF7A (VINT7)
The 8th scanline interrupt. It jumps to $78.
