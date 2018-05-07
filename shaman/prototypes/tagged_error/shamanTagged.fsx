
#load "../common_functions/Eft.fsx"
#load "../common_functions/Precision.fsx"

//-----------------------------------------------------------------------------
// TAGGER

/// a tag is an identifier for a block of code
type Tag = string

/// stores the current tag using a stack
let private tagList = System.Collections.Generic.Stack<Tag>()

/// sets the current tag
let inline startBlock blockName = 
   tagList.Push(blockName)

/// unsets the current tag
let inline endBlock () = 
   tagList.TryPop() |> ignore

/// returns the current tag
let inline private getTag () =
   if tagList.Count <= 0 then ("Main":Tag) else tagList.Peek()

//-----------------------------------------------------------------------------
// ERRORS

/// represents a colection of (tag->error)
type Errors = FSharp.Collections.Map<Tag,float>

/// an empty collection of errors
let emptyErrors = FSharp.Collections.Map.empty

/// applies a function to a collection of errors
let inline mapErrors f errors =
   FSharp.Collections.Map.map (fun _ error -> f error) errors

/// returns the sum of the individual errors
let inline totalError errors =
   FSharp.Collections.Map.fold (fun sum _ error -> sum + error) 0. errors

/// add some error to a given tag
let addError tag error (errors:Errors) =
   match errors.TryFind tag with
   | None -> errors.Add(tag, error)
   | Some previousError -> errors.Add(tag, error + previousError)

/// add two colection of errors
let inline sumErrors (e1:Errors) (e2:Errors) =
   FSharp.Collections.Map.fold (fun errors tag error -> addError tag error errors) e1 e2

/// substract two collection of errors
let inline subErrors (e1:Errors) (e2:Errors) =
   FSharp.Collections.Map.fold (fun errors tag error -> addError tag (-error) errors) e1 e2

/// multiplies a colelction of errors by a number
let inline multError scaling errors =
   mapErrors ((*) scaling) errors

/// divides a colelction of errors by a number
let inline divError scaling errors =
   mapErrors (fun previousError -> previousError / scaling) errors

/// takes two collections of error, return a collection of products o(n²)
/// when two tags differ, their product is cut into a sum propotionaly to their magnitude
///
/// a cheaper (o(n)) alternative is to ignore every second order terms and to keep only products of a tag with himself
let multErrors (errors1:Errors) (errors2:Errors) =
   let mutable result = emptyErrors
   for kv1 in errors1 do 
      for kv2 in errors2 do 
         let error = kv1.Value*kv2.Value
         if error <> 0. then
            if kv1.Key = kv2.Key then 
               result <- addError kv1.Key error result
            else
               let sum = abs kv1.Value + abs kv2.Value
               let error1 = ((abs kv1.Value) / sum)*error
               result <- addError kv1.Key error1 result
               let error2 = ((abs kv2.Value) /sum)*error
               result <- addError kv2.Key error2 result
   result

//-----------------------------------------------------------------------------
// Sdouble

/// represents a number and a collection of (tag->error)
type Sdouble = {number : float; errors : Errors}

/// creates an Sdouble
let inline Sdouble x = 
   {number = x; errors = emptyErrors}

/// creates an Sdouble from an int
let inline SdoubleOfInt n = 
   {number = float n; errors = emptyErrors}

/// returns the current best approximation for a given Sdouble
let inline SdoubleToDouble s =
   s.number + (totalError s.errors)

/// takes an Sdouble and produces a human readable string
/// Note : we might decide to only print error terms over a given relative error
/// (1 ulp or at least enough to lose a significant digit)
let toString s =
   let errors = 
      s.errors 
      |> Seq.sortByDescending (fun kv -> abs kv.Value)
      |> Seq.map (fun kv -> sprintf "%s:%.1e" kv.Key kv.Value)
      |> Seq.reduce (fun result s -> result + ", " + s)
   let number = Precision.toString s.number (totalError s.errors)
   number + " [" + errors + "]"

//-----------------------------------------------------------------------------
// OPERATORS

/// ~-
let inline (~-.) s =
   {number = - s.number; errors = mapErrors (~-) s.errors}

/// +
let inline (+.) s1 s2 =
   let (number,error) = Eft.sum s1.number s2.number
   let currentTag = getTag ()
   // s1.errors + s2.errors + error
   let errors = sumErrors s1.errors s2.errors |> addError currentTag error
   {number = number; errors = errors}

/// -
let inline (-.) s1 s2 =
   s1 +. (-. s2)

/// *
let inline ( *. ) s1 s2 =
   let (number,error) = Eft.mult s1.number s2.number
   let currentTag = getTag ()
   // s1.errors*s2.number + s2.errors*s1.number + error
   let errors = 
      sumErrors (multError s2.number s1.errors) (multError s1.number s2.errors) 
      |> sumErrors (multErrors s1.errors s2.errors)
      |> addError currentTag error
   {number = number; errors = errors}

/// /
let inline (/.) s1 s2 =
   let (number,remainder) = Eft.div s1.number s2.number
   let currentTag = getTag ()
   // ((remainder + s1.errors) - number*.s2.errors) / (s2.number + s2.error)
   let errors = 
      subErrors (addError currentTag remainder s1.errors) (multError number s2.errors)
      |> divError (SdoubleToDouble s2) // warning : this operation does not transmit responsability from the denom
   {number = number; errors = errors}

/// sqrt
let inline sqrt s =
   let (number,remainder) = Eft.sqrt s.number
   let currentTag = getTag ()
   // (remainder + s.errors) / (number + number)
   let errors = (addError currentTag remainder s.errors) |> divError (number+number)
   {number = number; errors = errors}

/// abs
let inline abs s =
   if s.number > 0. then s else -. s 

(*
   non linear functions could be added by :
   - computing the error of the fonction
   - splitting it proportionaly to the responsability of each tag from the input
     we can split by computing coeffiscients (alpha_i) so that : sum(alpha_i * f(x_i)) = f(sum(x_i))
     we could set alpha_i = alpha = f(sum(x_i)) / sum(f(x_i))
   it might work well for monotonous functions, not so much for cosinus/sinus/...
*)
