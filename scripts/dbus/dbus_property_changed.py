from gi.repository import Gio, GLib

bus_type = Gio.BusType.SYSTEM
bus_name = 'org.bluez'
object_path = '/org/bluez/hci0'
adapter_iface = 'org.bluez.Adapter1'


def adapter_props_handler(proxy: Gio.DBusProxy,
                          changed_props: GLib.Variant,
                          invalidated_props: list) -> None:
    props = changed_props.unpack()
    powered = props.get('Powered')
    if powered is True:
        print('Adapter is now powered on')
    elif powered is False:
        print('Adapter is now powered off')


adapter_proxy = Gio.DBusProxy.new_for_bus_sync(
            bus_type=bus_type,
            flags=Gio.DBusProxyFlags.NONE,
            info=None,
            name=bus_name,
            object_path=object_path,
            interface_name=adapter_iface,
            cancellable=None)

adapter_proxy.connect('g-properties-changed', adapter_props_handler)
mainloop = GLib.MainLoop()

try:
    mainloop.run()
except KeyboardInterrupt:
    mainloop.quit()


'''
# terminal 1
syscon@sys0124 ~$ bluetoothctl 
Agent registered
[bluetooth]# power off
Changing power off succeeded
[CHG] Controller 00:A5:54:7B:8B:F2 Powered: no
[CHG] Controller 00:A5:54:7B:8B:F2 Discovering: no
[CHG] Controller 00:A5:54:7B:8B:F2 Class: 0x00000000
[bluetooth]# power on
[CHG] Controller 00:A5:54:7B:8B:F2 Class: 0x003c010c
Changing power on succeeded
[CHG] Controller 00:A5:54:7B:8B:F2 Powered: yes


# terminal 2
syscon@sys0124 ~/catkin_ws/src/test_pkg/scripts/dbus$ python3 dbus_property_changed.py 
Adapter is now powered off
Adapter is now powered on
'''