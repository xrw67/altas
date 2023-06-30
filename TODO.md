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

// 加载插件
//  正常的插件
//  缺少字段的插件
//  检查init参数param
//  
//  检查依赖

//  卸载插件
//      没有退出函数的插件，无法卸载

//   检查依赖关系


// Builtin函数不在ModuleManager的管辖范围内，只是借用一下插件的架构而已。

