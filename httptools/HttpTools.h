/*
Copyright 2020 flaot All rights reserved.
This library applies to https://www.comblockengine.com

Desc: 这是一个KBEngine下的轻量级解析http协议的库(参考python库_httptools实现 Github:https://github.com/MagicStack/httptools)
Dependencies:https://github.com/nodejs/http-parser
*/

#ifndef _SCRIPT_HTTP_TOOLS_H
#define _SCRIPT_HTTP_TOOLS_H

#include "common/common.h"

namespace KBEngine {
    namespace script {
        namespace httptools {
            extern PyObject *PyExc_HttpParserError;
            extern PyObject *PyExc_HttpParserCallbackError;
            extern PyObject *PyExc_HttpParserInvalidStatusError;
            extern PyObject *PyExc_HttpParserInvalidMethodError;
            extern PyObject *PyExc_HttpParserInvalidURLError;
            extern PyObject *PyExc_HttpParserUpgrade;


            /** 安装HttpTools模块 */
            bool installModule();
            bool uninstallModule();
        }
    }
}


#endif //!_SCRIPT_HTTP_TOOLS_H