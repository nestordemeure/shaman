
#load "../shamanTagged.fsx"
open ShamanTagged

//-----------------------------------------------------------------------------

/// a test that should be simple
let testPol () =
   startBlock "Test:Pol"
   printfn "Rump equation."
   printfn "Can we differentiate a bad result from a good one ?"
   let polynom x y = 
      startBlock "Polynom"
      let result = Sdouble(9.0)*.x*.x*.x*.x -. y*.y*.y*.y +. Sdouble(2.0)*.y*.y
      endBlock ()
      result
   // stable
   let x2 = Sdouble(1.0) /. Sdouble(3.0)
   let y2 = Sdouble(2.0) /. Sdouble(3.0)
   let result2 = polynom x2 y2
   printfn "P(1/3,2/3) = %s ≈ %f (exact value : 0.8024691358024691)" (toString result2) (SdoubleToDouble result2)
   // unstable
   let x1 = Sdouble(10864.0)
   let y1 = Sdouble(18817.0)
   let result1 = polynom x1 y1
   printfn "P(10864,18817) = %s ≈ %f (exact value : 1)" (toString result1) (SdoubleToDouble result1)
   endBlock ()

/// a test with an unstable division that makes us unable to track the true precision
let testAttractor () =
   startBlock "Test:Attractor"
   printfn "Attractor."
   printfn "The unstable division make us unable to track the precision past them."
   let mutable a = Sdouble(5.5)
   let mutable b = Sdouble(61.0) /. Sdouble(11.0)
   startBlock "loop"
   for i = 1 to 50 do
      let c = b
      b <- Sdouble(111.0) -. Sdouble(1130.0)/.b +. Sdouble(3000.0)/.(a*.b)
      a <- c
      printfn "U(%d) = %s ≈ %f" i (toString b) (SdoubleToDouble b)
   endBlock ()
   printfn "The true limit is 6."
   endBlock ()

let kahanTest () =
   startBlock "Test:Kahan"
   /// a fucntion that should be equivalent to the identity
   let identity xInit =
      let mutable x = Sdouble(xInit)
      // sqrt(x) 128 times
      startBlock "sqrt"
      for i = 1 to 128 do
         x <- sqrt x
         //x <- (x -. Sdouble(1.)) /. Sdouble(2.)
         printfn "%s" (toString x)
      endBlock ()
      // x squared 128 times
      startBlock "square"
      for i = 1 to 128 do
         x <- x*.x
         //x <- x *. Sdouble(2.) +. Sdouble(1.)
         printfn "%s" (toString x)
      endBlock ()
      printfn "Identity test x = %f ≈ %s" xInit (toString x)
   // tests
   //identity 0.
   identity 0.5
   //identity 0.75
   //identity 1.0
   //identity 2.0
   //identity 0.9
   //identity 5.0
   endBlock ()

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
   while result <= minThreshold || result >= maxThreshold do  
      if result <= minThreshold then result <- result +. epsilon
      else result <- result -. epsilon
      printfn "%s" (toString result)
   endBlock ()
   printfn "result = %s" (toString result)
   endBlock ()

//-----------------------------------------------------------------------------
// TEST

testPol ()
//testAttractor ()
//kahanTest ()
//convergence ()