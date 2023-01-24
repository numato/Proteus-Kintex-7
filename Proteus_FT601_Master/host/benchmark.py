from ft import *

import random
import time

from colorama import init, Fore, Style, Back

BLOCK_LEN = 32*1024

tx = str(bytearray([random.randrange(0, 256) for i in range(BLOCK_LEN)]))

times = []

ts_main = time.time()
for i in range(10240):
	ts = time.time()
	ltx = dev.writePipe(0x02, tx, len(tx))
	rx_raw = dev.readPipeEx(0x82, len(tx), raw=True)
	te = time.time()
	times.append(te-ts)
	rx = rx_raw['bytes']
	lrx = rx_raw[rx_raw.keys()[1]]

	assert lrx == ltx
	assert rx == tx

te_main = time.time()

init()

print("\nAverage transfer time per {0} KB data: {1}{2:.3f} milliseconds{3}".format((BLOCK_LEN*2)//1024, 
																																									Style.BRIGHT + Fore.WHITE, 
																																									float(str(sum(times)/1024.))*1000, 
																																									Style.RESET_ALL))

print("Transfer rate: {0}{1:.3f} MB/s{2}".format(Style.BRIGHT + Fore.WHITE, 
																									BLOCK_LEN*2/float(str(sum(times)/10240.))/1024./1024, 
																									Style.RESET_ALL))


print("\nOverall transfer time (including Python functions) for {0} MB data: {1}{2:.3f} seconds{3}".format(BLOCK_LEN*2*10240/1024/1024, 
																																																					Style.BRIGHT + Fore.WHITE, 
																																																					float(str(te_main - ts_main)),
																																																					Style.RESET_ALL))

print("Overall transfer rate: {0}{1:.3f} MB/s{2}\n".format(Style.BRIGHT + Fore.WHITE,
																														BLOCK_LEN*2*10240/1024/1024/float(str(te_main - ts_main)),
																														Style.RESET_ALL))

print("Test finished. Thank you!")
dev.close()