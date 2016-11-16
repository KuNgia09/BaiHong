#工程说明

2016.11.15 by jack

```
使用zlib进行解压图片文件
```




## zlib的安装与使用



一 使用静态库
在vc6下，需要把zlib.h、zconf.h和从zlib工程中生成的zlib.lib文件放到你的代码目录下，并且设置项目属性：project->settings->link->category选择input，在library modules中的最后面输入一个空格和zlib.lib，在lgnore libraries输入MSVCRT（否则这个默认库和zlib库会有冲突），然后project-> settings-> c/c++->Category中选择Code   generation，在Use   run-time   Library中选择Debug Multithreaded DLL
如果library modules中输入zlib.lib了，那么在代码中添加
#include "zlib.h"即可，否则还要加入下面这句
#pragma comment( lib, "zlib.lib")

二 使用动态库
同样，把zlib.h、zconf.h和从zlib工程中生成的zlib1.lib和zlib1.dll复制到自己的源程序文件夹中，代码中添加
#include "zlib.h"
#pragma comment( lib, "zlib1.lib")

vc++使用zlib的一个小例子:http://www.cppblog.com/woaidongmao/archive/2009/09/07/95490.html

使用zlib解压zip压缩文件 http://www.cnblogs.com/menlsh/p/4480577.html




## 修改vc6.0编写的程序为当前系统使用的风格
[vc6.0修改控件风格](http://blog.csdn.net/lynnbest/article/details/11630735)

##文本编辑框拖拽的使用
[VC实现文件拖拽功能](http://blog.csdn.net/zyw_anquan/article/details/8589334)



