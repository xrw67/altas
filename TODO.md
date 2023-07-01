# TODO

log类

BBT_OPTION_HARDENED

- 插件机制
  - 对比分析
    - ys  
    - containerd
    - lkm
  
  - 插件管理器
  - 编写Helloworld插件


- StrFmt
- StrJoin
- StrSplit

- Mutex MutexGuard


## 插件机制

加载插件


  必须要有init函数
    init返回错误吗
  必须要有exit函数
  带有依赖关系
    依赖存在
    依赖不存在 -> 返回错误病告知依赖的插件是哪个。
  
卸载插件

从目录中加载所有插件
  自动解决依赖
