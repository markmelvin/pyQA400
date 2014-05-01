// TestQA400API.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdlib.h>
#include "QA400API.h"

int _tmain(int argc, _TCHAR* argv[])
{
	unsigned int nameLength = QA400API::GetNameLength();
	if (nameLength > 0) {
		char *pName = new char[nameLength + 1];
		QA400API::GetName(pName, nameLength);
		printf("%s\n", pName);
		delete [] pName;
	}
	printf("IsConnected?: %s\n", QA400API::IsConnected() ? "TRUE" : "FALSE");
	printf("Channel type right out: %d\n", QA400API::ChannelType::RightOut);

	printf("Calculating THD percent... ");
	QA400API::SetGenerator(QA400API::Gen2, true, -10, 1000);
	QA400API::RunSingle();
	while (QA400API::GetAcquisitionState() == QA400API::AcquisitionState::Busy)
		;

	// Compute THD assuming 1 KHz signal and look at harmonics up to 20KHz
	double thdpct = QA400API::ComputeTHDPctOnLastData(QA400API::ChannelType::RightIn, 1000, 20000);
	printf("%f%%\n", thdpct);

	//QA400API::Run();
	//QA400API::Stop();
	return 0;
}

