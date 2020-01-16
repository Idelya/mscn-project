#include "pch.h"
#include "CDiffEvol.h"


CDiffEvol::CDiffEvol(CMscnProblem &pcProblem, double dCrossProb, double dDiffWeight, int iSeed)
{
	i_seed = iSeed;
	pc_problem = new CMscnProblem(pcProblem);
	if (d_coss_prob <= 1 && d_coss_prob >= 0) d_coss_prob = dCrossProb;
	else d_coss_prob = DBL_DEFAULT_CROSS_PROB;
	d_diff_weight = dDiffWeight;
	pc_best_last_solution_with_restriction = new CMscnSolution();
}

double CDiffEvol::dFindBestSol(int iEndingCondition, int iPopulationSize)
{
	int i_best_quality_id;
	bool b_are_diffrent = false, b_find_correct_sol = false;
	double d_best_quality = -DBL_MAX, d_tmp_quality, d_tmp_gene;
	CPopulation c_population(iPopulationSize);
	CMscnSolution* pc_ind;
	CMscnSolution c_base_ind, c_add_ind0, c_add_ind1, c_ind_new, c_best_sol;
	CRandom c_random;

	c_random.bSetSeed(i_seed);

	c_population.vInitRandomPopulation(*pc_problem, i_seed);
	c_population.vPrintStatistic(*pc_problem);

	for (int j = 0; j < iEndingCondition; j++)
	{
		std::cout << j << std::endl;
		for (int i = 0; i < c_population.iGetPopulationSize(); i++)
		{
			pc_ind = c_population.pcGetInd(i);
			c_ind_new = *pc_ind;
			c_base_ind = c_population.cGetRandomInd(c_random);
			c_add_ind0 = c_population.cGetRandomInd(c_random);
			c_add_ind1 = c_population.cGetRandomInd(c_random);

			b_are_diffrent = bIndAreDifferent(*pc_ind, c_base_ind, c_add_ind0, c_add_ind1);

			if (b_are_diffrent)
			{
				for (int k = 0; k < pc_ind->iGetSolutionSize(); k++)
				{
					if (c_random.dGetRandomNumber(0, 1) < d_coss_prob)
					{
						d_tmp_gene = c_base_ind.dGetSolutionGene(k) + d_diff_weight *
							(c_add_ind0.dGetSolutionGene(k) - c_add_ind1.dGetSolutionGene(k));
						if (d_tmp_gene > pc_problem->dMinSolution(k) && d_tmp_gene < pc_problem->dMaxSolution(k)) c_ind_new.bSetSolutionGene(k, d_tmp_gene);
					}
					else
					{
						c_ind_new.bSetSolutionGene(k, pc_ind->dGetSolutionGene(k));
					}
				}

				if (pc_problem->dGetQuality(&pc_ind->pcGetSolutionTable()) < pc_problem->dGetQuality(&c_ind_new.pcGetSolutionTable()))
				{
					*pc_ind = c_ind_new;

					c_population.vPrintStatistic(*pc_problem);
				}
			}



		}

		i_best_quality_id = c_population.iGetBestIndId(*pc_problem);

		if (i_best_quality_id != -1)
		{
			d_tmp_quality = pc_problem->dGetQuality(&c_population.pcGetInd(i_best_quality_id)->pcGetSolutionTable()).first;

			if (d_tmp_quality > d_best_quality)
			{
				d_best_quality = d_tmp_quality;

				c_best_sol = *c_population.pcGetInd(i_best_quality_id);

				b_find_correct_sol = true;

				*pc_best_last_solution_with_restriction = c_best_sol;
			}
		}
	}

	return d_best_quality;
}

double CDiffEvol::dFindBestSol(int iEndingCondition, int iPopulationSize, FILE &pcFile)
{
	int i_seed = INT_DEFAULT_SEED;
	int i_best_quality_id;
	bool b_are_diffrent = false, b_find_correct_sol = false;
	double d_best_quality = -DBL_MAX, d_tmp_quality, d_tmp_gene;
	CPopulation c_population(iPopulationSize);
	CMscnSolution* pc_ind;
	CMscnSolution c_base_ind, c_add_ind0, c_add_ind1, c_ind_new, c_best_sol;
	CRandom c_random;


	c_population.vInitRandomPopulation(*pc_problem, i_seed);
	c_population.bWriteStatistics(pcFile, *pc_problem);

	for (int j = 0; j < iEndingCondition; j++)
	{
		for (int i = 0; i < c_population.iGetPopulationSize(); i++)
		{
			pc_ind = c_population.pcGetInd(i);
			c_ind_new = *pc_ind;
			c_base_ind = c_population.cGetRandomInd(c_random);
			c_add_ind0 = c_population.cGetRandomInd(c_random);
			c_add_ind1 = c_population.cGetRandomInd(c_random);

			b_are_diffrent = bIndAreDifferent(*pc_ind, c_base_ind, c_add_ind0, c_add_ind1);

			if (b_are_diffrent)
			{
				for (int k = 0; k < pc_ind->iGetSolutionSize(); k++)
				{
					if (c_random.dGetRandomNumber(0, 1) < d_coss_prob)
					{
						d_tmp_gene = c_base_ind.dGetSolutionGene(k) + d_diff_weight *
							(c_add_ind0.dGetSolutionGene(k) - c_add_ind1.dGetSolutionGene(k));
						if (d_tmp_gene > pc_problem->dMinSolution(k) && d_tmp_gene < pc_problem->dMaxSolution(k)) c_ind_new.bSetSolutionGene(k, d_tmp_gene);
					}
					else
					{
						c_ind_new.bSetSolutionGene(k, pc_ind->dGetSolutionGene(k));
					}
				}

				if (pc_problem->dGetQuality(&pc_ind->pcGetSolutionTable()) < pc_problem->dGetQuality(&c_ind_new.pcGetSolutionTable()))
				{
					*pc_ind = c_ind_new;

					c_population.bWriteStatistics(pcFile, *pc_problem);
				}
			}



		}

		i_best_quality_id = c_population.iGetBestIndId(*pc_problem);

		if (i_best_quality_id != -1)
		{
			d_tmp_quality = pc_problem->dGetQuality(&c_population.pcGetInd(i_best_quality_id)->pcGetSolutionTable()).first;

			if (d_tmp_quality > d_best_quality)
			{
				d_best_quality = d_tmp_quality;

				c_best_sol = *c_population.pcGetInd(i_best_quality_id);

				b_find_correct_sol = true;

				*pc_best_last_solution_with_restriction = c_best_sol;
			}
		}
	}

	return d_best_quality;
}

double CDiffEvol::dFindBestSolRestrictionEvol(int iEndingCondition, int iPopulationSize, FILE & pcFile)
{
	int i_seed = INT_DEFAULT_SEED;
	int i_best_quality_id;
	bool b_are_diffrent = false, b_find_correct_sol = false;
	double d_best_quality = -DBL_MAX, d_tmp_quality, d_tmp_gene;
	CPopulation c_population(iPopulationSize);
	CMscnSolution* pc_ind;
	CMscnSolution c_base_ind, c_add_ind0, c_add_ind1, c_ind_new, c_best_sol;
	CRandom c_random;


	c_population.vInitRandomPopulation(*pc_problem, i_seed);
	c_population.bWriteStatistics(pcFile, *pc_problem);

	for (int j = 0; j < iEndingCondition; j++)
	{
		for (int i = 0; i < c_population.iGetPopulationSize(); i++)
		{
			pc_ind = c_population.pcGetInd(i);
			c_ind_new = *pc_ind;
			c_base_ind = c_population.cGetRandomInd(c_random);
			c_add_ind0 = c_population.cGetRandomInd(c_random);
			c_add_ind1 = c_population.cGetRandomInd(c_random);

			b_are_diffrent = bIndAreDifferent(*pc_ind, c_base_ind, c_add_ind0, c_add_ind1);

			if (b_are_diffrent)
			{
				for (int k = 0; k < pc_ind->iGetSolutionSize(); k++)
				{
					if (c_random.dGetRandomNumber(0, 1) < d_coss_prob)
					{
						d_tmp_gene = c_base_ind.dGetSolutionGene(k) + d_diff_weight *
							(c_add_ind0.dGetSolutionGene(k) - c_add_ind1.dGetSolutionGene(k));
						if (d_tmp_gene > pc_problem->dMinSolution(k) && d_tmp_gene < pc_problem->dMaxSolution(k)) c_ind_new.bSetSolutionGene(k, d_tmp_gene);
					}
					else
					{
						c_ind_new.bSetSolutionGene(k, pc_ind->dGetSolutionGene(k));
					}
				}

				if (pc_problem->dGetQuality(&pc_ind->pcGetSolutionTable()) < pc_problem->dGetQuality(&c_ind_new.pcGetSolutionTable()) &&
					pc_problem->bConstraintsSatisfied(&c_ind_new.pcGetSolutionTable()))
				{
					*pc_ind = c_ind_new;

					c_population.bWriteStatistics(pcFile, *pc_problem);
				}
			}



		}

		i_best_quality_id = c_population.iGetBestIndId(*pc_problem);

		if (i_best_quality_id != -1)
		{
			d_tmp_quality = pc_problem->dGetQuality(&c_population.pcGetInd(i_best_quality_id)->pcGetSolutionTable()).first;

			if (d_tmp_quality > d_best_quality)
			{
				d_best_quality = d_tmp_quality;

				c_best_sol = *c_population.pcGetInd(i_best_quality_id);

				b_find_correct_sol = true;

				*pc_best_last_solution_with_restriction = c_best_sol;
			}
		}
	}

	return d_best_quality;
}

bool CDiffEvol::bIndAreDifferent(CMscnSolution& pcInd, CMscnSolution& pcBaseInd, CMscnSolution& pcAddInd0, CMscnSolution& pcAddInd1)
{
	if (pcInd != pcBaseInd) return true;
	if (pcInd != pcAddInd0) return true;
	if (pcInd != pcAddInd1) return true;
	if (pcBaseInd != pcAddInd0) return true;
	if (pcBaseInd != pcAddInd1) return true;

	return false;
}

CMscnSolution& CDiffEvol::pcGetBestSolution()
{
	return *pc_best_last_solution_with_restriction;
}

CDiffEvol::~CDiffEvol()
{
	delete pc_problem;
	delete pc_best_last_solution_with_restriction;
}

