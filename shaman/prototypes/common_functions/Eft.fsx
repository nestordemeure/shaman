
//-----------------------------------------------------------------------------
// usefull operations

/// splits a floating number into two non overlapping parts
let inline split x =
   let factor = 134217729.0 // 1<<27+1 (27 for double precision IEEE754)
   let p = factor * x
   let x1 = p - (p - x)
   let x2 = x - x1
   (x1, x2)

/// extract the high 26 bits of significand (half split)
let inline upperHalf x =
   let secator = 134217729.0 // 1<<27+1 (27 for double precision IEEE754)
   let p = x * secator // simplified... normally we should check if overflow and scale down
   p + (x - p)

/// Emulates a Fused-Multiply-Add: roundToNearestFloat(a*b+c)
/// Useful in a language that has no true acces to a genuine FMA
///
/// WARNING : use only when -c is an approximation of a*b
/// otherwise there is NO guaranty of correct rounding
/// see : https://stackoverflow.com/questions/37256728/minimize-rounding-error-when-dividing-by-integers
let inline emulatedFMA a b c =
   let aup = upperHalf a
   let alo = a-aup
   let bup = upperHalf b
   let blo = b-bup
   // compute exact product of a and b which is the exact sum of ab and a residual error resab
   let high = aup*bup
   let mid = aup*blo + alo*bup
   let low = alo*blo
   let ab = high + mid
   let resab = (high - ab) + mid + low
   // expected to be exact, so don't bother with residual error
   let fma = ab + c
   resab + fma

//-----------------------------------------------------------------------------
// Error free transform

/// returns the result of the sum and its error
let inline sum x y =
   let result = x + y
   let y2 = result - x
   let error = (x - (result - y2)) + (y - y2)
   (result, error)

/// returns the result of the multiplication and its error
let inline mult x y =
   let result = x * y
   let (x1,x2) = split x
   let (y1,y2) = split y
   let error = x2*y2 - (((result - x1*y1) - x2*y1) - x1*y2)
   (result, error)

/// returns the remainder of the division
let inline div x y =
   let result = x / y
   let remainder = -(emulatedFMA y result (-x)) // TODO compute exactly
   (result, remainder)

/// retrusn teh remainder of the squareRoot
let inline sqrt x =
   let result = sqrt x
   let remainder = - emulatedFMA result result (-x)
   (result, remainder)