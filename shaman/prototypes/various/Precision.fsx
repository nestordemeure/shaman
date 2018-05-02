
/// returns the number of significativ digits of a number given its error
/// the output is garanteed to be >= 0.
let significativDigits number error =
   match error, number with 
   | 0., _ -> infinity
   | _, 0. -> max 0. -(log10(abs error) + 1.)
   | _ -> let relativeError = abs (error/number)
          if (relativeError >= 1.) then 0. else -log10 relativeError

/// returns the couple with the lowest precision
let minPrecision (n1, e1) (n2, e2) =
    match abs(e1 * n2) > abs(n1 * e2) with
    | true -> (n1, e1) 
    | false -> (n2, e2)

/// returns true if a cancelation occured
/// cancelation = we have lost more than cancel_level significative digits
let isCancelation (number, error) (result, resultingError) =
   let cancelLevel = 4
   let baseNum = 10
   let threshold = (pown baseNum cancelLevel) |> float
   (error <> 0.) && (abs(resultingError * number) > threshold * abs(error * result))

/// returns true if a number is non significativ
let inline nonSignificativ number error = 
   let baseNum = 10. // the bigger the more cautious
   (error <> 0.) && (abs(number) < baseNum * abs(error))

/// returns a string with the correct number of significativ digits
let toString number error =
   let maxPrecision = 15.
   let precision = significativDigits number error |> min maxPrecision |> floor |> int
   if precision <= 0 then "@.0" else sprintf "%.*e" precision number