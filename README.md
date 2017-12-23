## 基于 Qt 的图片尺寸转换工具


![效果图](http://upload-images.jianshu.io/upload_images/606686-fa4cee68d06828b0.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
    
本质是辅以图形界面使用imagemagick实现。

### 背景

在高分屏横飞的今天，开发软件难免会需要弄多套图片( 1x 2x 3x ... )，那么问题来了，如何快速高效的实现多套图片呢？于是乎此程序应运而生。

### 功能


1. 进行图片尺寸转换
 
### 操作详解

* **拖拽**或者点击**添加按钮**添加图片；
* 选取目标文件夹。如果设置该选项，则所有图片生成到该目录下。默认是图片所在的路径，如果选取的图片分布在不同路径下，那么对应在不同路径下生成图片；
* 调整文件名前缀，后缀，扩展名；
* 选择倍率；
* 调整最小的1.0x下的尺寸；
* 调整缩放模式：保持比例是则不拉伸图片，否则拉伸图片；
* 点击执行按钮执行


### 基于 **imagemagick**

[官网](https://www.imagemagick.org/)
[Github](https://github.com/ImageMagick/ImageMagick)

#### mac 安装

    brew install imagemagick

### 其他

[官网](https://www.imagemagick.org/script/download.php)


