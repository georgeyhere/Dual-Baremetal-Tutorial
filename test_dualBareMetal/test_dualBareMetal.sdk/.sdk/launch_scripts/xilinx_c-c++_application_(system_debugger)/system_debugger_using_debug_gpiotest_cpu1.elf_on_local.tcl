connect -url tcp:127.0.0.1:3121
source /home/george/Documents/ngcp/repos/NGCP_2021-2022/test_dualBareMetal/test_dualBareMetal.sdk/dualBareMetal_test_wrapper_hw_platform_0/ps7_init.tcl
targets -set -filter {jtag_cable_name =~ "Digilent Zybo Z7 210351B04BB2A" && level==0} -index 1
fpga -file /home/george/Documents/ngcp/repos/NGCP_2021-2022/test_dualBareMetal/test_dualBareMetal.sdk/dualBareMetal_test_wrapper_hw_platform_0/dualBareMetal_test_wrapper.bit
targets -set -nocase -filter {name =~"APU*" && jtag_cable_name =~ "Digilent Zybo Z7 210351B04BB2A"} -index 0
loadhw -hw /home/george/Documents/ngcp/repos/NGCP_2021-2022/test_dualBareMetal/test_dualBareMetal.sdk/dualBareMetal_test_wrapper_hw_platform_0/system.hdf -mem-ranges [list {0x40000000 0xbfffffff}]
configparams force-mem-access 1
targets -set -nocase -filter {name =~"APU*" && jtag_cable_name =~ "Digilent Zybo Z7 210351B04BB2A"} -index 0
stop
ps7_init
ps7_post_config
targets -set -nocase -filter {name =~ "ARM*#0" && jtag_cable_name =~ "Digilent Zybo Z7 210351B04BB2A"} -index 0
rst -processor
targets -set -nocase -filter {name =~ "ARM*#0" && jtag_cable_name =~ "Digilent Zybo Z7 210351B04BB2A"} -index 0
dow /home/george/Documents/ngcp/repos/NGCP_2021-2022/test_dualBareMetal/test_dualBareMetal.sdk/gpioTest_cpu0/Debug/gpioTest_cpu0.elf
targets -set -nocase -filter {name =~ "ARM*#1" && jtag_cable_name =~ "Digilent Zybo Z7 210351B04BB2A"} -index 0
dow /home/george/Documents/ngcp/repos/NGCP_2021-2022/test_dualBareMetal/test_dualBareMetal.sdk/gpioTest_cpu1/Debug/gpioTest_cpu1.elf
configparams force-mem-access 0
targets -set -nocase -filter {name =~ "ARM*#0" && jtag_cable_name =~ "Digilent Zybo Z7 210351B04BB2A"} -index 0
con
targets -set -nocase -filter {name =~ "ARM*#1" && jtag_cable_name =~ "Digilent Zybo Z7 210351B04BB2A"} -index 0
con
