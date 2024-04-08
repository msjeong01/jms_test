from digi.xbee.devices import XBeeDevice, RemoteXBeeDevice # generic object, protocol-independent
# from digi.xbee.devices import DigiMeshDevice, RemoteDigimeshDevice  # digimesh protocol, protocol-specific
from digi.xbee.exception import FirmwareUpdateException, OperationNotSupportedException, XBeeException
from digi.xbee.models.address import XBee64BitAddress
from digi.xbee.models.status import PowerLevel
from digi.xbee.util import utils
import digi.xbee.firmware

## serial port, baud rate
PORT = "/dev/tty"  # replace the /dev/tty port with the one your sender XBee device is connected to
BAUD_RATE = 9600   # 115200, 57600, 38400, 19200, 9600

## parameters
PARAM_CHANNEL = "CH"
PARAM_NODE_ID = "NI"
PARAM_PAN_ID = "ID"
PARAM_DEST_ADDRESS_H = "DH"  # Set or read the upper 32 bits of the 64-bit destination address
PARAM_DEST_ADDRESS_L = "DL"
PARAM_DESTINATION_ADDR = XBee64BitAddress.BROADCAST_ADDRESS
PARAM_POWER_LEVEL = PowerLevel.LEVEL_MEDIUM

PARAM_VALUE_NODE_ID = "example"
# PARAM_VALUE_PAN_ID = bytearray(b'\x01\x23') 
PARAM_VALUE_PAN_ID = utils.hex_string_to_bytes("1234")
PARAM_VALUE_DEST_ADDRESS_H = utils.hex_string_to_bytes("00")
PARAM_VALUE_DEST_ADDRESS_L = utils.hex_string_to_bytes("FFFF")

## firmware
XML_FIRMWARE_FILE = "<path_to_file>"
XBEE_FIRMWARE_FILE = None  # Optional
BOOTLOADER_FIRMWARE_FILE = None  # Optional


def main():
    # Instantiate a local XBee object
    device = XBeeDevice(PORT, BAUD_RATE)

    try:
        # Open the device connection
        device.open()
        # Reset the module.
        # device.reset()

        # Update the XBee firmware.
        device.update_firmware(XML_FIRMWARE_FILE,
                               xbee_firmware_file=XBEE_FIRMWARE_FILE,
                               bootloader_firmware_file=BOOTLOADER_FIRMWARE_FILE,
                               progress_callback=progress_callback,)

        # Set parameters.
        device.set_parameter(PARAM_NODE_ID, bytearray(PARAM_VALUE_NODE_ID, 'utf8'))
        device.set_parameter(PARAM_PAN_ID, PARAM_VALUE_PAN_ID)
        device.set_parameter(PARAM_DEST_ADDRESS_H, PARAM_VALUE_DEST_ADDRESS_H)
        device.set_parameter(PARAM_DEST_ADDRESS_L, PARAM_VALUE_DEST_ADDRESS_L)

        # Get the network.
        xnet = device.get_network()

        # Configure the discovery options.
        xnet.set_deep_discovery_options(deep_mode=NeighborDiscoveryMode.CASCADE,
                                        del_not_discovered_nodes_in_last_scan=True)

        # Configure the discovery timeout, in SECONDS.
        xnet.set_deep_discovery_timeout(node_timeout=30, time_bw_requests=10,
                                        time_bw_scans=20)

        # Send broadcast data.
        device.send_data_broadcast("Hello XBees!")

        # Read data.
        xbee_message = device.read_data()

        # Get the message information
        remote = xbee_message.remote_device
        data = xbee_message.data
        is_broadcast = xbee_message.is_broadcast
        timestamp = xbee_message.timestamp


        # Read the cached values.
        device.read_device_info()

        ## cached parameters
        # Get the 64-bit address of the device.
        addr_64 = device.get_64bit_addr()
        addr_16 = device.get_16bit_addr()
        # Get the node identifier of the device.
        node_id = device.get_node_id()
        # Get the hardware version of the device.
        hardware_version = device.get_hardware_version()
        # Get the firmware version of the device.
        firmware_version = device.get_firmware_version()


        ## non-cached parameters
        # Set the destination address of the device.
        device.set_dest_address(PARAM_DESTINATION_ADDR)
        # Read the operating PAN ID of the device.
        dest_addr = device.get_dest_address()

        # Read the operating PAN ID of the device.
        device.set_pan_id(PARAM_VALUE_PAN_ID)
        pan_id = device.get_pan_id()

        # Configure and read non-cached parameters.
        device.set_power_level(PARAM_POWER_LEVEL)
        # Read the output power level.
        p_level = device.get_power_level()

        # Get the protocol of the device.
        protocol = device.get_protocol()


        #####
        # Instantiate a remote XBee
        remote = RemoteXBeeDevice(device, XBee64BitAddress.from_hex_string("0013A20012345678"))

        # Read the device information of the remote XBee.
        remote.read_device_info()


    except XBeeException as e:
        print("ERROR: %s" % str(e))
        return

    finally:
        if device is not None and device.is_open():
            device.close()


if __name__=="__main":
    main()