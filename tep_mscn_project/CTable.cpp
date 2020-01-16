#include "pch.h"
#include "CTable.h"
#include <iostream>
#include <string>

CTable::CTable()
{
	i_table_size = I_DEFAULT_SIZE;
	pd_table = new double[i_table_size]();
}

CTable::CTable(int iTableLen)
{
	if (iTableLen < 0) i_table_size = I_DEFAULT_SIZE;
	else i_table_size = iTableLen;

	pd_table = new double[i_table_size]();
}

CTable::CTable(const CTable & pcOther)
{
	i_table_size = pcOther.i_table_size;
	pd_table = new double[i_table_size];

	for (int i = 0; i < i_table_size; i++)
	{
		pd_table[i] = pcOther.pd_table[i];
	}
}

void CTable::vPrint()
{
	for (int i = 0; i < i_table_size; i++)
	{
		std::cout << pd_table[i] << " \n";
	}
}

bool CTable::bSetValue(int iOffset, double dValue)
{
	bool b_is_set = false;

	if (iOffset < 0 || iOffset > i_table_size) return false;
	else if (iOffset == i_table_size)
	{
		b_is_set = bSetNewSize(iOffset + 1);

		if (b_is_set) pd_table[iOffset] = dValue;
		else return false;
	}
	else pd_table[iOffset] = dValue;

	return true;
}

bool CTable::bSetNewSize(int iTableLen)
{
	if (iTableLen < 0) return false;

	double *pd_changed_table = new double[iTableLen]();

	for (int i = 0; i < i_table_size && i < iTableLen; i++)
	{
		pd_changed_table[i] = pd_table[i];
	}

	delete[] pd_table;

	pd_table = pd_changed_table;
	i_table_size = iTableLen;

	return true;
}

double CTable::dGetValue(int iOffset)
{
	if (iOffset >= i_table_size) return 0;
	else return pd_table[iOffset];
}

int CTable::iGetSize()
{
	return i_table_size;
}

double& CTable::pdClone()
{
	return *pd_table;
}

void CTable::vFillWithNumber(double dNumberToFillTable)
{
	for (int i = 0; i < i_table_size; i++)
	{
		pd_table[i] = dNumberToFillTable;
	}
}

bool CTable::bFillWithNumber(double dNumberToFillTable, int iStartOffset, int iRange)
{
	if (iStartOffset + iRange > i_table_size || iRange < 0) return false;

	for (int i = 0; i < iRange; i++)
	{
		pd_table[i + iStartOffset] = dNumberToFillTable;
	}
	return true;
}

void CTable::operator=(const CTable &pcOther)
{
	bSetNewSize(pcOther.i_table_size);
	for (int i = 0; i < i_table_size; i++)
	{
		pd_table[i] = pcOther.pd_table[i];
	}
}

bool CTable::operator!=(const CTable & pcOther)
{

	if (i_table_size != pcOther.i_table_size) return true;

	for (int i = 0; i < i_table_size; i++)
	{
		if (pd_table[i] != pcOther.pd_table[i]) return true;
	}

	return false;
}

void CTable::vFillWithTable(double * pdTable)
{
	for (int i = 0; i < i_table_size; i++)
	{
		pd_table[i] = pdTable[i];
	}
}

bool CTable::bWrite(FILE &pcFile)
{
	for (int i = 0; i < i_table_size; i++)
	{
		fprintf(&pcFile, "%.2f ", pd_table[i]);
	}

	fprintf(&pcFile, "\n");

	return true;
}

bool CTable::bRead(FILE & pcFile)
{
	for (int i = 0; i < i_table_size; i++)
	{
		fscanf(&pcFile, "%lf", &pd_table[i]);
	}
	return true;
}

bool CTable::bGenRandom(CRandom & pcRandomNumberGen)
{
	for (int i = 0; i < i_table_size; i++)
	{
		pd_table[i] = pcRandomNumberGen.dGetRandomNumber();
	}
	return true;
}

bool CTable::bGenRandom(CRandom & pcRandomNumberGen, double dMinRange, double dMaxRange, int iOffset)
{
	if (iOffset >= i_table_size) return false;

	pd_table[iOffset] = pcRandomNumberGen.dGetRandomNumber(dMinRange, dMaxRange);

	return true;
}

bool CTable::bGenRandom(CRandom & pcRandomNumberGen, double dMinRange, double dMaxRange, int iStartOffset, int iRange)
{
	if (iStartOffset + iRange >= i_table_size || iRange < 0) return false;

	for (int i = iStartOffset; i < iRange; i++)
	{
		pd_table[i] = pcRandomNumberGen.dGetRandomNumber(dMinRange, dMaxRange);
	}
	return true;
}

CTable::~CTable()
{
	if (pd_table != nullptr) delete[] pd_table;
}

