# k-Nearest Neighbor
#----------------------------------
#
# This function illustrates how to use
# k-nearest neighbors in tensorflow
#
# We will use the 1970s Boston housing dataset
# which is available through the UCI
# ML data repository.
#
# Data:
#----------x-values-----------
# CRIM   : per capita crime rate by town
# ZN     : prop. of res. land zones
# INDUS  : prop. of non-retail business acres
# CHAS   : Charles river dummy variable
# NOX    : nitrix oxides concentration / 10 M
# RM     : Avg. # of rooms per building
# AGE    : prop. of buildings built prior to 1940
# DIS    : Weighted distances to employment centers
# RAD    : Index of radian highway access
# TAX    : Full tax rate value per $10k
# PTRATIO: Pupil/Teacher ratio by town
# B      : 1000*(Bk-0.63)^2, Bk=prop. of blacks
# LSTAT  : % lower status of pop
#------------y-value-----------
# MEDV   : Median Value of homes in $1,000's
import matplotlib.pyplot as plt
import numpy as np
import tensorflow as tf
import requests
from tensorflow.python.framework import ops
ops.reset_default_graph()

# 创建一个计算图会话
sess = tf.Session()

# Load the data
housing_url = 'https://archive.ics.uci.edu/ml/machine-learning-databases/housing/housing.data'
housing_header = ['CRIM', 'ZN', 'INDUS', 'CHAS', 'NOX', 'RM', 'AGE', 'DIS', 'RAD', 'TAX', 'PTRATIO', 'B', 'LSTAT', 'MEDV']
cols_used = ['CRIM', 'INDUS', 'NOX', 'RM', 'AGE', 'DIS', 'TAX', 'PTRATIO', 'B', 'LSTAT']
num_features = len(cols_used)
# housing_file = requests.get(housing_url)

with open('..\\dataset\\housing.data', 'r') as f:
	housing_file = f.read()

# print(housing_file)
	
# housing_data = [[float(x) for x in y.split(' ') if len(x)>=1] for y in housing_file.text.split('\n') if len(y)>=1]
housing_data = [[float(x) for x in y.split(' ') if len(x)>=1] for y in housing_file.split('\n') if len(y)>=1]

# Declare batch size
batch_size = 50
learning_rate = 0.01

y_vals = np.transpose([np.array([y[13] for y in housing_data])])
x_vals = np.array([[x for i,x in enumerate(y) if housing_header[i] in cols_used] for y in housing_data])

## Min-Max Scaling
x_vals = (x_vals - x_vals.min(0)) / x_vals.ptp(0)

# Split the data into train and test sets
train_indices = np.random.choice(len(x_vals), round(len(x_vals)*0.8), replace=False)
test_indices = np.array(list(set(range(len(x_vals))) - set(train_indices)))
x_vals_train = x_vals[train_indices]
x_vals_test = x_vals[test_indices]
y_vals_train = y_vals[train_indices]
y_vals_test = y_vals[test_indices]

# Declare k-value and batch size
k = 4
batch_size=len(x_vals_test)

# Placeholders
x_data_train = tf.placeholder(shape=[None, num_features], dtype=tf.float32)
x_data_test = tf.placeholder(shape=[None, num_features], dtype=tf.float32)
y_target_train = tf.placeholder(shape=[None, 1], dtype=tf.float32)
y_target_test = tf.placeholder(shape=[None, 1], dtype=tf.float32)

# Declare distance metric
# L1
# 1.这里的x_data_train 减去 扩维的x_data_test，是为了让x_data_train的每一分数据都能减去x_data_test的每一分数据。这样才能计算，任意一个x_data_test的数据到x_data_train任意一个数据的距离
# 2.降维：把每一分数据相减后的和作为两份数据之前的距离，求的距离就是cols_used列的和
# 3.所以，最后得到的distance是一个shape=[batch_size, batch_size]的数据，表示的是x_data_test的每一个数据到x_data_train每一个数据的距离
distance = tf.reduce_sum(tf.abs(tf.subtract(x_data_train, tf.expand_dims(x_data_test,1))), reduction_indices=2)

# L2
#distance = tf.sqrt(tf.reduce_sum(tf.square(tf.sub(x_data_train, tf.expand_dims(x_data_test,1))), reduction_indices=1))
# test----------------------
def test_data():
	test_data1 = x_vals_train[0: 3] # shape=[3, 10]
	test_data2 = x_vals_test[0: 3] # shape=[3, 10]
	print(test_data1)
	print(test_data2)

	# test0 = shape = (3, 3, 10)
	test0 = tf.subtract(test_data1, tf.expand_dims(test_data2,1))
	print(test0)
	test00 = sess.run(test0)
	print(test00)

	test1 = sess.run(distance, feed_dict={x_data_train: test_data1, x_data_test: test_data2})
	# print(test1)
	# shape = (3, 3)
	print(tf.constant(test1))

# test_data()
# test---------------------- end


top_k_xvals, top_k_indices = tf.nn.top_k(tf.negative(distance), k = k)
# 对每一个测试数据求的他的k最邻近距离综合，因为求和之后降维了，在扩维回来 shape = (batch_size, 1)
x_sums = tf.expand_dims(tf.reduce_sum(top_k_xvals, 1), 1)
# 复制一份数据只是为了下面除法用的，就是k个最邻近的距离 除以 k个最邻近距离之和(k个公用一个和)，所以每一行加起来的结果是1
x_sums_repeated = tf.matmul(x_sums, tf.ones([1, k], tf.float32))
x_val_weights = tf.expand_dims(tf.div(top_k_xvals, x_sums_repeated), 1)

def test_study_data():
	test_size = 5
	test_data1 = x_vals_train[0: test_size] # shape=[test_size, 10]
	test_data2 = x_vals_test[0: test_size] # shape=[test_size, 10]
	print(test_data1)
	print(test_data2)
	# test_s1 = shape = (5, 5)
	test_s1 = sess.run(distance, feed_dict={x_data_train: test_data1, x_data_test: test_data2})
	# print(test_s1)
	# test_k_xvals = shape = (5, k)
	test_k_xvals, test_k_indices = tf.nn.top_k(tf.negative(distance), k = k)
	# print(test_k_xvals)
	print(sess.run(test_k_xvals, feed_dict={x_data_train: test_data1, x_data_test: test_data2}))
	# print(test_k_indices)
	# print(sess.run(test_k_indices, feed_dict={x_data_train: test_data1, x_data_test: test_data2}))
	
	test_sum = tf.reduce_sum(top_k_xvals, 1)
	print(sess.run(test_sum, feed_dict={x_data_train: test_data1, x_data_test: test_data2}))
	print(sess.run(x_sums, feed_dict={x_data_train: test_data1, x_data_test: test_data2}))
	print(sess.run(x_val_weights, feed_dict={x_data_train: test_data1, x_data_test: test_data2}))
	# 复制一份是要干嘛？
	# [[-8.329587 ]
	 # [-8.815878 ]
	 # [-7.228663 ]
	 # [-6.9582205]
	 # [-8.605508 ]]
	# [[-8.329587  -8.329587  -8.329587  -8.329587 ]
	 # [-8.815878  -8.815878  -8.815878  -8.815878 ]
	 # [-7.228663  -7.228663  -7.228663  -7.228663 ]
	 # [-6.9582205 -6.9582205 -6.9582205 -6.9582205]
	 # [-8.605508  -8.605508  -8.605508  -8.605508 ]]
	print(sess.run(x_sums_repeated, feed_dict={x_data_train: test_data1, x_data_test: test_data2}))
	 
# Tensor("TopKV2_1:0", shape=(?, 4), dtype=float32)
# [[-1.0120509 -1.3620812 -3.3650966 -3.6608112]
 # [-1.0597093 -1.4451206 -2.6025825 -2.8749502]
 # [-1.2977087 -1.368664  -2.331729  -2.5432925]
 # [-1.0782905 -1.2479448 -2.7325215 -2.7949667]
 # [-1.6063948 -1.6166384 -2.2844675 -2.3886318]]
# Tensor("TopKV2_1:1", shape=(?, 4), dtype=int32)
# [[0 3 2 4]
 # [0 3 2 4]
 # [0 3 2 4]
 # [3 0 2 4]
 # [3 0 2 1]]
 
test_study_data()

# 把top_k的x_vals对应的y_vals取出来
top_k_yvals = tf.gather(y_target_train, top_k_indices)
# 根据k个选中的最邻近训练数据，然后按照前面算的权重算一个平均值
prediction = tf.squeeze(tf.matmul(x_val_weights, top_k_yvals), squeeze_dims = [1])
mse = tf.div(tf.reduce_sum(tf.square(tf.subtract(prediction, y_target_test))), batch_size)

# Test:
num_loops = int(np.ceil(len(x_vals_test)/batch_size))

for i in range(num_loops):
	min_index = i*batch_size
	max_index = min((i+1) * batch_size, len(x_vals_train))
	x_batch = x_vals_test[min_index:max_index]
	y_batch = y_vals_test[min_index:max_index]
	predictions = sess.run(prediction, feed_dict={x_data_train: x_vals_train, x_data_test: x_batch, y_target_train: y_vals_train, y_target_test: y_batch})
	
	batch_mse = sess.run(mse, feed_dict={x_data_train: x_vals_train, x_data_test: x_batch, y_target_train: y_vals_train, y_target_test: y_batch})
	
	print('Batch #' + str(i+1) + ' MSE: ' + str(np.round(batch_mse,3)))

# Plot prediction and actual distribution
bins = np.linspace(5, 50, 45)

plt.hist(predictions, bins, alpha=0.5, label='Prediction')
plt.hist(y_batch, bins, alpha=0.5, label='Actual')
plt.title('Histogram of Predicted and Actual Values')
plt.xlabel('Med Home Value in $1,000s')
plt.ylabel('Frequency')
plt.legend(loc='upper right')
plt.show()


