/*
Copyright 2020 flaot All rights reserved.
This library applies to https://www.comblockengine.com

Desc: 这是一个KBEngine下的轻量级解析http协议的库(参考python库_httptools实现 Github:https://github.com/MagicStack/httptools)
Dependencies:https://github.com/nodejs/http-parser
*/

#include "pyscript/script.h"
#include "HttpRequestParser.h"
#include "pyscript/py_gc.h"

namespace KBEngine {
    namespace script {

        SCRIPT_METHOD_DECLARE_BEGIN(ScriptHttpRequestParser)
        SCRIPT_METHOD_DECLARE_END()

        SCRIPT_MEMBER_DECLARE_BEGIN(ScriptHttpRequestParser)
        SCRIPT_MEMBER_DECLARE_END()

        SCRIPT_GETSET_DECLARE_BEGIN(ScriptHttpRequestParser)
        SCRIPT_GETSET_DECLARE_END()

        SCRIPT_INIT(ScriptHttpRequestParser, 0, 0, 0, 0, 0)

        ScriptHttpRequestParser::ScriptHttpRequestParser(PyObject* protocol) :ScriptHttpParser(protocol, HTTP_REQUEST)
        {
            _proto_on_url = PyObject_GetAttrString(protocol, "on_url");
            if (_proto_on_url != nullptr)
                _csettings.on_url = cb_on_url;
            else
                PyErr_Clear();

            script::PyGC::incTracing("HttpRequestParser");
        }

        ScriptHttpRequestParser::~ScriptHttpRequestParser()
        {
            script::PyGC::decTracing("HttpRequestParser");
        }

        PyObject* ScriptHttpRequestParser::tp_new(PyTypeObject* type, PyObject* args, PyObject* kwds)
        {
            if (PyTuple_Size(args) != 1)
            {
                PyErr_SetString(PyExc_TypeError, "args != 1 error!\n");
                PyErr_PrintEx(0);
                S_Return;
            }

            int ret = -1;
            PyObject* pyobj = NULL;
            ret = PyArg_ParseTuple(args, "O", &pyobj);
            if (ret == -1)
            {
                PyErr_Format(PyExc_AssertionError, "HttpParser::new: args error!");
                PyErr_PrintEx(0);
                return NULL;
            }

            ScriptHttpRequestParser* v = new ScriptHttpRequestParser(pyobj);
            return v;
        }

        int ScriptHttpRequestParser::cb_on_url(http_parser *parser, const char *at, size_t length)
        {
            ScriptHttpRequestParser *pHttpRequestParser = (ScriptHttpRequestParser *)parser->data;
            PyObject *pyobj = PyBytes_FromStringAndSize(at, length);
            PyObject* pyRet = PyObject_CallFunction(pHttpRequestParser->_proto_on_url, "O", pyobj);
            if (pyRet == NULL)
            {
                SCRIPT_ERROR_CHECK();
            }
            else
            {
                Py_DECREF(pyRet);
            }
            Py_DECREF(pyobj);
            return PyErr_Occurred() ? -1 : 0;
        }
    }
}