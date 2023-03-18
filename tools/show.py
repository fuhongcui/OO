import matplotlib.pyplot as plt
import numpy as np

# 生成x值
x = np.arange(-500, 500, 0.1)

# 计算y值
y = (1 - (x / (0.2679 * 500))) * 0.5 * 640

# 绘制图像
plt.plot(x, y)

# 添加标题和轴标签
plt.title('y = x * x')
plt.xlabel('x')
plt.ylabel('y')

# 显示图像
plt.show()
