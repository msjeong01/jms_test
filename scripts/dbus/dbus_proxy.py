from gi.repository import Gio

bus_type = Gio.BusType.SYSTEM
bus_name = 'org.bluez'
object_path = '/org/bluez/hci0'
interface = 'org.bluez.Adapter1'

adapter_proxy = Gio.DBusProxy.new_for_bus_sync(
            bus_type=bus_type,
            flags=Gio.DBusProxyFlags.NONE,
            info=None,
            name=bus_name,
            object_path=object_path,
            interface_name=interface,
            cancellable=None)

disco_filters = adapter_proxy.GetDiscoveryFilters()
print(disco_filters)

'''
syscon@sys0124 ~/catkin_ws/src/test_pkg/scripts$ python3 dbus_proxy.py 
['UUIDs', 'RSSI', 'Pathloss', 'Transport', 'DuplicateData', 'Discoverable', 'Pattern']
'''