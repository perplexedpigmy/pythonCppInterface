/* MIT License

Copyright (c) 2021 nalaphandee

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#define PY_SSIZE_T_CLEAN
extern "C" {
#include <Python.h>
}
#include "py2cEx.h" // Only required if you want the Python scripts to call back the C application.
#include "py.h"

using namespace std;

static wchar_t *program;

PyIf *PyIf::getInstance(const char *progname)
{
    static PyIf *instance = new PyIf(progname);
    return instance;
}

PyIf::PyIf(const char *progname)
{
    program = Py_DecodeLocale(progname, NULL);
    if (program == NULL) {
        perror("Failed to obtain the program name.");
        exit(1);
    }
    
    //Python to C interface - If Required
    addcInterface();

    Py_SetProgramName(program);
    
    Py_Initialize();
}

PyIf::~PyIf()
{
    if (Py_FinalizeEx() < 0) {
        exit(120);
    }
    PyMem_RawFree(program);
}

//Run P1 as a Python program script
void PyIf::run(const std::string &script)
{
    PyRun_SimpleString(script.c_str());
}

//Run a Python script from a file
int PyIf::runScriptFile(const string filePath)
{
    const int closeFileOnExit=1;
    FILE *fp = fopen(filePath.c_str(),"r");
    if (fp != NULL) {
        return PyRun_SimpleFileEx(fp, filePath.c_str(), closeFileOnExit);
    }
    return -1;
}

/**************************************************************************************************/
//Load a Python 'plugin' script that can contain multiple functions that we can call as required.

//Creates a search path for the location of plugin scripts that gets used by LoadScript.
void PyIf::addPluginSearchPath(const char *path)
{
    PyObject *sysPath = PySys_GetObject((char *) "path");
    PyList_Append(sysPath, PyUnicode_FromString(path));
}

class LoadScriptPrivate {
public:
    PyObject *fp = NULL;
};

//Loads a plugin Python script
PyIf::LoadScript::LoadScript(const char *script)
{
    data = new LoadScriptPrivate();
    PyObject *scriptName = PyUnicode_FromString(script);
    data->fp = PyImport_Import(scriptName);
    Py_DECREF(scriptName);
}
PyIf::LoadScript::~LoadScript()
{
    if (data->fp != NULL)
        Py_DECREF(data->fp);
    delete data;
}

//Executes the specified function from within the Plugin Python Script.
int PyIf::LoadScript::callPythonFunction(const char *funcName, FunctionParameter *rtnParam, const std::vector <FunctionParameter> &callParams)
{
    if (data->fp==NULL) {
        PyErr_Print();
        return 1;
    }
    PyObject *pFunction = PyObject_GetAttrString(data->fp, funcName);
    PyObject *pCallParams = NULL;
    if (pFunction) {
        if (!PyCallable_Check(pFunction)) {
            Py_DECREF(pFunction);
            return 2;
        }
    } else
        return 3;
    if (callParams.size()>0) {
        pCallParams = PyTuple_New(callParams.size());
        int paramIdx = 0;
        for (FunctionParameter param : callParams)
        {
            PyObject *pythonParamVal;
            if (holds_alternative<long>(param)) {
                if (!(pythonParamVal = PyLong_FromLong(get<long>(param)))) {
                    // Cannot Convert Argument
                    Py_DECREF(pCallParams);
                    return 4;
                }
                PyTuple_SetItem(pCallParams, paramIdx++, pythonParamVal);
            } else if (holds_alternative<double>(param)) {
                if (!(pythonParamVal = PyFloat_FromDouble(get<double>(param)))) {
                    // Cannot Convert Argument
                    Py_DECREF(pCallParams);
                    return 5;
                }
                PyTuple_SetItem(pCallParams, paramIdx++, pythonParamVal);
            } else if (holds_alternative<string>(param)) {
                string &val=get<string>(param);
                if (!(pythonParamVal = PyBytes_FromString(val.c_str()))) {
                    // Cannot Convert Argument
                    Py_DECREF(pCallParams);
                    return 5;
                }
                PyTuple_SetItem(pCallParams, paramIdx++, pythonParamVal);
            }
        }
    }
    PyObject *pythonReturnVal = PyObject_CallObject(pFunction, pCallParams);
    Py_DECREF(pCallParams);
    if (pythonReturnVal==NULL) {
        PyErr_Print();
        return 6;
    }
    if (rtnParam != NULL) {
        if (holds_alternative<long>(*rtnParam)) {
            *rtnParam = PyLong_AsLong(pythonReturnVal);
        } else if (holds_alternative<double>(*rtnParam)) {
            *rtnParam = PyFloat_AsDouble(pythonReturnVal);
        } else if (holds_alternative<string>(*rtnParam)) {
            *rtnParam = PyBytes_AsString(pythonReturnVal);
        }
        Py_XDECREF(pythonReturnVal);
    }
    Py_DECREF(pFunction);
    return 0;
}
