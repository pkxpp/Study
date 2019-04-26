# Implementing Gates
#----------------------------------
#
# This function shows how to implement
# various gates in Tensorflow
#
# One gate will be one operation with
# a variable and a placeholder.
# We will ask Tensorflow to change the
# variable based on our loss function
import matplotlib.pyplot as plt
import numpy as np
import tensorflow as tf
from tensorflow.python.framework import ops
ops.reset_default_graph()

# 创建一个计算图会话
sess = tf.Session()

a = tf.Variable(tf.constant(4.))
x_val = 5.
x_data = tf.placeholder(dtype = tf.float32)

multiplication = tf.multiply(a, x_data)
loss = tf.square(tf.subtract(multiplication, 50.))

init = tf.global_variables_initializer();
sess.run(init)

my_opt = tf.train.GradientDescentOptimizer(0.01)
train_step = my_opt.minimize(loss)

print('Optimizing a Multiplication Gate Output to 50.')
for i in range(10):
    sess.run(train_step, feed_dict={x_data: x_val})
    a_val = sess.run(a)
    mult_output = sess.run(multiplication, feed_dict={x_data: x_val})
    print(str(a_val) + ' * ' + str(x_val) + ' = ' + str(mult_output))
    
#----------------------------------
# Create a nested gate:
#   f(x) = a * x + b
#
#  a --
#      |
#      |-- (multiply)--
#  x --|              |
#                     |-- (add) --> output
#                 b --|
#
#

from tensorflow.python.framework import ops
ops.reset_default_graph()
sess = tf.Session()

a = tf.Variable(tf.constant(1.))
b = tf.Variable(tf.constant(1.))
x_val = 5.
x_data = tf.placeholder(dtype=tf.float32)

two_gate = tf.add(tf.multiply(a, x_data), b)
loss = tf.square(tf.subtract(two_gate, 50.))

my_opt = tf.train.GradientDescentOptimizer(0.01)
train_step = my_opt.minimize(loss)

init = tf.global_variables_initializer();
sess.run(init)
print('Optimizing Two Gate Output to 50.')

for i in range(10):
	sess.run(train_step, feed_dict={x_data: x_val})
	a_val, b_val = (sess.run(a), sess.run(b))
	two_gate_output = sess.run(two_gate, feed_dict={x_data: x_val})
	print(str(a_val) + ' * ' + str(x_val) + ' + ' + str(b_val) + ' = ' + str(two_gate_output))
