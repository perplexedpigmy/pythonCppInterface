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
#include <iostream>
#include <vector>
#include "py.h"

using namespace std;

int main(int /*argc*/, char *argv[])
{
    /*****************************************************************************************
    * Example 1. Execute a file of python script. No parameter passing. Just a call.
    * ***************************************************************************************/
    PyIf *py=PyIf::getInstance(argv[0]);
    py->run("from time import time,ctime\n"
            "print('Today is', ctime(time()))\n");
    py->runScriptFile("./scripts/time.py");

    /*****************************************************************************************
    * Example 2. Read in Python as a 'plugin' - a set of Python callable functions to which 
    * we can pass parameters by value and receive a return parameter.
    * 
    * This example also shows how the called Python can then call C functions
    * ***************************************************************************************/

    //Add our scripts directory to the search path used for loading plugin modules
    py->addPluginSearchPath("./scripts/plugins/");
    
    //Test maths plugin
    PyIf::LoadScript mathPlugin("maths");
    PyIf::FunctionParameter returnLong = 0L;
    PyIf::FunctionParameter returnDec = 0.0;
    vector<PyIf::FunctionParameter> params;
    params.push_back(-7L);
    params.push_back(4L);
    int val=0;
    if ((val=mathPlugin.callPythonFunction("multiply", &returnLong, params))==0)
        cout << "multiply -7*4 returned " << get<long>(returnLong) << endl; 
    else
        cerr << "loadScript.callPythonFunction returned " << val << endl;

    if ((val=mathPlugin.callPythonFunction("divide", &returnLong, params))==0)
        cout << "divide -7/4 returned (long)" << get<long>(returnLong) << endl; 
    else
        cerr << "loadScript.callPythonFunction returned " << val << endl;

    if ((val=mathPlugin.callPythonFunction("divide", &returnDec, params))==0)
        cout << "divide -7/4 returned (double)" << get<double>(returnDec) << endl; 
    else
        cerr << "loadScript.callPythonFunction returned " << val << endl;

    if ((val=mathPlugin.callPythonFunction("add", &returnLong, params))==0)
        cout << "add -7+4 returned " << get<long>(returnLong) << endl; 
    else
        cerr << "loadScript.callPythonFunction returned " << val << endl;

    //Decimal Values via Double
    params.clear();
    params.push_back( -7.2);
    params.push_back( 4.3);

    if ((val=mathPlugin.callPythonFunction("multiply", &returnDec, params))==0)
        cout << "multiply returned " << get<double>(returnDec) << endl; 
    else
        cerr << "loadScript.callPythonFunction returned " << val << endl;

    if ((val=mathPlugin.callPythonFunction("divide", &returnDec, params))==0)
        cout << "divide returned " << get<double>(returnDec) << endl; 
    else
        cerr << "loadScript.callPythonFunction returned " << val << endl;

    if ((val=mathPlugin.callPythonFunction("add", &returnDec, params))==0)
        cout << "add returned " << get<double>(returnDec) << endl; 
    else
        cerr << "loadScript.callPythonFunction returned " << val << endl;
    
    //Test file plugin
    PyIf::LoadScript filePlugin("file");
    params.clear();
    params.push_back( "./scripts/plugins/file.py");
    PyIf::FunctionParameter returnStr = "";
    if ((val=filePlugin.callPythonFunction("cat", &returnStr, params))==0)
        cout << "cat returned " << get<std::string>(returnStr) << endl; 
    else
        cerr << "loadScript.callPythonFunction returned " << val << endl;

    params.clear();
    params.push_back( "ls");
    cout << "run cmd(\"ls\")\n";
    if ((val=filePlugin.callPythonFunction("cmd", NULL, params))!=0)
        cerr << "loadScript.callPythonFunction returned " << val << endl;

    return 0;
}
