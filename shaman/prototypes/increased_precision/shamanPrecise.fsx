
#load "../common_functions/Eft.fsx"
#load "../common_functions/Precision.fsx"

//-----------------------------------------------------------------------------
// BASE TYPE

/// represents a double and its error (each term in the list is the error of the previous term)
/// [] is considered as a 0
type Errors = double list
type Sdouble = {number : double ; errors : Errors}

/// converts a double to an Sdouble
let inline Sdouble x : Sdouble = 
   {number = x; errors = [0.]} // adding zeros to the error will augment the base precision of the type

/// returns our best approximation for the current total error
let rec approximateTotalError (l:Errors) = 
   match l with 
   | [] -> 0.
   | [x] -> x
   | t :: q -> t + approximateTotalError q

/// returns true if a number needs more precision
let inline needsPrecision number error = 
   Precision.nonSignificativ number error

/// returns our best approximation for an Sdouble
let inline approximateDouble (d:Sdouble) = d.number + approximateTotalError d.errors

/// improves the precision of the number when the penultimate composants needs it to stay accurate
let rec resetPrecision (l:Errors) =
   match l with 
   | [number; error] when needsPrecision number error -> [number; error; 0.]
   //| [number; error1; error2] when needsPrecision number error1 -> [number; error1; error2; 0.]
   | t::q -> t :: resetPrecision q
   | _ -> l

/// returns true if the two last digits need more precision
let rec errorsNeedsPrecision (l:Errors) =
   match l with 
   | [number; error] -> needsPrecision number error
   | _ :: q -> errorsNeedsPrecision q
   | _ -> false

//-----------------------------------------------------------------------------
// OPERATIONS

let addzero l = l @ [0.]

/// insert in a list sorted by absolute value
let rec insertAbsSorted x l = 
   match l with
   | t :: q when abs t > abs x -> 
      //printfn "changed order"
      t :: insertAbsSorted x q
   | _ -> x :: l

let insert x l = 
   //insertAbsSorted x l
   x :: l

/// uses an EFT to sum the list (without changing its length)
let EFTreduce l =
   let rec reduce result l = 
      match l with 
      | [] -> result
      | [t] -> t :: result
      | t1 :: t2 :: q ->
         let (sum,error) = Eft.sum t1 t2
         reduce (error::result) (sum::q)
   l |> List.rev |> reduce []

//-----

/// unary minus for an Sdouble
let (~-.) (l:Errors) = 
   List.map (~-) l

/// sum two Sdouble
let rec (+.) (l1:Errors) (l2:Errors) =
   match l1, l2 with 
   | [], l | l, [] -> l
   | [t1], [t2] -> [t1 + t2]
      //let (result, error) = EFTsum t1 t2
      //if needsPrecision result error then [result; error] else [result]
(*
   // cancelation correction
   | [n1; e1], n2 :: q2 ->
      let (result, error) = EFTsum n1 n2
      let e2 = approximateTotalError q2
      let newError = [e1] +. q2 +. [error]
      let newe = approximateTotalError newError
      if isCancelation2 (n1,e1) (n2,e2) (result,newe) then 
         printfn "cancelation spotted +"
         (l1 @ [0.]) +. (l2 @ [0.]) // computation is done again with increased precision
      else insert result newError
   | n1 :: q1, [n2 ; e2] ->
      let (result, error) = EFTsum n1 n2
      let e1 = approximateTotalError q1
      let newError = q1 +. [e2] +. [error]
      let newe = approximateTotalError newError
      if isCancelation2 (n1,e1) (n2,e2) (result,newe) then 
         printfn "cancelation spotted +"
         (l1 @ [0.]) +. (l2 @ [0.]) // computation is done again with increased precision
      else insert result newError
*)

   | t1::q1, t2::q2 ->
      let (result, error) = Eft.sum t1 t2
      let newError = q1 +. q2 +. [error]
      //insert result newError 
      //|> EFTreduce
      //|> resetPrecision
      let l = insert result newError
      if errorsNeedsPrecision l then (addzero l1) +. (addzero l2) else EFTreduce l

/// substract two Sdouble
let (-.) (l1:Errors) (l2:Errors) = 
   l1 +. (-. l2)

/// multiply two Sdouble
let rec ( *. ) (l1:Errors) (l2:Errors) =
   match l1, l2 with 
   | [], _ | _, [] -> []
   | [t1], [t2] -> [t1 * t2]
      //let (result, error) = EFTmult t1 t2
      //if needsPrecision result error then [result; error] else [result]
(*
   // cancelation correction
   | [t1; e1], t2 :: q2 ->
      let (result, error) = EFTmult t1 t2
      let e2 = approximateTotalError q2
      let newError = [t1]*.q2 +. [t2]*.[e1] +. [e1]*.q2 +. [error]
      let newe = approximateTotalError newError
      if isCancelation2 (t1,e1) (t2,e2) (result,newe) then 
         printfn "cancelation spotted *"
         (l1 @ [0.]) *. (l2 @ [0.]) // computation is done again with increased precision
      else insert result newError
   | t1 :: q1, [t2 ; e2] ->
      let (result, error) = EFTmult t1 t2
      let e1 = approximateTotalError q1
      let newError = [t1]*.[e2] +. [t2]*.q1 +. q1*.[e2] +. [error]
      let newe = approximateTotalError newError
      if isCancelation2 (t1,e1) (t2,e2) (result,newe) then 
         printfn "cancelation spotted *"
         (l1 @ [0.]) *. (l2 @ [0.]) // computation is done again with increased precision
      else insert result newError
*)
   | t1::q1, t2::q2 ->
      let (result, error) = Eft.mult t1 t2
      let fullError = [t1]*.q2 +. [t2]*.q1 +. q1*.q2 +. [error]
      //insert result fullError 
      //|> EFTreduce
      //|> resetPrecision

      let l = insert result fullError
      if errorsNeedsPrecision l then (addzero l1) *. (addzero l2) else EFTreduce l

/// divides two Sdouble
let rec (/.) (l1:Errors) (l2:Errors) =
   match l1, l2 with
   | [], _ -> []
   | _, [] -> failwith "Sdouble div : division by 0"
   | [t1], [t2] -> [t1 / t2]
      //let (result, error) = EFTdiv t1 t2
      //if needsPrecision result error then [result; error] else [result]
(*
   // cancelation correction
   | [t1; e1], t2 :: q2 ->
      let (result, error) = EFTdiv t1 t2
      let e2 = approximateTotalError q2
      let newError = (([error] +. [e1]) -. [result]*.q2) /. ([t2] +. q2)
      let newe = approximateTotalError newError
      if isCancelation2 (t1,e1) (t2,e2) (result,newe) then 
         printfn "cancelation spotted /"
         (l1 @ [0.]) /. (l2 @ [0.]) // computation is done again with increased precision
      else insert result newError
   | t1 :: q1, [t2 ; e2] ->
      let (result, error) = EFTdiv t1 t2
      let e1 = approximateTotalError q1
      let newError = (([error] +. q1) -. [result]*.[e2]) /. ([t2] +. [e2])
      let newe = approximateTotalError newError
      if isCancelation2 (t1,e1) (t2,e2) (result,newe) then 
         printfn "cancelation spotted /"
         (l1 @ [0.]) /. (l2 @ [0.]) // computation is done again with increased precision
      else insert result newError
*)
   | t1::q1, t2::q2 ->
      let (result, error) = Eft.div t1 t2
      let fullError = (([error] +. q1) -. [result]*.q2) /. ([t2] +. q2)
      //insert result fullError 
      //|> EFTreduce
      //|> resetPrecision

      let l = insert result fullError
      if errorsNeedsPrecision l then (addzero l1) /. (addzero l2) else EFTreduce l

let abs l =
   match l with 
   | t :: _ when t < 0. -> -. l
   | _ -> l

let rec sqrterror l =
   match l with
   | [] -> l
   | [x] -> [sqrt x]
   | t :: q ->
      let result = sqrt t
      let remainder = - Eft.emulatedFMA result result (-t)
      let error = if result = 0. then sqrterror (abs q) else ([remainder] +. q) /. ([result] +. [result])
      let ls = insert result error
      if errorsNeedsPrecision ls then sqrterror (addzero l) else EFTreduce ls

let inline unaryMinus x = -x

//------

/// builds an Sdouble while checking the precision
let makeSdouble number errors =
   match errors with 
   | [e] when needsPrecision number e -> { number = number; errors = [e;0.] }
   | _ -> { number = number; errors = errors }

/// unary minus for an Sdouble
let (~-) (l:Sdouble) = 
   { number = -l.number; errors = -. l.errors }

/// sum two Sdouble
let (+) (l1:Sdouble) (l2:Sdouble) =
   let (result, error) = Eft.sum l1.number l2.number
   let errors = l1.errors +. l2.errors +. [error]
   makeSdouble result errors

/// substract two Sdouble
let (-) (l1:Sdouble) (l2:Sdouble) = 
   l1 + (- l2)

/// multiply two Sdouble
let ( * ) (l1:Sdouble) (l2:Sdouble) =
   let (result, error) = Eft.mult l1.number l2.number
   let errors = [l1.number]*.l2.errors +. [l2.number]*.l1.errors +. l1.errors*.l2.errors +. [error]
   makeSdouble result errors

/// divides two Sdouble
let (/) (l1:Sdouble) (l2:Sdouble) =
   let (result, error) = Eft.div l1.number l2.number
   let errors = (([error] +. l1.errors) -. [result]*.l2.errors) /. ([l2.number] +. l2.errors)
   makeSdouble result errors

let sqrt (l:Sdouble) =
   let result = sqrt l.number
   let remainder = Eft.emulatedFMA (unaryMinus result) result l.number
   let errors = if result = 0. then sqrterror (abs l.errors) else ([remainder] +. l.errors) /. ([result] +. [result])
   makeSdouble result errors

//-----------------------------------------------------------------------------
// TEST

/// a test that should be simple
let testPol () =
   printfn "Rump equation."
   printfn "Can we differentiate a bad result from a good one ?"
   let polynom x y = Sdouble(9.0)*x*x*x*x - y*y*y*y + Sdouble(2.0)*y*y
   // stable
   let x2 = Sdouble(1.0) / Sdouble(3.0)
   let y2 = Sdouble(2.0) / Sdouble(3.0)
   let result2 = polynom x2 y2
   printfn "P(1/3,2/3) = %A ≈ %f (exact value : 0.8024691358024691)" result2 (approximateDouble result2)
   // unstable
   let x1 = Sdouble(10864.0)
   let y1 = Sdouble(18817.0)
   let result1 = polynom x1 y1
   printfn "P(10864,18817) = %A ≈ %f (exact value : 1)" result1 (approximateDouble result1)

/// a test with an unstable division that makes us unable to track the true precision
let testMuller () =
   printfn "Muller sequence."
   printfn "The unstable division make us unable to track the precision past them."
   let mutable a = Sdouble(5.5)
   let mutable b = Sdouble(61.0) / Sdouble(11.0)
   for i = 1 to 50 do
      let c = b
      b <- Sdouble(111.0) - Sdouble(1130.0)/b + Sdouble(3000.0)/(a*b)
      a <- c
      printfn "U(%d) = %A ≈ %f" i b (approximateDouble b)
   printfn "The true limit is 6."

let kahanTest () =
   /// a fucntion that should be equivalent to the identity
   let identity xInit =
      let mutable x = Sdouble(xInit)
      // sqrt(x) 128 times
      for i = 1 to 128 do
         x <- sqrt x
         //printfn "%A" x
      // x squared 128 times
      for i = 1 to 128 do
         x <- x*x
         //printfn "%A" x
      printfn "Identity test x=%f -> %f ≈ %A" xInit (approximateDouble x) x
   // tests
   //identity 0.
   identity 0.5
   identity 0.75
   //identity 1.0
   identity 2.0
   identity 0.9

//-----

//testPol ()
//testMuller ()
kahanTest ()

(*
   on peut :
   - ordonner les termes d'erreur du plus grand au plus petit (au fond c'est juste une somme)
   - faire les calculs intermédiaire à précision absolue (ajoutant un terme d'erreur par opération) 
     puis réduire la liste d'erreur selon un critére
   - augmenter la précision quand la liste d'erreur répond a un critére
   - avoir des termes d'erreur tampon, notre critére est fait sur la liste d'erreurs privé du tampon
   - faire un reduce avec une eft pour mettre la meilleur approximation possible de l'erreur en première position
   - dès qu'un chiffre non significatif est produit on ajoute un zéro en tenant en compte sa non significativitée

   cancelations are a symptom of a bad condition number, 
   whenever we spot a cancelation in the last two error terms we might increase the precision (and redo the computations ?)
   -> two cancelations are quickly discovered but it is not enough to solve this particular problem

   signes qu'il y a un problèmes :
   - cancelation dans l'avant dernier niveau d'erreur
   - avant dernier niveau d'erreur non significatif
   - erreurs non décroissantes
   dans le cas de cette suite dès qu'une approximation a eu lieu on arrive trop tard

   la seule chose qui ai un effet (temporaire) est l'augmentation de la précision de base du type
   tout le reste laisse la situation inchangée : 
   une petite approximation se glisse quelque part après quoi il est correcte que la somme converge vers 100
   plus tard cette approximation à lieu et plus résistant est le caclul
*)
