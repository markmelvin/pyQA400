This GitHub repository allows you to script the QuantAsylum QA400 audio analyzer
(http://www.quantasylum.com/content/Products/QA400.aspx) using Python!

First off, you'll need to install the QA400 USB drivers and the scope application from
QuantAsylum.

Installing the QA400 Application and Drivers
--------------------------------------------

- Download and install the .NET Framework v4.0 (required by the QA400 application) here,
  if required (note that it is already included in Windows 8.x and Windows 10):

    http://www.microsoft.com/en-us/download/details.aspx?id=17851

- Download and install the QA400 scope application from this page:

    http://www.quantasylum.com/content/Support/Downloads.aspx

  NOTE: I have been using the latest release candidate (v1.226)

- Plug in your QA400, and wait for the drivers to successfully install
- Run the QA400 application and verify that your analyzer works by clicking "Press to Run"
  (you may need to initially unplug and re-plug the analyzer - look for "Connected" in the
   status bar of the application)


Pre-Requisites
--------------

Required to make this work is:

Python         http://python.org/
                 Tested with version 3.3.3, 3.4.2, and 3.5.1 (all 32-bit versions)

And if you are building from scratch:

Cython         http://cython.org/
                 Used/tested with version 0.20, 0.22, and 0.23.4

Visual Studio  http://www.visualstudio.com/en-us/products/visual-studio-express-vs.aspx
                 I have used VS2010, VS Express v2013 for Desktop and VS 2015 Community

I have built and tested this with Windows 7, Windows 8.1, and Windows 10 (all 64-bit)

If you are using the pre-built binaries from the "binaries" folder of this repository, skip
down to "Testing the Python Application"

If you are building everything from scratch - let's move onto building the Python bindings.


Build and Installation Instructions
-----------------------------------

Here are the installation and build instructions:

- Install Python
- Install Cython
- Install Microsoft Visual Studio

To build the C++/C# bridge .dll using Microsoft Visual Studio:

- Run Visual Studio and open the QA400API.sln solution
- Right-click the QA400API project and select Properties
- Under Common Properties > References, you'll likely need to remove and re-add a
  reference to the QAAnalyzer application. Browse to wherever you installed it and
  add the reference to QAAnalyzer.exe.
- You probably need to fix the tools version as well, depending on your version of
  Visual Studio
- Click Build > Build Solution

If all goes well, the build should succeed and you will have a .dll and a test executable.
If you run TestQA400API.exe, it should fire up the analyzer application, do some stuff, and
exit with no issues.

Great! Now onto the Python (Cython) wrapper. To build the .pyd files (the Cython-ized Python
extension module):

- In the "python" folder, edit the set_env.bat and build.bat scripts to match your specific
  settings (in particular the paths to the Visual Studio and Python directories)
- Open a command prompt to the "python" subfolder of this respository and
  type:

  set_env
  build

Ideally, this will succeed, and all that is left to do now is test it out with a small Python
application.


Testing the Python Application
------------------------------

If you built everything from scratch, it should "just work" at this point.

If you did not build from scratch, but went for the pre-built binaries approach, you must do
the following manually before attempting to run any of the test applications:

- Copy pyQA400.pyd and QA400API.dll from the "binaries" folder in this repository to the
  same folder as the Python script you are attempting to run

- If you are attempting to run a script of your own that is not in the "python" folder
  chances are it will crash when trying to resolve the QAConnectionManager.dll as by
  default the samples augment the search path with the "dependencies" folder and the
  local script folder. To fix this, simply copy QAConnectionManager.dll from the
  "dependencies" folder in this repository to the same directory as your Python script
  and ensure you use the "AddToSearchPath" API function as shown in the samples to augment
  the .NET search path with your local script folder.


Now run:

    python QA400_test.py

Voila! You should see the analyzer application fire up and a bunch of measurements be printed
to the screen as the analyzer is stepped through various frequencies.

There are a couple other example scripts (frequency_response.py and input_versus_ouput.py) that
show some other functionality and can be used as a reference. They also have graphing
capabilities provided through pyqtgraph, if it is installed. To enable this, change the variable
SHOULD_PLOT to True in the script.

For more information on what API functions are available, refer to the comments in pyQA400.pyx
found in the python folder of this repository. For the raw QA400 API that has been wrapped, refer
to this page:

    http://www.quantasylum.com/content/Support/Developer/QA400API.aspx


Distributing Scripts
--------------------

In the future, if you want to run your own scripts on another machine, basically what you need
to do is the following:

- Install the QA400 application and drivers as described in "Installing the QA400 Application
  and Drivers"
  
- Install Python (obviously). It will need to be Python 3.4 or 3.5 unless you built your own Python
  wrapper from scratch for another version of Python.
  
- Put the following files all in the same folder somewhere:

    <your_python_script>.py
    pyQA400.pyd (or pyQA400.cp35-win32)
    QA400API.dll
    QAConnectionManager.dll

- Add the following lines to the start of your script so that QAConnectionManager.dll can
  be found at runtime:

    this_pathname = os.path.abspath(os.path.dirname(sys.argv[0]))
    pyQA400.add_to_search_path(this_pathname)

- Optionally, add another search path entry you installed the QA400 application to a non-
  standard location (not in C:\Program Files\QuantAsylum) so QAAnalyzer.exe can be found
  at runtime. For example:

    pyQA400.add_to_search_path("I:\\Programs\\QuantAsylum\\QA400")

- Now, you can run your Python script.

    
Known Issues
------------

- There is a bug in the QA400 API that prevents parts of the API from working properly
  when the analyzer is set to 192 KHz sampling. Use 48 KHz sampling.


