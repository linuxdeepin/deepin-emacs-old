module Case2 where
data T = C2 Int
 
caseIt x
    =   case x of
            42  ->  error
                        "f (C1 1 2) no longer defined for T at line: 3"
 