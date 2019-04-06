import tensorflow as tf;
import numpy as np;

# 创建一个计算图会话
sess = tf.Session()

# 1.整流线性单元(Rectifier linar unit, ReLU)
print(sess.run(tf.nn.relu([-3., 3., 10.])))

# 2.ReLU6
print(sess.run(tf.nn.relu6([-3., 3., 10.])))

# 3.sigmoid
print(sess.run(tf.nn.sigmoid([-1., 0., 1.])))

# 4.tanh
print(sess.run(tf.nn.tanh([-1., 0., 1.])))

# 5.softsign
print(sess.run(tf.nn.softsign([-1., 0., 1.])))

# 6.softplus
print(sess.run(tf.nn.softplus([-1., 0., 1.])))
