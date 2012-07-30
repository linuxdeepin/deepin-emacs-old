module D1 where

{-lift 'sq' to top level. This refactoring
  affects module 'D1' and 'C1' -}

sumSquares (x:xs) = (sq pow) x + sumSquares xs
  where 
     pow =2 
 
sumSquares [] = 0

sq pow x = x ^ pow


main = sumSquares [1..4]


