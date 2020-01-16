#pragma once
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>

#define DOUBLE_DEFAULT_MIN_OF_RANGE 0.0;
#define DOUBLE_DEFAULT_MAX_OF_RANGE 100.0;

class CRandom
{
public:
	CRandom();
	CRandom(double dMinRange, double dMaxRange);
	CRandom(double dMinRange, double dMaxRange, int iSeed);
	bool bSetNewRange(double dMinRange, double dMaxRange);
	bool bSetSeed(int iSeed);
	int iGetSeed();
	double dGetRandomNumber();
	double dGetRandomNumber(double dMinRange, double dMaxRange);
	int iGetRandomNumber();
	int iGetRandomNumber(double dMinRange, double dMaxRange);
	~CRandom();
private:
	double d_min_of_range, d_max_of_range;
	int i_seed;
};



