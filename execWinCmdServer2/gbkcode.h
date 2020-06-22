#ifndef GBKCODE_H
#define GBKCODE_H

/**
  **
关于中文编码 中文乱码
1.如果程序中内存使用的是unicode,那么汉字必须通过某种形式翻译成unicode，也就是某种码流转换成unicode。
utf8转换成gbk必须通过unicode。就是两种不同的编码必须通过中间介质unicode来操作
2.QString 就是unicode编码
3.FE  BB  FB是utf-8保存格式带BOM的开头前三个字节
4.unicode和utf-8是两种不同编码格式，存在一定的差异，比如严字在两个不同的编码16进制值是不同的。

  **/


#endif // GBKCODE_H
