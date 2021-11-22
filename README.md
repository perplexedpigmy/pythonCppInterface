# pythonCppInterface
Interface class for Cpp to call Python and vice versa

# build
```
cmake .
make
```

# run
```
bin/epc
```

# Source Files
**main.cpp** The Example C++ program with examples of how to call the Python scripts

**py.c/py.h** Class PyIf. This is a wrapper to simplyfy the parameterised calls to python functions that get loaded in Python script files that become 'plugins' to the C++ program calling them.

**py2cEx.cpp/h** This is an example of a set of C Functions callable from the Python scripts.

# Call Interface C++ to Python

First we add our plugin directory to the Python search path for loadable scripts
```
    py->addPluginSearchPath("./scripts/plugins/");
```

Then we create an instance of our interface class and supply the name of the Python script plugin to load
'''
    PyIf::LoadScript mathPlugin("maths");
'''

This loads the script file './scripts/plugins/maths.py'. As example we then call function 'multiply' from the Python.
```
def multiply(a,b):
   c = a*b;
   print(a,"*",b,"=",c)
   return c
```
This function requires two numerical parameters and returns the product of them. So we need to pass two parameters to be mltiplied. For our class wrapper we create a vector of varients, one per parameter. To pass integer values we push 'long' values
```
    params.push_back(-7L);
    params.push_back(4L);
```
For decimal values we would push doubles
```
    params.push_back( -7.2);
    params.push_back( 4.3);
```
We define the type of return parameter (long) or (double)
```
    PyIf::FunctionParameter returnLong = 0L;
    PyIf::FunctionParameter returnDec = 0.0;
```
Then we call the Python function
```
    if ((val=mathPlugin.callPythonFunction("multiply", &returnLong, params))==0)
        cout << "multiply -7*4 returned " << get<long>(returnLong) << endl; 
    else
        cerr << "loadScript.callPythonFunction returned " << val << endl;
```

#Call Interface Python to C++
Within our Python class we have the option to reference the C Interface defined in 'py2cEx.h'
```
PyIf::PyIf(const char *progname)
{
    . . .

    //Python to C interface - If Required
    addcInterface();

    . . . 
```
This defines our C Interface as 'cIntf' to the Python script, which gets imported by the Python via an 'import cIntf' statement at the top of the Python script. In our example, we define 3 C methods:
- system
- addInt
- addDec

Here is the C code that defines this
```
static PyMethodDef cMethods[] = {
    {"system", systemcall, METH_VARARGS, "Execute a shell command"},
    {"addInt", addInt, METH_VARARGS, "addInt two integers"},
    {"addDec", addDouble, METH_VARARGS, "addInt two decimal numbers"},
    {NULL, NULL, 0 , NULL} /* Marks the end of the list of callable function */
};

static struct PyModuleDef cModule = {
    PyModuleDef_HEAD_INIT, "cIntf", NULL, -1, cMethods, NULL, NULL, NULL, NULL
};
```

Here is the Python script that calls the 'addInt' or 'addDec' C functions based on the type of parameter value used to call  it.
```
import cIntf

def add(a,b):
   if isinstance(a,int) and isinstance(b,int):
      c = cIntf.addInt(a,b)
   else:
      c = cIntf.addDec(a,b)
   print(a,"+",b,"=",c)
   return c
```

Running the example will test that the code works. The class PyIf currently supports parameters of type
- Integer
- Decimal
- String

This needs/will be extended in time to support other variable tyes and as such this class hides the complexity of calling Python from within C++.


