module CaseIn1 where

--A definition can be demoted to the local 'where' binding of a friend declaration,
--if it is only used by this friend declaration.

--Demoting a definition narrows down the scope of the definition.
--In this example, demote the top level 'addthree' to 'main'
--This example also aims to test the removing of parameters
--The 'addthree' was originally lifted from the first case alternative. the example
--show that lifting and demoting are not inverse.

main x y z = case x of
                0 -> addthree x y z  
                1 -> inc y        
                     where inc a =a+1
  where
    addthree a b c=a+b+c

  