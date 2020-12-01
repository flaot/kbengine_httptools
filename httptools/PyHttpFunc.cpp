/*
Copyright 2020 flaot All rights reserved.
This library applies to https://www.comblockengine.com

Desc: 这是一个KBEngine下的轻量级解析http协议的库(参考python库_httptools实现 Github:https://github.com/MagicStack/httptools)
Dependencies:https://github.com/nodejs/http-parser
*/

#include "pyscript/script.h"
#include "PyHttpFunc.h"

#include "httptools/http_parser.h"
#include "HttpTools.h"

namespace KBEngine {
    namespace script {
        bool PyHttpFunc::isInit = false;


        //-------------------------------------------------------------------------------------
        bool PyHttpFunc::initialize(PyObject *pModule)
        {
            if (isInit)
                return true;

            isInit = true;

            // 注册产生uuid方法到py
            APPEND_SCRIPT_MODULE_METHOD(pModule, parse_url, __py_parse_url, METH_VARARGS, 0);
            return isInit;
        }

        //-------------------------------------------------------------------------------------
        void PyHttpFunc::finalise(void)
        {
            if (!isInit)
                return;

            isInit = false;
        }

        PyObject* PyHttpFunc::__py_parse_url(PyObject* self, PyObject* args)
        {
            if (PyTuple_Size(args) != 1)
            {
                PyErr_SetString(PyExc_TypeError, "args != 1 error!\n");
                PyErr_PrintEx(0);
                S_Return;
            }

            SCRIPT_ERROR_CHECK(); //开始解析前先检查下错误,下面将做支持str及bytes类型参数传递

            int ret = -1;
            char* surl = NULL;
            Py_ssize_t surl_length = 0;
            ret = PyArg_ParseTuple(args, "s", &surl);
            if (surl == NULL || ret == -1)
            {
                PyErr_Clear();
                PyObject *pyObj;
                ret = PyArg_ParseTuple(args, "O", &pyObj);
                if (ret != -1 && PyBytes_Check(pyObj))
                {
                    if (PyBytes_AsStringAndSize(pyObj, &surl, &surl_length) < 0)
                    {
                        SCRIPT_ERROR_CHECK();
                        return NULL;
                    }
                }
                else
                {
                    PyErr_Format(PyExc_AssertionError, "PyHttpFunc::parse_url: args error!");
                    PyErr_PrintEx(0);
                    return NULL;
                }
            }
            else
            {
                surl_length = strlen(surl);
            }

            http_parser_url parsed;
            http_parser_url_init(&parsed);

            int res = http_parser_parse_url(surl, surl_length, 0, &parsed);
            if (!res)
            {
                PyObject* pyargs = PyTuple_New(7);

                uint16_t off, len;
                if (parsed.field_set & (1 << UF_SCHEMA))
                {
                    off = parsed.field_data[UF_SCHEMA].off;
                    len = parsed.field_data[UF_SCHEMA].len;
                    PyTuple_SET_ITEM(pyargs, 0, PyUnicode_FromStringAndSize(surl + off, len));
                }
                if (parsed.field_set & (1 << UF_HOST))
                {
                    off = parsed.field_data[UF_HOST].off;
                    len = parsed.field_data[UF_HOST].len;
                    PyTuple_SET_ITEM(pyargs, 1, PyUnicode_FromStringAndSize(surl + off, len));
                }
                if (parsed.field_set & (1 << UF_PORT))
                {
                    PyTuple_SET_ITEM(pyargs, 2, PyLong_FromLong(parsed.port));
                }
                if (parsed.field_set & (1 << UF_PATH))
                {
                    off = parsed.field_data[UF_PATH].off;
                    len = parsed.field_data[UF_PATH].len;
                    PyTuple_SET_ITEM(pyargs, 3, PyUnicode_FromStringAndSize(surl + off, len));
                }
                if (parsed.field_set & (1 << UF_QUERY))
                {
                    off = parsed.field_data[UF_QUERY].off;
                    len = parsed.field_data[UF_QUERY].len;
                    PyTuple_SET_ITEM(pyargs, 4, PyUnicode_FromStringAndSize(surl + off, len));
                }
                if (parsed.field_set & (1 << UF_FRAGMENT))
                {
                    off = parsed.field_data[UF_FRAGMENT].off;
                    len = parsed.field_data[UF_FRAGMENT].len;
                    PyTuple_SET_ITEM(pyargs, 5, PyUnicode_FromStringAndSize(surl + off, len));
                }
                if (parsed.field_set & (1 << UF_USERINFO))
                {
                    off = parsed.field_data[UF_USERINFO].off;
                    len = parsed.field_data[UF_USERINFO].len;
                    PyTuple_SET_ITEM(pyargs, 6, PyUnicode_FromStringAndSize(surl + off, len));
                }
                return pyargs;
            }
            PyErr_Format(httptools::PyExc_HttpParserInvalidURLError, "invalid url {!r}", surl);
            return NULL;
        }
    }
}