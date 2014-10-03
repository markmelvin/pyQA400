#pragma once

#include <string>

// .Net System Namespaces
using namespace System;
using namespace System::Collections;
using namespace System::Threading;
using namespace System::Runtime::InteropServices;
using namespace System::Reflection;

// C# Projects
using namespace Com::QuantAsylum;


// A managed class that holds a singleton-like reference
// to the QA400Interface managed object, and provides other
// static methods involved with initialization and
// Assembly resolution.
ref class QA400Application
{
public:
	static QA400Interface^ getAnalyzer();
	static void addToPath(char* path);
private:
	static QA400Interface^ analyzer;
	static bool has_custom_resolver = false;
	static bool has_resolved_connectionmanagerdll = false;
	static ArrayList^ lookupPaths = gcnew ArrayList();
	static ArrayList^ resolvedPaths = gcnew ArrayList();
	static Assembly^ AssemblyResolveEventHandler(Object^ sender, ResolveEventArgs^ args);
};


//////////////////////////////////////////////////
// String Conversion Functions

inline
String^ ToManagedString(const char* pString) {
	return Marshal::PtrToStringAnsi(IntPtr((char *) pString));
}

inline
const std::string ToStdString(String^ strString) {
	IntPtr ptrString = IntPtr::Zero;
	std::string strStdString;
	try {
		ptrString = Marshal::StringToHGlobalAnsi(strString);
		strStdString = (char *) ptrString.ToPointer();
	}
	finally {
		if (ptrString != IntPtr::Zero) {
			Marshal::FreeHGlobal(ptrString);
		}
	}
	return strStdString;
}
