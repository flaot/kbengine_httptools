/*
Copyright 2020 flaot All rights reserved.
This library applies to https://www.comblockengine.com

Desc: 这是一个KBEngine下的轻量级解析http协议的库(参考python库_httptools实现 Github:https://github.com/MagicStack/httptools)
Dependencies:https://github.com/nodejs/http-parser
*/

#include "pyscript/script.h"
#include "HttpParser.h"
#include "pyscript/py_gc.h"
#include "HttpTools.h"

namespace KBEngine {
    namespace script {

        SCRIPT_METHOD_DECLARE_BEGIN(ScriptHttpParser)
            SCRIPT_METHOD_DECLARE("get_http_version", pyget_http_version, METH_VARARGS, 0)
            SCRIPT_METHOD_DECLARE("should_keep_alive", pyshould_keep_alive, METH_VARARGS, 0)
            SCRIPT_METHOD_DECLARE("should_upgrade", pyshould_upgrade, METH_VARARGS, 0)
            SCRIPT_METHOD_DECLARE("feed_data", pyfeed_data, METH_VARARGS, 0)
            
            SCRIPT_METHOD_DECLARE("get_method", pyget_method, METH_VARARGS, 0)
            SCRIPT_METHOD_DECLARE("get_status_code", pyget_status_code, METH_VARARGS, 0)
        SCRIPT_METHOD_DECLARE_END()

        SCRIPT_MEMBER_DECLARE_BEGIN(ScriptHttpParser)
        SCRIPT_MEMBER_DECLARE_END()

        SCRIPT_GETSET_DECLARE_BEGIN(ScriptHttpParser)
        SCRIPT_GETSET_DECLARE_END()

        SCRIPT_INIT(ScriptHttpParser, 0, 0, 0, 0, 0)

        ScriptHttpParser::ScriptHttpParser(PyObject* protocol, int mode) : ScriptObject(getScriptType(), false)
        {
            http_parser_init(&_cparser, (http_parser_type)mode);
            _cparser.data = this;
            http_parser_settings_init(&_csettings);
            if (!(_proto_on_header = PyObject_GetAttrString(protocol, "on_header"))) PyErr_Clear();
            if (!(_proto_on_headers_complete = PyObject_GetAttrString(protocol, "on_headers_complete"))) PyErr_Clear();
            if (!(_proto_on_body = PyObject_GetAttrString(protocol, "on_body"))) PyErr_Clear();
            if (!(_proto_on_message_begin = PyObject_GetAttrString(protocol, "on_message_begin"))) PyErr_Clear();
            if (!(_proto_on_message_complete = PyObject_GetAttrString(protocol, "on_message_complete"))) PyErr_Clear();
            if (!(_proto_on_chunk_header = PyObject_GetAttrString(protocol, "on_chunk_header"))) PyErr_Clear();
            if (!(_proto_on_chunk_complete = PyObject_GetAttrString(protocol, "on_chunk_complete"))) PyErr_Clear();

            _csettings.on_headers_complete = cb_on_headers_complete;
            _csettings.on_chunk_header = cb_on_chunk_header;
            _csettings.on_chunk_complete = cb_on_chunk_complete;

            if (_proto_on_header != nullptr)
            {
                _csettings.on_header_field = cb_on_header_field;
                _csettings.on_header_value = cb_on_header_value;
            }
            if (_proto_on_body != nullptr)
                _csettings.on_body = cb_on_body;
            if (_proto_on_message_begin != nullptr)
                _csettings.on_message_begin = cb_on_message_begin;
            if (_proto_on_message_complete != nullptr)
                _csettings.on_message_complete = cb_on_message_complete;

            script::PyGC::incTracing("HttpParser");
        }

        PyObject* ScriptHttpParser::tp_new(PyTypeObject* type, PyObject* args, PyObject* kwds)
        {
            if (PyTuple_Size(args) != 2)
            {
                PyErr_SetString(PyExc_TypeError, "args != 2 error!\n");
                PyErr_PrintEx(0);
                S_Return;
            }

            int level;
            PyObject* pyobj = NULL;
            if (!PyArg_ParseTuple(args, "Oi", &pyobj, &level))
            {
                PyErr_Format(PyExc_AssertionError, "HttpParser::new: args error!");
                PyErr_PrintEx(0);
                return NULL;
            }

            ScriptHttpParser* v = new ScriptHttpParser(pyobj, level);
            return v;
        }

        ScriptHttpParser::~ScriptHttpParser()
        {
            script::PyGC::decTracing("HttpParser");
        }

        int ScriptHttpParser::cb_on_message_begin(http_parser *parser)
        {
            ScriptHttpParser *pHttpParser = (ScriptHttpParser *)parser->data;
            PyObject* pyRet = PyObject_CallFunction(pHttpParser->_proto_on_message_begin, NULL);
            if (pyRet == NULL)
            {
                SCRIPT_ERROR_CHECK();
            }
            else
            {
                Py_DECREF(pyRet);
            }
            return PyErr_Occurred() ? -1 : 0;
        }

        int ScriptHttpParser::cb_on_header_field(http_parser *parser, const char *at, size_t length)
        {
            ScriptHttpParser *pHttpParser = (ScriptHttpParser *)parser->data;
            MemoryStream memoryStream;
            memoryStream.append(at, length);
            pHttpParser->_on_header_field(&memoryStream);
            return PyErr_Occurred() ? -1 : 0;
        }

        int ScriptHttpParser::cb_on_header_value(http_parser *parser, const char *at, size_t length)
        {
            ScriptHttpParser *pHttpParser = (ScriptHttpParser *)parser->data;
            MemoryStream memoryStream;
            memoryStream.append(at, length);
            pHttpParser->_on_header_value(&memoryStream);
            return PyErr_Occurred() ? -1 : 0;
        }

        int ScriptHttpParser::cb_on_headers_complete(http_parser *parser)
        {
            ScriptHttpParser *pHttpParser = (ScriptHttpParser *)parser->data;
            pHttpParser->_on_headers_complete();
            return PyErr_Occurred() ? -1 : (pHttpParser->_cparser.upgrade ? 1 : 0);
        }

        int ScriptHttpParser::cb_on_body(http_parser *parser, const char *at, size_t length)
        {
            ScriptHttpParser *pHttpParser = (ScriptHttpParser *)parser->data;

            PyObject *pyobj = PyBytes_FromStringAndSize(at, length);
            PyObject* pyRet = PyObject_CallFunction(pHttpParser->_proto_on_body, "O", pyobj);
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

        int ScriptHttpParser::cb_on_message_complete(http_parser *parser)
        {
            ScriptHttpParser *pHttpParser = (ScriptHttpParser *)parser->data;
            PyObject* pyRet = PyObject_CallFunction(pHttpParser->_proto_on_message_complete, NULL);
            if (pyRet == NULL)
            {
                SCRIPT_ERROR_CHECK();
            }
            else
            {
                Py_DECREF(pyRet);
            }
            return PyErr_Occurred() ? -1 : 0;
        }

        int ScriptHttpParser::cb_on_chunk_header(http_parser *parser)
        {
            ScriptHttpParser *pHttpParser = (ScriptHttpParser *)parser->data;
            pHttpParser->_on_chunk_header();
            return PyErr_Occurred() ? -1 : 0;
        }

        int ScriptHttpParser::cb_on_chunk_complete(http_parser *parser)
        {
            ScriptHttpParser *pHttpParser = (ScriptHttpParser *)parser->data;
            pHttpParser->_on_chunk_complete();
            return PyErr_Occurred() ? -1 : 0;
        }

        void ScriptHttpParser::_maybe_call_on_header()
        {
            if (_current_header_value.length() != 0)
            {
                if (_proto_on_header != nullptr)
                {
                    //由于'MemoryStream'主要用于网络字节,所以此处不能直接调用成员方法'read';防止字节序被改变.
                    uint8 *read_name_begin = _current_header_name.data() + _current_header_name.rpos();
                    uint8 *read_value_begin = _current_header_value.data() + _current_header_value.rpos();

                    PyObject *pyobj_name = PyBytes_FromStringAndSize((const char *)read_name_begin, _current_header_name.length());
                    PyObject *pyobj_value = PyBytes_FromStringAndSize((const char *)read_value_begin, _current_header_value.length());
                    _current_header_name.clear(false);
                    _current_header_value.clear(false);
                    PyObject* pyRet = PyObject_CallFunction(_proto_on_header, "OO", pyobj_name, pyobj_value);
                    if (pyRet == NULL)
                    {
                        SCRIPT_ERROR_CHECK();
                    }
                    else
                    {
                        Py_DECREF(pyRet);
                    }
                    Py_DECREF(pyobj_name);
                    Py_DECREF(pyobj_value);
                }
            }
        }

        void ScriptHttpParser::_on_header_field(const MemoryStream * field)
        {
            _maybe_call_on_header();
            _current_header_name.append(*field);
        }

        void ScriptHttpParser::_on_header_value(const MemoryStream * field)
        {
            _current_header_value.append(*field);
        }

        void ScriptHttpParser::_on_headers_complete()
        {
            _maybe_call_on_header();
            if (_proto_on_headers_complete != nullptr)
            {
                PyObject* pyRet = PyObject_CallFunction(_proto_on_headers_complete, "");
                if (pyRet == NULL)
                {
                    SCRIPT_ERROR_CHECK();
                }
                else
                {
                    Py_DECREF(pyRet);
                }
            }
        }

        void ScriptHttpParser::_on_chunk_header()
        {
            if (_current_header_value.length() || _current_header_name.length())
            {
                PyErr_SetString(httptools::PyExc_HttpParserError, "invalid headers state");
                return;
            }
            if (_proto_on_chunk_header != nullptr)
            {
                PyObject* pyRet = PyObject_CallFunction(_proto_on_chunk_header, "");
                if (pyRet == NULL)
                {
                    SCRIPT_ERROR_CHECK();
                }
                else
                {
                    Py_DECREF(pyRet);
                }
            }
        }

        void ScriptHttpParser::_on_chunk_complete()
        {
            _maybe_call_on_header();
            if (_proto_on_chunk_complete != nullptr)
            {
                PyObject* pyRet = PyObject_CallFunction(_proto_on_chunk_complete, "");
                if (pyRet == NULL)
                {
                    SCRIPT_ERROR_CHECK();
                }
                else
                {
                    Py_DECREF(pyRet);
                }
            }
        }


        PyObject* ScriptHttpParser::__py_pyget_http_version(PyObject* self, PyObject* args)
        {
            if (PyTuple_Size(args) != 0)
            {
                PyErr_SetString(PyExc_TypeError, "args > 0 error!\n");
                PyErr_PrintEx(0);
                S_Return;
            }
            ScriptHttpParser* sv = static_cast<ScriptHttpParser*>(self);
            return PyUnicode_FromFormat("%d.%d", sv->_cparser.http_major, sv->_cparser.http_minor);
        }

        PyObject* ScriptHttpParser::__py_pyshould_keep_alive(PyObject* self, PyObject* args)
        {
            if (PyTuple_Size(args) != 0)
            {
                PyErr_SetString(PyExc_TypeError, "args > 0 error!\n");
                PyErr_PrintEx(0);
                S_Return;
            }
            ScriptHttpParser* sv = static_cast<ScriptHttpParser*>(self);
            return PyBool_FromLong(http_should_keep_alive(&sv->_cparser));
        }

        PyObject* ScriptHttpParser::__py_pyshould_upgrade(PyObject* self, PyObject* args)
        {
            if (PyTuple_Size(args) != 0)
            {
                PyErr_SetString(PyExc_TypeError, "args > 0 error!\n");
                PyErr_PrintEx(0);
                S_Return;
            }
            ScriptHttpParser* sv = static_cast<ScriptHttpParser*>(self);
            return PyBool_FromLong(sv->_cparser.upgrade);
        }
      
        PyObject* ScriptHttpParser::__py_pyfeed_data(PyObject* self, PyObject* args)
        {
            if (PyTuple_Size(args) != 1)
            {
                PyErr_SetString(PyExc_TypeError, "args != 1 error!\n");
                PyErr_PrintEx(0);
                S_Return;
            }
            PyObject* pyobj = NULL;
            PyArg_ParseTuple(args, "O", &pyobj);

            char* feedData = NULL;
            Py_ssize_t feedDataLength = 0;
            if (PyBytes_Check(pyobj))
            {
                if (PyBytes_AsStringAndSize(pyobj, &feedData, &feedDataLength) < 0)
                {
                    SCRIPT_ERROR_CHECK();
                    return NULL;
                }
            }
            else
            {
                PyErr_SetString(PyExc_AssertionError, "HttpParser::feed_data: args1 is not data(bytes)!\n");
                PyErr_PrintEx(0);
                return NULL;
            }
            if (feedDataLength > 0)
            {
                ScriptHttpParser* sv = static_cast<ScriptHttpParser*>(self);
                Py_ssize_t nb = (Py_ssize_t)http_parser_execute(&sv->_cparser, &sv->_csettings, feedData, feedDataLength);
                if (sv->_cparser.http_errno != HPE_OK)
                {
                    parser_error_from_errno(sv->_cparser.http_errno);
                    return NULL;
                }
                if (sv->_cparser.upgrade)
                {
                    PyErr_Format(httptools::PyExc_HttpParserUpgrade, "I", nb);
                    return NULL;
                }
                if (nb != feedDataLength)
                {
                    PyErr_SetString(httptools::PyExc_HttpParserError, "not all of the data was parsed");
                    return NULL;
                }
            }
            return Py_None;
        }

        PyObject* ScriptHttpParser::__py_pyget_method(PyObject* self, PyObject* args)
        {
            if (PyTuple_Size(args) != 0)
            {
                PyErr_SetString(PyExc_TypeError, "args > 0 error!\n");
                PyErr_PrintEx(0);
                S_Return;
            }
            ScriptHttpParser* sv = static_cast<ScriptHttpParser*>(self);
            return PyUnicode_FromFormat("%s", http_method_str((http_method)sv->_cparser.method));
        }

        PyObject* ScriptHttpParser::__py_pyget_status_code(PyObject* self, PyObject* args)
        {
            if (PyTuple_Size(args) != 0)
            {
                PyErr_SetString(PyExc_TypeError, "args > 0 error!\n");
                PyErr_PrintEx(0);
                S_Return;
            }
            ScriptHttpParser* sv = static_cast<ScriptHttpParser*>(self);
            return PyLong_FromLong(sv->_cparser.status_code);
        }


        void ScriptHttpParser::parser_error_from_errno(unsigned int uErrno)
        {
            PyObject *cls;
            const char * desc = http_errno_description((http_errno)uErrno);
            switch (uErrno)
            {
            case HPE_CB_message_begin:
            case HPE_CB_url:
            case HPE_CB_header_field:
            case HPE_CB_header_value:
            case HPE_CB_headers_complete:
            case HPE_CB_body:
            case HPE_CB_message_complete:
            case HPE_CB_status:
            case HPE_CB_chunk_header:
            case HPE_CB_chunk_complete:
                cls = httptools::PyExc_HttpParserCallbackError;
                break;
            case HPE_INVALID_STATUS:
                cls = httptools::PyExc_HttpParserInvalidStatusError;
                break;
            case HPE_INVALID_METHOD:
                cls = httptools::PyExc_HttpParserInvalidMethodError;
                break;
            case HPE_INVALID_URL:
                cls = httptools::PyExc_HttpParserInvalidURLError;
                break;
            default:
                cls = httptools::PyExc_HttpParserError;
                break;
            }
            PyErr_SetString(cls, desc);
        }
    }
}