:- module(hardware, [sections/1, adjacent/2]).

sections([sA, sB, sC, sD, sE]).
adjacent(sA,sC).
adjacent(sB,sC).
adjacent(sC,sD).
adjacent(sC,sE).  