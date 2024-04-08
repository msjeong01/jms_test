from gi.repository import Gio, GLib

bus_type = Gio.BusType.SYSTEM
bus_name = 'org.bluez'
object_path = '/org/bluez/hci0'
prop_iface = 'org.freedesktop.DBus.Properties'
adapter_iface = 'org.bluez.Adapter1'


adapter_props_proxy = Gio.DBusProxy.new_for_bus_sync(
            bus_type=bus_type,
            flags=Gio.DBusProxyFlags.NONE,
            info=None,
            name=bus_name,
            object_path=object_path,
            interface_name=prop_iface,
            cancellable=None)

all_props = adapter_props_proxy.GetAll('(s)', adapter_iface)
print(all_props)

# powered = adapter_props_proxy.Get('(ss)', adapter_iface, 'Powered')
# print(powered)


# # then turn the adapter off
# adapter_props_proxy.Set('(ssv)',
#                         adapter_iface,
#                         'Powered',
#                         GLib.Variant.new_boolean(False)
#                         )
# powered = adapter_props_proxy.Get('(ss)', adapter_iface, 'Powered')
# print(powered)

# # and back on again
# adapter_props_proxy.Set('(ssv)',
#                         adapter_iface,
#                         'Powered',
#                         GLib.Variant.new_boolean(True)
#                         )
# powered = adapter_props_proxy.Get('(ss)', adapter_iface, 'Powered')
# print(powered)


'''
syscon@sys0124 ~/catkin_ws/src/test_pkg/scripts$ python3 dbus_find_all_property.py
{'Address': '00:A5:54:7B:8B:F2', 
'AddressType': 'public', 
'Name': 'sys0124', 
'Alias': 'sys0124', 
'Class': 3932428, 
'Powered': True, 
'Discoverable': True, 
'DiscoverableTimeout': 0, 
'Pairable': True, 
'PairableTimeout': 0, 
'Discovering': False, 
'UUIDs': ['00001133-0000-1000-8000-00805f9b34fb', '0000110e-0000-1000-8000-00805f9b34fb', '00001105-0000-1000-8000-00805f9b34fb', '00001132-0000-1000-8000-00805f9b34fb', '00001200-0000-1000-8000-00805f9b34fb', '00001104-0000-1000-8000-00805f9b34fb', '00001108-0000-1000-8000-00805f9b34fb', '00001112-0000-1000-8000-00805f9b34fb', '0000110c-0000-1000-8000-00805f9b34fb', '00001801-0000-1000-8000-00805f9b34fb', '0000112f-0000-1000-8000-00805f9b34fb', '0000110b-0000-1000-8000-00805f9b34fb', '0000180a-0000-1000-8000-00805f9b34fb', '00001800-0000-1000-8000-00805f9b34fb', '0000110a-0000-1000-8000-00805f9b34fb', '00001106-0000-1000-8000-00805f9b34fb'], 
'Modalias': 'usb:v1D6Bp0246d0542', 
'Roles': ['central', 'peripheral']}
# True
# False
# True
'''

'''
syscon@sys0122:~/catkin_ws/src/test$ python dbus_find_all_property.py 
{'AddressType': 'public', 
'Name': 'sys0122', 
'Powered': True, 
'Modalias': 'usb:v1D6Bp0246d0530', 
'DiscoverableTimeout': 0L, 
'Alias': 'sys0122', 
'PairableTimeout': 0L, 
'Discoverable': True, 
'Address': '98:AF:65:D6:73:C8', 
'Discovering': False, 
'Pairable': True, 
'Class': 786700L, 
'UUIDs': ['00001112-0000-1000-8000-00805f9b34fb', '00001801-0000-1000-8000-00805f9b34fb', '0000110e-0000-1000-8000-00805f9b34fb', '00001800-0000-1000-8000-00805f9b34fb', '00001200-0000-1000-8000-00805f9b34fb', '0000110c-0000-1000-8000-00805f9b34fb', '0000110a-0000-1000-8000-00805f9b34fb', '0000110b-0000-1000-8000-00805f9b34fb', '00001108-0000-1000-8000-00805f9b34fb']
'''

'''
syscon@sys0108:~/catkin_ws/src/test$ python dbus_find_all_property.py 
{'AddressType': 'public', 
'Name': 'sys0108', 
'Powered': True, 
'Modalias': 'usb:v1D6Bp0246d0530', 
'DiscoverableTimeout': 0L, 
'Alias': 'sys0108', 
'PairableTimeout': 0L, 
'Discoverable': True, 
'Address': '08:5B:D6:B9:DA:A6', 
'Discovering': False, 
'Pairable': True, 
'Class': 1835276L, 
'UUIDs': ['00001112-0000-1000-8000-00805f9b34fb', '00001801-0000-1000-8000-00805f9b34fb', '0000110e-0000-1000-8000-00805f9b34fb', '00001106-0000-1000-8000-00805f9b34fb', '00001800-0000-1000-8000-00805f9b34fb', '00001105-0000-1000-8000-00805f9b34fb', '00001200-0000-1000-8000-00805f9b34fb', '0000110c-0000-1000-8000-00805f9b34fb', '00001104-0000-1000-8000-00805f9b34fb', '0000110a-0000-1000-8000-00805f9b34fb', '0000110b-0000-1000-8000-00805f9b34fb', '00005005-0000-1000-8000-0002ee000001', '00001133-0000-1000-8000-00805f9b34fb', '0000112f-0000-1000-8000-00805f9b34fb', '00001132-0000-1000-8000-00805f9b34fb', '00001108-0000-1000-8000-00805f9b34fb']}
'''