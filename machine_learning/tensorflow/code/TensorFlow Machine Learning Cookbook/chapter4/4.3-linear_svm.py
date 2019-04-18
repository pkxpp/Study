import matplotlib.pyplot as plt
import numpy as np
import tensorflow as tf
from sklearn import datasets
from tensorflow.python.framework import ops
ops.reset_default_graph()

# 创建一个计算图会话
sess = tf.Session()

iris = datasets.load_iris()
# iris.data有四列Sepal(花萼) length, Sepal width, Petal(花瓣) length,Petal width
# iris.target有三个结果：I. setosa(山鸢尾), I. virginica(弗吉尼亚盐角草), I. versicolor(变色鸢尾)
x_vals = np.array([x[3] for x in iris.data])
y_vals = np.array([y[0] for y in iris.data])

train_indices = np.random.choice(len(x_vals), round(len(x_vals)*0.8), replace = False)
test_indices = np.array(list(set(range(len(x_vals))) - set(train_indices)))
x_vals_train = x_vals[train_indices]
x_vals_test = x_vals[test_indices]
y_vals_train = y_vals[train_indices]
y_vals_test = y_vals[test_indices]

# Declare batch size
batch_size = 50
learning_rate = 0.075

x_data = tf.placeholder(shape=[None, 1], dtype=tf.float32)
y_target = tf.placeholder(shape=[None, 1], dtype=tf.float32)
A = tf.Variable(tf.random_normal(shape=[1, 1]))
b = tf.Variable(tf.random_normal(shape=[1, 1]))
model_output = tf.add(tf.matmul(x_data, A), b)

epsilon = tf.constant([0.5])
loss = tf.reduce_mean(tf.maximum(0., tf.subtract(tf.abs(tf.subtract(model_output, y_target)), epsilon)))

# init = tf.initialize_all_variables()
init = tf.global_variables_initializer();
sess.run(init)

my_opt = tf.train.GradientDescentOptimizer(learning_rate)
train_step = my_opt.minimize(loss)



train_loss = []
test_loss = []
for i in range(200):
	rand_index = np.random.choice(len(x_vals_train), size = batch_size)
	rand_x = np.transpose([x_vals_train[rand_index]]) # shape = (batch_size, 2)
	rand_y = np.transpose([y_vals_train[rand_index]]) # after tanspose shape = (1, 20)
	sess.run(train_step, feed_dict={x_data: rand_x, y_target: rand_y})
	# temp_train_loss = sess.run(loss, feed_dict={x_data: rand_x, y_target: rand_y})
	# train_loss.append(temp_train_loss)
	temp_train_loss = sess.run(loss, feed_dict={x_data: np.transpose([x_vals_train]), y_target: np.transpose([y_vals_train])})
	train_loss.append(temp_train_loss)
	temp_test_loss = sess.run(loss, feed_dict={x_data: np.transpose([x_vals_test]), y_target: np.transpose([y_vals_test])})
	test_loss.append(temp_test_loss)
	if (i+1) % 50 == 0:
		print('---------------------------')
		print('Generation: ' + str(i))
		print('Step #' + str(i+1) + ' A = ' + str(sess.run(A)) +', b = ' + str(sess.run(b)))
		print('Train Loss = ' + str(temp_train_loss))
		print('Test Loss = ' + str(temp_test_loss))

# Get the optimal coefficients
[[slope]] = sess.run(A)
[[y_intercept]] = sess.run(b)
[width] = sess.run(epsilon)

best_fit = []
best_fit_upper = []
best_fit_lower = []
for i in x_vals:
	best_fit.append(slope * i + y_intercept)
	best_fit_upper.append(slope * i + y_intercept + width)
	best_fit_lower.append(slope * i + y_intercept - width)



plt.plot(x_vals, y_vals, 'o', label = 'Data Points')
plt.plot(x_vals, best_fit, 'r-', label='SVM Regression Line', linewidth=3)
plt.plot(x_vals, best_fit_upper, 'r--', linewidth=2)
plt.plot(x_vals, best_fit_lower, 'r--', linewidth=2)
plt.ylim([0, 10])
plt.legend(loc='lower right')
plt.title('Sepal Length vs Pedal Width')
plt.xlabel('Pedal Width')
plt.ylabel('Sepal Length')
plt.show()

plt.plot(train_loss, 'k-', label='Training Set Loss')
plt.plot(test_loss, 'r--', label='Test Set Loss')
plt.title('L2 Loss Generation')
plt.xlabel('Generation')
plt.ylabel('L2 Loss')
plt.legend(loc='upper right')
plt.show()

