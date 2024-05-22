import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv('/home/syscon/test_repo/TypeA-2_lidar_scan.csv')


plt.figure(figsize=(12, 8))

for i in range(len(df)):
    plt.plot(df.columns[1:], df.iloc[i, 1:], marker='o', label=f'seq {df.iloc[i, 0]}')

plt.title('LiDAR Scan Data - Type A-2')
plt.xlabel('ranges[]')
plt.ylabel('seq')

plt.legend()

plt.tight_layout()

plt.savefig('/home/syscon/test_repo/TypeA-2_lidar_scan_graph.png')

plt.show()
