import matplotlib.pyplot as plt
import numpy as np
import tensorflow as tf
import requests
import os
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

# np_meshgrid_study()

def tf_expand_dims_study():
	t = tf.placeholder(shape=[2], dtype=tf.float32)
	print(t)
	x = np.array([1, 2])
	xtensor = tf.constant(x)
	t_dims0 = tf.expand_dims(xtensor, 0)
	print(t_dims0)
	expand_dims0 = sess.run(t_dims0)
	print(expand_dims0)
	
	t_dims1 = tf.expand_dims(xtensor, 1)
	print(t_dims1)
	expand_dims1 = sess.run(t_dims1)
	print(expand_dims1)
	
	t_dims2 = tf.expand_dims(xtensor, -1)
	print(t_dims2)
	expand_dims2 = sess.run(t_dims2)
	print(expand_dims2)

# tf_expand_dims_study()

def tf_subtract_study():
	a = tf.constant([[1, 2, 3], [4, 5, 6]])# shape = (2, 3)
	b = tf.constant([[0, 0, 2], [2, 0, 0]])
	c = tf.expand_dims(b, 1)	# shape=(2, 1, 3)
	print(a)
	print(sess.run(a))
	print(b)
	print(sess.run(tf.subtract(a, b)))
	print(c)
	print(sess.run(c))
	d = tf.subtract(a, c)	# shape = (2, 2, 3)
	print(d)
	print(sess.run(d))
	
	# the simplest example
	e = tf.constant([1, 2, 3])
	f = tf.constant([[0, 0, 3]])
	print(sess.run(tf.subtract(e, f)))
	print(sess.run(tf.subtract(f, e)))
	
	# ValueError: Dimensions must be equal, but are 2 and 3 for 'Sub_4' (op: 'Sub') with input shapes: [2], [1,3].
	# g = tf.constant([1, 2])
	# print(sess.run(tf.subtract(g, f)))
	h = tf.constant([1])
	print(sess.run(tf.subtract(h, b)))

# tf_subtract_study()


# def get_data_frome_file():
# with open('..\\dataset\\housing.data', 'r') as f:
	# print(f.read())

# get_data_frome_file()

def tf_squeeze_study():
	
	# 't' is a tensor of shape [1, 2, 1, 3, 1, 1]
	t = tf.constant([[[[[[1]], [[2]], [[3]]]],[[[[4]], [[5]], [[6]]]]]])
	tf.shape(tf.squeeze(t))  # [2, 3]
	print(t)
	print(tf.squeeze(t))
	t1 = tf.squeeze(t)
	print(sess.run(t1))
	print(tf.shape(tf.squeeze(t)))
	
	# 't' is a tensor of shape [1, 2, 1, 3, 1, 1]
	print(sess.run(tf.squeeze(t, [2, 4])))
	tf.shape(tf.squeeze(t, [2, 4]))  # [1, 2, 3, 1]

# tf_squeeze_study()

def tf_SparseTensor_study():
	a = tf.constant([
	["a"],
	["b"]
	])
	print(a)
	hypothesis = tf.SparseTensor(
	[[0, 0, 0],[1, 0, 0]],
	["a", "b"],
	(2, 1, 1))
	print(hypothesis)
	
	# b = tf.constant([
	# [
		# [],
		# ["a"]
	# ],
	# [
		# ["b", "c"],
		# ["a"]
	# ]
	# ])
	# print(b)
	# 'truth' is a tensor of shape `[2, 2]` with variable-length values:
	#   (0,0) = []
	#   (0,1) = ["a"]
	#   (1,0) = ["b", "c"]
	#   (1,1) = ["a"]
	truth = tf.SparseTensor(
		[[0, 1, 0],[1, 0, 0],[1, 0, 1],[1, 1, 0]],
		["a", "b", "c", "a"],
		(2, 2, 2))
	print(truth)

# tf_SparseTensor_study()

def tf_random_normal_stduy():
	data = tf.constant([1, 2, 3])
	print(data)
	weight_shape = tf.shape(data)
	print(weight_shape)
	print(sess.run(weight_shape))
	weight = tf.random_normal(weight_shape, stddev=0.1)
	print(weight)

tf_random_normal_stduy()