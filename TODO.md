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

module对象
loader  负责从目录中加载，加载完成的结果是module_info数组(LoadAll)
manager 把Module对象管理起来

1. loader起头，加载所有模块
2. 期间调用manager去

DllModuleLoader
MockModuleLoader
ModuleManager

插件管理：管理依赖关系
插件加载则：
  加载：dll -> BBT_MODULE_HEADER
  卸载RA


从  dll -> Module，由Loader完成
  检查插件是否合法
  解决依赖
  初始化函数
  DllModuleLoader
  FakeModuleLoader

ModuleLoader
  LoadAllModules
  Load
  UnloadAllModules
  Unload
```
1. 创建一个ModuleFactory，加载所有模块，每个模块是一个Module接口
2. 
```

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
