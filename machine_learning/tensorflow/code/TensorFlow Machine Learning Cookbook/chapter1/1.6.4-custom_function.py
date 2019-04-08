import tensorflow as tf;
import numpy as np;

# 创建一个计算图会话
sess = tf.Session()

# 自定义函数3x^2 - x + 10
def custom_polynomial(value):
	return (tf.subtract(3 * tf.square(value), value) + 10)

print(sess.run(custom_polynomial(11)))