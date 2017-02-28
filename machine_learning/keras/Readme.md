[TOC]

# 安装
命令：
```
pip install keras
```

报错：
```
MB 72kB/s
Building wheels for collected packages: keras, theano, scipy
  Running setup.py bdist_wheel for keras ... done
  Stored in directory: C:\Users\peikai\AppData\Local\pip\Cache\wheels\f6\c5\63\9
7d96b41bf822858027c70b04448c19deaccf1cf518148fa82
  Running setup.py bdist_wheel for theano ... done
  Stored in directory: C:\Users\peikai\AppData\Local\pip\Cache\wheels\96\2b\3d\7
1d54e24a7171a4afb7144d1e944a7be643b448b23a35b9937
  Running setup.py bdist_wheel for scipy ... error
  Complete output from command d:\programfiles\python\python35\python.exe -u -c
"import setuptools, tokenize;__file__='C:\\Users\\peikai\\AppData\\Local\\Temp\\
pip-build-gr7nb30i\\scipy\\setup.py';f=getattr(tokenize, 'open', open)(__file__)
;code=f.read().replace('\r\n', '\n');f.close();exec(compile(code, __file__, 'exe
c'))" bdist_wheel -d C:\Users\peikai\AppData\Local\Temp\tmpk_39egl7pip-wheel- --
python-tag cp35:
  lapack_opt_info:
  lapack_mkl_info:
    libraries mkl_rt not found in ['d:\\programfiles\\python\\python35\\lib', 'C
:\\', 'd:\\programfiles\\python\\python35\\libs']
    NOT AVAILABLE

  openblas_lapack_info:
    libraries openblas not found in ['d:\\programfiles\\python\\python35\\lib',
'C:\\', 'd:\\programfiles\\python\\python35\\libs']
    NOT AVAILABLE

  atlas_3_10_threads_info:
  Setting PTATLAS=ATLAS
  d:\programfiles\python\python35\lib\site-packages\numpy\distutils\system_info.
py:1051: UserWarning: Specified path C:\projects\numpy-wheels\windows-wheel-buil
der\atlas-builds\atlas-3.11.38-sse2-64\lib is invalid.
    pre_dirs = system_info.get_paths(self, section, key)
  <class 'numpy.distutils.system_info.atlas_3_10_threads_info'>
    NOT AVAILABLE

  atlas_3_10_info:
  <class 'numpy.distutils.system_info.atlas_3_10_info'>
    NOT AVAILABLE

  atlas_threads_info:
  Setting PTATLAS=ATLAS
  <class 'numpy.distutils.system_info.atlas_threads_info'>
    NOT AVAILABLE

  atlas_info:
  <class 'numpy.distutils.system_info.atlas_info'>
    NOT AVAILABLE

  d:\programfiles\python\python35\lib\site-packages\numpy\distutils\system_info.
py:572: UserWarning:
      Atlas (http://math-atlas.sourceforge.net/) libraries not found.
      Directories to search for the libraries can be specified in the
      numpy/distutils/site.cfg file (section [atlas]) or by setting
      the ATLAS environment variable.
    self.calc_info()
  lapack_info:
    libraries lapack not found in ['d:\\programfiles\\python\\python35\\lib', 'C
:\\', 'd:\\programfiles\\python\\python35\\libs']
    NOT AVAILABLE

  d:\programfiles\python\python35\lib\site-packages\numpy\distutils\system_info.
py:572: UserWarning:
      Lapack (http://www.netlib.org/lapack/) libraries not found.
      Directories to search for the libraries can be specified in the
      numpy/distutils/site.cfg file (section [lapack]) or by setting
      the LAPACK environment variable.
    self.calc_info()
  lapack_src_info:
    NOT AVAILABLE

  d:\programfiles\python\python35\lib\site-packages\numpy\distutils\system_info.
py:572: UserWarning:
      Lapack (http://www.netlib.org/lapack/) sources not found.
      Directories to search for the sources can be specified in the
      numpy/distutils/site.cfg file (section [lapack_src]) or by setting
      the LAPACK_SRC environment variable.
    self.calc_info()
    NOT AVAILABLE

  Running from scipy source directory.
  Traceback (most recent call last):
    File "<string>", line 1, in <module>
    File "C:\Users\peikai\AppData\Local\Temp\pip-build-gr7nb30i\scipy\setup.py",
 line 415, in <module>
      setup_package()
    File "C:\Users\peikai\AppData\Local\Temp\pip-build-gr7nb30i\scipy\setup.py",
 line 411, in setup_package
      setup(**metadata)
    File "d:\programfiles\python\python35\lib\site-packages\numpy\distutils\core
.py", line 135, in setup
      config = configuration()
    File "C:\Users\peikai\AppData\Local\Temp\pip-build-gr7nb30i\scipy\setup.py",
 line 335, in configuration
      config.add_subpackage('scipy')
    File "d:\programfiles\python\python35\lib\site-packages\numpy\distutils\misc
_util.py", line 1001, in add_subpackage
      caller_level = 2)
    File "d:\programfiles\python\python35\lib\site-packages\numpy\distutils\misc
_util.py", line 970, in get_subpackage
      caller_level = caller_level + 1)
    File "d:\programfiles\python\python35\lib\site-packages\numpy\distutils\misc
_util.py", line 907, in _get_configuration_from_setup_py
      config = setup_module.configuration(*args)
    File "scipy\setup.py", line 15, in configuration
      config.add_subpackage('linalg')
    File "d:\programfiles\python\python35\lib\site-packages\numpy\distutils\misc
_util.py", line 1001, in add_subpackage
      caller_level = 2)
    File "d:\programfiles\python\python35\lib\site-packages\numpy\distutils\misc
_util.py", line 970, in get_subpackage
      caller_level = caller_level + 1)
    File "d:\programfiles\python\python35\lib\site-packages\numpy\distutils\misc
_util.py", line 907, in _get_configuration_from_setup_py
      config = setup_module.configuration(*args)
    File "scipy\linalg\setup.py", line 20, in configuration
      raise NotFoundError('no lapack/blas resources found')
  numpy.distutils.system_info.NotFoundError: no lapack/blas resources found

  ----------------------------------------
  Failed building wheel for scipy
  Running setup.py clean for scipy
  Complete output from command d:\programfiles\python\python35\python.exe -u -c
"import setuptools, tokenize;__file__='C:\\Users\\peikai\\AppData\\Local\\Temp\\
pip-build-gr7nb30i\\scipy\\setup.py';f=getattr(tokenize, 'open', open)(__file__)
;code=f.read().replace('\r\n', '\n');f.close();exec(compile(code, __file__, 'exe
c'))" clean --all:

  `setup.py clean` is not supported, use one of the following instead:

    - `git clean -xdf` (cleans all files)
    - `git clean -Xdf` (cleans all versioned files, doesn't touch
                        files that aren't checked into the git repo)

  Add `--force` to your command to use it anyway if you must (unsupported).


  ----------------------------------------
  Failed cleaning build dir for scipy
Successfully built keras theano
Failed to build scipy
Installing collected packages: scipy, theano, pyyaml, keras
  Running setup.py install for scipy ... error
    Complete output from command d:\programfiles\python\python35\python.exe -u -
c "import setuptools, tokenize;__file__='C:\\Users\\peikai\\AppData\\Local\\Temp
\\pip-build-gr7nb30i\\scipy\\setup.py';f=getattr(tokenize, 'open', open)(__file_
_);code=f.read().replace('\r\n', '\n');f.close();exec(compile(code, __file__, 'e
xec'))" install --record C:\Users\peikai\AppData\Local\Temp\pip-dizisluh-record\
install-record.txt --single-version-externally-managed --compile:

    Note: if you need reliable uninstall behavior, then install
    with pip instead of using `setup.py install`:

      - `pip install .`       (from a git repo or downloaded source
                               release)
      - `pip install scipy`   (last SciPy release on PyPI)


    lapack_opt_info:
    lapack_mkl_info:
      libraries mkl_rt not found in ['d:\\programfiles\\python\\python35\\lib',
'C:\\', 'd:\\programfiles\\python\\python35\\libs']
      NOT AVAILABLE

    openblas_lapack_info:
      libraries openblas not found in ['d:\\programfiles\\python\\python35\\lib'
, 'C:\\', 'd:\\programfiles\\python\\python35\\libs']
      NOT AVAILABLE

    atlas_3_10_threads_info:
    Setting PTATLAS=ATLAS
    d:\programfiles\python\python35\lib\site-packages\numpy\distutils\system_inf
o.py:1051: UserWarning: Specified path C:\projects\numpy-wheels\windows-wheel-bu
ilder\atlas-builds\atlas-3.11.38-sse2-64\lib is invalid.
      pre_dirs = system_info.get_paths(self, section, key)
    <class 'numpy.distutils.system_info.atlas_3_10_threads_info'>
      NOT AVAILABLE

    atlas_3_10_info:
    <class 'numpy.distutils.system_info.atlas_3_10_info'>
      NOT AVAILABLE

    atlas_threads_info:
    Setting PTATLAS=ATLAS
    <class 'numpy.distutils.system_info.atlas_threads_info'>
      NOT AVAILABLE

    atlas_info:
    <class 'numpy.distutils.system_info.atlas_info'>
      NOT AVAILABLE

    d:\programfiles\python\python35\lib\site-packages\numpy\distutils\system_inf
o.py:572: UserWarning:
        Atlas (http://math-atlas.sourceforge.net/) libraries not found.
        Directories to search for the libraries can be specified in the
        numpy/distutils/site.cfg file (section [atlas]) or by setting
        the ATLAS environment variable.
      self.calc_info()
    lapack_info:
      libraries lapack not found in ['d:\\programfiles\\python\\python35\\lib',
'C:\\', 'd:\\programfiles\\python\\python35\\libs']
      NOT AVAILABLE

    d:\programfiles\python\python35\lib\site-packages\numpy\distutils\system_inf
o.py:572: UserWarning:
        Lapack (http://www.netlib.org/lapack/) libraries not found.
        Directories to search for the libraries can be specified in the
        numpy/distutils/site.cfg file (section [lapack]) or by setting
        the LAPACK environment variable.
      self.calc_info()
    lapack_src_info:
      NOT AVAILABLE

    d:\programfiles\python\python35\lib\site-packages\numpy\distutils\system_inf
o.py:572: UserWarning:
        Lapack (http://www.netlib.org/lapack/) sources not found.
        Directories to search for the sources can be specified in the
        numpy/distutils/site.cfg file (section [lapack_src]) or by setting
        the LAPACK_SRC environment variable.
      self.calc_info()
      NOT AVAILABLE

    Running from scipy source directory.
    Traceback (most recent call last):
      File "<string>", line 1, in <module>
      File "C:\Users\peikai\AppData\Local\Temp\pip-build-gr7nb30i\scipy\setup.py
", line 415, in <module>
        setup_package()
      File "C:\Users\peikai\AppData\Local\Temp\pip-build-gr7nb30i\scipy\setup.py
", line 411, in setup_package
        setup(**metadata)
      File "d:\programfiles\python\python35\lib\site-packages\numpy\distutils\co
re.py", line 135, in setup
        config = configuration()
      File "C:\Users\peikai\AppData\Local\Temp\pip-build-gr7nb30i\scipy\setup.py
", line 335, in configuration
        config.add_subpackage('scipy')
      File "d:\programfiles\python\python35\lib\site-packages\numpy\distutils\mi
sc_util.py", line 1001, in add_subpackage
        caller_level = 2)
      File "d:\programfiles\python\python35\lib\site-packages\numpy\distutils\mi
sc_util.py", line 970, in get_subpackage
        caller_level = caller_level + 1)
      File "d:\programfiles\python\python35\lib\site-packages\numpy\distutils\mi
sc_util.py", line 907, in _get_configuration_from_setup_py
        config = setup_module.configuration(*args)
      File "scipy\setup.py", line 15, in configuration
        config.add_subpackage('linalg')
      File "d:\programfiles\python\python35\lib\site-packages\numpy\distutils\mi
sc_util.py", line 1001, in add_subpackage
        caller_level = 2)
      File "d:\programfiles\python\python35\lib\site-packages\numpy\distutils\mi
sc_util.py", line 970, in get_subpackage
        caller_level = caller_level + 1)
      File "d:\programfiles\python\python35\lib\site-packages\numpy\distutils\mi
sc_util.py", line 907, in _get_configuration_from_setup_py
        config = setup_module.configuration(*args)
      File "scipy\linalg\setup.py", line 20, in configuration
        raise NotFoundError('no lapack/blas resources found')
    numpy.distutils.system_info.NotFoundError: no lapack/blas resources found

    ----------------------------------------
Command "d:\programfiles\python\python35\python.exe -u -c "import setuptools, to
kenize;__file__='C:\\Users\\peikai\\AppData\\Local\\Temp\\pip-build-gr7nb30i\\sc
ipy\\setup.py';f=getattr(tokenize, 'open', open)(__file__);code=f.read().replace
('\r\n', '\n');f.close();exec(compile(code, __file__, 'exec'))" install --record
 C:\Users\peikai\AppData\Local\Temp\pip-dizisluh-record\install-record.txt --sin
gle-version-externally-managed --compile" failed with error code 1 in C:\Users\p
eikai\AppData\Local\Temp\pip-build-gr7nb30i\scipy\

C:\Users\peikai>
```

解决：
参考：[scipy安装报错解决](http://blog.csdn.net/riverflowrand/article/details/52768489)
手动下载whl包，手动安装

* scipy安装成功，import报错
* numpy-mkl下载和安装

成功安装：
```
E:\study\machine_learning>pip install keras
Collecting keras
  Retrying (Retry(total=4, connect=None, read=None, redirect=None)) after connec
tion broken by 'ConnectTimeoutError(<pip._vendor.requests.packages.urllib3.conne
ction.VerifiedHTTPSConnection object at 0x0000000003EA7BE0>, 'Connection to pypi
.python.org timed out. (connect timeout=15)')': /simple/keras/
Collecting theano (from keras)
Requirement already satisfied: six in d:\programfiles\python\python35\lib\site-p
ackages (from keras)
Requirement already satisfied: pyyaml in d:\programfiles\python\python35\lib\sit
e-packages (from keras)
Requirement already satisfied: scipy>=0.11 in d:\programfiles\python\python35\li
b\site-packages (from theano->keras)
Requirement already satisfied: numpy>=1.7.1 in d:\programfiles\python\python35\l
ib\site-packages (from theano->keras)
Installing collected packages: theano, keras
Successfully installed keras-1.2.2 theano-0.8.2
```
# 测试
* 参考http://blog.csdn.net/nnnnnnnnnnnny/article/details/56845456