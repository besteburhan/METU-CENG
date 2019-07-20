:- module(hw5,[configuration/3]).
:- [hardware].



isOuter(Component,Constraints,IsOuter) :- member(outer_edge(Component),Constraints),!, IsOuter is 1.
isOuter(Component,Constraints,IsOuter) :- IsOuter is 0.

isCloseTo(Component,Constraints,IsClose,[A]) :- member(close_to(A,Component),Constraints); member(close_to(Component,A),Constraints),!, IsClose is 1 .
isCloseTo(Component,Constraints,IsClose,[]) :-  IsClose is 0 .


helper(Comp,0,0,[],SectList,PlacementList,A):-
	member(Sect,SectList),
	A = put(Comp,Sect).


helper(Comp,1,0,[],SectList,PlacementList,A):-
	member(Sect,SectList),
	sections(List),
	findAdj(Sect,List,N),
	N<4,
	A = put(Comp,Sect).
	

helper(Comp,0,1,[CloseMember],SectList,PlacementList,A):-
	member(put(CloseMember,Sect), PlacementList), !,
	(adjacent(B,Sect); adjacent(Sect,B)),
	member(B, SectList),
	A=put(Comp, B).
helper(Comp,0,1,[CloseMember],SectList,_,A) :-
	member(Sect, SectList),
	A = put(Comp, Sect).


confhelp([],_,_,[]).
confhelp([C|ComponentList],Constraints,SectionList,[A|PlacementList]) :-
	isCloseTo(C,Constraints,IsClose,CloseMember),
	isOuter(C,Constraints,IsOuter),
	helper(C,IsOuter,IsClose,CloseMember,SectionList,PlacementList,A),
	put(_,Sect) = A,
	delete(SectionList,Sect,NewList),
	confhelp(ComponentList,Constraints,NewList,PlacementList).

configuration(ComponentList,Constraints,PlacementList) :- sections(SectionList),confhelp(ComponentList,Constraints,SectionList,PlacementList).



findAdj(_,[],0).
findAdj(Sect,[A|SectList] , N2) :- (adjacent(Sect, A);adjacent(A, Sect)), !, findAdj(Sect,SectList,N), N2 is N + 1.
findAdj(Sect,[A|SectList], N) :- findAdj(Sect,SectList, N) .
