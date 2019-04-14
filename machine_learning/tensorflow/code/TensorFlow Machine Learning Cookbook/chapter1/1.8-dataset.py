import tensorflow as tf;
import numpy as np;
import requests

# 创建一个计算图会话
sess = tf.Session()

# 1.Iris data
# from sklearn import datasets
# iris = datasets.load_iris()
# print(len(iris.data))

# print(len(iris.target))
# print(iris.data[0]) #Sepal length, Sepal width, Petal length,Petal width
# print(set(iris.target)) # I. setosa, I. virginica, I. versicolor


# 2.
# import requests

# birthdata_url = 'https://www.umass.edu/statdata/statdata/data/lowbwt.dat'
# birth_file = requests.get(birthdata_url)
# birth_data = birth_file.text.split('\r\n')[5:]
# 网址访问不了，所以这里的数据是0
# print(len(birth_data))
# birth_header = [x for x in birth_data[0].split(' ') if len(x) >= 1]
# birth_data = [[float(x) for x in y.split(' ') if len(x)>=1] for y in birth_data[1:] if len(y)>=1]
# print(len(birth_data))
# print(len(birth_data[0]))
def GetBirthData():
	# 这个下载不了，网上找到下面这个
	# birthdata_url = 'https://www.umass.edu/statdata/statdata/data/lowbwt.dat'
	birthdata_url = 'https://github.com/nfmcclure/tensorflow_cookbook/raw/master/01_Introduction/07_Working_with_Data_Sources/birthweight_data/birthweight.dat'
	birth_file = requests.get(birthdata_url)
	# 源数据前面应该有5行是一些无用的，所以跳过去，但是换了这个链接是去掉的，所以不需要跳过
	# birth_data = birth_file.text.split('\r\n')[5:]
	birth_data = birth_file.text.split('\r\n')
	print(birth_data[0])
	# 源数据的每列之间是用空格分开的，但是这份新数据是\t分开的
	birth_header = [x for x in birth_data[0].split('\t') if len(x)>=1]
	#LOW     AGE     LWT     RACE    SMOKE(是否吸烟)   PTL     HT      UI      BWT(出生体重)
	print(birth_header)
	birth_data = [[float(x) for x in y.split('\t') if len(x)>=1] for y in birth_data[1:] if len(y)>= 1]
	print(len(birth_data))
	print(len(birth_data[0]))
	print(np.array(birth_data).shape)
GetBirthData()

# 3.Housing Price Data
# housing_url = 'https://archive.ics.uci.edu/ml/machine-learning-databases/housing/housing.data'
# housing_header = ['CRIM', 'ZN', 'INDUS', 'CHAS', 'NOX', 'RM', 'AGE', 'DIS', 'RAD', 'TAX', 'PTRATIO', 'B', 'LSTAT', 'MEDV']
# housing_file = requests.get(housing_url)
# housing_data = [[float(x) for x in y.split(' ') if len(x)>=1] for y in housing_file.text.split('\n') if len(y)>=1]
# print(len(housing_data))
# print(len(housing_data[0]))

# 4.MNIST Handwriting Data
# from tensorflow.examples.tutorials.mnist import input_data

# mnist = input_data.read_data_sets("MNIST_data/", one_hot=True)
# print(len(mnist.train.images))
# print(len(mnist.test.images))
# print(len(mnist.validation.images))
# print(mnist.train.labels[1,:])

# 5.Ham/Spam Text Data
# import requests
# import io
# from zipfile import ZipFile

## Get/read zip file
# zip_url = 'http://archive.ics.uci.edu/ml/machine-learning-databases/00228/smsspamcollection.zip'
# r = requests.get(zip_url)
# z = ZipFile(io.BytesIO(r.content))
# file = z.read('SMSSpamCollection')
## Format Data
# text_data = file.decode()
# text_data = text_data.encode('ascii',errors='ignore')
# text_data = text_data.decode().split('\n')
# text_data = [x.split('\t') for x in text_data if len(x)>=1]
# [text_data_target, text_data_train] = [list(x) for x in zip(*text_data)]
# print(len(text_data_train))
# print(set(text_data_target))
# print(text_data_train[1])


# 6.Movie Review Data
import requests
import io
import tarfile

def GetMovieReviewData():
	movie_data_url = 'http://www.cs.cornell.edu/people/pabo/movie-review-data/rt-polaritydata.tar.gz'
	r = requests.get(movie_data_url)
	# Stream data into temp object
	stream_data = io.BytesIO(r.content)
	tmp = io.BytesIO()
	while True:
		s = stream_data.read(16384)
		if not s:  
			break
		tmp.write(s)
	stream_data.close()
	tmp.seek(0)
	# Extract tar file
	tar_file = tarfile.open(fileobj=tmp, mode="r:gz")
	pos = tar_file.extractfile('rt-polaritydata/rt-polarity.pos')
	neg = tar_file.extractfile('rt-polaritydata/rt-polarity.neg')
	# Save pos/neg reviews
	pos_data = []
	for line in pos:
		pos_data.append(line.decode('ISO-8859-1').encode('ascii',errors='ignore').decode())
	neg_data = []
	for line in neg:
		neg_data.append(line.decode('ISO-8859-1').encode('ascii',errors='ignore').decode())
	tar_file.close()

	print(len(pos_data))
	print(len(neg_data))
	print(neg_data[0])

# GetMovieReviewData()

# 8.The Works of Shakespeare Data
import requests
def GetShakespeareData():
	shakespeare_url = 'http://www.gutenberg.org/cache/epub/100/pg100.txt'
	# Get Shakespeare text
	response = requests.get(shakespeare_url)
	shakespeare_file = response.content
	# Decode binary into string
	shakespeare_text = shakespeare_file.decode('utf-8')
	# Drop first few descriptive paragraphs.
	shakespeare_text = shakespeare_text[7675:]
	print(len(shakespeare_text))

# GetShakespeareData()
