#pragma once

#include <string>

// .Net System Namespaces
using namespace System;
using namespace System::Threading;
using namespace System::Runtime::InteropServices;

// C# Projects
using namespace Com::QuantAsylum;

/* Just testing
class PathExtender {
public:
	PathExtender() {
		// Add additional paths here
		QAConnectionManager::AddSearchPath("i:\\programs\\quantasylum\\");
	}

	~PathExtender() {
	}
};
*/

// A managed class that holds a singleton-like reference
// to the QA400Interface managed object.
ref class QA400Application
{
public:
	static QA400Interface^ getAnalyzer();
private:
	static QA400Interface^ analyzer;
};


//////////////////////////////////////////////////
// String Conversion Functions

inline
String ^ ToManagedString(const char * pString) {
	return Marshal::PtrToStringAnsi(IntPtr((char *) pString));
}

inline
const std::string ToStdString(String ^ strString) {
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
