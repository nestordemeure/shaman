//
// Created by chevalier on 08/08/18.
//

#pragma once


/*
 * displays the absolute difference between a computed value and the expected value
 */
template<typename numberType, typename errorType, typename preciseType>
inline void displayError(S<numberType,errorType,preciseType> result, double expectedResult)
{
  double error = result.number - expectedResult;
  double digits = -log10(std::abs(error / result.number));
  double estimatedDigits = result.digits();

  std::cout << "result=" << result << " (" << result.number << ')'
            << '\n'
            << " absolut_error=" << error
            << " estimated_absolut_error=" << result.error
            << '\n'
            << " significativ_digits=" << digits
            << " estimated_significativ_digits=" << estimatedDigits
            << '\n' << std::endl;
}

/*
 * displays the absolute difference between a computed value and the expected value
 */
template<typename T> inline void displayError(T result, double expectedResult)
{
  double fresult = result;
  double error = fresult - expectedResult;
  double digits = -log10(std::abs(error / fresult));

  std::cout << "result=" << result
            << std::endl
            << " absolut_error=" << error
            << std::endl
            << " significativ_digits=" << digits
            << '\n' << std::endl;
}
