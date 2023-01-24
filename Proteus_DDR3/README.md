Proteus DDR3 Project
Numato Lab
https://www.numato.com

This package contains source and other files necessary to build this project for Proteus Kintex 7 USB 3.1 Development board.
The project is built on Vivado design suite 2022.1 and Vitis 2022.1.

This project is created by selecting the part number of Proteus K7 board instead of selecting the board itself.

Open the project(.xpr) file and Open the Block Design. Then Generate the Bitstream along with binary file
(select it in Generate Bitstream settings). After the Bitstream is generated successfully, the .bit and .bin 
files will be created in the "Proteus_DDR3.runs-->impl_1" folder. Then export the generated bitstream
by selecting "Export Hardware" under "File--> Export". Include the bitstream while exporting the hardware. Then launch Vitis IDE.
After launching Vitis if the MemoryTest project is not opened, go to File--> Import and open the "Proteus_DDR3_Vitis" folder.
This will open the MemoryTest project. Now program the board from Vitis and observe the output in the Vitis terminal.
