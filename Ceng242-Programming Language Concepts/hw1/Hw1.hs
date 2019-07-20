module Hw1 where

data Cell = H | P | O | X deriving (Read,Show,Eq)
data Result = Fail | Caught (Int,Int) deriving (Read,Show,Eq)
data Direction = N | S | E | W deriving (Read,Show,Eq)

simulate :: [[Cell]] -> [(Direction, Direction)] -> Result
-- DO NOT CHANGE ABOVE THIS LINE, WRITE YOUR CODE BELOW --

findPositionY :: [[Cell]] -> Cell -> Int
findPositionY (x:xs) y
   | elem y x  = 0
   | otherwise = (findPositionY xs y) + 1

findPositionX :: [Cell] -> Cell -> Int
findPositionX (x:xs) y
   | (y==x)    = 0
   | otherwise = (findPositionX xs y) + 1

add :: (Int,Int) -> (Int,Int) -> (Int,Int)
add (a,b) (c,d) = (a+c,b+d)

isIt :: [[Cell]] -> (Int,Int) -> Bool
isIt [] _                     = False
isIt [[]] _                   = False
isIt xs (a,b)
   | b<0 || a<0                 = False
   | length xs <= b             = False
   | length (xs !! 0) <= a      = False
   | ((xs !! b) !! a) == X      = False
   | otherwise                  = True

direc :: Direction -> (Int,Int)
direc a 
   | (a == W) = (-1,0)
   | (a == E) = (1,0)
   | (a == N) = (0,-1)
   | (a == S) = (0,1)

updateXS :: [[Cell]] -> Cell -> (Int, Int) -> [[Cell]]
updateXS xs x (c,r) =
  take r xs ++ [take c (xs !! r) ++ [x] ++ drop (c + 1) (xs !! r)] ++ drop (r + 1) xs

simulate xs [] = Fail
simulate xs (y:ys)
   | (p' == h') = Caught p'
   | otherwise = simulate zs ys
   where p    = ( findPositionX (xs !! (findPositionY xs P)) P , findPositionY xs P)
         h    = ( findPositionX (xs !! (findPositionY xs H)) H , findPositionY xs H)
         dirP = direc (snd y)
         dirH = direc (fst y)
         h'
            | isIt xs (add h dirH) = add h dirH
            | otherwise            = h
         p'
            | isIt xs (add p dirP) = add p dirP
            |otherwise             = p
         zs = z4
         z4
            | p' == p   = z3
            |otherwise  = updateXS z3 P p'
         z3 
            | h' == h   = z2
            |otherwise  = updateXS z2 H h'
         z2
            | p' == p   = z1
            |otherwise  = updateXS z1 O p
         z1 
            | h' == h   = xs
            |otherwise  = updateXS xs O h