module PatBindIn1 where

--A definition can be demoted to the local 'where' binding of a friend declaration,
--if it is only used by this friend declaration.

--Demoting a definition narrows down the scope of the definition.
--In this example, demote the top level 'tup' to 'foo'
--This example aims to test demoting pattern binding.

main :: Int
main = foo 3

foo :: Int -> Int
foo x = h + t + (snd tup)
  where
    tup :: (Int, Int)
    h :: Int
    t :: Int
    tup@(h,t) = head $ zip [1..10] [3..15]






