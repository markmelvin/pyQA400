// TestQA400API.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdlib.h>
#include "QA400API.h"
#include <Windows.h> // For Sleep()

bool connect_to_analyzer(void)
{
	int timeout = 100;
	while (timeout > 0)
	{
		if (QA400API::IsConnected())
		{
			return TRUE;
		}
		Sleep(100);
		timeout--;
	}
	return FALSE;
}

int _tmain(int argc, _TCHAR* argv[])
{
	if (!connect_to_analyzer())
	{
		printf("Failed to connect to analyzer. Exiting.\n");
		exit(1);
	}

	Sleep(2000);
	unsigned int nameLength = QA400API::GetNameLength();
	if (nameLength > 0) {
		char *pName = new char[nameLength + 1];
		QA400API::GetName(pName, nameLength);
		printf("%s\n", pName);
		delete [] pName;
	}
	printf("Channel type left out: %d\n", QA400API::ChannelType::LeftOut);

	printf("Calculating THD percent... ");
	QA400API::SetGenerator(QA400API::Gen1, false, -10, 1000);
	QA400API::SetGenerator(QA400API::Gen2, false, -10, 1000);
	Sleep(500);
	QA400API::SetGenerator(QA400API::Gen1, true, -10, 1000);
	Sleep(500);
	QA400API::RunSingle();
	while (QA400API::GetAcquisitionState() == QA400API::AcquisitionState::Busy)
		;

	PointFVector data;
	data.length = QA400API::GetLastDataLength(QA400API::ChannelType::LeftIn);
	data.values = (PointF *) malloc(sizeof(PointF) * data.length);
	QA400API::GetData(QA400API::ChannelType::LeftIn, &data);

	// Compute THD assuming 1 KHz signal and look at harmonics up to 20KHz
	double thdpct1 = QA400API::ComputeTHDPctOnLastData(QA400API::ChannelType::LeftIn, 1000, 20000);
	printf("%f%%\n", thdpct1);
	double thdpct2 = QA400API::ComputeTHDPct(&data, 1000, 20000);
	printf("%f%%\n", thdpct2);
	double pwr = QA400API::ComputePowerDB(&data);
	printf("%fdB\n", pwr);
	pwr = QA400API::ComputePeakPowerDB(&data);
	printf("%fdB\n", pwr);
	QA400API::RunSingleFR(0.65);
	free(data.values);

	return 0;
}

