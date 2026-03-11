% Реализация задачи коммивояжёра.
% Данные (матрица расстояний) передаются из Python.
%
% Точка входа для Python:
%   answer(+Matrix, -BestPath, -ShortestDistance).
%
% Matrix: квадратная матрица расстояний (список списков), N <= 10, расстояния неотрицательные.
% Узлы (аэропорты) кодируются целыми 0..N-1.

:- module(flights, [answer/3, solve_path/3, fly/5]).

:- use_module(library(lists)).

% edge(+Matrix, ?X, ?Y, ?D)
% Делаем граф неориентированным как в исходнике:
edge(Matrix, X, Y, D) :- dist(Matrix, X, Y, D).
edge(Matrix, X, Y, D) :- dist(Matrix, Y, X, D).

% dist(+Matrix, +I, +J, -D)
dist(Matrix, I, J, D) :-
    nth0(I, Matrix, Row),
    nth0(J, Row, D).

airport(Matrix, A) :- edge(Matrix, A, _, _).

airport_count(Matrix, N) :-
    setof(A, airport(Matrix, A), AirportList),
    length(AirportList, N).

% fly(+Matrix, +Current, +Visited, -Path, -Distance)
fly(Matrix, _, Visited, Path, 0) :-
    airport_count(Matrix, N),
    length(Visited, N),
    reverse(Visited, Path).
fly(Matrix, X, Visited, Path, TotalDist) :-
    edge(Matrix, X, Y, D),
    \+ member(Y, Visited),
    fly(Matrix, Y, [Y | Visited], Path, RestDist),
    TotalDist is RestDist + D.

% solve_path(+Matrix, -Path, -Dist)
solve_path(Matrix, Path, Dist) :-
    edge(Matrix, Start, _, _),
    fly(Matrix, Start, [Start], Path, Dist).

% answer(+Matrix, -BestPath, -ShortestDistance)
answer(Matrix, BestPath, ShortestDistance) :-
    setof(Dist-Path, solve_path(Matrix, Path, Dist), [ShortestDistance-BestPath | _]).

