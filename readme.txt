This GitHub repository allows you to script the QuantAsylum QA400 audio analyzer
(http://www.quantasylum.com/content/Products/QA400.aspx) using Python!

First off, you'll need to install the QA400 USB drivers and the scope application from
QuantAsylum.

Installing the QA400 Application and Drivers
--------------------------------------------

- Download and install the .NET Framework v4.0 (required by the QA400 application) here,
  if required (note that it is already included in Windows 8.x):

    http://www.microsoft.com/en-us/download/details.aspx?id=17851

- Download and install the QA400 scope application from this page:

    http://www.quantasylum.com/content/Support/Downloads.aspx

  NOTE: I have been using the latest release candidate (v1.0696)

- Plug in your QA400, and wait for the drivers to successfully install
- Run the QA400 application and verify that your analyzer works by clicking "Press to Run"
  (you may need to initially unplug and re-plug the analyzer - look for "Connected" in the
   status bar of the application)


Pre-Requisites
--------------

Required to make this work is:

Python         http://python.org/
                 Tested with version 3.3.3

And if you are building from scratch:

Cython         http://cython.org/
                 Used/tested with version 0.20

Visual Studio  http://www.visualstudio.com/en-us/products/visual-studio-express-vs.aspx
                 I am using VS Express v2013 for Desktop

I have built and tested this with Windows 7 and Windows 8.1 (both 64-bit)

If you are using the pre-built binaries from the "binaries" folder of this repository, skip
down to "Testing the Python Application"

If you are building everything from scratch - let's move onto building the Python bindings.


Build and Installation Instructions
-----------------------------------

Here are the installation and build instructions:

- Install Python 3.3
- Install Cython 0.20
- Install Microsoft Visual Studio Express 2013 for Desktop

To build the C++/C# bridge .dll using Microsoft Visual Studio:

- Run Visual Studio and open the QA400API.sln solution
- Right-click the QA400API project and select Properties
- Under Common Properties > References, you'll likely need to remove and re-add a
  reference to the QAAnalyzer application. Browse to wherever you installed it and
  add the reference to QAAnalyzer.exe.
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

You'll notice that the build.bat script copied both the QAConnectionManager.dll as well as
the QAAnalyzer.exe to the Python directory. This appears to be required unless you want to
mess with the C# Global Assembly Cache (GAC). An alternative approach would be to make a small
py2exe'd "launcher" application and keep the QAConnectionManager.dll and QAAnalyzer.exe in the
same folder as that launcher .exe - but I have yet to dig more into this to properly figure
this out.

For now, because the Python script is launched by "python.exe", the QAConnectionManager.dll
and a copy of the QAAnalyzer.exe application must reside in that folder to be automatically
found by .NET, or it must be installed in the GAC. Again, this will be fixed eventually such
that neither file needs to be copied to the Python directory.

So, if you did not build from scratch, but went for the binaries approach, you must do the
following manually before attempting to run any of the test applications:

- Copy QAConnectionManager.dll from the "dependencies" folder in this repository, and
  QAAnalyzer.exe from your installed application directory to your Python folder alongside
  python.exe
- Copy pyQA400.pyd and QA400API.dll from the "binaries" folder in this repository to the
  same folder as the Python script you are attempting to run

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

TODO
----

- Implement a wrapper around the QAConnectionManager::AddSearchPath API function
- Fix things such that you do not need to copy the QAConnectionManager.dll or the QAAnalyzer.exe
  to the Python directory

Known Issues
------------

- There is a bug in the QA400 API that prevents the ComputePhase API from working properly
  when the analyzer is set to 192 KHz sampling


