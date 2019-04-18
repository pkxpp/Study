import matplotlib.pyplot as plt
import numpy as np
import tensorflow as tf
from sklearn import datasets
from tensorflow.python.framework import ops
ops.reset_default_graph()

# 创建一个计算图会话
sess = tf.Session()

def square_study():
	a = tf.constant([[1, 2], [3, 4]])
	b = tf.square(a)
	print(a)
	print(sess.run(a))
	print(sess.run(b))

# square_study()

def shape_study():
	# [3, 1, 1, 3]四维矩阵，第一维3个元数，第二维1个元数，第三维1个元数，第四维3个元数
	M=np.array([[[[1,2,3]]],[[[4,5,6]]],[[[7,8,9]]]])
	print(M)
	# M重组成若干个3维的向量
	M1 = tf.reshape(M, [-1, 3])
	print(M1)
	print(sess.run(M1))
	# 想得到若干个[3,3]的矩阵
	M2 = tf.reshape(M, [-1, 3, 3])
	print(M2)
	print(sess.run(M2))

# shape_study()

def np_meshgrid_study():
	x = np.array([[0, 1, 2], [0, 1, 2]])
	y = np.array([[0, 0, 0], [1, 1, 1]])


	plt.plot(x, y,
			 color='red',  # 全部点设置为红色
			 marker='.',  # 点的形状为圆点
			 linestyle='')  # 线型为空，也即点与点之间不用线连接
	plt.grid(True)
	plt.show()

np_meshgrid_study()
