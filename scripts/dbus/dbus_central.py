from time import sleep
from datetime import datetime
from gi.repository import Gio, GLib

DEVICE_ADDR = '08:5B:D6:B9:DA:A6'
UART_TX = '6e400002-b5a3-f393-e0a9-e50e24dcca9e'
UART_RX = '6e400003-b5a3-f393-e0a9-e50e24dcca9e'

# DBus Information
bus_type = Gio.BusType.SYSTEM
BLUEZ_NAME = 'org.bluez'
ADAPTER_PATH = '/org/bluez/hci0'
device_path = f"{ADAPTER_PATH}/dev_{DEVICE_ADDR.replace(':', '_')}"
MNGR_IFACE = 'org.freedesktop.DBus.ObjectManager'
PROP_IFACE = 'org.freedesktop.DBus.Properties'
DEVICE_IFACE = 'org.bluez.Device1'
BLE_CHRC_IFACE = 'org.bluez.GattCharacteristic1'


def bluez_proxy(object_path, interface):
    return Gio.DBusProxy.new_for_bus_sync(
        bus_type=bus_type,
        flags=Gio.DBusProxyFlags.NONE,
        info=None,
        name=BLUEZ_NAME,
        object_path=object_path,
        interface_name=interface,
        cancellable=None)


# setup dbus
mngr = bluez_proxy('/', MNGR_IFACE)
device = bluez_proxy(device_path, DEVICE_IFACE)
dev_props = bluez_proxy(device_path, PROP_IFACE)

# Connect to device
device.Connect()

# Wait for the cache of services on the remote device to be updated
while not dev_props.Get('(ss)', DEVICE_IFACE, 'ServicesResolved'):
    print(f"Services Resolved: {dev_props.Get('(ss)', DEVICE_IFACE, 'ServicesResolved')}")
    sleep(0.5)
print(f"Services Resolved: {dev_props.Get('(ss)', DEVICE_IFACE, 'ServicesResolved')}")


def get_characteristic_path(dev_path, uuid):
    """Look up DBus path for characteristic UUID"""
    mng_objs = mngr.GetManagedObjects()
    for path in mng_objs:
        chr_uuid = mng_objs[path].get(BLE_CHRC_IFACE, {}).get('UUID')
        if path.startswith(dev_path) and chr_uuid == uuid.casefold():
            return path


# Characteristic DBus information
tx_path = get_characteristic_path(device_path, UART_TX)
tx_proxy = bluez_proxy(tx_path, BLE_CHRC_IFACE)
rx_path = get_characteristic_path(device_path, UART_RX)
rx_proxy = bluez_proxy(rx_path, BLE_CHRC_IFACE)

rx_proxy.WriteValue('(aya{sv})', b'Test#', {})


def tx_handler(proxy: Gio.DBusProxy,
                changed_props: GLib.Variant,
                invalidated_props: list) -> None:
    """Notify event handler for messages received from UART Service"""
    props = changed_props.unpack()
    value = props.get('Value')
    if value:
        text = bytes(value).strip()
        print(f'Echo: {text}')


def send_time():
    """Send time over UART. `#` is the message termination character"""
    now = datetime.now().strftime('%M:%S#')
    print(f'Send: {now.encode()}')
    rx_proxy.WriteValue('(aya{sv})', now.encode(), {})
    return True


# Enable eventloop for notifications
mainloop = GLib.MainLoop()
GLib.timeout_add_seconds(interval=9, function=send_time)
tx_proxy.connect('g-properties-changed', tx_handler)
tx_proxy.StartNotify()

try:
    mainloop.run()
except KeyboardInterrupt:
    mainloop.quit()
    tx_proxy.StopNotify()
    device.Disconnect()