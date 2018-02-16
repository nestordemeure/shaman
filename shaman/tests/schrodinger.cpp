//
// Created by demeuren on 26/01/18.
//

#include <iostream>
#include "schrodinger.h"

//-------------------------------------------------------------------------------------------------
// INITIALIZATION

/*
 * constructor
 *
 * initialize the constants
 */
Schrodinger::Schrodinger()
{
    ECurrent = EMin;
    psi = std::vector<real>(numberDivisions+1);
    psi[0] = 0.0;
    psi[1] = -1e-9;
    psi[numberDivisions] = 1.0;
    hZero = (xMax - xMin) / ((double) numberDivisions);
}

//-------------------------------------------------------------------------------------------------
// COMPUTATIONS

/*
 * compute Kn using the numeric values that we already found
 */
real Schrodinger::calculateKSSquared(int n)
{
    //real x = (hZero * n) + xMin;
    real x = (((double) n)*xMax + xMin*((double) numberDivisions-n)) / ((double) numberDivisions);
    return ((0.05 * ECurrent) - ((x*x) * 5.63e-3));
}

/*
 * uses the Numerov algorithm to get the next value of psi
 */
real Schrodinger::calculateNextPsi(int n)
{
    real KSqNMinusOne = calculateKSSquared(n - 1);
    real KSqN = calculateKSSquared(n);
    real KSqNPlusOne = calculateKSSquared(n + 1);

    real nextPsi = 2.0 * (1.0 - (5.0 * hZero * hZero * KSqN / 12.0)) * psi[n];
    nextPsi = nextPsi - (1.0 + (hZero * hZero * KSqNMinusOne / 12.0)) * psi[n-1];
    nextPsi = nextPsi / (1.0 + (hZero * hZero * KSqNPlusOne / 12.0));

    return nextPsi;
}

//-----

/*
 * computation
 */
void Schrodinger::calculate()
{
    unsigned int k = 0;

    // if abs(psi[200]) < maximum allowed psi, we have the answer, our guess for the energy is correct
    while(fabs(psi[numberDivisions]) > maxPsi)
    {
        k++;

        for(int i = 1; i < numberDivisions; i++)
        {
            psi[i+1] = calculateNextPsi(i);
        }

        // if abs(psi[200]) > maximum allowed psi && psi[200] > 0, we need to subtract deltaE and try again
        if (psi[numberDivisions] > 0.0)
        {
            ECurrent = ECurrent - EDelta;
        }
        // if abs(psi[200]) > maximum allowed psi && psi[200] < 0, we need to add deltaE and try again
        else
        {
            ECurrent = ECurrent + EDelta;
        }

        //std::cout << k << " Psi200: " << psi[numberDivisions] << " E: " << ECurrent << '\n';
    }

    std::cout << "The ground state energy is " << ECurrent << " MeV (analytic solution : 1.5 MeV, psi : " << psi[numberDivisions] << ")." << std::endl;
}

