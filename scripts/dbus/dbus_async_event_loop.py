from datetime import datetime
from gi.repository import GLib


def show_time():
    now = datetime.now().strftime('%M:%S')
    print(f'Now: {now}')
    return True


GLib.timeout_add_seconds(interval=5,
                         function=show_time)

mainloop = GLib.MainLoop()

try:
    mainloop.run()
except KeyboardInterrupt:
    print('exit')
    mainloop.quit()

'''
syscon@sys0124 ~/catkin_ws/src/test_pkg/scripts$ python3 dbus_async_event_loop.py 
Now: 40:42
Now: 40:47
Now: 40:52
Now: 40:57
...
'''