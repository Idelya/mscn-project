#include "pch.h"
#include "CRandom.h"


CRandom::CRandom()
{
	bSetSeed(time(nullptr));
	d_min_of_range = DOUBLE_DEFAULT_MIN_OF_RANGE;
	d_max_of_range = DOUBLE_DEFAULT_MAX_OF_RANGE;
}

CRandom::CRandom(double dMinRange, double dMaxRange)
{
	bSetSeed(time(nullptr));
	if (dMinRange < dMaxRange)
	{
		d_min_of_range = dMinRange;
		d_max_of_range = dMaxRange;
	}
	else
	{
		d_min_of_range = DOUBLE_DEFAULT_MIN_OF_RANGE;
		d_max_of_range = DOUBLE_DEFAULT_MAX_OF_RANGE;
	}
}

CRandom::CRandom(double dMinRange, double dMaxRange, int iSeed)
{
	if (bSetSeed(iSeed)) bSetSeed(time(nullptr));
	if (dMinRange < dMaxRange)
	{
		d_min_of_range = dMinRange;
		d_max_of_range = dMaxRange;
	}
	else
	{
		d_min_of_range = DOUBLE_DEFAULT_MIN_OF_RANGE;
		d_max_of_range = DOUBLE_DEFAULT_MAX_OF_RANGE;
	}
}

bool CRandom::bSetNewRange(double dMinRange, double dMaxRange)
{
	if (dMinRange < dMaxRange)
	{
		d_min_of_range = dMinRange;
		d_max_of_range = dMaxRange;
		return true;
	}

	return false;
}

bool CRandom::bSetSeed(int iSeed)
{
	i_seed = iSeed;
	srand(iSeed);
	return true;
}

int CRandom::iGetSeed()
{
	return i_seed;
}

double CRandom::dGetRandomNumber()
{
	return (double)std::rand() / RAND_MAX * (d_max_of_range - d_min_of_range) + d_min_of_range;
}

double CRandom::dGetRandomNumber(double dMinRange, double dMaxRange)
{
	return (double)std::rand() / RAND_MAX * (dMaxRange - dMinRange) + dMinRange;
}

int CRandom::iGetRandomNumber()
{
	if (round(d_max_of_range) <= round(d_min_of_range)) return 0;
	return std::rand() / RAND_MAX * (floor(d_max_of_range) - ceil(d_min_of_range)) + ceil(d_min_of_range);
}

int CRandom::iGetRandomNumber(double dMinRange, double dMaxRange)
{
	double d_number_rand;
	if (floor(dMaxRange) <= ceil(dMinRange)) return 0;
	d_number_rand = (double)std::rand() / RAND_MAX * (floor(dMaxRange) - ceil(dMinRange)) + ceil(dMinRange);
	return round(d_number_rand);
}

CRandom::~CRandom()
{
}
