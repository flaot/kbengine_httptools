# kbengine_httptools
基于[kbengine](https://github.com/kbengine/kbengine)及[http-parser](https://github.com/nodejs/http-parser)的kbengine插件httptools 

为[kbengine-httpserver](https://github.com/asynchronoust/kbengine-httpserver)写的kbengine内置httptools插件。参考了python的[httptools](https://github.com/MagicStack/httptools)插件

## APIs
参考 ./tips/httptools.py 

## 不同点

```
class HttpRequestParser:
    xxx
    
    def get_method(self) -> str:
    # 返回值从 bytes 改为 str 
    xxx
    
def parse_url(url: bytes) -> tuple:
    # 参数(url)支持传递 bytes 及 str 类型
    
    # 返回值从URL数据类 
    # 改为了 tuple(schema, host, port, path, query, fragment, userinfo) 

    xxx
```

## Usage
将 ./tips/httptools.py 添加进工程,但不参与编译以获得代码提示\
TODO:待完善, 一般也不会专门用这个库吧

## Compile
### Windows
windows上使用Visual Studio完成编译(以下简称VS)
#### 1.添加插件到工程
拷贝httptools目录 放置到   ./kbengine/kbe/src/lib/

使用VS打开kbengine源码解决方案,添加'httptools'项目到解决方案中 'libs'文件夹下

详细操作:右键筛选文件夹'libs'->右键添加->现有项目->'./src/lib/httptools/httptools.vcxproj')
#### 2.添加初始化代码
打开文件
**./src/lib/pyscript/script.cpp**
增加如下注释包含'httptools'的行
```
...
#include "thread/concurrency.h"
#include "httptools/HttpTools.h"//httptools的头文件
...
bool Script::install(const wchar_t *xxx, ...)
{
    ...
    PyPlatform::initialize();
    httptools::installModule();//httptools的安装
    ...
}
bool Script::uninstall()
{
    ...
    PyPlatform::finalise();
    httptools::uninstallModule();//httptools的卸载
    ...
}
```
#### 3.编译
'pyscript'项目添加'httptools'项目引用, 然后生成解决方案
   
详细操作:选中'pyscript'项目下的引用->右键添加引用->勾选'httptools'项目->确定
   
#### 4.生成时问题解决
生成解决方案时, 会报找不到xxx之类的错误;在报错的项目中添加'httptools'项目引用即可
   

### Liunx
#### 1.添加插件到工程
拷贝httptools目录 放置到   ./kbengine/kbe/src/lib/
#### 2.添加初始化代码
与Windows一致
#### 3.编译
文件 **./src/lib/Makefile** 追加进入httptools文件夹并 make 的命令文本

```
    ...
    cd xml && $(MAKE) $@
    cd httptools && $(MAKE) $@
```
以下MakeFile文件的'MY_LIBS'下增加条目  httptools

  **./src/server/baseapp/Makefile**\
  **./src/server/cellapp/Makefile**\
  **./src/server/dbmgr/Makefile**\
  **./src/server/loginapp/Makefile**\
  **./src/server/tools/bots/Makefile**\
  **./src/server/tools/interfaces/Makefile**\
  **./src/server/tools/kbcmd/Makefile**\
  **./src/server/tools/logger/Makefile**
  
示范:

```
# 下面 httptools 是新增加的,顺序无所谓
MY_LIBS =		\
	server		\
	entitydef	\
	pyscript	\
	network		\
	httptools	\
	thread
```

配置好之后到src目录去make工程
#### 4.生成时问题解决
Make中如果出现 'Error: xxx未定义的引用' 等\
在该工程目录MakeFile文件的'MY_LIBS'下增加条目  httptools\
重新 Make 即可
