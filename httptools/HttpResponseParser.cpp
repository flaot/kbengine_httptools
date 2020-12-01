/*
Copyright 2020 flaot All rights reserved.
This library applies to https://www.comblockengine.com

Desc: 这是一个KBEngine下的轻量级解析http协议的库(参考python库_httptools实现 Github:https://github.com/MagicStack/httptools)
Dependencies:https://github.com/nodejs/http-parser
*/

#include "pyscript/script.h"
#include "HttpResponseParser.h"
#include "pyscript/py_gc.h"

namespace KBEngine {
    namespace script {

        SCRIPT_METHOD_DECLARE_BEGIN(ScriptHttpResponseParser)
        SCRIPT_METHOD_DECLARE_END()

        SCRIPT_MEMBER_DECLARE_BEGIN(ScriptHttpResponseParser)
        SCRIPT_MEMBER_DECLARE_END()

        SCRIPT_GETSET_DECLARE_BEGIN(ScriptHttpResponseParser)
        SCRIPT_GETSET_DECLARE_END()

        SCRIPT_INIT(ScriptHttpResponseParser, 0, 0, 0, 0, 0)

        ScriptHttpResponseParser::ScriptHttpResponseParser(PyObject* protocol) : ScriptHttpParser(protocol, HTTP_RESPONSE)
        {
            _proto_on_status = PyObject_GetAttrString(protocol, "on_status");

            if (_proto_on_status != nullptr)
                _csettings.on_status = cb_on_status;
            else
                PyErr_Clear();

            script::PyGC::incTracing("HttpResponseParser");
        }

        ScriptHttpResponseParser::~ScriptHttpResponseParser()
        {
            script::PyGC::decTracing("HttpResponseParser");
        }

        PyObject* ScriptHttpResponseParser::tp_new(PyTypeObject* type, PyObject* args, PyObject* kwds)
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

            ScriptHttpResponseParser* v = new ScriptHttpResponseParser(pyobj);
            return v;
        }

        int ScriptHttpResponseParser::cb_on_status(http_parser *parser, const char *at, size_t length)
        {
            ScriptHttpResponseParser *pHttpResponseParser = (ScriptHttpResponseParser *)parser->data;
            PyObject *pyobj = PyBytes_FromStringAndSize(at, length);
            PyObject* pyRet = PyObject_CallFunction(pHttpResponseParser->_proto_on_status, "O", pyobj);
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