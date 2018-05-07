
#load "../shamanLoop.fsx"
open ShamanLoop

//-----------------------------------------------------------------------------

(*
   x <- initialisation()
   while not valid(x) do
      x <- correction(x)
   return x

   shaman will take the initialisation and correction error into account in the quality of the result
   but they have no impact since we use an iterativ process that validates x
   (we can even make those errors arbitrarly large)
   such cases can be detected and manually corrected with tagged error (but i see no possible automatic solution)

   asynchronous stochastic approaches can deal with such algorithms
   but, for the same reasons, they fail for systems that converges toward a wrong answer
*)
let convergence () =
   startBlock "Test:Convergence"
   startBlock "initialisation"
   let targetNumber = Sdouble(1.)
   let finalError = Sdouble(2.)
   let bigNumber = Sdouble(1e100)
   let mutable result = (finalError +. bigNumber) -. bigNumber
   printfn "intermediate result = %s" (toString result)
   endBlock ()
   startBlock "correction"
   let epsilon = Sdouble(0.1)
   let minThreshold = targetNumber -. epsilon /. Sdouble(2.)
   let maxThreshold = targetNumber +. epsilon /. Sdouble(2.)
   let loop = ShamanLoop()
   while loop.Cond((result <=. minThreshold) ||. (result >=. maxThreshold), result) do  
      if result <= minThreshold then result <- result +. epsilon
      else result <- result -. epsilon
      printfn "%s (%f + %f)" (toString result) result.number result.error
   let [|result|] = loop.RectifyVariables(result)
   endBlock ()
   printfn "result = %s" (toString result)
   endBlock ()

//-----------------------------------------------------------------------------
// TEST

//testPol ()
//testAttractor ()
//kahanTest ()
convergence ()