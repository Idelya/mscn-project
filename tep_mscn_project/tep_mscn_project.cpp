#include "pch.h"
#include "CMatrix.h"
#include "CMscnProblem.h"
#include "CMscnSolution.h"
#include "CRandomSearch.h"
#include "CDiffEvol.h"
#include "CRandom.h"
#include <iostream>

int main()
{
	bool b_is_working;
	CMscnProblem c_my_implement;
	CMscnSolution c_my_sol;

	int i_seed = 10;
	int i_iterations = 100000;

	double d_cross_prob = 0.4;
	double d_diff_weight = 2;
	int i_populatin_iter = 40;
	int i_population_size = 10;
	double d_sol;
	FILE * pf_file_statistics = fopen("MyRandomProblemStatistics.csv", "w+");
	FILE * pf_file_statistics_restriction = fopen("MyRandomProblemStatisticsRestriction.csv", "w+");

	b_is_working = c_my_implement.bImportProblem("MyRandomProblem.txt");

	if (!b_is_working)
	{
		std::cout << "Problem z wczytaniem pliku \n" << std::endl;
		return 0;
	}
	std::cout << "Przeszukiwanie rozwiazan dla losowego problemu MSCN metoda Diff Evolution" << std::endl;

	CDiffEvol c_search_evol_solution(c_my_implement, d_cross_prob, d_diff_weight, i_seed);

	d_sol = c_search_evol_solution.dFindBestSol(i_populatin_iter, i_population_size, *pf_file_statistics);

	std::cout << std::endl << "Najlepsze rozwiazanie: ";
	if (d_sol > -DBL_MAX) {
		std::cout << d_sol;
		c_my_sol.operator=(c_search_evol_solution.pcGetBestSolution());
		c_my_sol.bExportSolution("MyEvolProblemSol.txt");
	}
	else std::cout << "nie znaleziono poprawnego rozwiazania";

	CDiffEvol c_search_evol_solution_restriction(c_my_implement, d_cross_prob, d_diff_weight, i_seed);

	d_sol = c_search_evol_solution_restriction.dFindBestSolRestrictionEvol(i_populatin_iter, i_population_size, *pf_file_statistics_restriction);

	std::cout << std::endl << "Najlepsze rozwiazanie: ";
	if (d_sol > -DBL_MAX) {
		std::cout << d_sol;
		c_my_sol.operator=(c_search_evol_solution_restriction.pcGetBestSolution());
		c_my_sol.bExportSolution("MyEvolProblemSolRestriction.txt");
	}
	else std::cout << "nie znaleziono poprawnego rozwiazania";


	fclose(pf_file_statistics);
	fclose(pf_file_statistics_restriction);
	return 0;
}