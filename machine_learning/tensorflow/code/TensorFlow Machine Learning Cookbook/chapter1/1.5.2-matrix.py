import tensorflow as tf;
import numpy as np;

# 创建一个计算图会话
sess = tf.Session()
identify_matrix = tf.diag([1.0, 1.0, 1.0])

A = tf.truncated_normal([2, 3])
B = tf.fill([2, 3], 5.0)
C = tf.random_uniform([3, 2])
D = tf.convert_to_tensor(np.array([[1., 2., 3.], [-3., -7., -1.], [0., 5., -2.]]))

# 创建矩阵
# print(sess.run(identify_matrix))
# print(sess.run(A))
# print(sess.run(B))
# print(sess.run(C))
# print(sess.run(D))

# 矩阵乘法和减法
# print(sess.run(A+B))
# print(sess.run(B - B))
# print(sess.run(tf.matmul(B, identify_matrix)))

# 矩阵转置
# print(sess.run(tf.transpose(C)))

# 矩阵行列式
# print(sess.run(tf.matrix_determinant(D)))

# 矩阵的逆矩阵
# inverseD = sess.run(tf.matrix_inverse(D))
# print(inverseD)
# print(sess.run(tf.matmul(D, inverseD)))

# 矩阵分解

# 矩阵的特征值和特征向量
print(sess.run(tf.self_adjoint_eig(D)))
# 结果第一行为特征值，剩下的为特征向量
