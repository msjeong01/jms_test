import pandas as pd
import matplotlib.pyplot as plt

# csv 파일 읽기
df = pd.read_csv('/home/syscon/test_repo/TypeB-2_lidar_scan.csv')


# 10m 이하 감지 데이터
data_below_10 = df[df.iloc[:, 1:] <= 10].dropna(how='all')
# 1m 이하 감지 데이터
data_below_1 = df[df.iloc[:, 1:] <= 1].dropna(how='all')

# 각 데이터 개수 산출
count_below_10 = data_below_10.count().sum()
count_below_1= data_below_1.count().sum()
total_data_count = df.iloc[:, 1:].count().sum()

# 각 데이터 비율
ratio_below_10 = count_below_10 / total_data_count * 100
ratio_below_1 = count_below_1 / total_data_count * 100

print("10m 이하로 감지된 데이터 비율")
print("데이터 개수: %d, 비율: %.3f%%" %(count_below_10, ratio_below_10))

print("1m 이하로 감지된 데이터 비율")
print("데이터 개수: %d, 비율: %.3f%%" %(count_below_1, ratio_below_1))

print("total: %s" %total_data_count)

# 그래프 표시
plt.figure(figsize=(12, 6))

# 10 이하
plt.subplot(1, 2, 1)
for index, row in data_below_10.iterrows():
    plt.plot(range(len(row[1:])), row[1:], marker='o', label=f'seq (10m)', linestyle='None')
plt.title('[B-1] Lidar Scan Data - Below 10m')
plt.xlabel('ranges[]')
plt.ylabel('ranges value')
plt.grid(True)
plt.legend(loc='upper right')

# 1 이하
plt.subplot(1, 2, 2)
for index, row in data_below_1.iterrows():
    plt.plot(range(len(row[1:])), row[1:], marker='x', label=f'seq (1m)', linestyle='None')
plt.title('[B-1] Lidar Scan Data - Below 1m')
plt.xlabel('ranges[]')
plt.ylabel('ranges value')
plt.grid(True)
plt.legend(loc='upper right')

# 그래프 표시
plt.tight_layout()
plt.savefig('/home/syscon/test_repo/TypeB-1_lidar_scan_graph_below.png')
plt.show()
