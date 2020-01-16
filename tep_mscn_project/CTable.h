#pragma once
#include "CRandom.h"
#include <iostream>

#define I_DEFAULT_SIZE 10

class CTable
{
public:
	CTable();
	~CTable();
	CTable(int iTableLen);
	CTable(const CTable & pcOther);
	void vPrint();
	bool bSetValue(int iOffset, double iValue);
	bool bSetNewSize(int iTableLen);
	double dGetValue(int iOffset);
	int iGetSize();
	double& pdClone();
	void vFillWithNumber(double dNumberToFillTable);
	bool bFillWithNumber(double dNumberToFillTable, int iStartOffset, int iRange);
	void operator=(const CTable &pcOther);
	bool operator!=(const CTable &pcOther);
	void vFillWithTable(double *pdOther);
	bool bWrite(FILE &pcFile);
	bool bRead(FILE &pcFile);
	bool bGenRandom(CRandom &pcRandomNumberGen);
	bool bGenRandom(CRandom &pcRandomNumberGen, double dMinRange, double dMaxRange, int iOffset);
	bool bGenRandom(CRandom &pcRandomNumberGen, double dMinRange, double dMaxRange, int iStartOffset, int iRange);
private:
	int i_table_size;
	double *pd_table;
};



