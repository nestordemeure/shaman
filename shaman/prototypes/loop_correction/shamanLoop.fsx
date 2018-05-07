
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
// BOOLEAN OPERATORS

type MultiBool = {IEEE:Boolean; corrected:Boolean}

/// turns a comparaison operator into an operator that produces a MultiBool
let inline multiOperator op s1 s2 =
    let i3e = op s1.number s2.number
    let cor = op (SdoubleToDouble s1) (SdoubleToDouble s2)
    {IEEE=i3e; corrected=cor}

let inline (<.) = multiOperator (<)
let inline (<=.) = multiOperator (<=)
let inline (>.) = multiOperator (>)
let inline (>=.) = multiOperator (>=)
let inline (=.) = multiOperator (=)
let inline (<>.) = multiOperator (<>)

let inline (&&.) b1 b2 =
    {IEEE = b1.IEEE && b2.IEEE; corrected = b1.corrected && b2.corrected}

let inline (||.) b1 b2 =
    {IEEE = b1.IEEE || b2.IEEE; corrected = b1.corrected || b2.corrected}

//-----------------------------------------------------------------------------
// Dummy functions (for compatibility with shaman tagged examples)

let startBlock = ignore
let endBlock = ignore
