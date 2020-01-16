#pragma once
#include "CMscnSolution.h"
#include "CMscnProblem.h"
#include <time.h>

class CRandomSearch
{
public:
	CRandomSearch(CMscnProblem &pcProblem);
	bool bSearchBestSolution(int iEndingCondition, int iSeed);
	CMscnSolution& pcGetBestSolution();
	CMscnSolution& pcGetBestSolutionWithRestriction();
	~CRandomSearch();
private:
	CMscnSolution *pc_best_last_solution, *pc_best_last_solution_with_restriction;
	CMscnProblem *pc_problem;
	int iSeed;
};
