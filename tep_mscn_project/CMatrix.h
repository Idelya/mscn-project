#pragma once
#include "CTable.h"
#include <iostream>
#define I_DEFAULT_SIZE_ROWS 2
#define I_DEFAULT_SIZE_COLUMNS 2

class CMatrix
{
public:
	CMatrix();
	CMatrix(int iRows, int iColums);
	CMatrix(CMatrix & pcMatrixOther);
	void vPrint();
	bool bWrite(FILE &pcFile);
	bool bRead(FILE &pcFile);
	bool bCheckOffset(int iRowsOffset, int iColumsOffset);
	bool bSetMatrixValue(int iRowsOffset, int iColumsOffset, double iValue);
	double dGetMatrixValue(int iRowsOffset, int iColumsOffset);
	bool bNewColumnSize(int iNewSize);
	bool bNewRowsSize(int iNewSize);
	bool bGenRandom(CRandom &pcRandomNumberGenerator);
	~CMatrix();
private:
	int i_rows;
	int i_columns;
	CTable ** pc_matrix;
};



