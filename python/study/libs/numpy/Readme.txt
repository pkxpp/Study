1. 2011/11/10
解决报错：numpy ImportError: DLL load failed: %1
* 总之就是32后者64位不对的
* 发现自己的numpy库是没有64位的，网上也是有人提供了64位的whl程序，需要pip安装
* 另外，这种问题还是google给力多了
* pip install numpy-1.10.1+mkl-cp27-none-win_amd64.whl 
* import cv2 和 import numpy都没有报错了 ^_^

参考：
* http://www.lfd.uci.edu/~gohlke/pythonlibs/
* http://m.blog.csdn.net/blog/johnnyconstantine/41321203