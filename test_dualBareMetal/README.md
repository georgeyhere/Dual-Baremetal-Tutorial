# Dual Baremetal Application Testing

__Hello World Test__
- run a baremetal application simultaneously on each core
- use shared memory to pass data from CPU0 to CPU1
- works on hardware

__GPIO Test__


When both cores need to write to shared memory, they need
to take turns to avoid data corruption. This is done by
assigning an address in shared memory that is used as
a flag. When the flag is 0, it is CPU0's turn to write.
When the flag is 1, it is CPU1's turn. Only CPU0 can set
the flag to 1 and vice versa.

- run an application on each core that reads two switches
and writes their values to shared memory
- each core sets two LEDs based on value of switches
- works on hardware
