#pragma once
#include "pch.h"
#include "CMscnProblem.h"

CMscnProblem::CMscnProblem()
{
	int i_solution_length;
	i_distrib_amount = INT_DEFAULT_DISTR_AMOUNT;
	i_fabric_amount = INT_DEFAULT_FABRIC_AMOUNT;
	i_magazine_amount = INT_DEFAULT_MAGAZIE_AMOUNT;
	i_shop_amount = INT_DEFAULT_SHOP_AMOUNT;
	i_solution_length = i_distrib_amount * i_fabric_amount + i_fabric_amount * i_magazine_amount + i_magazine_amount * i_shop_amount;

	pc_sd = new CTable(i_distrib_amount);
	pc_sf = new CTable(i_fabric_amount);
	pc_sm = new CTable(i_magazine_amount);
	pc_ss = new CTable(i_shop_amount);

	pc_cd = new CMatrix(i_distrib_amount, i_fabric_amount);
	pc_cf = new CMatrix(i_fabric_amount, i_magazine_amount);
	pc_cm = new CMatrix(i_magazine_amount, i_shop_amount);

	pc_ud = new CTable(i_distrib_amount);
	pc_uf = new CTable(i_fabric_amount);
	pc_um = new CTable(i_magazine_amount);
	pc_p = new CTable(i_shop_amount);

	pc_min_sol = new CTable(i_solution_length);
	pc_max_sol = new CTable(i_solution_length);
	pc_min_sol->vFillWithNumber(INT_MIN_RANGE_OF_SOL);
	pc_max_sol->vFillWithNumber(INT_MAX_RANGE_OF_SOL);
}

CMscnProblem::CMscnProblem(int iDistribAmount, int iFabricAmount, int iMagazineAmount, int iShopAmount)
{
	int i_solution_length;
	i_distrib_amount = iDistribAmount;
	i_fabric_amount = iFabricAmount;
	i_magazine_amount = iMagazineAmount;
	i_shop_amount = iShopAmount;
	i_solution_length = i_distrib_amount * i_fabric_amount + i_fabric_amount * i_magazine_amount + i_magazine_amount * i_shop_amount;

	pc_sd = new CTable(i_distrib_amount);
	pc_sf = new CTable(i_fabric_amount);
	pc_sm = new CTable(i_magazine_amount);
	pc_ss = new CTable(i_shop_amount);

	pc_cd = new CMatrix(i_distrib_amount, i_fabric_amount);
	pc_cf = new CMatrix(i_fabric_amount, i_magazine_amount);
	pc_cm = new CMatrix(i_magazine_amount, i_shop_amount);

	pc_ud = new CTable(i_distrib_amount);
	pc_uf = new CTable(i_fabric_amount);
	pc_um = new CTable(i_magazine_amount);
	pc_p = new CTable(i_shop_amount);

	pc_min_sol = new CTable(i_solution_length);
	pc_max_sol = new CTable(i_solution_length);

	pc_min_sol->vFillWithNumber(INT_MIN_RANGE_OF_SOL);
	pc_max_sol->vFillWithNumber(INT_MAX_RANGE_OF_SOL);

}

CMscnProblem::CMscnProblem(CMscnProblem & pcOtherProblem)
{
	i_distrib_amount = pcOtherProblem.i_distrib_amount;
	i_fabric_amount = pcOtherProblem.i_fabric_amount;
	i_magazine_amount = pcOtherProblem.i_magazine_amount;
	i_shop_amount = pcOtherProblem.i_shop_amount;

	pc_sd = new CTable(*pcOtherProblem.pc_sd);
	pc_sf = new CTable(*pcOtherProblem.pc_sf);
	pc_sm = new CTable(*pcOtherProblem.pc_sm);
	pc_ss = new CTable(*pcOtherProblem.pc_ss);

	pc_cd = new CMatrix(*pcOtherProblem.pc_cd);
	pc_cf = new CMatrix(*pcOtherProblem.pc_cf);
	pc_cm = new CMatrix(*pcOtherProblem.pc_cm);

	pc_ud = new CTable(*pcOtherProblem.pc_ud);
	pc_uf = new CTable(*pcOtherProblem.pc_uf);
	pc_um = new CTable(*pcOtherProblem.pc_um);
	pc_p = new CTable(*pcOtherProblem.pc_p);

	pc_min_sol = new CTable(*pcOtherProblem.pc_min_sol);
	pc_max_sol = new CTable(*pcOtherProblem.pc_max_sol);
}

std::pair <double, bool> CMscnProblem::dGetQuality(double * pdSolution)
{
	if (!bCheckSolutionTechnic(pdSolution)) return std::pair <double, bool>(0.0, false);
	double d_profit = dGetProfit(&pdSolution[i_distrib_amount*i_fabric_amount + i_fabric_amount * i_magazine_amount]);
	return std::pair <double, bool>(d_profit - dGetTransportCost(pdSolution) - dGetContractCost(pdSolution), true);
}

double CMscnProblem::dGetTransportCost(double * pdSolution)
{
	int i_id_solution = 0;
	double d_transport_cost = 0.0;

	for (int i = 0; i < i_distrib_amount; i++)
	{
		for (int j = 0; j < i_fabric_amount; j++)
		{
			d_transport_cost += pc_cd->dGetMatrixValue(i, j) * pdSolution[i_id_solution++];
		}
	}
	for (int i = 0; i < i_fabric_amount; i++)
	{
		for (int j = 0; j < i_magazine_amount; j++)
		{
			d_transport_cost += pc_cf->dGetMatrixValue(i, j) * pdSolution[i_id_solution++];
		}
	}

	for (int i = 0; i < i_magazine_amount; i++)
	{
		for (int j = 0; j < i_shop_amount; j++)
		{
			d_transport_cost += pc_cm->dGetMatrixValue(i, j) * pdSolution[i_id_solution++];
		}
	}

	return d_transport_cost;
}

double CMscnProblem::dGetContractCost(double * pdSolution)
{

	int i_id_solution = 0;
	double d_tmp_cost = 0;
	double d_contract_cost = 0.0;

	for (int i = 0; i < i_distrib_amount; i++)
	{
		for (int j = 0; j < i_fabric_amount; j++)
		{
			d_tmp_cost += pdSolution[i_id_solution++];
		}

		if (d_tmp_cost > 0) d_contract_cost += pc_ud->dGetValue(i);
		d_tmp_cost = 0;
	}

	for (int i = 0; i < i_fabric_amount; i++)
	{
		for (int j = 0; j < i_magazine_amount; j++)
		{
			d_tmp_cost += pdSolution[i_id_solution++];
		}

		if (d_tmp_cost > 0) d_contract_cost += pc_uf->dGetValue(i);;
		d_tmp_cost = 0;
	}

	for (int i = 0; i < i_magazine_amount; i++)
	{
		for (int j = 0; j < i_shop_amount; j++)
		{
			d_tmp_cost += pdSolution[i_id_solution++];
		}

		if (d_tmp_cost > 0) d_contract_cost += pc_um->dGetValue(i);;
		d_tmp_cost = 0;
	}

	return d_contract_cost;
}

double CMscnProblem::dGetProfit(double * pdSolution)
{
	double d_profit = 0.0;
	int i_id_solution = 0;

	for (int i = 0; i < i_magazine_amount; i++)
	{
		for (int j = 0; j < i_shop_amount; j++)
		{
			d_profit += pdSolution[i_id_solution++] * pc_p->dGetValue(j);
		}
	}

	return d_profit;
}

bool CMscnProblem::bConstraintsSatisfied(double * pdSolution)
{
	CTable c_sum_of_shop_products, c_sum_of_fabric_products, c_sum_of_magazin_products;
	int i_id_xf_start = i_distrib_amount * i_fabric_amount;
	int i_id_xm_start = i_id_xf_start + i_magazine_amount * i_fabric_amount;
	if (!bCheckSolutionTechnic(pdSolution)) return false;
	if (!bCheckPower(&pdSolution[0], pc_sd, i_id_xf_start, i_fabric_amount)) return false;
	if (!bCheckPower(&pdSolution[i_id_xf_start], pc_sf, i_magazine_amount*i_fabric_amount, i_magazine_amount)) return false;
	if (!bCheckPower(&pdSolution[i_id_xm_start], pc_sm, i_magazine_amount*i_shop_amount, i_shop_amount)) return false;

	c_sum_of_shop_products = cSumOfColumns(&pdSolution[i_id_xm_start], i_magazine_amount*i_shop_amount, i_shop_amount);

	for (int i = 0; i < i_shop_amount; i++)
	{
		if (c_sum_of_shop_products.dGetValue(i) > pc_ss->dGetValue(i)) return false;
	}

	c_sum_of_fabric_products = cSumOfColumns(&pdSolution[0], i_id_xf_start, i_fabric_amount);
	if (!bCheckPower(&pdSolution[i_id_xf_start], &c_sum_of_fabric_products, i_magazine_amount*i_fabric_amount, i_magazine_amount)) return false;

	c_sum_of_magazin_products = cSumOfColumns(&pdSolution[i_id_xf_start], i_magazine_amount*i_fabric_amount, i_magazine_amount);
	if (!bCheckPower(&pdSolution[i_id_xm_start], &c_sum_of_magazin_products, i_magazine_amount*i_shop_amount, i_shop_amount)) return false;

	return true;
}

bool CMscnProblem::bCheckSolutionTechnic(double * pdSolution)
{
	int i_max_solution_id = i_distrib_amount * i_fabric_amount + (i_fabric_amount + i_shop_amount) * i_magazine_amount;

	if (pdSolution == NULL) return false;

	for (int i = 0; i < i_max_solution_id; i++)
	{
		if (pdSolution[i] < 0) return false;
	}

	return true;
}

bool CMscnProblem::bCheckPower(double * pdSolution, CTable * pcPower, int iRange, int iColumn)
{
	int i_row_id = 0;
	int i_col_id = 0;
	double d_tmp_solution = 0;

	for (int i = 0; i < iRange; i++)
	{
		d_tmp_solution += pdSolution[i];
		i_col_id++;

		if (i_col_id == iColumn)
		{
			if (d_tmp_solution > pcPower->dGetValue(i_row_id)) return false;
			d_tmp_solution = 0;
			i_col_id = 0;
			i_row_id++;
		}
	}
	return true;
}

CTable CMscnProblem::cSumOfColumns(double * pdSolution, int iRange, int iColum)
{
	int i_column_id = 0;
	CTable c_tmp_test_tab(i_shop_amount);

	for (int i = 0; i < iRange; i++)
	{
		c_tmp_test_tab.bSetValue(i_column_id, c_tmp_test_tab.dGetValue(i_column_id) + pdSolution[i]);
		i_column_id++;

		if (i_column_id == iColum)
		{
			i_column_id = 0;
		}
	}

	return c_tmp_test_tab;
}

bool CMscnProblem::bSetAmountOfDistrib(int iDistribAmount)
{
	bool b_is_correct = true;
	int i_solution_length = iDistribAmount * i_fabric_amount + (i_fabric_amount + i_shop_amount) * i_magazine_amount;
	if (iDistribAmount <= 0) return false;

	b_is_correct = pc_sd->bSetNewSize(iDistribAmount);
	if (b_is_correct) b_is_correct = pc_ud->bSetNewSize(iDistribAmount);
	if (b_is_correct) b_is_correct = pc_cd->bNewRowsSize(iDistribAmount);
	if (b_is_correct) b_is_correct = pc_min_sol->bSetNewSize(i_solution_length);
	if (b_is_correct) b_is_correct = pc_max_sol->bSetNewSize(i_solution_length);

	i_distrib_amount = iDistribAmount;

	return b_is_correct;
}

bool CMscnProblem::bSetAmountOfFabrics(int iFabricAmount)
{
	int i_solution_length = i_distrib_amount * i_fabric_amount + (iFabricAmount + i_shop_amount) * i_magazine_amount;
	bool b_is_correct = true;
	if (iFabricAmount <= 0) return false;

	b_is_correct = pc_sf->bSetNewSize(iFabricAmount);
	if (b_is_correct) b_is_correct = pc_uf->bSetNewSize(iFabricAmount);
	if (b_is_correct) b_is_correct = pc_cd->bNewColumnSize(iFabricAmount);
	if (b_is_correct) b_is_correct = pc_cf->bNewRowsSize(iFabricAmount);
	if (b_is_correct) b_is_correct = pc_min_sol->bSetNewSize(i_solution_length);
	if (b_is_correct) b_is_correct = pc_max_sol->bSetNewSize(i_solution_length);

	i_fabric_amount = iFabricAmount;
	return b_is_correct;
}

bool CMscnProblem::bSetAmountOfMagazines(int iMagazinesAmount)
{
	bool b_is_correct = true;
	int i_solution_length = i_distrib_amount * i_fabric_amount + (i_fabric_amount + i_shop_amount) * iMagazinesAmount;
	if (iMagazinesAmount <= 0) return false;

	b_is_correct = pc_sm->bSetNewSize(iMagazinesAmount);
	if (b_is_correct) b_is_correct = pc_um->bSetNewSize(iMagazinesAmount);
	if (b_is_correct) b_is_correct = pc_cf->bNewColumnSize(iMagazinesAmount);
	if (b_is_correct) b_is_correct = pc_cm->bNewRowsSize(iMagazinesAmount);
	if (b_is_correct) b_is_correct = pc_min_sol->bSetNewSize(i_solution_length);
	if (b_is_correct) b_is_correct = pc_max_sol->bSetNewSize(i_solution_length);

	i_magazine_amount = iMagazinesAmount;
	return b_is_correct;
}

bool CMscnProblem::bSetAmountOfShops(int iShopsAmount)
{
	bool b_is_correct = true;
	int i_solution_length = i_distrib_amount * i_fabric_amount + (i_fabric_amount + iShopsAmount) * i_magazine_amount;
	if (iShopsAmount <= 0) return false;

	b_is_correct = pc_ss->bSetNewSize(iShopsAmount);
	if (b_is_correct) pc_p->bSetNewSize(iShopsAmount);
	if (b_is_correct) b_is_correct = pc_cm->bNewColumnSize(iShopsAmount);
	if (b_is_correct) b_is_correct = pc_min_sol->bSetNewSize(i_solution_length);
	if (b_is_correct) b_is_correct = pc_max_sol->bSetNewSize(i_solution_length);

	i_shop_amount = iShopsAmount;
	return b_is_correct;
}

int CMscnProblem::bGetAmountOfDistrib()
{
	return i_distrib_amount;
}

int CMscnProblem::bGetAmountOfFabrics()
{
	return i_fabric_amount;
}

int CMscnProblem::bGetAmountOfMagazines()
{
	return i_magazine_amount;
}

int CMscnProblem::bGetAmountOfShops()
{
	return i_shop_amount;
}

bool CMscnProblem::bSetDistribPower(int iOffset, double iDistribPower)
{
	bool b_is_correct = true;
	b_is_correct = pc_sd->bSetValue(iOffset, iDistribPower);
	return b_is_correct;
}

bool CMscnProblem::bSetFabricsPower(int iOffset, double iFabricPower)
{
	bool b_is_correct = true;
	b_is_correct = pc_sf->bSetValue(iOffset, iFabricPower);
	return b_is_correct;
}

bool CMscnProblem::bSetMagazinesPower(int iOffset, double iMagazinePower)
{
	bool b_is_correct = true;
	b_is_correct = pc_sm->bSetValue(iOffset, iMagazinePower);
	return b_is_correct;
}

bool CMscnProblem::bSetShopsPower(int iOffset, double iShopPower)
{
	bool b_is_correct = true;
	b_is_correct = pc_ss->bSetValue(iOffset, iShopPower);
	return b_is_correct;
}

bool CMscnProblem::bSetDistribContractCost(int iOffset, double iDistribContractCost)
{
	bool b_is_correct = true;
	b_is_correct = pc_ud->bSetValue(iOffset, iDistribContractCost);
	return b_is_correct;
}

bool CMscnProblem::bSetFabricsContractCost(int iOffset, double iFabricContractCost)
{
	bool b_is_correct = true;
	b_is_correct = pc_uf->bSetValue(iOffset, iFabricContractCost);
	return b_is_correct;
}

bool CMscnProblem::bSetMagazinesContractCost(int iOffset, double iMagazineContractCost)
{
	bool b_is_correct = true;
	b_is_correct = pc_um->bSetValue(iOffset, iMagazineContractCost);
	return b_is_correct;
}

bool CMscnProblem::bSetShopsProfit(int iOffset, double iShopProfit)
{
	bool b_is_correct = true;
	b_is_correct = pc_p->bSetValue(iOffset, iShopProfit);
	return b_is_correct;
}

bool CMscnProblem::bSetDistribValue(int iRowsOffset, int iColumsOffset, double iDistribValue)
{
	bool b_is_correct = true;
	b_is_correct = pc_cd->bSetMatrixValue(iRowsOffset, iColumsOffset, iDistribValue);
	return false;
}

bool CMscnProblem::bSetFabricsValue(int iRowsOffset, int iColumsOffset, double iFabricValue)
{
	bool b_is_correct = true;
	b_is_correct = pc_cf->bSetMatrixValue(iRowsOffset, iColumsOffset, iFabricValue);
	return false;
}

bool CMscnProblem::bSetMagazinesValue(int iRowsOffset, int iColumsOffset, double iMagazineValue)
{
	bool b_is_correct = true;
	b_is_correct = pc_cm->bSetMatrixValue(iRowsOffset, iColumsOffset, iMagazineValue);
	return false;
}

void CMscnProblem::vPrint()
{
	std::cout << "from d to f ";
	pc_cd->vPrint();

	std::cout << std::endl << "from f to m ";
	pc_cf->vPrint();

	std::cout << std::endl << "from m to s ";
	pc_cm->vPrint();
}

bool CMscnProblem::bImportProblem(std::string sFileName)
{
	int i_tmp_amount;
	FILE * pf_problem = fopen(sFileName.c_str(), "r");
	if (pf_problem == NULL) return false;

	fscanf(pf_problem, "%*s %d", &i_tmp_amount);
	bSetAmountOfDistrib(i_tmp_amount);
	fscanf(pf_problem, "%*s %d", &i_tmp_amount);
	bSetAmountOfFabrics(i_tmp_amount);
	fscanf(pf_problem, "%*s %d", &i_tmp_amount);
	bSetAmountOfMagazines(i_tmp_amount);
	fscanf(pf_problem, "%*s %d", &i_tmp_amount);
	bSetAmountOfShops(i_tmp_amount);

	fscanf(pf_problem, "%*s");
	pc_sd->bRead(*pf_problem);
	fscanf(pf_problem, "%*s");
	pc_sf->bRead(*pf_problem);
	fscanf(pf_problem, "%*s");
	pc_sm->bRead(*pf_problem);
	fscanf(pf_problem, "%*s");
	pc_ss->bRead(*pf_problem);

	fscanf(pf_problem, "%*s");
	pc_cd->bRead(*pf_problem);
	fscanf(pf_problem, "%*s");
	pc_cf->bRead(*pf_problem);
	fscanf(pf_problem, "%*s");
	pc_cm->bRead(*pf_problem);

	fscanf(pf_problem, "%*s");
	pc_ud->bRead(*pf_problem);
	fscanf(pf_problem, "%*s");
	pc_uf->bRead(*pf_problem);
	fscanf(pf_problem, "%*s");
	pc_um->bRead(*pf_problem);
	fscanf(pf_problem, "%*s");
	pc_p->bRead(*pf_problem);

	bMinMaxTableImport(*pf_problem);

	fclose(pf_problem);
	return true;
}

bool CMscnProblem::bExportProblem(std::string sFileName)
{
	FILE * pf_problem = fopen(sFileName.c_str(), "w+");
	if (pf_problem == NULL) return false;

	fprintf(pf_problem, "D %d \n", i_distrib_amount);
	fprintf(pf_problem, "F %d \n", i_fabric_amount);
	fprintf(pf_problem, "M %d \n", i_magazine_amount);
	fprintf(pf_problem, "S %d \n", i_shop_amount);

	fprintf(pf_problem, "sd \n");
	pc_sd->bWrite(*pf_problem);
	fprintf(pf_problem, "sf \n");
	pc_sf->bWrite(*pf_problem);
	fprintf(pf_problem, "sm \n");
	pc_sm->bWrite(*pf_problem);
	fprintf(pf_problem, "ss \n");
	pc_ss->bWrite(*pf_problem);

	fprintf(pf_problem, "cd \n");
	pc_cd->bWrite(*pf_problem);
	fprintf(pf_problem, "cf \n");
	pc_cf->bWrite(*pf_problem);
	fprintf(pf_problem, "cm \n");
	pc_cm->bWrite(*pf_problem);

	fprintf(pf_problem, "ud \n");
	pc_ud->bWrite(*pf_problem);
	fprintf(pf_problem, "uf \n");
	pc_uf->bWrite(*pf_problem);
	fprintf(pf_problem, "um \n");
	pc_um->bWrite(*pf_problem);
	fprintf(pf_problem, "p \n");
	pc_p->bWrite(*pf_problem);

	bMinMaxTableExport(*pf_problem);

	fclose(pf_problem);
	return true;
}

double CMscnProblem::dMinSolution(int iOffset)
{
	if (iOffset < 0 || iOffset > pc_min_sol->iGetSize()) return 0;
	return pc_min_sol->dGetValue(iOffset);
}

double CMscnProblem::dMaxSolution(int iOffset)
{
	if (iOffset < 0 || iOffset>pc_max_sol->iGetSize()) return DBL_MAX;
	return pc_max_sol->dGetValue(iOffset);
}

bool CMscnProblem::bMinMaxTableImport(FILE & pcFile)
{
	double d_tmp_min, d_tmp_max;

	fscanf(&pcFile, "%*s");

	for (int i = 0; i < pc_min_sol->iGetSize(); i++)
	{
		if (i == i_distrib_amount * i_fabric_amount)
		{
			fscanf(&pcFile, "%*s");
		}
		if (i == (i_distrib_amount + i_magazine_amount) * i_fabric_amount)
		{
			fscanf(&pcFile, "%*s");
		}
		fscanf(&pcFile, "%lf %lf", &d_tmp_min, &d_tmp_max);
		pc_min_sol->bSetValue(i, d_tmp_min);
		pc_max_sol->bSetValue(i, d_tmp_max);
	}

	return true;
}

bool CMscnProblem::bMinMaxTableExport(FILE & pcFile)
{
	fprintf(&pcFile, "xdminmax\n");

	for (int i = 0; i < pc_min_sol->iGetSize(); i++)
	{
		if (i == i_distrib_amount * i_fabric_amount) fprintf(&pcFile, "xfminmax\n");
		if (i == (i_distrib_amount + i_magazine_amount) * i_fabric_amount) fprintf(&pcFile, "xmminmax\n");

		fprintf(&pcFile, "%0.2f %0.2f \n", pc_min_sol->dGetValue(i), pc_max_sol->dGetValue(i));
	}
	return true;
}

void CMscnProblem::vGenerateInstance(int iInstanceSeed)
{
	int i_min_range = INT_MIN_RANGE_OF_DISTR;
	int i_max_range = INT_MAX_RANGE_OF_DISTR;

	CRandom c_rand_gen(i_min_range, i_max_range, iInstanceSeed);

	bSetAmountOfDistrib(c_rand_gen.iGetRandomNumber());
	bSetAmountOfFabrics(c_rand_gen.iGetRandomNumber());
	bSetAmountOfMagazines(c_rand_gen.iGetRandomNumber());
	bSetAmountOfShops(c_rand_gen.iGetRandomNumber());

	c_rand_gen.bSetNewRange(INT_MIN_RANGE_OF_POWER, INT_MAX_RANGE_OF_POWER);

	pc_sd->bGenRandom(c_rand_gen);
	pc_sf->bGenRandom(c_rand_gen);
	pc_sm->bGenRandom(c_rand_gen);
	pc_ss->bGenRandom(c_rand_gen);

	c_rand_gen.bSetNewRange(INT_MIN_RANGE_OF_VAR_COSTS, INT_MAX_RANGE_OF_VAR_COSTS);

	pc_cd->bGenRandom(c_rand_gen);
	pc_cf->bGenRandom(c_rand_gen);
	pc_cm->bGenRandom(c_rand_gen);

	c_rand_gen.bSetNewRange(INT_MIN_RANGE_OF_CONTRACT_COST, INT_MAX_RANGE_OF_CONTRACT_COST);

	pc_ud->bGenRandom(c_rand_gen);
	pc_uf->bGenRandom(c_rand_gen);
	pc_um->bGenRandom(c_rand_gen);

	c_rand_gen.bSetNewRange(INT_MIN_RANGE_OF_PROFIT, INT_MAX_RANGE_OF_PROFIT);
	pc_p->bGenRandom(c_rand_gen);

	pc_min_sol->bFillWithNumber(INT_MIN_RANGE_OF_XD, 0, i_fabric_amount*i_distrib_amount);
	pc_min_sol->bFillWithNumber(INT_MIN_RANGE_OF_XF, i_fabric_amount*i_distrib_amount, i_fabric_amount*i_magazine_amount);
	pc_min_sol->bFillWithNumber(INT_MIN_RANGE_OF_XM, i_fabric_amount*(i_distrib_amount + i_magazine_amount), i_fabric_amount*i_shop_amount);

	pc_max_sol->bFillWithNumber(INT_MAX_RANGE_OF_XD, 0, i_fabric_amount*i_distrib_amount);
	pc_max_sol->bFillWithNumber(INT_MAX_RANGE_OF_XF, i_fabric_amount*i_distrib_amount, i_fabric_amount*i_magazine_amount);
	pc_max_sol->bFillWithNumber(INT_MAX_RANGE_OF_XM, i_fabric_amount*(i_distrib_amount + i_magazine_amount), i_fabric_amount*i_shop_amount);
}

CMscnProblem::~CMscnProblem()
{
	delete pc_sd;
	delete pc_sf;
	delete pc_sm;
	delete pc_ss;

	delete pc_ud;
	delete pc_uf;
	delete pc_um;
	delete pc_p;

	delete pc_cd;
	delete pc_cf;
	delete pc_cm;

	delete pc_max_sol;
	delete pc_min_sol;
}

