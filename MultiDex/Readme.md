# 项目说明
实现Dex动态加载,动态加载assets目录下的Dex文件

#assets目录：
classes2.dex:实现百度定位功能的dex
liblocSDK7.so:百度jar包所需要的so

#实现原理:
当app的dex方法超过65535之后，是无法进行安装的,为此，google提出了一种dex分包方案:`MultiDex`
-------------------------------------------------------------


#参考资料:
- [android逆向随笔之遇见MultiDex](http://drops.wiki/index.php/2016/10/26/android-multidex/)
- [Android dex分包方案](http://blog.csdn.net/jiangwei0910410003/article/details/50799573?locationNum=1&fps=1)
- [Android热更新方案Robust](http://tech.meituan.com/android_robust.html)
- [MultiDex与热修复实现原理](http://www.itdadao.com/articles/c15a96749p0.html)
- [安卓App热补丁动态修复技术介绍](https://zhuanlan.zhihu.com/p/20308548?columnSlug=magilu)
- [Android 热补丁动态修复框架小结](http://blog.csdn.net/lmj623565791/article/details/49883661)
- [Android dex注入方案](http://blog.csdn.net/vurtne_ye/article/details/39666381)
- [Android dex分包之旅](http://yydcdut.com/2016/03/20/split-dex/)


[Android外部存储权限](http://blog.csdn.net/zjbpku/article/details/25161131)
tag:^(?!System.out) tag:^(?!dalvikvm) tag:^(?!mdpservice) tag:^(?!LocSvc) tag:^(?!beacon) tag:^(?!Cloud) tag:^(?!Device)