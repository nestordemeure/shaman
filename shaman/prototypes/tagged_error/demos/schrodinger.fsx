
#load "../shamanTagged.fsx"
open ShamanTagged

//#load "../../classic_shaman/shamanClassic.fsx"
//open ShamanClassic

//-------------------------------------------------------------------------------------------------
// INITIALIZATION
startBlock "Initialisation"

let numberDivisions = 200
let psi = 
   let psi = Array.create (numberDivisions+1) (Sdouble 0.)
   psi.[0] <- Sdouble(0.0)
   psi.[1] <- Sdouble(-1e-9)
   psi.[numberDivisions] <- Sdouble(1.0)
   psi
let EMin = Sdouble(1.490)
let mutable ECurrent = EMin
let xMin = Sdouble(-15.)
let xMax = Sdouble(15.)
let hZero = (xMax -. xMin) /. (SdoubleOfInt numberDivisions)
let EDelta = Sdouble(1e-7)
let maxPsi = Sdouble(1e-8)

endBlock ()
//-------------------------------------------------------------------------------------------------
// COMPUTATIONS

(*
 * compute Kn using the numeric values that we already found
 *)
let calculateKSSquared n =
   startBlock "KSSquared"
   let x = (hZero *. (SdoubleOfInt n)) +. xMin
   //let x = ((SdoubleOfInt n)*.xMax +. xMin*.(SdoubleOfInt (numberDivisions - n))) /. (SdoubleOfInt numberDivisions)
   let result = (Sdouble(0.05) *. ECurrent) -. ((x*.x) *. Sdouble(5.63e-3))
   endBlock ()
   result

(*
 * uses the Numerov algorithm to get the next value of psi
 *)
let calculateNextPsi n =
   startBlock "NextPsi"
   let KSqNMinusOne = calculateKSSquared (n - 1)
   let KSqN = calculateKSSquared n
   let KSqNPlusOne = calculateKSSquared (n + 1)
   let mutable nextPsi = Sdouble(2.0) *. (Sdouble(1.0) -. (Sdouble(5.0) *. hZero *. hZero *. KSqN /. Sdouble(12.0))) *. psi.[n]
   nextPsi <- nextPsi -. (Sdouble(1.0) +. (hZero *. hZero *. KSqNMinusOne /. Sdouble(12.0))) *. psi.[n-1]
   nextPsi <- nextPsi /. (Sdouble(1.0) +. (hZero *. hZero *. KSqNPlusOne /. Sdouble(12.0)))
   endBlock ()
   nextPsi

//-----

(*
 * computation
 *)
let calculate () =
   printfn "starting computations"
   let mutable k = 0
   startBlock "main loop"
   // if abs(psi[200]) < maximum allowed psi, we have the answer, our guess for the energy is correct
   while abs psi.[numberDivisions] > maxPsi do
      k <- k+1
      // recomputes psi
      for i = 1 to numberDivisions-1 do
         psi.[i+1] <- calculateNextPsi(i)
      // correct ECurrent
      match psi.[numberDivisions] > Sdouble(0.0) with 
      | true -> ECurrent <- ECurrent -. EDelta // we need to subtract deltaE and try again
      | false -> ECurrent <- ECurrent +. EDelta // we need to add deltaE and try again
      // print the current result
      if k % 1000 = 0 then printfn "%d\tPsi200: %s\n%d\t E: %s" k (toString psi.[numberDivisions]) k (toString ECurrent)
   endBlock ()
   printfn "The ground state energy is %s MeV (analytic solution : 1.5 MeV, psi : %s)." (toString ECurrent) (toString psi.[numberDivisions])

//-----------------------------------------------------------------------------
// TEST

calculate ()