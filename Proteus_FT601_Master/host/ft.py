from __future__ import print_function
import ftd3xx

print ("Number of Devices: {}".format(ftd3xx.listDevices()))
ftd3xx.createDeviceInfoList()
print(ftd3xx.getDeviceInfoDetail(0))

dev = ftd3xx.create(0)
print(dev.getVIDPID())

desc = dev.getDeviceDescriptor()
confdesc = dev.getConfigurationDescriptor()
print ("Number of Interfaces: {}".format(confdesc.bNumInterfaces))

intdesc = dev.getInterfaceDescriptor(1)
print ("Number of Endpoints: {}".format(intdesc.bNumEndpoints))

p1inf = dev.getPipeInformation(1, 0)
p2inf = dev.getPipeInformation(1, 1)
print ("Enpoint : p1inf, Address: {} ".format(hex(p1inf.PipeId)))
print ("Enpoint : p2inf, Address: {} ".format(hex(p2inf.PipeId)))

dev.setPipeTimeout(p1inf.PipeId, 5000)
dev.setPipeTimeout(p2inf.PipeId, 5000)

# dev.writePipe(0x02, b'\xca\xfe\xba\xbe\xca\xfe\xba\xbe\xca\xfe\xba\xbe', 12)
# res = dev.readPipeEx(0x82, 4, raw=True)
# print(res)
