//
// Created by demeuren on 20/02/18.
//

#ifndef SHAMAN_DEMO_EIGEN_H
#define SHAMAN_DEMO_EIGEN_H

#include <Eigen/Dense>

/*
 * short demo of eigen
 */
void eigenDemo()
{
    Eigen::MatrixXd m(2,2);
    m(0,0) = 3;
    m(1,0) = 2.5;
    m(0,1) = -1;
    m(1,1) = m(1,0) + m(0,1);
    std::cout << m << std::endl;
}

#endif //SHAMAN_DEMO_EIGEN_H