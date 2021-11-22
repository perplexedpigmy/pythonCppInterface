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

/****************************************************************************************************
 * Service Functions   
 *  setPyError - Returns a Pythin style error back to the caling Python Script     
 *     exceptionObject - e.g. (PyExc_ZeroDivisionError, PyExc_TypeError, PyExc_ValueError)
 ****************************************************************************************************/
static void setPyError(PyObject *exceptionObject, const char *errMsg)
{
    PyErr_SetString(exceptionObject, errMsg);
}
/****************************************************************************************************
 * Callable Functions   
 *  systemcall - Calls system with a null terminated command string (p1). Returns void or an exception  
 *  addInt - addInt 2 integers                                                                          *
 ****************************************************************************************************/
static PyObject *systemcall(PyObject * /*self*/, PyObject *args)
{
    const char *command;

    if (PyArg_ParseTuple(args, "s", &command) != true) {
        return NULL;
    }
    int rtnStatus = system(command);
    if (rtnStatus < 0) {
        setPyError(PyExc_IOError, "syscall: Failed System Command");
        return NULL;
    }
    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *addInt(PyObject * /*self*/, PyObject *args)
{
    long val1, val2;

    if (PyArg_ParseTuple(args, "ll", &val1, &val2) != true) {
        return NULL;
    }
    return PyLong_FromLong(val1 + val2);
}

static PyObject *addDouble(PyObject * /*self*/, PyObject *args)
{
    double val1, val2;

    if (PyArg_ParseTuple(args, "dd", &val1, &val2) != true) {
        return NULL;
    }
    return PyFloat_FromDouble(val1 + val2);
}

static PyMethodDef cMethods[] = {
    {"system", systemcall, METH_VARARGS, "Execute a shell command"},
    {"addInt", addInt, METH_VARARGS, "addInt two integers"},
    {"addDec", addDouble, METH_VARARGS, "addInt two decimal numbers"},
    {NULL, NULL, 0 , NULL} /* Marks the end of the list of callable function */
};

static struct PyModuleDef cModule = {
    PyModuleDef_HEAD_INIT, "cIntf", NULL, -1, cMethods, NULL, NULL, NULL, NULL
};

static PyObject *PyInit_cInterface(void)
{
    return PyModule_Create(&cModule);
}
}

void addcInterface() {
    if (PyImport_AppendInittab("cIntf", PyInit_cInterface) == -1) {
        perror("Could not extend the built in modules table");
        exit(5);
    }
}
