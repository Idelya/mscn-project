#include "pch.h"
#include "CMscnSolution.h"


CMscnSolution::CMscnSolution()
{
	i_distrib_amount = INT_DEFAULT_DISTR_AMOUNT;
	i_fabric_amount = INT_DEFAULT_FABRIC_AMOUNT;
	i_magazine_amount = INT_DEFAULT_MAGAZIE_AMOUNT;
	i_shop_amount = INT_DEFAULT_SHOP_AMOUNT;

	i_solution_length = (i_distrib_amount + i_magazine_amount) * i_fabric_amount + i_magazine_amount * i_shop_amount;

	pc_solution = new CTable(i_solution_length);
}

CMscnSolution::CMscnSolution(int iDistribAmount, int iFabricAmount, int iMagazineAmount, int iShopAmount)
{
	i_distrib_amount = iDistribAmount;
	i_fabric_amount = iFabricAmount;
	i_magazine_amount = iMagazineAmount;
	i_shop_amount = iShopAmount;

	i_solution_length = (i_distrib_amount + i_magazine_amount) * i_fabric_amount + i_magazine_amount * i_shop_amount;

	pc_solution = new CTable(i_solution_length);
}

CMscnSolution::CMscnSolution(const CMscnSolution & pcOther)
{
	i_distrib_amount = pcOther.i_distrib_amount;
	i_fabric_amount = pcOther.i_fabric_amount;
	i_magazine_amount = pcOther.i_magazine_amount;
	i_shop_amount = pcOther.i_shop_amount;
	i_solution_length = pcOther.i_solution_length;

	pc_solution = new CTable(i_solution_length);
	pc_solution->operator=(*pcOther.pc_solution);
}

bool CMscnSolution::bSetAmountOfDistrib(int iDistribAmount)
{
	bool b_is_correct = true;
	i_solution_length = iDistribAmount * i_fabric_amount + (i_fabric_amount + i_shop_amount) * i_magazine_amount;
	if (iDistribAmount <= 0) return false;

	b_is_correct = pc_solution->bSetNewSize(i_solution_length);
	i_distrib_amount = iDistribAmount;
	return b_is_correct;
}

bool CMscnSolution::bSetAmountOfFabrics(int iFabricAmount)
{
	i_solution_length = i_distrib_amount * i_fabric_amount + (iFabricAmount + i_shop_amount) * i_magazine_amount;
	bool b_is_correct = true;
	if (iFabricAmount <= 0) return false;

	b_is_correct = pc_solution->bSetNewSize(i_solution_length);

	i_fabric_amount = iFabricAmount;
	return b_is_correct;
}

bool CMscnSolution::bSetAmountOfMagazines(int iMagazinesAmount)
{
	bool b_is_correct = true;
	i_solution_length = i_distrib_amount * i_fabric_amount + (i_fabric_amount + i_shop_amount) * iMagazinesAmount;
	if (iMagazinesAmount <= 0) return false;

	b_is_correct = pc_solution->bSetNewSize(i_solution_length);

	i_magazine_amount = iMagazinesAmount;
	return b_is_correct;
}

bool CMscnSolution::bSetAmountOfShops(int iShopsAmount)
{
	bool b_is_correct = true;
	i_solution_length = i_distrib_amount * i_fabric_amount + (i_fabric_amount + iShopsAmount) * i_magazine_amount;
	if (iShopsAmount <= 0) return false;

	b_is_correct = pc_solution->bSetNewSize(i_solution_length);

	i_shop_amount = iShopsAmount;
	return b_is_correct;
}

double CMscnSolution::dGetSolutionGene(int iOffset)
{
	return pc_solution->dGetValue(iOffset);
}

bool CMscnSolution::bSetSolutionGene(int iOffset, double dGeneValue)
{
	bool b_is_correct = true;
	b_is_correct = pc_solution->bSetValue(iOffset, dGeneValue);
	return b_is_correct;
}

bool CMscnSolution::bSetSolutionGene(int iOffset, double dGeneValue, CMscnProblem & pcProblem)
{
	bool b_is_correct = true;

	if (pcProblem.dMinSolution(iOffset) > dGeneValue) return false;
	if (pcProblem.dMaxSolution(iOffset) < dGeneValue) return false;

	b_is_correct = pc_solution->bSetValue(iOffset, dGeneValue);

	return b_is_correct;
}

int CMscnSolution::iGetSolutionSize()
{
	return pc_solution->iGetSize();
}

double& CMscnSolution::pcGetSolutionTable()
{
	return pc_solution->pdClone();
}

void CMscnSolution::vGenerateSolution(CRandom& pcRandGen, CMscnProblem& pcProblem)
{
	for (int i = 0; i < pc_solution->iGetSize(); i++)
	{
		pc_solution->bGenRandom(pcRandGen, pcProblem.dMinSolution(i), pcProblem.dMaxSolution(i), i);
	}
}

bool CMscnSolution::bImportSolution(std::string sFileName)
{
	int i_tmp_amount;
	double d_tmp_value;
	FILE * pf_sol = fopen(sFileName.c_str(), "r");
	if (pf_sol == NULL) return false;

	fscanf(pf_sol, "%*s %d", &i_tmp_amount);
	bSetAmountOfDistrib(i_tmp_amount);
	fscanf(pf_sol, "%*s %d", &i_tmp_amount);
	bSetAmountOfFabrics(i_tmp_amount);
	fscanf(pf_sol, "%*s %d", &i_tmp_amount);
	bSetAmountOfMagazines(i_tmp_amount);
	fscanf(pf_sol, "%*s %d", &i_tmp_amount);
	bSetAmountOfShops(i_tmp_amount);

	fscanf(pf_sol, "%*s ");

	for (int i = 0; i < pc_solution->iGetSize(); i++)
	{
		if (i == i_distrib_amount * i_fabric_amount) fscanf(pf_sol, "%*s");
		if (i == (i_distrib_amount + i_magazine_amount) * i_fabric_amount) fscanf(pf_sol, "%*s");
		fscanf(pf_sol, "%lf", &d_tmp_value);
		pc_solution->bSetValue(i, d_tmp_value);
	}

	fclose(pf_sol);

	return true;
}

bool CMscnSolution::bExportSolution(std::string sFileName)
{
	FILE * pf_sol = fopen(sFileName.c_str(), "w+");
	if (pf_sol == NULL) return false;

	fprintf(pf_sol, "D %d \n", i_distrib_amount);
	fprintf(pf_sol, "F %d \n", i_fabric_amount);
	fprintf(pf_sol, "M %d \n", i_magazine_amount);
	fprintf(pf_sol, "S %d \n", i_shop_amount);

	fprintf(pf_sol, "xd\n");

	for (int i = 0; i < pc_solution->iGetSize(); i++)
	{
		if (i == i_distrib_amount * i_fabric_amount) fprintf(pf_sol, "xf\n");
		if (i == (i_distrib_amount + i_magazine_amount) * i_fabric_amount) fprintf(pf_sol, "xm\n");

		fprintf(pf_sol, "%0.2f \n", pc_solution->dGetValue(i));
	}

	fclose(pf_sol);

	return true;
}

void CMscnSolution::operator=(const CMscnSolution & pcOther)
{
	i_distrib_amount = pcOther.i_distrib_amount;
	i_fabric_amount = pcOther.i_fabric_amount;
	i_magazine_amount = pcOther.i_magazine_amount;
	i_shop_amount = pcOther.i_shop_amount;
	i_solution_length = pcOther.i_solution_length;

	pc_solution->operator=(*pcOther.pc_solution);
}

bool CMscnSolution::operator!=(const CMscnSolution & pcOther)
{
	if (i_distrib_amount != pcOther.i_distrib_amount) return true;
	else if (i_fabric_amount != pcOther.i_fabric_amount) return true;
	else if (i_magazine_amount != pcOther.i_magazine_amount) return true;
	else if (i_shop_amount != pcOther.i_shop_amount) return true;
	else if (pc_solution->operator!=(*pcOther.pc_solution)) return true;
	return false;
}

CMscnSolution::~CMscnSolution()
{
	if (pc_solution != nullptr) delete pc_solution;
}

