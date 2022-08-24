# Zephyr80-Emu
I designed this Z80 emulator around ZephyrOS, my custom OS for the Z80.

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

### VRAM Map
| Name 	| Banking 	| Size 	| Description 	| Note 	|
|---	|---	|---	|---	|---	|
| Palettes 	| - 	| 8K 	| The Zephyr80 uses 16-bit colors and 16 palettes, with 16 colors each. 	|  	|
| Blending 	| - 	| 1B 	| The first 4 bits specify the background layers to blended, while the last ones specify the blending modes activated (Bit 0 = Add/Sub-R, Bit 1 = Add/Sub-G, Bit 2 = Add/Sub-B, Bit 3 = Add/Subtract) 	|  	|
| Tileset #0 	| Tileset 0-15 	| 8K 	| In the Zephyr80 each tileset contains 256 4bpp 8x8 tiles, each being 32 bytes. 	|  	|
| Tilemap Tiles 	| Tilemap Tiles 0-15 	| 4K 	| A 64x64 grid of tile indexes. 	|  	|
| Tilemap Palettes 	| Tilemap Palettes 0-15 	| 2K 	| A 64x64 grid of palette indexes. 	|  	|
| Unused 	| - 	| 2K 	|  	|  	|

### I/O Map
| Name 	| Banking 	| Size 	| Description 	| Note 	|
|---	|---	|---	|---	|---	|
| Video RAM 	| - 	| 24K 	| The memory used by the GPU to draw the frame 	|  	|
| Audio RAM 	| - 	| 16K 	| The memory used by the APU to play audio 	| ARAM is currently unused becuase i have no idea how to implement audio. 	|
| Unused 	| - 	| 24K 	|  	|  	|

#### Banking

##### $FF00 (RAMBANK)
Controls the memory bank used by WRAM.

##### $FF01 (FLASHBANKA)
Controls the memory bank used by the FLASH memory region A.

##### $FF02 (FLASHBANKB)
Controls the memory bank used by the FLASH memory region B.

##### $FF03 (VTILESBANK)
Controls the memory bank used by the Tileset in VRAM.

##### $FF04 (VMAPTILES)
Controls the memory bank used by the Tilemap Tiles in VRAM.

##### $FF05 (VMAPPALS)
Controls the memory bank used by the Tilemap Palettes in VRAM.

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
