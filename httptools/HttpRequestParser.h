/*
Copyright 2020 flaot All rights reserved.
This library applies to https://www.comblockengine.com

Desc: 这是一个KBEngine下的轻量级解析http协议的库(参考python库_httptools实现 Github:https://github.com/MagicStack/httptools)
Dependencies:https://github.com/nodejs/http-parser
*/

#ifndef _SCRIPT_HTTP_REQUEST_PARSER_H
#define _SCRIPT_HTTP_REQUEST_PARSER_H

#include "httptools/HttpParser.h"

namespace KBEngine {
    namespace script {

        class ScriptHttpRequestParser : public ScriptHttpParser
        {
            /** 子类化 将一些py操作填充进派生类 */
            INSTANCE_SCRIPT_HREADER(ScriptHttpRequestParser, ScriptHttpParser)
        private:
            PyObject *_proto_on_url;

        public:
            ScriptHttpRequestParser(PyObject* protocol);
            virtual ~ScriptHttpRequestParser();

            static int cb_on_url(http_parser *parser, const char *at, size_t length);

            /**
                脚本模块对象从python中创建
            */
            static PyObject* tp_new(PyTypeObject* type, PyObject* args, PyObject* kwds);
        };
    }
}
#endif
