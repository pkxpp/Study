# -*- coding: utf-8 -*-
# Parallelizing Tensorflow
#----------------------------------
#
# We will show how to use Tensorflow distributed

import tensorflow as tf
from tensorflow.python.framework import ops
ops.reset_default_graph()

cluster = tf.train.ClusterSpec({'local': ['localhost:2222', 'localhost:2223']})

server = tf.train.Server(cluster, job_name='local', task_index=0)
server = tf.train.Server(cluster, job_name='local', task_index=1)

mat_dim = 25
matrix_list = {}
with tf.device('/job:local/task:0'):
	for i in range(0, 2):
		m_label = 'm_{}'.format(i)
		matrix_list[m_label] = tf.random_normal([mat_dim, mat_dim])

sum_outs = {}
with tf.device('/job:local/task:1'):
	for i in range(0, 2):
		A = matrix_list['m_{}'.format(i)]
		sum_outs['m_{}'.format(i)] = tf.reduce_sum(A)
	summed_out = tf.add_n(list(sum_outs.values()))

with tf.Session(server.target) as sess:
	result = sess.run(summed_out)
	print('Summed Values:{}'.format(result))

