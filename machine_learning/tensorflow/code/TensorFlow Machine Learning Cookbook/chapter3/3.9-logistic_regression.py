import matplotlib.pyplot as plt
import numpy as np
import tensorflow as tf
import requests
from sklearn import datasets
from sklearn.preprocessing import normalize
from tensorflow.python.framework import ops
ops.reset_default_graph()

# 创建一个计算图会话
sess = tf.Session()

# 这个下载不了，网上找到下面这个
# birthdata_url = 'https://www.umass.edu/statdata/statdata/data/lowbwt.dat'
birthdata_url = 'https://github.com/nfmcclure/tensorflow_cookbook/raw/master/01_Introduction/07_Working_with_Data_Sources/birthweight_data/birthweight.dat'
birth_file = requests.get(birthdata_url)
# 源数据前面应该有5行是一些无用的，所以跳过去，但是换了这个链接是去掉的，所以不需要跳过
# birth_data = birth_file.text.split('\r\n')[5:]
birth_data = birth_file.text.split('\r\n')
# print(birth_data[0])
# 源数据的每列之间是用空格分开的，但是这份新数据是\t分开的
birth_header = [x for x in birth_data[0].split('\t') if len(x)>=1]
# print(birth_header)
birth_data = [[float(x) for x in y.split('\t') if len(x)>=1] for y in birth_data[1:] if len(y)>= 1]
# print(len(birth_data))
# print(len(birth_data[0]))
# print(np.array(birth_data).shape)
# print(birth_data[0])
# 这里的数据错误，导致最后结果和书上不一样，书上写的是x[0], x[2:9]，但是索引是从0开始的，坑爹
# 书上用的是源数据，而我这里用的是处理过的数据，书上也说了去掉了实际出生体特征和ID两列，估计是源数据里面的第一列和最后一列
y_vals = np.array([x[0] for x in birth_data])
x_vals = np.array([x[1:8] for x in birth_data])
print(y_vals)
print(x_vals)

train_indices = np.random.choice(len(x_vals), round(len(x_vals) * 0.8), replace = False)
test_indices = np.array(list(set(range(len(x_vals))) - set(train_indices)))
x_vals_train = x_vals[train_indices]
x_vals_test = x_vals[test_indices]
y_vals_train = y_vals[train_indices]
y_vals_test = y_vals[test_indices]

def normalize_cols(m):
	col_max = m.max(axis = 0)
	col_min = m.min(axis = 0)
	return (m - col_min)/(col_max - col_min)

x_vals_train = np.nan_to_num(normalize_cols(x_vals_train))
x_vals_test = np.nan_to_num(normalize_cols(x_vals_test))
# print(x_vals_train)
# print(len(x_vals_test))

# Declare batch size
batch_size = 25
learning_rate = 0.01

x_data = tf.placeholder(shape=[None, 7], dtype=tf.float32)
y_target = tf.placeholder(shape=[None, 1], dtype=tf.float32)
A = tf.Variable(tf.random_normal(shape=[7, 1]))
b = tf.Variable(tf.random_normal(shape=[1, 1]))
model_output = tf.add(tf.matmul(x_data, A), b)

loss = tf.reduce_mean(tf.nn.sigmoid_cross_entropy_with_logits(logits=model_output, labels=y_target))

# init = tf.initialize_all_variables()
init = tf.global_variables_initializer();
sess.run(init)

my_opt = tf.train.GradientDescentOptimizer(learning_rate)
train_step = my_opt.minimize(loss)

prediction = tf.round(tf.sigmoid(model_output))
predictions_correct = tf.cast(tf.equal(prediction, y_target), tf.float32)
# 因为是batch，所以这里的预测计算模型也是用的求平均值
accuracy = tf.reduce_mean(predictions_correct)

loss_vec = []
train_acc = []
test_acc = []
for i in range(1500):
	rand_index = np.random.choice(len(x_vals_train), size = batch_size)
	rand_x = x_vals_train[rand_index]
	rand_y = np.transpose([y_vals_train[rand_index]])
	sess.run(train_step, feed_dict={x_data: rand_x, y_target: rand_y})
	temp_loss = sess.run(loss, feed_dict={x_data: rand_x, y_target: rand_y})
	loss_vec.append(temp_loss)
	temp_acc_train = sess.run(accuracy, feed_dict={x_data: x_vals_train, y_target: np.transpose([y_vals_train])})
	train_acc.append(temp_acc_train)
	temp_acc_test = sess.run(accuracy, feed_dict={x_data: x_vals_test, y_target: np.transpose([y_vals_test])})
	test_acc.append(temp_acc_test)
	if (i+1) % 300 == 0:
		# print('Step #' + str(i+1) + ' A = ' + str(sess.run(A)) +', b = ' + str(sess.run(b)))
		print('Loss = ' + str(temp_loss))
		print("Train accuracy = " + str(temp_acc_train))
		print("Test accuracy = " + str(temp_acc_test))

plt.plot(loss_vec, 'k-')
plt.title('Cross Entropy Loss per Generation')
plt.xlabel('Generation')
plt.ylabel('Cross Entropy Loss')
plt.show()

plt.plot(train_acc, 'k-', label='Train Set Accuracy')
plt.plot(test_acc, 'r--', label='Test Set Accuracy')
plt.title('Train and Test Accuracy')
plt.xlabel('Generation')
plt.ylabel('Accuracy')
plt.legend(loc='lower right')
plt.show()


