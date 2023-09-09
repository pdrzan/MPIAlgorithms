# Odd Even Algorithm

This algorithm uses Introsort to sort the local arrays in each processor before Odd Even effectively begin.

To compile:

`mpic++ oddEven.c -o <name_executable>`

To run:

`mpirun -np <number_processors> <name_executable>`

Pseudocode:

```

1. Procedure ODD-EVEN PAR(n)
2. Begin
3.  id := label
4.  For i := 1 to n do
5.  Begin
6.      If i is odd then
7.          If id is odd then
8.              compare_split_min(id + 1);
9.          Else
10.             compare_split_max(id − 1);
11.     If i is even then
12.         If id is even then
13.             compare_split_min(id + 1);
14.         Else
15.             compare_split_max(id − 1);
16.     End for
17. End ODD-EVEN PAR

```
   