/* 
Copyright 2020 flaot All rights reserved.
This library applies to https://www.comblockengine.com

Desc: 这是一个KBEngine下的轻量级解析http协议的库(参考python库_httptools实现 Github:https://github.com/MagicStack/httptools)
Dependencies:https://github.com/nodejs/http-parser
*/

#ifndef PY_HTTP_FUNC_H
#define PY_HTTP_FUNC_H

#include "common/common.h"
#include "pyscript/scriptobject.h"


namespace KBEngine {
    namespace script {

        class Script;

        class PyHttpFunc
        {
        public:
            /**
                初始化
            */
            static bool initialize(PyObject *pModule);
            static void finalise(void);

            static PyObject* __py_parse_url(PyObject* self, PyObject* args);


        private:
            static bool	isInit; // 是否已经被初始化

        };

    }
}

#endif // PY_HTTP_FUNC_H