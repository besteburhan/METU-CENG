module Hw2 where

import Data.List -- YOU MAY USE THIS MODULE FOR SORTING THE AGENTS

data Level = Newbie | Intermediate | Expert deriving (Enum, Eq, Ord, Show, Read)
data Hunter = Hunter {hID::Int, hlevel::Level, hEnergy::Int, hNumberOfCatches::Int, hActions::[Direction]} deriving (Eq, Show, Read)
data Prey = Prey { pID::Int, pEnergy::Int, pActions::[Direction]} deriving (Eq, Show, Read)
data Cell = O | X | H Hunter | P Prey | T deriving (Eq, Show, Read)
data Direction = N | S | E | W deriving (Eq, Show, Read)
type Coordinate = (Int, Int)
-- DO NOT CHANGE THE DEFINITIONS ABOVE. --
-- WRITE THE REST OF YOUR CODE HERE --

instance Ord Prey where
    compare (Prey pId1 pEn1 _) (Prey pId2 pEn2 _)  = if pEn1 == pEn2 then (compare pId2 pId1) else (compare pEn1 pEn2)

instance Ord Hunter where
    compare (Hunter hId1 hLevel1 hEn1 hNumberOfCatches1 _) (Hunter hId2 hLevel2 hEn2 hNumberOfCatches2 _) = 
                                                            if hLevel1 == hLevel2 then
                                                                if hEn1 == hEn2 then 
                                                                    if hNumberOfCatches1 == hNumberOfCatches2 then
                                                                       compare hId2 hId1 
                                                                    else compare hNumberOfCatches1 hNumberOfCatches2 
                                                                else compare hEn1 hEn2
                                                            else compare hLevel1 hLevel2

add :: (Int,Int) -> (Int,Int) -> (Int,Int)
add (a,b) (c,d) = (a+c,b+d)

convertCellToPrey:: Cell -> Prey
convertCellToPrey (P prey) = prey
convertCellToHunter:: Cell -> Hunter
convertCellToHunter (H hunter) = hunter

isHunter:: Cell -> Bool
isHunter (H _) = True
isHunter x =False
isPrey:: Cell -> Bool
isPrey (P _) = True
isPrey x = False

findP:: [[Cell]] -> Coordinate -> Coordinate -> [(Prey,Coordinate)] 
findP [[]]  _  _      = []
findP ((x:xs):ys) (lengthX,lengthY) (i,j) 
     | isPrey x        = if ((i+1)== lengthX) then
                             if j+1 == lengthY then [((convertCellToPrey x),(i,j))] 
                             else [((convertCellToPrey x),(i,j))] ++ (findP ys (lengthX,lengthY) (0,j+1))
                         else   [((convertCellToPrey x),(i,j))] ++ (findP (xs:ys) (lengthX,lengthY) (i+1,j))
     | otherwise       = if ((i+1)== lengthX) then
                           if (j+1)==lengthY then []
                           else (findP ys (lengthX,lengthY) (0,j+1))
                         else (findP (xs:ys) (lengthX,lengthY) (i+1,j))
findH:: [[Cell]] -> Coordinate -> Coordinate -> [(Hunter,Coordinate)]
findH [[]]  _  _      = []
findH ((x:xs):ys) (lengthX,lengthY) (i,j) 
     | isHunter x        = if ((i+1)== lengthX) then
                             if j+1 == lengthY then [((convertCellToHunter x),(i,j))]
                             else [((convertCellToHunter x),(i,j))] ++ (findH ys (lengthX,lengthY) (0,j+1))
                           else   [((convertCellToHunter x),(i,j))] ++ (findH (xs:ys) (lengthX,lengthY) (i+1,j))
     | otherwise       = if ((i+1)== lengthX) then
                           if j+1 ==lengthY then []
                           else (findH ys (lengthX,lengthY) (0,j+1))
                         else (findH (xs:ys) (lengthX,lengthY) (i+1,j))

whichPrey :: [(Prey,Coordinate)] -> (Prey,Coordinate) -> Int
whichPrey [] _ = 0
whichPrey (x:xs) p 
    | pID (fst x) == pID (fst p)   = 0
    | otherwise                    = 1+(whichPrey xs p)
whichHunter :: [(Hunter,Coordinate)] -> (Hunter,Coordinate) -> Int
whichHunter [] _ = 0
whichHunter (x:xs) h 
    | (hID (fst x) == hID (fst h) )  = 0
    | otherwise                      = 1+(whichHunter xs h)

updateTupleHunt :: ([(Hunter,Coordinate)] , [(Prey,Coordinate)]) -> (Hunter,Coordinate) -> ([(Hunter,Coordinate)] , [(Prey,Coordinate)])
updateTupleHunt (first,second) newHunterTuple = ((take indexHunter first )++ [newHunterTuple] ++ (drop (indexHunter+1) first) , second)
                                              where indexHunter  = whichHunter first newHunterTuple

updateTuplePrey :: ([(Hunter,Coordinate)] , [(Prey,Coordinate)]) -> (Prey,Coordinate) -> ([(Hunter,Coordinate)] , [(Prey,Coordinate)])
--updateTuplePrey (first,second) () = 
updateTuplePrey (first,second) newPreyTuple = (first , ((take indexPrey second) ++ [newPreyTuple] ++ (drop (indexPrey+1) second)) )
                                              where indexPrey  = whichPrey second newPreyTuple

moveable :: [[Cell]] -> (Int,Int) -> Int -> Bool
moveable [] _ _                    = False
moveable [[]] _ _                  = False
moveable xs (a,b) energy
   | energy < 10                = False
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

isTrap:: [[Cell]] -> Coordinate -> Bool
isTrap xs (x,y)
   | ((xs !! y) !! x) == T  = True
   | otherwise              = False

eating :: ([(Hunter,Coordinate)] , [(Prey,Coordinate)]) -> (Int,Int) ->(Int,Int) -> Int -> ([(Hunter,Coordinate)] , [(Prey,Coordinate)])
eating (hunterList,preyList) (numberOfHunter,numberOfPrey) (i,j) numberCatching
   | i == numberOfHunter                                 = (reverse(sort hunterList),(sort preyList))
   | ((numberCatching == 2 )  || (j >= numberOfPrey))    = eating ( hunterList, preyList) (numberOfHunter,numberOfPrey) (i+1,0) 0
   | ((i< numberOfHunter) && (j < numberOfPrey))         = if ((snd (hunterList !! i)) == (snd (preyList !! j))) 
                                                             then let hunter        = fst (hunterList !! i)
                                                                      coordHunter   = snd (hunterList !! i)
                                                                      newHunter     = Hunter (hID hunter) (hlevel hunter) (if ((hEnergy hunter) >= 80)  then 100 else  ((hEnergy hunter)+20)) ((hNumberOfCatches hunter)+1) (hActions hunter)
                                                                      prey          = fst (preyList !! j)
                                                                      tupleHuntPrey = (updateTupleHunt (hunterList,preyList) (newHunter,coordHunter))
                                                                      newHunterList = (sort (fst tupleHuntPrey))
                                                                      newPreyList   = ((take j preyList) ++ (drop (j+1) preyList))
                                                                  in eating ( newHunterList, newPreyList) (numberOfHunter,(numberOfPrey-1)) (i,j+1) (numberCatching+1)
                                                          else eating ( hunterList, preyList) (numberOfHunter,numberOfPrey) (i,j+1) numberCatching

simulateRecursion :: [[Cell]] -> ([(Hunter,Coordinate)] , [(Prey,Coordinate)]) -> (Int,Int) -> (Int,Int) -> ([(Hunter,Coordinate)] , [(Prey,Coordinate)])
simulateRecursion xs (hunterList,preyList) (numberOfHunter,numberOfPrey) (i,j)
   |  numberOfPrey == 0                                                                       = ( reverse(sort hunterList), (sort preyList))
   | ((i==numberOfHunter) && (j==numberOfPrey))            = let newTuple = eating (reverse (sort hunterList), sort preyList) (numberOfHunter,numberOfPrey) (0,0) 0
                                                             in simulateRecursion xs newTuple (length (fst newTuple),length (snd newTuple)) (0,0)
                                   
   |  ( (length (pActions (fst (preyList !! (numberOfPrey-1))))) == 0)  && ( (length (hActions (fst (hunterList !! (numberOfHunter-1))))) == 0 )   = ( reverse(sort hunterList),(sort  preyList))
   |  ((not (i == numberOfHunter)) && (j == 0))             =
                                  let  hunter         = fst (hunterList !! i)
                                       oldCoordinate  = snd (hunterList !! i)
                                       newCoordinate  = add (direc ((hActions hunter) !! 0))  (oldCoordinate)
                                       energyH        = hEnergy hunter

                                  in  if moveable xs newCoordinate energyH 
                                        then let newHunter  = Hunter (hID hunter) (hlevel hunter) (energyH-1) (hNumberOfCatches hunter) (drop 1 (hActions hunter))
                                                 newTuple   = updateTupleHunt (hunterList,preyList) (newHunter,newCoordinate)
                                             in simulateRecursion xs newTuple (numberOfHunter,numberOfPrey) (i+1,j)
                                      else if energyH < 10 then let newHunter = Hunter (hID hunter) (hlevel hunter) ((energyH)+1) (hNumberOfCatches hunter) (drop 1 (hActions hunter))
                                                                    newTuple  = updateTupleHunt (hunterList,preyList) (newHunter,oldCoordinate)
                                                                in simulateRecursion xs newTuple (numberOfHunter,numberOfPrey) (i+1,j)
                                           else let newHunter = Hunter (hID hunter) (hlevel hunter) ((energyH)-1) (hNumberOfCatches hunter) (drop 1 (hActions hunter))
                                                    newTuple  = updateTupleHunt (hunterList,preyList) (newHunter,oldCoordinate)
                                                in simulateRecursion xs newTuple (numberOfHunter,numberOfPrey) (i+1,j)
   | ((i == numberOfHunter ) && (j < numberOfPrey))        =
                                  let prey            = fst (preyList !! j)
                                      oldCoordinate   = snd (preyList !! j)
                                      newCoordinate   = add (direc ((pActions prey) !! 0))  (oldCoordinate)
                                      energyP         = pEnergy prey
                                  in  if (moveable xs newCoordinate energyP)
                                             then if (isTrap xs newCoordinate)
                                                     then let newPrey   = Prey (pID prey) (if (energyP <= 10) then 0 else (energyP-11)) (drop 1 (pActions prey))
                                                              newTuple  = updateTuplePrey (hunterList,preyList) (newPrey,newCoordinate)
                                                          in simulateRecursion xs newTuple (numberOfHunter,numberOfPrey) (i,j+1)
                                                  else let newPrey   = Prey (pID prey) (energyP-1) (drop 1 (pActions prey))
                                                           newTuple  = updateTuplePrey (hunterList,preyList) (newPrey,newCoordinate)
                                                       in simulateRecursion xs newTuple (numberOfHunter,numberOfPrey) (i,j+1)
                                      else if (energyP < 10) 
                                             then if (isTrap xs oldCoordinate) then let newPrey   = Prey (pID prey) 0 (drop 1 (pActions prey))
                                                                                        newTuple  = updateTuplePrey (hunterList,preyList) (newPrey,oldCoordinate)
                                                                                    in simulateRecursion  xs newTuple (numberOfHunter,numberOfPrey) (i,j+1)
                                                  else  let newPrey    = Prey (pID prey) (energyP + 1) (drop 1 (pActions prey))
                                                            newTuple   = updateTuplePrey (hunterList,preyList) (newPrey,oldCoordinate)
                                                        in simulateRecursion xs  newTuple (numberOfHunter,numberOfPrey) (i,j+1)
                                           else   if ((energyP >= 10 ) && (isTrap xs oldCoordinate)) 
                                                     then let newPrey   = Prey (pID prey) (if energyP == 10 then 0 else (energyP-11)) (drop 1 (pActions prey))
                                                              newTuple  = updateTuplePrey (hunterList,preyList) (newPrey,oldCoordinate)
                                                          in simulateRecursion xs newTuple (numberOfHunter,numberOfPrey) (i,j+1)
                                                  else let newPrey   = Prey (pID prey) (energyP-1) (drop 1 (pActions prey))
                                                           newTuple  = updateTuplePrey (hunterList,preyList) (newPrey,oldCoordinate)
                                                       in simulateRecursion xs newTuple (numberOfHunter,numberOfPrey) (i,j+1)


simulate :: [[Cell]] -> ([(Hunter,Coordinate)] , [(Prey,Coordinate)])
simulate xs = simulateRecursion xs result (length (fst result),length (snd result)) (0,0)
   where result = ( (findH xs (length (xs!!0),length xs) (0,0)) , (findP xs (length (xs!!0),length xs) (0,0)) )
