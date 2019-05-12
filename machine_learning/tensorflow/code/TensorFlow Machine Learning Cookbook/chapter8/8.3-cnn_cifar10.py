# More Advanced CNN Model: CIFAR-10
#---------------------------------------
#
# In this example, we will download the CIFAR-10 images
# and build a CNN model with dropout and regularization
#
# CIFAR is composed ot 50k train and 10k test
# images that are 32x32.

import os
import sys
import tarfile
import matplotlib.pyplot as plt
import numpy as np
import tensorflow as tf
from six.moves import urllib
import tensorflow_datasets as tfds
from datetime import datetime
import time

# from tensorflow.python.framework import ops
# ops.reset_default_graph()

# Change Directory
abspath = os.path.abspath(__file__)
dname = os.path.dirname(abspath)
os.chdir(dname)

# Start a graph session
# sess = tf.Session()
# GPU memory alloc failed: CUBLAS_STATUS_ALLOC_FAILED, but python crash too 
config = tf.ConfigProto()
config.gpu_options.allow_growth = True
sess = tf.Session(config=config)

# Set model parameters
batch_size = 128
output_every = 50
generations = 1000
eval_every = 500
# evaluation_size = 500
image_width = 32
image_height = 32
crop_height = 24
crop_width = 24
# target_size = max(train_labels) + 1
num_channels = 3
num_targets = 10
data_dir = '..\\dataset'
extract_folder = '..\\dataset\\cifar-10-batches-bin'

# Exponential Learning Rate Decay Params
learning_rate = 0.1
lr_decay = 0.1
num_gens_to_wait = 250.

image_vec_length = image_height * image_width * num_channels
record_length = 1 + image_vec_length

if not os.path.exists(data_dir):
	os.makedirs(data_dir)

cifar10_url = 'http://www.cs.toronto.edu/~kriz/cifar-10-binary.tar.gz'
data_file = os.path.join(data_dir, 'cifar-10-binary.tar.gz')
if not os.path.isfile(data_file):
	# download
	def progress(block_num, block_size, total_size):
		progress_info = [cifar10_url, float(block_num * block_size) / float(total_size) * 100.0]
		print('\r Downloading {} - {:.2f}%'.format(*progress_info), end="")
	filepath, _ = urllib.request.urlretrieve(cifar10_url, data_file, progress)
	tarfile.open(filepath, 'r:gz').extractall(data_dir)

def get_images_labels(batch_size, split, distords=False):
	"""Returns Dataset for given split."""
	dataset = tfds.load(name='cifar10', split=split)
	scope = 'data_augmentation' if distords else 'input'
	with tf.name_scope(scope):
		dataset = dataset.map(DataPreprocessor(distords), num_parallel_calls=10)
	# Dataset is small enough to be fully loaded on memory:
	dataset = dataset.prefetch(-1)
	dataset = dataset.repeat().batch(batch_size)
	iterator = dataset.make_one_shot_iterator()
	images_labels = iterator.get_next()
	images, labels = images_labels['input'], images_labels['target']
	tf.summary.image('images', images)
	return images, labels
  
def read_cifar_files(filename_queue, distort_images = True):
	reader = tf.FixedLengthRecordReader(record_bytes = record_length)
	key, record_string = reader.read(filename_queue)
	record_bytes = tf.decode_raw(record_string, tf.uint8)
	# Extract label
	image_label = tf.cast(tf.slice(record_bytes, [0], [1]), tf.int32)
	# Extract image
	image_extracted = tf.reshape(tf.slice(record_bytes, [1], [image_vec_length]), [num_channels, image_height, image_width])
	
	# reshape image
	image_uint8image = tf.transpose(image_extracted, [1, 2, 0])
	reshaped_image = tf.cast(image_uint8image, tf.float32)
	
	# Randomly Crop image
	final_image = tf.image.resize_image_with_crop_or_pad(reshaped_image, crop_width, crop_height)
	
	if distort_images:
		final_image = tf.image.random_flip_left_right(final_image)
		final_image = tf.image.random_brightness(final_image, max_delta=63)
		final_image = tf.image.random_contrast(final_image, lower=0.2, upper=1.8)
	
	# final_image = tf.image.per_image_whitening(final_image)
	final_image = tf.image.per_image_standardization(final_image)
	return (final_image, image_label)

def input_pipeline(batch_size, train_logical=True):
	if train_logical:
		files = [os.path.join(data_dir, extract_folder, 'data_batch{}.bin'.format(i)) for i in range(1, 6)]
	else:
		files = [os.path.join(data_dir, extract_folder, 'test_batch.bin')]
	filename_queue = tf.train.string_input_producer(files)
	image, label = read_cifar_files(filename_queue)
	
	min_after_dequeue = 5000
	capacity = min_after_dequeue + 3 * batch_size
	example_batch, label_batch = tf.train.shuffle_batch([image, label], batch_size, capacity, min_after_dequeue)
	return (example_batch, label_batch)
	
def _get_images_labels(batch_size, split, distords=False):
	"""Returns Dataset for given split."""
	dataset = tfds.load(name='cifar10', split=split)
	print("load successed.")
	print(dataset)
	scope = 'data_augmentation' if distords else 'input'
	with tf.name_scope(scope):
		dataset = dataset.map(DataPreprocessor(distords), num_parallel_calls=10)
	# Dataset is small enough to be fully loaded on memory:
	dataset = dataset.prefetch(-1)
	dataset = dataset.repeat().batch(batch_size)
	iterator = dataset.make_one_shot_iterator()
	images_labels = iterator.get_next()
	images, labels = images_labels['input'], images_labels['target']
	tf.summary.image('images', images)
	return images, labels

class DataPreprocessor(object):
	"""Applies transformations to dataset record."""

	def __init__(self, distords):
		self._distords = distords
	
	def __call__(self, record):
		"""Process img for training or eval."""
		img = record['image']
		img = tf.cast(img, tf.float32)
		if self._distords:  # training
			# Randomly crop a [height, width] section of the image.
			img = tf.random_crop(img, [image_width, image_height, 3])
			# Randomly flip the image horizontally.
			img = tf.image.random_flip_left_right(img)
			# Because these operations are not commutative, consider randomizing
			# the order their operation.
			# NOTE: since per_image_standardization zeros the mean and makes
			# the stddev unit, this likely has no effect see tensorflow#1458.
			img = tf.image.random_brightness(img, max_delta=63)
			img = tf.image.random_contrast(img, lower=0.2, upper=1.8)
		else:  # Image processing for evaluation.
			# Crop the central [height, width] of the image.
			img = tf.image.resize_image_with_crop_or_pad(img, image_width, image_height)
		# Subtract off the mean and divide by the variance of the pixels.
		img = tf.image.per_image_standardization(img)
		return dict(input=img, target=record['label'])

def cifar_cnn_model(input_images, batch_size, train_logical=True):
	def truncated_normal_var(name, shape, dtype):
		return (tf.get_variable(name=name, shape=shape, dtype=dtype, initializer=tf.truncated_normal_initializer(stddev=0.05)))
	def zero_var(name, shape, dtype):
		return (tf.get_variable(name=name, shape=shape, dtype=dtype, initializer=tf.constant_initializer(0.0)))
	# First Convolutional Layer
	with tf.variable_scope('conv1') as scope:
		conv1_kernel = truncated_normal_var(name='conv_kernel1', shape=[5, 5, 3, 64], dtype=tf.float32)
		conv1 = tf.nn.conv2d(input_images, conv1_kernel, [1,1,1,1], padding='SAME')
		conv1_bias = zero_var(name='conv_bias1', shape=[64], dtype=tf.float32)
		conv1_add_bias = tf.nn.bias_add(conv1, conv1_bias)
		relu_conv1 = tf.nn.relu(conv1_add_bias)
	# max pooling
	pool1 = tf.nn.max_pool(relu_conv1, ksize=[1,3,3,1], strides=[1,2,2,1], padding='SAME', name='pool_layer1')
	
	# Local response normalization
	norm1 = tf.nn.lrn(pool1, depth_radius=5, bias=2.0, alpha=1e-3, beta=0.75, name='norm1')
	
	# Second Convolutional Layer
	with tf.variable_scope('conv2') as scope:
		conv2_kernel = truncated_normal_var(name='conv_kernel2', shape=[5, 5, 64, 64], dtype=tf.float32)
		conv2 = tf.nn.conv2d(norm1, conv2_kernel, [1,1,1,1], padding='SAME')
		conv2_bias = zero_var(name='conv_bias2', shape=[64], dtype=tf.float32)
		conv2_add_bias = tf.nn.bias_add(conv2, conv2_bias)
		relu_conv2 = tf.nn.relu(conv2_add_bias)
	# max pooling
	pool2 = tf.nn.max_pool(relu_conv2, ksize=[1,3,3,1], strides=[1,2,2,1], padding='SAME', name='pool_layer2')
	
	# Local response normalization
	norm2 = tf.nn.lrn(pool2, depth_radius=5, bias=2.0, alpha=1e-3, beta=0.75, name='norm2')
	
	# reshape
	reshaped_output = tf.reshape(norm2, [batch_size, -1])
	print(reshaped_output)
	reshaped_dim = reshaped_output.get_shape()[1].value
	
	# First Fully Connected Layer
	num1 = 30#384
	with tf.variable_scope('full1') as scope:
		full_weight1 = truncated_normal_var(name='full_mult1', shape=[reshaped_dim, num1], dtype=tf.float32)
		full_bias1 = zero_var(name='full_bias1', shape=[num1], dtype=tf.float32)
		full_layer1 = tf.nn.relu(tf.add(tf.matmul(reshaped_output, full_weight1), full_bias1))
	
	# Second Fully Connected Layer
	num2 = 20#192
	with tf.variable_scope('full2') as scope:
		full_weight2 = truncated_normal_var(name='full_mult2', shape=[num1, num2], dtype=tf.float32)
		full_bias2 = zero_var(name='full_bias2', shape=[num2], dtype=tf.float32)
		full_layer2 = tf.nn.relu(tf.add(tf.matmul(full_layer1, full_weight2), full_bias2))
	
	# Final
	with tf.variable_scope('full3') as scope:
		full_weight3 = truncated_normal_var(name='full_mult3', shape=[num2, num_targets], dtype=tf.float32)
		full_bias3 = zero_var(name='full_bias3', shape=[num_targets], dtype=tf.float32)
		final_output = tf.add(tf.matmul(full_layer2, full_weight3), full_bias3)
	
	return(final_output)

def cifar_loss(logits, targets):
	targets = tf.squeeze(tf.cast(targets, tf.int32))
	cross_entropy = tf.nn.sparse_softmax_cross_entropy_with_logits(logits=logits, labels=targets)
	cross_entropy_mean = tf.reduce_mean(cross_entropy, name='cross_entropy')
	return (cross_entropy_mean)

def train_step(loss_value, global_step):
	print("global_step = ", global_step)
	# num_batches_per_epoch = 5000 / batch_size
	# decay_steps = int(num_batches_per_epoch * num_gens_to_wait)
	model_learning_rate = tf.train.exponential_decay(learning_rate, global_step, num_gens_to_wait, lr_decay, staircase=True)
	my_optimizer = tf.train.GradientDescentOptimizer(model_learning_rate)
	train_step = my_optimizer.minimize(loss_value)
	return (train_step)

def accuracy_of_batch(logits, targets):
	targets = tf.squeeze(tf.cast(targets, tf.int32))
	batch_predictions = tf.cast(tf.argmax(logits, 1), tf.int32)
	predicted_correctly = tf.equal(batch_predictions, targets)
	accuracy = tf.reduce_mean(tf.cast(predicted_correctly, tf.float32))
	return (accuracy)

def _variable_on_cpu(name, shape, initializer):
	"""Helper to create a Variable stored on CPU memory.

	Args:
	name: name of the variable
	shape: list of ints
	initializer: initializer for Variable

	Returns:
	Variable Tensor
	"""
	with tf.device('/cpu:0'):
		dtype = tf.float32
		var = tf.get_variable(name, shape, initializer=initializer, dtype=dtype)
	return var

def _variable_with_weight_decay(name, shape, stddev, wd):
	"""Helper to create an initialized Variable with weight decay.

	Note that the Variable is initialized with a truncated normal distribution.
	A weight decay is added only if one is specified.

	Args:
	name: name of the variable
	shape: list of ints
	stddev: standard deviation of a truncated Gaussian
	wd: add L2Loss weight decay multiplied by this float. If None, weight
		decay is not added for this Variable.

	Returns:
	Variable Tensor
	"""
	dtype = tf.float32
	var = _variable_on_cpu(
	  name,
	  shape,
	  tf.truncated_normal_initializer(stddev=stddev, dtype=dtype))
	if wd is not None:
		weight_decay = tf.multiply(tf.nn.l2_loss(var), wd, name='weight_loss')
		tf.add_to_collection('losses', weight_decay)
	return var

def inference(images):
	"""Build the CIFAR-10 model.

	Args:
	images: Images returned from distorted_inputs() or inputs().

	Returns:
	Logits.
	"""
	# We instantiate all variables using tf.get_variable() instead of
	# tf.Variable() in order to share variables across multiple GPU training runs.
	# If we only ran this model on a single GPU, we could simplify this function
	# by replacing all instances of tf.get_variable() with tf.Variable().
	#
	# conv1
	with tf.variable_scope('conv1') as scope:
		kernel = _variable_with_weight_decay('weights',
											 shape=[5, 5, 3, 64],
											 stddev=5e-2,
											 wd=None)
		conv = tf.nn.conv2d(images, kernel, [1, 1, 1, 1], padding='SAME')
		biases = _variable_on_cpu('biases', [64], tf.constant_initializer(0.0))
		pre_activation = tf.nn.bias_add(conv, biases)
		conv1 = tf.nn.relu(pre_activation, name=scope.name)
	
	# pool1
	pool1 = tf.nn.max_pool(conv1, ksize=[1, 3, 3, 1], strides=[1, 2, 2, 1],
						 padding='SAME', name='pool1')
	# norm1
	norm1 = tf.nn.lrn(pool1, 4, bias=1.0, alpha=0.001 / 9.0, beta=0.75,
					name='norm1')
	
	# conv2
	with tf.variable_scope('conv2') as scope:
		kernel = _variable_with_weight_decay('weights',
											 shape=[5, 5, 64, 64],
											 stddev=5e-2,
											 wd=None)
		conv = tf.nn.conv2d(norm1, kernel, [1, 1, 1, 1], padding='SAME')
		biases = _variable_on_cpu('biases', [64], tf.constant_initializer(0.1))
		pre_activation = tf.nn.bias_add(conv, biases)
		conv2 = tf.nn.relu(pre_activation, name=scope.name)
	
	# norm2
	norm2 = tf.nn.lrn(conv2, 4, bias=1.0, alpha=0.001 / 9.0, beta=0.75,
					name='norm2')
	# pool2
	pool2 = tf.nn.max_pool(norm2, ksize=[1, 3, 3, 1],
						 strides=[1, 2, 2, 1], padding='SAME', name='pool2')
	
	# local3
	with tf.variable_scope('local3') as scope:
		# Move everything into depth so we can perform a single matrix multiply.
		reshape = tf.keras.layers.Flatten()(pool2)
		print(reshape)
		dim = reshape.get_shape()[1].value
		weights = _variable_with_weight_decay('weights', shape=[dim, 384],
											  stddev=0.04, wd=0.004)
		biases = _variable_on_cpu('biases', [384], tf.constant_initializer(0.1))
		local3 = tf.nn.relu(tf.matmul(reshape, weights) + biases, name=scope.name)
	# local4
	with tf.variable_scope('local4') as scope:
		weights = _variable_with_weight_decay('weights', shape=[384, 192],
											  stddev=0.04, wd=0.004)
		biases = _variable_on_cpu('biases', [192], tf.constant_initializer(0.1))
		local4 = tf.nn.relu(tf.matmul(local3, weights) + biases, name=scope.name)
	
	# linear layer(WX + b),
	# We don't apply softmax here because
	# tf.nn.sparse_softmax_cross_entropy_with_logits accepts the unscaled logits
	# and performs the softmax internally for efficiency.
	with tf.variable_scope('softmax_linear') as scope:
		weights = _variable_with_weight_decay('weights', [192, num_targets],
											  stddev=1/192.0, wd=None)
		biases = _variable_on_cpu('biases', [num_targets],
								  tf.constant_initializer(0.0))
		softmax_linear = tf.add(tf.matmul(local4, weights), biases, name=scope.name)
	
	return softmax_linear


def _loss(logits, labels):
	"""Add L2Loss to all the trainable variables.

	Add summary for "Loss" and "Loss/avg".
	Args:
	logits: Logits from inference().
	labels: Labels from distorted_inputs or inputs(). 1-D tensor
			of shape [batch_size]

	Returns:
	Loss tensor of type float.
	"""
	# Calculate the average cross entropy loss across the batch.
	labels = tf.cast(labels, tf.int64)
	labels = tf.squeeze(labels)
	cross_entropy = tf.nn.sparse_softmax_cross_entropy_with_logits(
	  labels=labels, logits=logits, name='cross_entropy_per_example')
	cross_entropy_mean = tf.reduce_mean(cross_entropy, name='cross_entropy')
	tf.add_to_collection('losses', cross_entropy_mean)

	# The total loss is defined as the cross entropy loss plus all of the weight
	# decay terms (L2 loss).
	return tf.add_n(tf.get_collection('losses'), name='total_loss')

def train_old():
	# with tf.device('/cpu:0'):
		# images, targets = input_pipeline(batch_size, train_logical=True)
		# test_images, test_targets = input_pipeline(batch_size, train_logical=False)
	# print(images)

	# with tf.variable_scope('model_definition') as scope:
		# model_output = cifar_cnn_model(images, batch_size)
		# use the same variables within scope
		# scope.reuse_variables()
		# test_output = cifar_cnn_model(test_images, batch_size)

	# loss = cifar_loss(model_output, targets)
	# accuracy = accuracy_of_batch(test_output, test_targets)
	# generation_num = tf.Variable(0, trainable=False)
	# generation_num = tf.train.get_or_create_global_step()
	# train_op = train_step(loss, generation_num)

	with tf.device('/cpu:0'):
		images, labels = _get_images_labels(batch_size, tfds.Split.TRAIN, distords=True)
		test_images, test_targets = _get_images_labels(batch_size, tfds.Split.TEST)
		
	logits = inference(images)
	# Calculate loss.
	loss = _loss(logits, labels)
	accuracy = accuracy_of_batch(logits, test_targets)
	generation_num = tf.Variable(0, trainable=False)
	train_op = train_step(loss, generation_num)
	
	init = tf.global_variables_initializer()
	sess.run(init)

	# Initialize queue (This queue will feed into the model, so no placeholders necessary)
	tf.train.start_queue_runners(sess=sess)

	train_loss = []
	test_accuracy = []

	for i in range(generations):
		_, loss_value = sess.run([train_op, loss])
		if (i+1)%output_every == 0:
			train_loss.append(loss_value)
			output = 'Generation {}: Loss = {:.5f}'.format((i+1), loss_value)
			print(output)
		if (i+1)%eval_every == 0:
			[temp_accuracy] = sess.run([accuracy])
			test_accuracy.append(temp_accuracy)
			acc_output = '--- Test Accuracy = {:.2f}%.'.format(100. * temp_accuracy)
			print(acc_output)
	
	# Print loss and accuracy
	# Matlotlib code to plot the loss and accuracies
	eval_indices = range(0, generations, eval_every)
	output_indices = range(0, generations, output_every)

	# Plot loss over time
	plt.plot(output_indices, train_loss, 'k-')
	plt.title('Softmax Loss per Generation')
	plt.xlabel('Generation')
	plt.ylabel('Softmax Loss')
	# plt.show()

	# Plot accuracy over time
	plt.plot(eval_indices, test_accuracy, 'k-')
	plt.title('Test Accuracy')
	plt.xlabel('Generation')
	plt.ylabel('Accuracy')
	# plt.show()

train_old()

def train():
	with tf.Graph().as_default():
		global_step = tf.train.get_or_create_global_step()
		
		with tf.device('/cpu:0'):
			images, labels = _get_images_labels(batch_size, tfds.Split.TRAIN, distords=True)
		
		logits = inference(images)
		# Calculate loss.
		loss = _loss(logits, labels)
		train_op = train_step(loss, global_step)

		class _LoggerHook(tf.train.SessionRunHook):
			"""Logs loss and runtime."""
			
			def begin(self):
				self._step = -1
				self._start_time = time.time()

			def before_run(self, run_context):
				self._step += 1
				return tf.train.SessionRunArgs(loss)  # Asks for loss value.
			
			def after_run(self, run_context, run_values):
				if self._step % eval_every == 0:
					current_time = time.time()
					duration = current_time - self._start_time
					self._start_time = current_time

					loss_value = run_values.results
					examples_per_sec = eval_every * batch_size / duration
					sec_per_batch = float(duration / eval_every)
					
					format_str = ('%s: step %d, loss = %.2f (%.1f examples/sec; %.3f '
								'sec/batch)')
					print (format_str % (datetime.now(), self._step, loss_value,
									   examples_per_sec, sec_per_batch))
		
		with tf.train.MonitoredTrainingSession(
			checkpoint_dir=data_dir,
			hooks=[tf.train.StopAtStepHook(last_step=generations),
				   tf.train.NanTensorHook(loss),
				   _LoggerHook()],
			config=tf.ConfigProto(
				log_device_placement=False)) as mon_sess:
				while not mon_sess.should_stop():
					mon_sess.run(train_op)


# def main(argv=None):  # pylint: disable=unused-argument
	# train()


# if __name__ == '__main__':
	# tf.app.run()

