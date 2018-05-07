
#load "../common_functions/Eft.fsx"
#load "../common_functions/Precision.fsx"

//-----------------------------------------------------------------------------
// Sdouble

/// represents a number its numerical error
type Sdouble = {number : float; error : float}

/// creates an Sdouble
let inline Sdouble x = 
   {number = x; error = 0.}

/// creates an Sdouble from an int
let inline SdoubleOfInt n = 
   {number = float n; error = 0.}

/// returns the current best approximation for a given Sdouble
let inline SdoubleToDouble s =
   s.number + s.error

/// takes an Sdouble and produces a human readable string
let toString s =
   Precision.toString s.number s.error

//-----------------------------------------------------------------------------
// OPERATORS

/// ~-
let inline (~-.) s =
   {number = - s.number; error = - s.error}

/// +
let inline (+.) s1 s2 =
   let (number,error) = Eft.sum s1.number s2.number
   let newError = s1.error + s2.error + error
   {number = number; error = newError}

/// -
let inline (-.) s1 s2 =
   s1 +. (-. s2)

/// *
let inline ( *. ) s1 s2 =
   let (number,error) = Eft.mult s1.number s2.number
   let newError = s1.error*s2.number + s2.error*s1.number + s1.error*s2.error + error
   {number = number; error = newError}

/// /
let inline (/.) s1 s2 =
   let (number,remainder) = Eft.div s1.number s2.number
   let newError = ((remainder + s1.error) - number*s2.error) / (SdoubleToDouble s2)
   {number = number; error = newError}

/// sqrt
let inline sqrt s =
   let (number,remainder) = Eft.sqrt s.number
   let newError =  (remainder + s.error) / (number + number)
   {number = number; error = newError}

/// abs
let inline abs s =
   if s.number > 0. then s else -. s 

//-----------------------------------------------------------------------------
// Loop

/// a types that stores both the result of a boolean operation with number
/// and the result corrected by taking rounding errors into account
type Sbool = {ieee : bool; corrected : bool}

/// transforms a boolean comparaison into an Sbool comparaison
let SboolComparaison comparaison s1 s2 =
   let ieee = comparaison s1.number s2.number
   let corrected = comparaison (SdoubleToDouble s1) (SdoubleToDouble s2)
   //if ieee <> corrected then failwith "ERROR : inconsistant loop"
   {ieee = ieee; corrected = corrected}

/// transforms a boolean operator into an Sbool operator
/// WARNING : it does not preserve lazyness
let SboolOperator operator b1 b2 =
   let ieee = operator b1.ieee b2.ieee
   let corrected = operator b1.corrected b2.corrected
   {ieee = ieee; corrected = corrected}

let (<.) = SboolComparaison (<)
let (>.) = SboolComparaison (>)
let (<=.) = SboolComparaison (<)
let (>=.) = SboolComparaison (>)
let (=.) = SboolComparaison (=)
let (<>.) = SboolComparaison (<>)

let (||.) = SboolOperator (||)
let (&&.) = SboolOperator (&&)

//-----

/// takes two sdouble and return the number of the first
/// with the error it would have if the correction of the second was the true result
let correctError sIeee sCorrected =
   let number = sIeee.number
   let error = (SdoubleToDouble sCorrected) - number
   {number = number; error = error}

type ShamanLoop() = 
   /// stores the out variables
   let mutable temporaryVariables = [||]
   /// analyses the loop condition and save the variables if needed
   member this.Cond(loopCond, [<System.ParamArray>] outVariables) =
      match loopCond.ieee, loopCond.corrected, Array.isEmpty temporaryVariables with
      | false, true, true -> 
         // the corrected loop would run longer
         printfn "WARNING : Loop, the corrected loop would run longer"
      | true, false, true -> 
         // it is not the last iteration but the corrected loop would stop here
         printfn "WARNING : Loop, an early stop was detected"
         temporaryVariables <- outVariables
      | _ -> () // either both conditions agree or we already had an earlystop
      loopCond.ieee
   /// corrects the given variables if needed
   member this.RectifyVariables([<System.ParamArray>] outVariables) =
      match Array.isEmpty temporaryVariables with
      | true -> 
         printfn "Loop : Nothing to rectify."; outVariables
      | false -> 
         (Array.map2 correctError temporaryVariables outVariables)

//-----------------------------------------------------------------------------
// Dummy functions (for compatibility with shaman tagged examples)

let startBlock = ignore
let endBlock = ignore
