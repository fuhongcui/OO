import matplotlib.pyplot as plt
import numpy as np

near = 1
far = 1000
# 生成x值
x = np.arange(-far, -near, 1)

# 计算y值
y = 2.0 * far * near / (x * (far - near)) + (near + far) / (far - near)
# 绘制图像
plt.plot(x, y)

# 添加标题和轴标签
plt.title('depth')
plt.xlabel('x')
plt.ylabel('y')

# 显示图像
plt.show()
