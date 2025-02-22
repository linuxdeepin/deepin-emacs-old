module HsAssocStruct where

import Data.Generics

-- Formerly known as InfixAssoc...
data HsFixity = HsFixity HsAssoc Int deriving (Eq,Show,Read, Data, Typeable)

data HsAssoc
    = HsAssocNone
    | HsAssocLeft
    | HsAssocRight
      deriving (Eq, Show, Read, Data, Typeable)
