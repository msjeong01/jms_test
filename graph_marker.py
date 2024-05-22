import pandas as pd
import matplotlib.pyplot as plt

# 데이터 로드
data = pd.read_csv('/home/syscon/test_repo/pose_result/dataB-2(0.05)_marker.csv')

# x, y 열 추출
x = data['x'].astype(float)
y = data['y'].astype(float)

# 그래프 크기 설정
plt.figure(figsize=(12, 9))

# 좌표에 점 찍기
plt.scatter(x, y, color='b')

# 축 레이블 설정
plt.xlabel('x')
plt.ylabel('y')

plt.xlim(-1.1, -0.6)
plt.ylim(0.0, 0.5)


# 그래프 출력
plt.grid(True)
plt.gca().set_aspect('equal', adjustable='box')
plt.savefig('/home/syscon/test_repo/pose_result/dataB-2(0.05)_marker.png')
plt.show()

plt.close()
