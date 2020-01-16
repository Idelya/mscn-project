#pragma once
#include "CMatrix.h"
#include "CRandom.h"
#include "CMscnProblem.h"
#include <iostream>

#define INT_DEFAULT_DISTR_AMOUNT 2;
#define INT_DEFAULT_FABRIC_AMOUNT 3;
#define INT_DEFAULT_MAGAZIE_AMOUNT 4;
#define INT_DEFAULT_SHOP_AMOUNT 5;

class CMscnSolution
{
public:
	CMscnSolution();
	CMscnSolution(int iDistribAmount, int iFabricAmount, int iMagazineAmount, int iShopAmount);
	CMscnSolution(const CMscnSolution &pcOther);
	bool bSetAmountOfDistrib(int iDistribAmount);
	bool bSetAmountOfFabrics(int iFabricAmount);
	bool bSetAmountOfMagazines(int iMagazineAmount);
	bool bSetAmountOfShops(int iShopAmount);
	double dGetSolutionGene(int iOffset);
	bool bSetSolutionGene(int iOffset, double dGeneValue);
	bool bSetSolutionGene(int iOffset, double dGeneValue, CMscnProblem& pcProblem);
	int iGetSolutionSize();
	double& pcGetSolutionTable();
	void vGenerateSolution(CRandom& pcRandGen, CMscnProblem& pcProblem);
	bool bImportSolution(std::string sFileName);
	bool bExportSolution(std::string sFileName);
	void operator=(const CMscnSolution & pcOther);
	bool operator!=(const CMscnSolution & pcOther);
	~CMscnSolution();
private:
	int i_distrib_amount, i_fabric_amount, i_magazine_amount, i_shop_amount, i_solution_length;
	CTable *pc_solution;
};



