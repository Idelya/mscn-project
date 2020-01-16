#pragma once
#include "CMatrix.h"
#include "CRandom.h"
#include "const.h"
#include <iostream>

class CMscnProblem
{
public:
	CMscnProblem();
	CMscnProblem(int iDistribAmount, int iFabricAmount, int iMagazineAmount, int iShopAmount);
	CMscnProblem(CMscnProblem & pcOtherSolution);
	std::pair <double, bool> dGetQuality(double *pdSolution);
	double dGetTransportCost(double *pdSolution);
	double dGetContractCost(double *pdSolution);
	double dGetProfit(double *pdSolution);
	bool bConstraintsSatisfied(double *pdSolution);
	bool bCheckSolutionTechnic(double *pdSolution);
	bool bCheckPower(double *pdSolution, CTable *pcPower, int iRange, int iColumn);
	CTable cSumOfColumns(double *pdSolution, int iRange, int iColum);

	bool bSetAmountOfDistrib(int iDistribAmount);
	bool bSetAmountOfFabrics(int iFabricAmount);
	bool bSetAmountOfMagazines(int iMagazineAmount);
	bool bSetAmountOfShops(int iShopAmount);

	int bGetAmountOfDistrib();
	int bGetAmountOfFabrics();
	int bGetAmountOfMagazines();
	int bGetAmountOfShops();

	bool bSetDistribPower(int iOffset, double iDistribPower);
	bool bSetFabricsPower(int iOffset, double iFabricPower);
	bool bSetMagazinesPower(int iOffset, double iMagazinePower);
	bool bSetShopsPower(int iOffset, double iShopPower);

	bool bSetDistribContractCost(int iOffset, double iDistribContractCost);
	bool bSetFabricsContractCost(int iOffset, double iFabricContractCost);
	bool bSetMagazinesContractCost(int iOffset, double iMagazineContractCost);
	bool bSetShopsProfit(int iOffset, double iShopProfit);

	bool bSetDistribValue(int iRowsOffset, int iColumsOffset, double iDistribValue);
	bool bSetFabricsValue(int iRowsOffset, int iColumsOffset, double iFabricValue);
	bool bSetMagazinesValue(int iRowsOffset, int iColumsOffset, double iMagazineValue);
	void vPrint();

	bool bImportProblem(std::string sFileName);
	bool bExportProblem(std::string sFileName);

	double dMinSolution(int iOffset);
	double dMaxSolution(int iOffset);
	bool bMinMaxTableImport(FILE &pcFile);
	bool bMinMaxTableExport(FILE &pcFile);

	void vGenerateInstance(int iInstanceSeed);
	~CMscnProblem();
private:
	int i_distrib_amount, i_fabric_amount, i_magazine_amount, i_shop_amount;
	CTable * pc_sd, *pc_sf, *pc_sm, *pc_ss;
	CTable * pc_p, *pc_ud, *pc_uf, *pc_um;
	CMatrix * pc_cd, *pc_cf, *pc_cm;
	CTable *pc_min_sol, *pc_max_sol;
};



