connect
fpga sample_design_wrapper.bit
targets 3
set port [jtagterminal -socket]
puts "JTAG Terminal Started on Port $port"
exec putty -raw -load JTAGTerminal localhost $port &
dow Helloworld.elf
con