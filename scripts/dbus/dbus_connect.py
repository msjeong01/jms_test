import dbus
import time

DEVICE_ADDR = '08:5B:D6:B9:DA:A6'
BLUEZ_SERVICE_NAME = 'org.bluez'
DEVICE_PATH = f'/org/bluez/hci0/dev_{DEVICE_ADDR.replace(":", "_")}'
# DEVICE_PATH = '/org/bluez/hci0/dev_{}'.format(DEVICE_ADDR.replace(":", "_"))
DEVICE_IFACE = 'org.bluez.Device1'

bus = dbus.SystemBus()

def connect_device():
    device = dbus.Interface(bus.get_object(BLUEZ_SERVICE_NAME, DEVICE_PATH), DEVICE_IFACE)
    device.Connect()
    print("Conected time: ", time.time())
    print(f"Connected to {DEVICE_ADDR}")
    # print("Connected to %s" % DEVICE_ADDR)

def disconnect_device():
    device = dbus.Interface(bus.get_object(BLUEZ_SERVICE_NAME, DEVICE_PATH), DEVICE_IFACE)
    device.Disconnect()
    print(f"Disconnected from {DEVICE_ADDR}")

if __name__ == '__main__':
    try:
        print("Start time: ", time.time())
        connect_device()
        time.sleep(5)
    finally:
        disconnect_device()


'''
syscon@sys0124 ~/catkin_ws/src/test_pkg/scripts/dbus$ python3 test.py 
Connected to 08:5B:D6:B9:DA:A6
Disconnected from 08:5B:D6:B9:DA:A6
'''
