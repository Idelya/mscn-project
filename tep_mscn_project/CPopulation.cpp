#include "pch.h"
#include "CPopulation.h"


CPopulation::CPopulation(int iSize)
{
	pc_population = new CMscnSolution*[iSize];
	i_population_size = iSize;

	for (int i = 0; i < iSize; ++i)
	{
		pc_population[i] = new CMscnSolution();
	}
}

void CPopulation::vInitRandomPopulation(CMscnProblem & pcProblem, int iSeed)
{
	CRandom c_random;
	c_random.bSetSeed(iSeed);

	for (int i = 0; i < i_population_size; ++i)
	{
		pc_population[i]->bSetAmountOfDistrib(pcProblem.bGetAmountOfDistrib());
		pc_population[i]->bSetAmountOfFabrics(pcProblem.bGetAmountOfFabrics());
		pc_population[i]->bSetAmountOfMagazines(pcProblem.bGetAmountOfMagazines());
		pc_population[i]->bSetAmountOfShops(pcProblem.bGetAmountOfShops());


		pc_population[i]->vGenerateSolution(c_random, pcProblem);
	}


}

CMscnSolution CPopulation::cGetRandomInd(CRandom & pcRandomGener)
{
	int i_random_number = pcRandomGener.iGetRandomNumber(0, i_population_size - 1);
	return *pc_population[i_random_number];
}

CMscnSolution* CPopulation::pcGetInd(int iOffset)
{
	if (iOffset < 0 && iOffset >= i_population_size) return pc_population[0];
	return pc_population[iOffset];
}

int CPopulation::iGetBestIndId(CMscnProblem & pcProblem)
{
	double d_best_quality = -DBL_MAX;
	double d_tmp_quality;
	int i_best_sol_id = -1;

	for (int i = 0; i < i_population_size; ++i)
	{
		d_tmp_quality = pcProblem.dGetQuality(&pc_population[i]->pcGetSolutionTable()).first;
		if (pcProblem.bConstraintsSatisfied(&pc_population[i]->pcGetSolutionTable()) && d_best_quality < d_tmp_quality)
		{
			d_best_quality = d_tmp_quality;
			i_best_sol_id = i;
		}
	}
	return i_best_sol_id;
}

void CPopulation::vPrintStatistic(CMscnProblem & pcProblem)
{
	for (int i = 0; i < i_population_size; ++i)
	{
		std::cout << i << ". " << pcProblem.dGetQuality(&pc_population[i]->pcGetSolutionTable()).first << " ";
		std::cout << pcProblem.bConstraintsSatisfied(&pc_population[i]->pcGetSolutionTable()) << "  ";
	}
	std::cout << "\n";
}

bool CPopulation::bWriteStatistics(FILE & pcFile, CMscnProblem & pcProblem)
{
	int i_best_sol_id = -1;
	for (int i = 0; i < i_population_size; ++i)
	{

		fprintf(&pcFile, "%.2f; ", pcProblem.dGetQuality(&pc_population[i]->pcGetSolutionTable()).first);

	}
	for (int i = 0; i < i_population_size; ++i)
	{
		fprintf(&pcFile, "%i;", pcProblem.bConstraintsSatisfied(&pc_population[i]->pcGetSolutionTable()));

	}
	fprintf(&pcFile, "Best solution: ;");

	i_best_sol_id = iGetBestIndId(pcProblem);

	if (i_best_sol_id >= 0) fprintf(&pcFile, "%.2f; ", pcProblem.dGetQuality(&pcGetInd(i_best_sol_id)->pcGetSolutionTable()).first);

	fprintf(&pcFile, "\n");

	return true;
}

int CPopulation::iGetPopulationSize()
{
	return i_population_size;
}


CPopulation::~CPopulation()
{
	for (int i = 0; i < i_population_size; i++)
	{
		delete pc_population[i];
	}

	delete[] pc_population;
}

