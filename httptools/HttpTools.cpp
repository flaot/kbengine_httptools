/*
Copyright 2020 flaot All rights reserved.
This library applies to https://www.comblockengine.com

Desc: 这是一个KBEngine下的轻量级解析http协议的库(参考python库_httptools实现 Github:https://github.com/MagicStack/httptools)
Dependencies:https://github.com/nodejs/http-parser
*/

#include "pyscript/script.h"
#include "HttpTools.h"
#include "HttpParser.h"
#include "HttpRequestParser.h"
#include "HttpResponseParser.h"
#include "PyHttpFunc.h"

#define MODULE_NAME "httptools"   //模块名称

namespace KBEngine {
    namespace script {
        namespace httptools {

            PyObject *PyExc_HttpParserError = NULL;
            PyObject *PyExc_HttpParserCallbackError = NULL;
            PyObject *PyExc_HttpParserInvalidStatusError = NULL;
            PyObject *PyExc_HttpParserInvalidMethodError = NULL;
            PyObject *PyExc_HttpParserInvalidURLError = NULL;
            PyObject *PyExc_HttpParserUpgrade = NULL;
 
            //-------------------------------------------------------------------------------------
            bool installModule()
            {
                // 初始化一个HTTP相关的模块
                PyObject *httpModule = PyImport_AddModule(MODULE_NAME);
                PyObject* pyDoc = PyUnicode_FromString("This module is created by KBEngine!");
                PyObject_SetAttrString(httpModule, "__doc__", pyDoc);
                Py_DECREF(pyDoc);

                //初始化 要使用的异常类型
                PyExc_HttpParserError = PyErr_NewException(MODULE_NAME ".HttpParserError", PyExc_Exception, NULL);
                if (PyExc_HttpParserError == NULL)
                    return false;
                Py_INCREF(PyExc_HttpParserError);
                PyModule_AddObject(httpModule, "HttpParserError", PyExc_HttpParserError);
                PyExc_HttpParserCallbackError = PyErr_NewException(MODULE_NAME ".HttpParserCallbackError", PyExc_HttpParserError, NULL);
                if (PyExc_HttpParserCallbackError == NULL)
                    return false;
                Py_INCREF(PyExc_HttpParserCallbackError);
                PyModule_AddObject(httpModule, "HttpParserCallbackError", PyExc_HttpParserCallbackError);
                PyExc_HttpParserInvalidStatusError = PyErr_NewException(MODULE_NAME ".HttpParserInvalidStatusError", PyExc_HttpParserError, NULL);
                if (PyExc_HttpParserInvalidStatusError == NULL)
                    return false;
                Py_INCREF(PyExc_HttpParserInvalidStatusError);
                PyModule_AddObject(httpModule, "HttpParserInvalidStatusError", PyExc_HttpParserInvalidStatusError);
                PyExc_HttpParserInvalidMethodError = PyErr_NewException(MODULE_NAME ".HttpParserInvalidMethodError", PyExc_HttpParserError, NULL);
                if (PyExc_HttpParserInvalidMethodError == NULL)
                    return false;
                Py_INCREF(PyExc_HttpParserInvalidMethodError);
                PyModule_AddObject(httpModule, "HttpParserInvalidMethodError", PyExc_HttpParserInvalidMethodError);
                PyExc_HttpParserInvalidURLError = PyErr_NewException(MODULE_NAME ".HttpParserInvalidURLError", PyExc_HttpParserError, NULL);
                if (PyExc_HttpParserInvalidURLError == NULL)
                    return false;
                Py_INCREF(PyExc_HttpParserInvalidURLError);
                PyModule_AddObject(httpModule, "HttpParserInvalidURLError", PyExc_HttpParserInvalidURLError);
                PyExc_HttpParserUpgrade = PyErr_NewException(MODULE_NAME ".HttpParserUpgrade", PyExc_Exception, NULL);
                if (PyExc_HttpParserUpgrade == NULL)
                    return false;
                Py_INCREF(PyExc_HttpParserUpgrade);
                PyModule_AddObject(httpModule, "HttpParserUpgrade", PyExc_HttpParserUpgrade);

                //初始化 常量
                //PyModule_AddIntConstant(httpModule, "HPE_CB_url", 1);


                // 初始化ScriptHttpParser
                ScriptHttpParser::installScript(httpModule, "HttpParser");
                // 初始化ScriptHttpRequestParser
                ScriptHttpRequestParser::installScript(httpModule, "HttpRequestParser");
                // 初始化ScriptHttpResponseParser
                ScriptHttpResponseParser::installScript(httpModule, "HttpResponseParser");

                // 初始化'parse_url'函数
                PyHttpFunc::initialize(httpModule);

                return true;
            }

            //-------------------------------------------------------------------------------------
            bool uninstallModule()
            {
                ScriptHttpParser::uninstallScript();
                ScriptHttpRequestParser::uninstallScript();
                ScriptHttpResponseParser::uninstallScript();
                PyHttpFunc::finalise();
                return true;
            }

        }
    }
}