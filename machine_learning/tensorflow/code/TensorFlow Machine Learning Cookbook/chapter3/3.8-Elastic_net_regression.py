import matplotlib.pyplot as plt
import numpy as np
import tensorflow as tf
from sklearn import datasets
from tensorflow.python.framework import ops
ops.reset_default_graph()

# 创建一个计算图会话
sess = tf.Session()

iris = datasets.load_iris()
# iris.data有四列Sepal length, Sepal width, Petal length,Petal width
# iris.target有三个结果：I. setosa(山鸢尾), I. virginica(弗吉尼亚盐角草), I. versicolor(变色鸢尾)
x_vals = np.array([[x[1], x[2], x[3]] for x in iris.data])
y_vals = np.array([y[0] for y in iris.data])

# Declare batch size
batch_size = 50
learning_rate = 0.001

x_data = tf.placeholder(shape=[None, 3], dtype=tf.float32)
y_target = tf.placeholder(shape=[None, 1], dtype=tf.float32)
A = tf.Variable(tf.random_normal(shape=[3, 1]))
b = tf.Variable(tf.random_normal(shape=[1, 1]))
model_output = tf.add(tf.matmul(x_data, A), b)

elastic_param1 = tf.constant(1.)
elastic_param2 = tf.constant(1.)
l1_a_loss = tf.reduce_mean(tf.abs(A))
l2_a_loss = tf.reduce_mean(tf.square(A))
e1_term = tf.multiply(elastic_param1, l1_a_loss)
e2_term = tf.multiply(elastic_param2, l2_a_loss)
loss = tf.expand_dims(tf.add(tf.add(tf.reduce_mean(tf.square(y_target - model_output)), e1_term), e2_term), 0)

# init = tf.initialize_all_variables()
init = tf.global_variables_initializer();
sess.run(init)

my_opt = tf.train.GradientDescentOptimizer(learning_rate)
train_step = my_opt.minimize(loss)

loss_vec = []
for i in range(1000):
	rand_index = np.random.choice(len(x_vals), size = batch_size)
	rand_x = x_vals[rand_index] # shape = (20, 3)
	rand_y = np.transpose([y_vals[rand_index]]) # after tanspose shape = (1, 20)
	sess.run(train_step, feed_dict={x_data: rand_x, y_target: rand_y})
	temp_loss = sess.run(loss, feed_dict={x_data: rand_x, y_target: rand_y})
	loss_vec.append(temp_loss[0])
	if (i+1) % 250 == 0:
		print(rand_x)
		print(rand_y)
		print('Step #' + str(i+1) + ' A = ' + str(sess.run(A)) +', b = ' + str(sess.run(b)))
		print('Loss = ' + str(temp_loss))

# Get the optimal coefficients
[[sw_coef], [pl_coef], [pw_ceof]] = sess.run(A)
[[y_intercept]] = sess.run(b)

# Plot loss over time
plt.plot(loss_vec, 'k-')
plt.title('L2 Loss per Generation')
plt.xlabel('Generation')
plt.ylabel('L2 Loss')
plt.show()
