#include "pch.h"
#include "CRandomSearch.h"


CRandomSearch::CRandomSearch(CMscnProblem &pcProblem)
{
	pc_problem = new CMscnProblem(pcProblem);
	pc_best_last_solution = new CMscnSolution(pc_problem->bGetAmountOfDistrib(),
		pc_problem->bGetAmountOfFabrics(), pc_problem->bGetAmountOfMagazines(), pc_problem->bGetAmountOfShops());
	pc_best_last_solution_with_restriction = new CMscnSolution(pc_problem->bGetAmountOfDistrib(),
		pc_problem->bGetAmountOfFabrics(), pc_problem->bGetAmountOfMagazines(), pc_problem->bGetAmountOfShops());
}

bool CRandomSearch::bSearchBestSolution(int iEndingCondition, int iSeed)
{
	CRandom c_random_gen;
	double d_best_quality = -DBL_MAX;
	double d_best_quality_with_restriction = -DBL_MAX;
	double d_quality_of_searched_sol;
	bool b_is_constraints_satisfied = false;

	CMscnSolution c_tmp_solution(pc_problem->bGetAmountOfDistrib(),
		pc_problem->bGetAmountOfFabrics(), pc_problem->bGetAmountOfMagazines(), pc_problem->bGetAmountOfShops());

	c_random_gen.bSetSeed(iSeed);

	if (iEndingCondition <= 0) return false;

	while (iEndingCondition > 0)
	{
		c_tmp_solution.vGenerateSolution(c_random_gen, *pc_problem);
		d_quality_of_searched_sol = pc_problem->dGetQuality(&c_tmp_solution.pcGetSolutionTable()).first;
		b_is_constraints_satisfied = pc_problem->bConstraintsSatisfied(&c_tmp_solution.pcGetSolutionTable());

		if (d_best_quality < d_quality_of_searched_sol)
		{
			d_best_quality = d_quality_of_searched_sol;
			*pc_best_last_solution = c_tmp_solution;
		}

		if (d_best_quality_with_restriction < d_quality_of_searched_sol && b_is_constraints_satisfied)
		{
			d_best_quality_with_restriction = d_quality_of_searched_sol;
			*pc_best_last_solution_with_restriction = c_tmp_solution;
		}

		iEndingCondition--;
	}
	return true;
}

CMscnSolution& CRandomSearch::pcGetBestSolution()
{
	return *pc_best_last_solution;
}

CMscnSolution& CRandomSearch::pcGetBestSolutionWithRestriction()
{
	return *pc_best_last_solution_with_restriction;
}

CRandomSearch::~CRandomSearch()
{
	if (pc_problem != nullptr) delete pc_problem;
	if (pc_best_last_solution != nullptr) delete pc_best_last_solution;
	if (pc_best_last_solution_with_restriction != nullptr) delete pc_best_last_solution_with_restriction;
}

