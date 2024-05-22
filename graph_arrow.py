import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import sys

data = pd.read_csv('/home/syscon/test_repo/pose_result/dataB-2(0.05)_robot.csv')

x = data['x'].astype(float)
y = data['y'].astype(float)
theta_rad = data['theta'].astype(float)

dx = x + np.cos(theta_rad)*0.05
dy = y + np.sin(theta_rad)*0.05

try:
    plt.figure(figsize=(12, 9))

    plt.scatter(x, y, color='b', label='[B-2]Robot Position')

    plt.scatter(dx, dy, color='r', label='temp position')

    for i in range(len(x)):
        plt.arrow(x[i], y[i], dx[i]-x[i], dy[i]-y[i], head_width=0.01, head_length=0.01)

    plt.xlabel('x')
    plt.ylabel('y')

    plt.xlim(3.5, 4)
    plt.ylim(4, 4.5)

    plt.legend()
    plt.grid(True)
    plt.savefig('/home/syscon/test_repo/pose_result/dataB-2(0.05)_robot_plot.png')
    plt.show()

except Exception as e:
    plt.close()
