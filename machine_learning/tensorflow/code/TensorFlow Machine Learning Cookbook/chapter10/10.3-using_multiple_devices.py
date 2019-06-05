# -*- coding: utf-8 -*-
# Using Multiple Devices
#----------------------------------
#
# This function gives us the ways to use
#  multiple devices (executors) in Tensorflow.

import tensorflow as tf
from tensorflow.python.framework import ops
ops.reset_default_graph()

# Start a graph session
sess = tf.Session(config=tf.ConfigProto(log_device_placement=True))

a = tf.constant([1.0, 2.0, 3.0, 4.0, 5.0, 6.0], shape=[2, 3], name='a')
b = tf.constant([1.0, 2.0, 3.0, 4.0, 5.0, 6.0], shape=[3, 2], name='b')

c = tf.matmul(a, b)

print(sess.run(c))

# 
config = tf.ConfigProto()
config.allow_soft_placement = True
sess_soft = tf.Session(config = config)

# Gpu memory
config.gpu_options.allow_growth = True
sess_grow = tf.Session(config=config)

config.gpu_options.per_process_gpu_memory_fraction = 0.4
sess_limited = tf.Session(config=config)

ops.reset_default_graph()
sess = tf.Session(config=tf.ConfigProto(allow_soft_placement=True, log_device_placement=True))

if tf.test.is_built_with_cuda():
	with tf.device('/cpu:0'):
		a = tf.constant([1.0, 3.0, 5.0], shape=[1,3], name = 'a')
		b = tf.constant([2.0, 4.0, 6.0], shape=[3,1], name = 'b')
		with tf.device('/gpu:0'):
			c = tf.matmul(a, b, name = 'c')
			c = tf.reshape(c, [-1])
		with tf.device('/gpu:1'):
			d = tf.matmul(b, a, name = 'd')
			flat_d = tf.reshape(d, [-1])
		combined = tf.multiply(c, flat_d)
	print(sess.run(combined))


from tensorflow.python.client import device_lib

# print all
local_device_protos = device_lib.list_local_devices()
print(local_device_protos)

# 
[print(x) for x in local_device_protos if x.device_type == 'GPU']

