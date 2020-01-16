#pragma once
#include "CMscnSolution.h"
#include "CMscnProblem.h"
#include "CPopulation.h"
#include "CRandom.h"
#include <iostream>

#define INT_DEFAULT_SEED 10;
#define DBL_DEFAULT_CROSS_PROB 0.5;
#define DBL_DEFAULT_DIFF_WEIGHT 1.5;

class CDiffEvol
{
public:
	CDiffEvol(CMscnProblem &pcProblem, double dCrossProb, double dDiffWeight, int iSeed);
	double dFindBestSol(int iEndingCondition, int iPopulationSize);
	double dFindBestSol(int iEndingCondition, int iPopulationSize, FILE &pcFile);
	double dFindBestSolRestrictionEvol(int iEndingCondition, int iPopulationSize, FILE &pcFile);
	bool bIndAreDifferent(CMscnSolution& pcInd, CMscnSolution& pcBaseInd, CMscnSolution& pcAddInd0, CMscnSolution& pcAddInd1);
	CMscnSolution& pcGetBestSolution();
	~CDiffEvol();
private:
	CMscnProblem *pc_problem;
	CMscnSolution *pc_best_last_solution_with_restriction;
	double d_coss_prob;
	double d_diff_weight;
	int i_seed;
};



