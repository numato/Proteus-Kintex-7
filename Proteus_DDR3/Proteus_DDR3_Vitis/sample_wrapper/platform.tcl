# 
# Usage: To re-create this platform project launch xsct with below options.
# xsct C:\Users\Milna\Downloads\Proteus_DDR3\Proteus_DDR3_Vitis\sample_wrapper\platform.tcl
# 
# OR launch xsct and run below command.
# source C:\Users\Milna\Downloads\Proteus_DDR3\Proteus_DDR3_Vitis\sample_wrapper\platform.tcl
# 
# To create the platform in a different location, modify the -out option of "platform create" command.
# -out option specifies the output directory of the platform project.

platform create -name {sample_wrapper}\
-hw {C:\Users\Milna\Downloads\Proteus_DDR3\sample_wrapper.xsa}\
-out {C:/Users/Milna/Downloads/Proteus_DDR3/Proteus_DDR3_Vitis}

platform write
domain create -name {standalone_microblaze_0} -display-name {standalone_microblaze_0} -os {standalone} -proc {microblaze_0} -runtime {cpp} -arch {32-bit} -support-app {memory_tests}
platform generate -domains 
platform active {sample_wrapper}
platform generate -quick
platform generate
