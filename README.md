# Zephyr80-Emu
I designed this Z80 emulator around my dream computer, that is the one 

## Features
### System
  - Using a Z80 which runs at 12.5875MHz.
  - SRC and DEST instead of HL and IX/IY
  - F is not the flags register, but an actual register.
  - Register pairs changed from BC,DE,HL to AB,CD,EF,SRC,DEST
  - Built-in multiplication/division inctructions
  - Built-in double-speed mode (25.175MHz)
  - RST $40, $48, $50, $58, $60, $68, $70, $78, $80
  - Block copy instruction (Copy AB bytes from SRC to DEST)
  - Exception handlers:
    - Illegal instruction = RST $80,
    - Arithmetic exception = RST $88,
    - Invalid memory bank = RST $90
  - Register swapping
  - Pointer calling (Call address that has the value pointed to by SRC)
  - Pointer offsetting (eg. Call address that has the value pointed to by SRC+A), (eg, Set A to the value pointed to by SRC+A)


- Graphics:
  - 16 palettes, each with 16 16-bit colors
  - 16 switchable tilesets, each 256 tiles
  - 16 switchable tilemaps, each 64x64 tiles big
  - A video resolution of VGA 640x480 (40x30 tiles)
  - 128 sprites

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
| Sprite #1 Palette and Tileset bank 	| - 	| 1B 	| The first 4 bits are the palette the sprite uses, while the last ones represent which Tileset bank they use. 	|  	|
| Sprite #1 Flags 	|  	| 1B 	| Bit 0: Horizontal Flip<br>Bit 1: Vertical Flip<br>Bit 2: X Position's 9th bit<br>Bit 3: Y Position's 9th bit 	|  	|
| Reserved 	| - 	| 3B 	| Unused 	|  	|
| Sprite #2 X Position 	| - 	| 1B 	|  	|  	|
| Sprite #2 Y Position 	| - 	| 1B 	|  	|  	|
| Sprite #2 Tile 	| - 	| 1B 	|  	|  	|
| Sprite #2 Palette and Tileset bank 	| - 	| 1B 	| The first 4 bits are the palette the sprite uses, while the last ones represent which Tileset bank they use. 	|  	|
| Sprite #2 Flags 	|  	| 1B 	| Bit 0: Horizontal Flip<br>Bit 1: Vertical Flip<br>Bit 2: X Position's 9th bit<br>Bit 3: Y Position's 9th bit 	|  	|
| Reserved 	| - 	| 3B 	| Unused 	|  	|
| ... 	|  	|  	|  	|  	|
| Sprite #2048 X Position 	| - 	| 1B 	|  	|  	|
| Sprite #2048 Y Position 	| - 	| 1B 	|  	|  	|
| Sprite #2048 Tile 	| - 	| 1B 	|  	|  	|
| Sprite #2048 Palette and Tileset bank 	| - 	| 1B 	| The first 4 bits are the palette the sprite uses, while the last ones represent which Tileset bank they use. 	|  	|
| Sprite #2048 Flags 	|  	| 1B 	| Bit 0: Horizontal Flip<br>Bit 1: Vertical Flip<br>Bit 2: X Position's 9th bit<br>Bit 3: Y Position's 9th bit 	|  	|
| Reserved 	| - 	| 3B 	| Unused 	|  	|

### VRAM Map
| Name 	| Banking 	| Size 	| Description 	| Note 	|
|---	|---	|---	|---	|---	|
| Palettes 	| - 	| 8K 	| The Zephyr80 uses 16-bit colors and 16 palettes, with 16 colors each. 	|  	|
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

##### $FF06 (OAMBANK)
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
