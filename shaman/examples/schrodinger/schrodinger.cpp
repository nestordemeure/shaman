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
    psi = std::vector<Sdouble>(numberDivisions+1);
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
Sdouble Schrodinger::calculateKSSquared(int n)
{
    FUNCTION_BLOCK;
    //number x = (hZero * n) + xMin;
    Sdouble x = (((double) n)*xMax + xMin*((double) numberDivisions-n)) / ((double) numberDivisions);
    return ((0.05 * ECurrent) - ((x*x) * 5.63e-3));
}

/*
 * uses the Numerov algorithm to get the next value of psi
 */
Sdouble Schrodinger::calculateNextPsi(int n)
{
    FUNCTION_BLOCK;
    Sdouble KSqNMinusOne = calculateKSSquared(n - 1);
    Sdouble KSqN = calculateKSSquared(n);
    Sdouble KSqNPlusOne = calculateKSSquared(n + 1);

    Sdouble nextPsi = 2.0 * (1.0 - (5.0 * hZero * hZero * KSqN / 12.0)) * psi[n];
    nextPsi -= (1.0 + (hZero * hZero * KSqNMinusOne / 12.0)) * psi[n-1];
    nextPsi /= (1.0 + (hZero * hZero * KSqNPlusOne / 12.0));

    return nextPsi;
}

//-----

/*
 * computation
 */
void Schrodinger::calculate()
{
    FUNCTION_BLOCK;
    unsigned int k = 0;

    // if abs(psi[200]) < maximum allowed psi, we have the answer, our guess for the energy is correct
    while(Sstd::abs(psi[numberDivisions]) > maxPsi)
    {
        k++;

        for(int i = 1; i < numberDivisions; i++)
        {
            psi[i+1] = calculateNextPsi(i);
        }

        // if abs(psi[200]) > maximum allowed psi && psi[200] > 0, we need to subtract deltaE and try again
        if (psi[numberDivisions] > 0.0)
        {
            ECurrent -= EDelta;
        }
        // if abs(psi[200]) > maximum allowed psi && psi[200] < 0, we need to add deltaE and try again
        else
        {
            ECurrent += EDelta;
        }

        if(k % 500 == 0)
        {
            std::cout << k << " Psi200: " << psi[numberDivisions] << " E: " << ECurrent << '\n';
        }
    }

    std::cout << "The ground state energy is " << ECurrent << " MeV (analytic solution : 1.5 MeV, psi : " << psi[numberDivisions] << ")." << std::endl;
}

