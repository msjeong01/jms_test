import rospy
from sensor_msgs.msg import LaserScan
import csv

class ScanRanges:
    def __init__(self):
        self.scan_sub = rospy.Subscriber('/R_014/scan', LaserScan, self.scan_callback)
        self.data = []
        self.seq = []
        self.count = 0

    def __del__(self):
        self.scan_sub.unregister()

    def scan_callback(self, msg):
        self.count += 1
        # print("Scan callback: %d", self.count)

        if len(self.data) < 10:
            self.seq.append(msg.header.seq)
            self.data.append(list(msg.ranges))
        else:
            self.convert_to_csv()

    def convert_to_csv(self):
        with open('/home/syscon/test_repo/TypeA-2_lidar_scan.csv', 'w', newline='') as csvfile:
            csvwriter = csv.writer(csvfile)
            header = ['seq'] + ['ranges[{}]'.format(i) for i in range(len(self.data[0]))]
            csvwriter.writerow(header)
            for i in range(len(self.data)):
                row = [self.seq[i]] + self.data[i]
                csvwriter.writerow(row)
        print("Success to convert as csv file.")

if __name__ == '__main__':
    rospy.init_node('scan_ranges', anonymous=True)
    scan_range = ScanRanges()
    rospy.spin()
