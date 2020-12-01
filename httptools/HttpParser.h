/*
Copyright 2020 flaot All rights reserved.
This library applies to https://www.comblockengine.com

Desc: 这是一个KBEngine下的轻量级解析http协议的库(参考python库_httptools实现 Github:https://github.com/MagicStack/httptools)
Dependencies:https://github.com/nodejs/http-parser
*/

#ifndef _SCRIPT_HTTP_PARSER_H
#define _SCRIPT_HTTP_PARSER_H
#include "common/common.h"
#include "common/memorystream.h"
#include "httptools/http_parser.h"
#include "pyscript/scriptobject.h"

namespace KBEngine {

    namespace script {

        class ScriptHttpParser : public ScriptObject
        {
            /** 子类化 将一些py操作填充进派生类 */
            INSTANCE_SCRIPT_HREADER(ScriptHttpParser, ScriptObject)

        protected:
            http_parser _cparser;
            http_parser_settings _csettings;

        private:
            MemoryStream _current_header_name;
            MemoryStream _current_header_value;

            PyObject *_proto_on_body;
            PyObject *_proto_on_header;
            PyObject *_proto_on_headers_complete;
            PyObject *_proto_on_message_complete;
            PyObject *_proto_on_chunk_header;
            PyObject *_proto_on_chunk_complete;
            PyObject *_proto_on_message_begin;
        public:
            ScriptHttpParser(PyObject* protocol, int mode);
            virtual ~ScriptHttpParser();

            /**
                脚本模块对象从python中创建
            */
            static PyObject* tp_new(PyTypeObject* type, PyObject* args, PyObject* kwds);

            static int cb_on_message_begin(http_parser *parser);
            static int cb_on_header_field(http_parser *parser, const char *at, size_t length);
            static int cb_on_header_value(http_parser *parser, const char *at, size_t length);
            static int cb_on_headers_complete(http_parser *parser);
            static int cb_on_body(http_parser *parser, const char *at, size_t length);
            static int cb_on_message_complete(http_parser *parser);
            static int cb_on_chunk_header(http_parser *parser);
            static int cb_on_chunk_complete(http_parser *parser);

            void _maybe_call_on_header();
            void _on_header_field(const MemoryStream *field);
            void _on_header_value(const MemoryStream *field);
            void _on_headers_complete();
            void _on_chunk_header();
            void _on_chunk_complete();

            /**
                暴漏一些方法
            */
            static PyObject* __py_pyget_http_version(PyObject* self, PyObject* args);
            static PyObject* __py_pyshould_keep_alive(PyObject* self, PyObject* args);
            static PyObject* __py_pyshould_upgrade(PyObject* self, PyObject* args);
            static PyObject* __py_pyfeed_data(PyObject* self, PyObject* args);

            static PyObject* __py_pyget_method(PyObject* self, PyObject* args);
            static PyObject* __py_pyget_status_code(PyObject* self, PyObject* args);

        protected:
            static void parser_error_from_errno(unsigned int uErrno);
        };
    }
}
#endif
