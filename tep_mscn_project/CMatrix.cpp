#pragma once
#include "pch.h"
#include "CMatrix.h"


CMatrix::CMatrix()
{
	i_rows = I_DEFAULT_SIZE_ROWS;
	i_columns = I_DEFAULT_SIZE_COLUMNS;

	pc_matrix = new CTable*[i_rows];

	for (int i = 0; i < i_rows; ++i)
	{
		pc_matrix[i] = new CTable(i_columns);
	}
}

CMatrix::CMatrix(int iRows, int iColums)
{
	if (iRows > 0) i_rows = iRows;
	else i_rows = I_DEFAULT_SIZE_ROWS;

	if (iColums > 0) i_columns = iColums;
	else i_columns = I_DEFAULT_SIZE_COLUMNS;

	pc_matrix = new CTable*[i_rows];

	for (int i = 0; i < i_rows; ++i)
	{
		pc_matrix[i] = new CTable(i_columns);
	}
}

CMatrix::CMatrix(CMatrix & pcMatrixOther)
{
	i_columns = pcMatrixOther.i_columns;
	i_rows = pcMatrixOther.i_rows;
	pc_matrix = new CTable*[i_rows];

	for (int i = 0; i < i_rows; i++)
	{
		pc_matrix[i] = new CTable(*pcMatrixOther.pc_matrix[i]);
	}
}

void CMatrix::vPrint()
{
	for (int i = 0; i < i_rows; i++)
	{
		std::cout << std::endl << "|";

		pc_matrix[i]->vPrint();

		std::cout << " |";
	}
}

bool CMatrix::bWrite(FILE &pcFile)
{
	for (int i = 0; i < i_rows; i++)
	{
		pc_matrix[i]->bWrite(pcFile);
	}

	return true;
}

bool CMatrix::bRead(FILE & pcFile)
{
	for (int i = 0; i < i_rows; i++)
	{
		pc_matrix[i]->bRead(pcFile);
	}

	return true;
}

bool CMatrix::bCheckOffset(int iRowsOffset, int iColumsOffset)
{
	if (iRowsOffset < 0 || iColumsOffset < 0) return false;
	if (iRowsOffset >= i_rows || iColumsOffset >= i_columns) return false;
	return true;
}

bool CMatrix::bSetMatrixValue(int iRowsOffset, int iColumsOffset, double dValue)
{
	if (!bCheckOffset(iRowsOffset, iColumsOffset)) return false;

	pc_matrix[iRowsOffset]->bSetValue(iColumsOffset, dValue);
	return true;
}

double CMatrix::dGetMatrixValue(int iRowsOffset, int iColumsOffset)
{
	if (bCheckOffset(iRowsOffset, iColumsOffset))
	{
		return pc_matrix[iRowsOffset]->dGetValue(iColumsOffset);
	}
	return 0;
}

bool CMatrix::bNewColumnSize(int iNewSize)
{
	if (iNewSize < 0) return false;

	for (int i = 0; i < i_rows; i++)
	{
		pc_matrix[i]->bSetNewSize(iNewSize);
	}

	i_columns = iNewSize;

	return true;
}

bool CMatrix::bNewRowsSize(int iNewSize)
{
	if (iNewSize < 0) return false;

	CTable  **pc_changed_matrix = new CTable*[iNewSize];

	for (int i = 0; i < iNewSize; i++)
	{
		if (i < i_rows) pc_changed_matrix[i] = new CTable(*pc_matrix[i]);
		else pc_changed_matrix[i] = new CTable(i_columns);
	}

	for (int i = 0; i < i_rows; i++)
	{
		delete pc_matrix[i];
	}

	delete[] pc_matrix;

	pc_matrix = pc_changed_matrix;
	i_rows = iNewSize;

	return true;
}

bool CMatrix::bGenRandom(CRandom & cRandomNumberGenerator)
{
	for (int i = 0; i < i_rows; i++)
	{
		pc_matrix[i]->bGenRandom(cRandomNumberGenerator);
	}

	return true;
}

CMatrix::~CMatrix()
{
	for (int i = 0; i < i_rows; i++)
	{
		delete pc_matrix[i];
	}

	delete[] pc_matrix;
}

