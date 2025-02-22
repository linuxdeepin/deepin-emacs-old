module ConstructorIn3 where
data BTree b
    = Empty | T b (BTree b) (BTree b) deriving Show
 
buildtree :: Ord a => [a] -> BTree a
 
buildtree [] = Empty
buildtree ((x : xs)) = insert x (buildtree xs)
 
insert :: Ord a => a -> (BTree a) -> BTree a
 
insert val Empty = T val Empty Empty
insert val tree@(T tval left right)
    | val > tval = T tval left (insert val right)
    | otherwise = T tval (insert val left) right
 
main :: BTree Int
 
main = buildtree [3, 1, 2]
 