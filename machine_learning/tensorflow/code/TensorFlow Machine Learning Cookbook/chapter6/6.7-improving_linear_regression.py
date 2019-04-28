# Improving Linear Regression with Neural Networks (Logistic Regression)
#----------------------------------
#
# This function shows how to use Tensorflow to
# solve logistic regression with a multiple layer neural network
# y = sigmoid(A3 * sigmoid(A2* sigmoid(A1*x + b1) + b2) + b3)
#
# We will use the low birth weight data, specifically:
#  y = 0 or 1 = low birth weight
#  x = demographic and medical history data
#
# Low Birthrate data:
#
#Columns    Variable                                              Abbreviation
#-----------------------------------------------------------------------------
# Identification Code                                     ID
# Low Birth Weight (0 = Birth Weight >= 2500g,            LOW
#                          1 = Birth Weight < 2500g)
# Age of the Mother in Years                              AGE
# Weight in Pounds at the Last Menstrual Period           LWT
# Race (1 = White, 2 = Black, 3 = Other)                  RACE
# Smoking Status During Pregnancy (1 = Yes, 0 = No)       SMOKE
# History of Premature Labor (0 = None  1 = One, etc.)    PTL
# History of Hypertension (1 = Yes, 0 = No)               HT
# Presence of Uterine Irritability (1 = Yes, 0 = No)      UI
# Number of Physician Visits During the First Trimester   FTV
#                (0 = None, 1 = One, 2 = Two, etc.)
# Birth Weight in Grams                                   BWT
#------------------------------
# The multiple neural network layer we will create will be composed of
# three fully connected hidden layers, with node sizes 25, 10, and 3

import matplotlib.pyplot as plt
import numpy as np
import tensorflow as tf
import requests
from tensorflow.python.framework import ops
ops.reset_default_graph()

# 创建一个计算图会话
sess = tf.Session()

# birthdata_url = 'https://www.umass.edu/statdata/statdata/data/lowbwt.dat'
birthdata_url = 'https://github.com/nfmcclure/tensorflow_cookbook/raw/master/01_Introduction/07_Working_with_Data_Sources/birthweight_data/birthweight.dat'
birth_file = requests.get(birthdata_url)
birth_data = birth_file.text.split('\r\n')
birth_header = [x for x in birth_data[0].split('\t') if len(x) >=1]
birth_data = [[float(x) for x in y.split('\t') if len(x)>=1] for y in birth_data[1:] if len(y) >=1]
# 因为源数据访问不了，用了这个数据是处理过的，原来的第10列在次为最后一列BWT
y_vals = np.array([x[0] for x in birth_data])
cols_of_interest = ['AGE', 'LWT', 'RACE', 'SMOKE', 'PTL', 'HT', 'UI', 'LOW']
x_vals = np.array([x[1:8] for x in birth_data])
print(len(x_vals[0]))

# print(len(birth_data))

# seed = 3
# tf.set_random_seed(seed)
# np.random.seed(seed)
batch_size = 90

train_indices = np.random.choice(len(x_vals), round(len(x_vals)*0.8), replace = False)
test_indices = np.array(list(set(range(len(x_vals))) - set(train_indices)))
x_vals_train = x_vals[train_indices]
x_vals_test = x_vals[test_indices]
y_vals_train = y_vals[train_indices]
y_vals_test = y_vals[test_indices]

def normalize_cols(m):
	col_max = m.max(axis=0)
	col_min = m.min(axis = 0)
	return (m-col_min) / (col_max - col_min)

x_vals_train = np.nan_to_num(normalize_cols(x_vals_train))
x_vals_test = np.nan_to_num(normalize_cols(x_vals_test))

x_data = tf.placeholder(shape = [None, 7], dtype = tf.float32)
y_target = tf.placeholder(shape = [None, 1], dtype = tf.float32)

def init_variable(shape):
	return (tf.Variable(tf.random_normal(shape = shape)))

# def init_weight(shape, st_dev):
	# weight = tf.Variable(tf.random_normal(shape, stddev = st_dev))
	# return weight

# def init_bias(shape, st_dev):
	# bias = tf.Variable(tf.random_normal(shape, stddev = st_dev))
	# return bias

def logistic(input_layer, multiplication_weight, bias_weight, activation = True):
	linear_layer = tf.add(tf.matmul(input_layer, multiplication_weight), bias_weight)
	if activation:
		return(tf.nn.sigmoid(linear_layer))
	else:
		return (linear_layer)


# def fully_connected(input_layer, weights, biases):
	# layer = tf.add(tf.matmul(input_layer, weights), biases)
	# return (tf.nn.relu(layer))

# 14 hidden nodes
A1 = init_variable(shape=[7, 14])
b1 = init_variable(shape=[14])
logistic_layer1 = logistic(x_data, A1, b1)

# 5 hidden nodes
A2 = init_variable(shape=[14, 5])
b2 = init_variable(shape=[5])
logistic_layer2 = logistic(logistic_layer1, A2, b2)

# output
A3 = init_variable(shape=[5, 1])
b3 = init_variable(shape=[1])
final_output = logistic(logistic_layer2, A3, b3, activation = False)

loss = tf.reduce_mean(tf.nn.sigmoid_cross_entropy_with_logits(logits=final_output, labels=y_target))
my_opt = tf.train.AdamOptimizer(learning_rate=0.002)
train_step = my_opt.minimize(loss)
init = tf.global_variables_initializer()
sess.run(init)

prediction = tf.round(tf.nn.sigmoid(final_output))
predictions_correct = tf.cast(tf.equal(prediction, y_target), tf.float32)
accuracy = tf.reduce_mean(predictions_correct)

loss_vec = []
train_acc = []
test_acc = []
for i in range(1500):
	rand_index = np.random.choice(len(x_vals_train), size = batch_size)
	rand_x = x_vals_train[rand_index]
	rand_y = np.transpose([y_vals_train[rand_index]])
	sess.run(train_step, feed_dict = {x_data: rand_x, y_target: rand_y})
	
	temp_loss = sess.run(loss, feed_dict = {x_data: rand_x, y_target: rand_y})
	loss_vec.append(temp_loss)
	
	temp_acc_train = sess.run(accuracy, feed_dict = {x_data: x_vals_train, y_target: np.transpose([y_vals_train])})
	train_acc.append(temp_acc_train)
	
	temp_acc_test = sess.run(accuracy, feed_dict = {x_data: x_vals_test, y_target: np.transpose([y_vals_test])})
	test_acc.append(temp_acc_test)
	
	if (i+1)%150==0:
		print('Generation: ' + str(i+1) + '. Loss = ' + str(temp_loss))

plt.plot(loss_vec, 'k-', label='Train Loss')
plt.title('Cross Entropy Loss per Generation')
plt.xlabel('Generation')
plt.ylabel('Cross Entropy Loss')
# plt.legend(loc='upper right')
plt.show()

plt.plot(train_acc, 'k-', label='Train Set Accuracy')
plt.plot(test_acc, 'r--', label='Test Set Accuracy')
plt.title('Train and Test Accuracy')
plt.xlabel('Generation')
plt.ylabel('Accuracy')
plt.legend(loc='lower right')
plt.show()
