[friends of ED网站](http://friendsofed.com)

# 第2章 JavaScript语法

# 第3章 DOM

## 3.1 文档：DOM中的'D'
* document: 网页文档转换成一个文档对象

## 3.2 文档：DOM中的'O'
* 对象object

JavaScript语言里的对象：
* 用户定义对象（user-defined object）：由程序员自行创建的对象。
* 内建对象（native object）： 内建在JavaScript语言里的对象，如Array、Math、Date等
* 宿主对象（host object）：由浏览器提供的对象

## 3.3 文档：DOM中的'M'
* 模型Model：
* DOM把一份文档表示为一棵树：(X)HTML

## 3.4 节点
### 3.4.1 元素节点
* p：文本段落元素
* ul：无序清单元素
* li：列表项元素

### 3.4.2 文本节点
* 在XHTML文档里，文本节点总是被包含在元素节点的内部。

### 3.4.3 属性节点
属性节点用来对元素做出更具体的描述。

> 属性总是被放在起始标签里。

代码：

	<p titile = "a gentle reminder">Don't forget to buy this stuff.</p>

![属性标签](image\属性节点.png)

### 3.4.4 CSS

* 放在文档的'<'head>部分('<'style>标签之间)
* 继承(inheritance)是CSS技术中的一项强大功能。类似于DOM，CSS也把文档的内容视为一颗节点树。节点树上的各个元素将继承其父元素的样式属性。

#### class属性
元素添加class属性

	<p class="special">This paragraph has the special class</p>

样式：

```
	.special{
		font-style: italic;
	}
```

#### id属性
id属性

```
	<ul id="purchases">
```

样式表：

```
#puchases{
	border: 1px solid whi9te;
	background-color: #333;
	colro: #ccc;
	padding: 1em;
}
```

### 3.4.5 获取元素
* getElementById
* getElementsByTagName：返回一个对象数组
* getElementsByClassName：通过class属性中的类名来访问元素。
用空格分隔类名，指定多个类名。


### 3.4.6 盘点知识点

## 3.5 获取和设置属性

### 3.5.1 getAttribut

* 函数。不属于document对象，通过元素节点对象调用。


### 3.5.2 setAttribute

> 注意细节：通过setAttribute对文档做出修改后，在通过浏览器的view souce(查看源代码)选项去查看文档的源代码时看到的仍将是改变前的属性值，也就是说，setAttribute做出的修改不会反映在文档本身的源代码里。这种“表里不一”的现象源自DOM的工作模式：*先加载文档的静态内容，再动态刷新，动态刷新不会影响文档的静态内容。*这正是DOM的真正威力：对页面内容进行刷新却不需要在浏览器里刷新页面。

# 第4章 案例研究：JavaScript图片库

## 4.1 标记

## 4.2 JavaScript

## 4.3 应用这个JavaScript函数
### 事件处理函数

语法：


	event = "JavaScript stagement(s)"


> 注意：JavaScript代码包含在一对引号之间。多条语句用分号隔开即可。

* 使用this关键字：表示当前对象

#### 事件处理函数机制

如果代码返回的值true，onclick事件处理函数就认为“这个链接被点击了”；反之，如果返回的值是false，onclick事件处理函数就认为“这个链接没有被点击。”

但是，不是在JavaScript函数返回return false。而是如下代码所示：

	onclick="showPic(this);return false;"

最主要的思想是引号("")括起来的JavaScript代码块，返回的值，而不是某条语句返回的值，所以也可以这么如下写法(其中：**showPic函数返回false**)：

	onclick=" return showPic(this)"

### 问题
* event有哪些？

## 4.4 对这个函数进行扩展


几个新的DOM属性

### 4.4.1 childNodes属性
在一棵节点树上，childNodes属性可以用来获取任何一个元素的所有子元素，它是一个包含这个元素全部子元素的数组。

### 4.4.2 nodeType属性
获取nodeType属性

	node.nodeType


> nodeType是数字，而不是英文名之类的东西。

#### nodeType属性可取值

nodeType属性总共有12种可取值，但其中仅有3种具有实用价值

* 元素节点的nodeType属性值是1
* 属性节点的nodeType属性值是2
* 文本节点的nodeType属性值是3

> 每一个元素节点之间的换行是一个文本节点。这也是为什么<body>的childNodes有那么多个，而其中nodeType为3的内容就是换行，可以看nodeValue的值。

### 4.4.3 在标记里增加一段描述

### 4.4.4 用JavaScript改变这段描述

### 4.4.5 nodeValue属性

如果想改变一个文本节点的值，那就用DOM提供的nodeValue属性。

	node.nodeValue

> 注意：用nodeValue属性获取description对象的值时，得到的并不是包含在这个段落里的文本。因为<p>元素是元素节点，<p>元素里面是另一种节点：文本节点。


### 4.4.6 firstChild和lastChild属性

	node.firstChild

等价于

	node.childNodes[0]


同样的，还有lastChild

	node.lastChild
	// 等价于
	node.childNodes[node.chillNodes.length-1]

### 4.4.7 利用nodeValue属性刷新这段描述

#### CSS的语法问题

* ‘#333这是什么颜色的表示法？

## 4.5 小节

> 道理：达成目标的过程与目标本身同样重要！


# 第5章 最佳实践

### 5.1 过去的错误
### 5.2 平稳退化

平稳退化：可以让访问者在他们的浏览器不支持JavaScript的情况下仍能顺利的浏览你的网站。也就是说，虽然某些功能无法使用，但最基本的操作仍能顺利完成。

window对象的open()方法：

	window.open(url, name, features)

### 5.2.1 "javascript:"伪协议
"javascript:"伪协议让我们通过一个链接来调用JavaScript函数。具体做法：

	<a href="javascript:popUp('http://www.example.com');">Example</a>


总之，在HTML文档里通过"javascript:"伪协议调用JavaScript代码的做法非常不好。

### 5.2.2 内嵌的时间处理函数
具体做法：

	<a href="#" onclick="popUp('http://www.example.com/');return false;">Example<a>


'#'符号：是一个仅供文档内部使用的链接几号（但就这条指令而已， 是未指向任何目标的内部链接。）

很遗憾，这个技巧与用"javascript:"伪协议调用JavaScript代码的做法同样糟糕。
，因为他们都不能平稳化。如果用户已经禁用了浏览器的JavaScript功能，这样的链接将毫无作用。

### 5.2.3 谁关心这个

搜索机器人很少能能够理解JavaScript代码，影响搜索排名。

	<a href="http://www.example.com/" onclick="popUp('htttp://www.example.com';return false;">Example</a>
	// 更简洁的写法
	<a href="http://www.example.com/" onclick="popUp(this.href;return false;">Example</a>


## 5.3 向CSS学习

### 5.3.1 结构与样式的分离
CSS技术最大的优点： 它能够帮助你将Web文档的内容结构（标记）和版面设计（样式）分离开来。

文档结构与文档样式的分离可以确保网页都能平稳退化。不支持或禁用了CSS功能的浏览器同样可以把网页的内容按照正确的结构显示出来。

### 5.3.2 渐进增强


CSS指令构成了一个表示层，负责提供关于”表示“的信息；JavaScript指令构成了行为层，负责提供关于”行为“的信息。*行为层的应用方式与表示层一样*


## 5.4 分离JavaScript
用一个”挂钩“，就像CSS机制中的class或id属性那样，把JavaScript代码调用行为与HTMLwend的结构和内容分离开。

问题：必须让这些代码在HTML文档全部加载到浏览器之后马上开始执行。

当window对象触发onload事件时， document对象已经存在。

## 5.5 向后兼容

### 5.5.1 对象检测（object detection）
### 5.5.2 浏览器嗅探技术

“浏览器嗅探”：指通过提取浏览器供应商提供的信息来解决向后兼容问题。

## 5.6 性能考虑

### 5.6.1 尽量少访问DOM和尽量减少标记


* 不管什么时候，只要是查询DOM中的某些元素，浏览器都会搜索整个DOM树。
* 尽量减少文档中的标记数量。过多不必要的元素只会增加DOM树的规模

### 5.6.2 合并和放置脚本

* 把所有'<'script>标签都放到文档的末尾，</body>标记之前，就可以让页面变得更快。

### 5.6.3 压缩脚本

* 通常，为了与非精简版本区分开，最好在精简副本的文件名中加上min字样

> 终于知道为毛有些是min.js了

# 第6章 案例研究：图片库改进版

## 6.3 它的JavaScript与HTML标记是分离的吗？

### 6.3.1 添加事件处理函数

## 6.4 不要太多的假设

## 6.7 把JavaScript与CSS结合起来

## 6.8 DOM Core和HTML-DOM
方法和属性的互换

## 6.9 小结

# 第7章 动态创建标记

## 7.1. 一些传统方法

### 7.1.1 document.write

### 7.1.2 innerHTML属性

## 7.2 DOM方法

### 7.2.1 createElement方法
创建元素节点
### 7.2.2 appendChild方法
把新创建的节点插入某个文档。

### 7.2.3 createTextNode方法
创建文本节点

### 7.2.4 一个更复杂的组合

## 7.3 重回图片库
### 7.3.1 在已有元素前插入一个新元素
* insertBefore方法

* parentNode属性

### 7.3.2 在现有方法后插入一个新元素
实例：

```
parent.insertBefore(newElement, targetElement.nextSibling);
```

* nextSibling属性
把目标插入到目标元素的下一个兄弟元素之前

### 7.3.3 图片库二次改进版

## 7.4 Ajax
怎么才能真正得到原来并不存在于初始页面中的内容呢？

Ajax：用于概括异步加载页面内容的技术。

### 7.4.1 XMLHttpRequest对象
Ajax技术的核心就是XMLHttpRequest对象

readyState属性的值：
* 0表示未初始化
* 1表示正在加载
* 2表示加载完毕
* 3表示正在交互
* 4表示完成

### 7.4.2 渐进增强与Ajax
让Ajax功能扮演一个位于常规站点之上的层

### 7.4.3 Hijax

# 第8章 充实文档的内容


## 8.1 不应该做什么
两项原则牢记在心
* 渐进增强
* 平稳退化

## 8.2 把“不可见”变成“可见”

## 8.3 内容
<abbr>

### 8.3.1 选用HTML、XHMTL还是HTML5
XHMTL： 使用一个DOCTYPE让浏览器采用更严格的呈现方案。

### 8.3.2 CSS

### 8.3.3 JavaScript

## 8.4 显示“缩略语列表”
### 8.4.1 编写displayAbbreviations
### 8.4.2 创建标记

### 8.4.3 一个浏览器“地雷”

## 8.5 显示“文献来源链接表”

看blockquote的最后一个节点

	<blockquote cite="http://www.w3.org/DOM/">
		<p>
			A platform- and language-neutral interface that will allow programs
			and scripts to dynamically access and update the
			content, structur and style of documents.
		</p>
	</blockquote>

> 注意：blockquote的lastChild并不是p元素节点，而是文本节点。因为'<'/p>'和'<'/blockqote>标签之间还存在着一个换行符。

这样做有一个很直接的原因：
文本节点没有applendChild方法，这个方法是元素节点的方法，所以必须找到最后一个元素节点，而不是最后一个任意类型的节点。

* 获取所有元素节点

getElementsByTagName("*");

## 8.6 显示“快捷键清单”


* accesskey属性

* 快捷键约定俗成的设置方法
[快捷键约定俗成的设置方法](http://www.clagnut.com/blog/193/)

 - accesskey="1"对应着一个“返回到本网站主页”的链接；
 - accesskey="2"对应着一个“后退到前一页面”的链接；
 - accesskey="4"对应着一个“打开本网站的搜索表单/页面”的链接；
 - accesskey="9"对应着一个“本网站联系办法”的链接；
 - accesskey="0"对应着一个“查看本网站的快捷键清单”的链接。

## 8.7 检索和添加信息

# 第9章 CSS-DOM


## 9.1 三位一体的页面

### 9.1.1 结构层

### 9.1.2 表示层

### 9.1.3 行为层


### 9.1.4 分离

## 9.2 style属性

代码示例：

	var para = document.getElementById("example");
	alert(typeof para.nodeName);
	alert(typeof para.style);

**不仅文档里的每个元素都是一个对象，每个元素都有一个style属性，它们也是一个对象。**

### 9.2.1 获取样式


*当你需要引用一个中间带减号的CSS属性时，DOM要求你用驼峰命名法。*CSS属性的font-family变为DOM属性fontFamily：

	element.style.fontFamily;


问题：

* 12px和1em，单位之间的关系。

*只有把CSS style属性插入到标记里，才可以用DOM style属性去查询那些信息*也就是说把内嵌样式放到css文件使用link的方式就不能使用了。例如，书中来自外部文件style.css的样式已经不能再使用DOM style属性检索出来了。*放到'<'style>标签里也是同样的结果*

使用DOM设置的样式，就可以用DOM再把它们检索出来。

### 9.2.2 设置样式



## 9.3 何时该用DOM脚本设置样式

### 9.3.1 根据元素在节点树里的位置来设置样式

**主要的三种方法：**

* 第一种：为标签元素(eg:p)统一声明

eg:

	p{
		font-size: 1em;
	}

* 第二种：为特定class属性的所有元素统一声明

eg:

	.fineprint {
		font-size: 1.2em;
	}

* 第三种：为有独一无二的id属性的元素单独声明

eg:

	#intro {
		font-size: 1.2em;
	}

### 9.3.2 根据某种条件反复设置某种样式
表格型数据'<'table>

### 9.3.3 响应事件
如果想改变某个元素的呈现效果，使用CSS；如果想改变某个元素的行为，使用DOM；

### 9.4 className属性

# 第10章 用JavaScript实现动画效果

## 10.1 动画基础知识
简单的说，动画就是让元素的位置随着时间而不断地发生变化。

### 10.1.1 位置
position属性

* static：默认值，有关元素将按它们在标记里出现的先后顺序出现在浏览器窗口里。
* fixed：
* relative：和static类型，另外还可以通过应用float属性从文档的正常显示顺序里脱离出来（不明所以）
* absolute：把它摆放到容纳他的“容器”的任何位置。这个容器要么是文档本身（document），要么是一个有着fixed或absolute属性的父元素。


### 10.1.2 时间

延迟函数

	setTimeout("function", interval)

取消函数

	clearTimeout(variable)

其中variable是setTimeout接口调用的返回值。

### 10.1.3 时间递增量

	parseInt

### 10.1.4 抽象


## 10.2 实用的动画

Web Content Accessibility Guidelines(Web内容可访问性指南)7.2节建议：

> 除非浏览器允许用户“冻结”移动着的内容，否则就应该避免让内容在页面中移动。（优先级2）。如果页面上有移动着的内容，就应该用脚本或插件的机制允许用户冻结这种移动或动态更新行为。


### 10.2.1 提出问题

### 10.2.2 解决问题

### 10.2.3 CSS

css的overflow属性：用来处理一个元素的尺寸超出其容器尺寸的情况。

overflow可取值：

* visible：不裁剪溢出的内容。
* hidden：隐藏溢出的内容。
* scroll：类似于hidden，浏览器将对溢出的内容进行隐藏，但显示一个滚动条以便让用户能够看到内容的其他部分。
* auto：类似于scroll，但浏览器只在确实发生溢出时才显示滚动条。如果内容没有溢出，就不显示滚动条。


### 10.2.5 变量作用域问题


JavaScript允许我们为元素创建属性

> 注意：要注意这一点。如果自定义属性和自带的属性名相同会覆盖了，难道要记住所有属性名？如果很多的话，比较蛋疼。


### 10.2.6 改进动画效果

数据函数

	// 上取整
	Math.ceil(number)
	// 下取整
	Math.floor(number)
	// 将把任意浮点数舍入与之最接近的整数(四舍五入)
	Math.round(number)


### 10.2.7 添加安全检查

### 10.2.8 生成HTML标记


# 第11章 HTML5


