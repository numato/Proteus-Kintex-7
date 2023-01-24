# Usage with Vitis IDE:
# In Vitis IDE create a Single Application Debug launch configuration,
# change the debug type to 'Attach to running target' and provide this 
# tcl script in 'Execute Script' option.
# Path of this script: G:\Sample_codes\proteus\Proteus_Helloworld\Proteus_Helloworld_Vitis\Helloworld_system\_ide\scripts\systemdebugger_helloworld_system_standalone.tcl
# 
# 
# Usage with xsct:
# To debug using xsct, launch xsct and run below command
# source G:\Sample_codes\proteus\Proteus_Helloworld\Proteus_Helloworld_Vitis\Helloworld_system\_ide\scripts\systemdebugger_helloworld_system_standalone.tcl
# 
connect -url tcp:127.0.0.1:3121
targets -set -filter {jtag_cable_name =~ "Platform Cable USB 00000000000000" && level==0 && jtag_device_ctx=="jsn-DLC9LP-00000000000000-0364c093-0"}
fpga -file G:/Sample_codes/proteus/Proteus_Helloworld/Proteus_Helloworld_Vitis/Helloworld/_ide/bitstream/download.bit
targets -set -nocase -filter {name =~ "*microblaze*#0" && bscan=="USER2" }
loadhw -hw G:/Sample_codes/proteus/Proteus_Helloworld/Proteus_Helloworld_Vitis/sample_design_wrapper/export/sample_design_wrapper/hw/sample_design_wrapper.xsa -regs
configparams mdm-detect-bscan-mask 2
targets -set -nocase -filter {name =~ "*microblaze*#0" && bscan=="USER2" }
rst -system
after 3000
targets -set -nocase -filter {name =~ "*microblaze*#0" && bscan=="USER2" }
dow G:/Sample_codes/proteus/Proteus_Helloworld/Proteus_Helloworld_Vitis/Helloworld/Debug/Helloworld.elf
targets -set -nocase -filter {name =~ "*microblaze*#0" && bscan=="USER2" }
con
