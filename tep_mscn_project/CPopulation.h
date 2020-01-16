#pragma once
#include "CMscnSolution.h"
#include "CMscnProblem.h"

class CPopulation
{
public:
	CPopulation(int iSize);
	int iGetPopulationSize();
	void vInitRandomPopulation(CMscnProblem& pcProblem, int iSeed);
	CMscnSolution cGetRandomInd(CRandom& pcRandomGener);
	CMscnSolution* pcGetInd(int iOffset);
	int iGetBestIndId(CMscnProblem& pcProblem);
	void vPrintStatistic(CMscnProblem& pcProblem);
	bool bWriteStatistics(FILE &pcFile, CMscnProblem & pcProblem);
	~CPopulation();
private:
	CMscnSolution ** pc_population;
	int i_population_size;
};


