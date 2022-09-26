# mixture-differential-for-AES
This is codes repository for paper "A New Mixture Differential Cryptanalysis on Round-reduced AES" under submission.


[MILP]: /MILP
[Verification]: /Verification
[Gurobi]: https://www.gurobi.com/
[small scale AES]: https://www.iacr.org/archive/fse2005/35570143/35570143.pdf

This repository contains two folders: [MILP] and [Verification].
[MILP] folder contains source codes to search for the mixture differential distinguishers covering 5- and 6-round of AES. The codes are launched on Linux OS. To run our codes preinstallation is [Gurobi] for solving constraint programming problems. To get the 15 mixture trails for 5-round AES, run

`make 3round_optimal_enum`

The output is

```
Set objBound to 8
NumVar 720
NumConstr 3395
15 Feasible solutions.
0-th solution: vobj = 8
-xc---xcc--xxc----------cc-c---xxxxxccccccccccccs****s****s****s
1-th solution: vobj = 8
-xc---xcc--xxc----------ccccxxxx****************s****s****s****s
2-th solution: vobj = 8
-xc---xcc--xxc----------c--c--xxxxxxxxxxccccccccs****s****s****s
3-th solution: vobj = 8
-xc---xcc--xxc----------c-cc--x-ccccxxxxccccccccs****s****s****s
4-th solution: vobj = 8
-xc---xcc--xxc----------c-c-x-x-ccccxxxxccccxxxxs****s****s****s
5-th solution: vobj = 8
-xc---xcc--xxc------------cc-xx-ccccxxxxxxxxccccs****s****s****s
6-th solution: vobj = 8
-xc---xcc--xxc------------c-xxx-ccccxxxxxxxxxxxxs****s****s****s
7-th solution: vobj = 8
-xc---xcc--xxc-----------ccc-x--ccccccccxxxxccccs****s****s****s
8-th solution: vobj = 8
-xc---xcc--xxc-----------c-c-x-xxxxxccccxxxxccccs****s****s****s
9-th solution: vobj = 8
-xc---xcc--xxc-----------cc-xx--ccccccccxxxxxxxxs****s****s****s
10-th solution: vobj = 8
-xc---xcc--xxc-------------c-xxxxxxxxxxxxxxxccccs****s****s****s
11-th solution: vobj = 8
-xc---xcc--xxc----------ccc-x---ccccccccccccxxxxs****s****s****s
12-th solution: vobj = 8
-xc---xcc--xxc-----------c--xx-xxxxxccccxxxxxxxxs****s****s****s
13-th solution: vobj = 8
-xc---xcc--xxc----------c---x-xxxxxxxxxxccccxxxxs****s****s****s
14-th solution: vobj = 8
-xc---xcc--xxc----------cc--x--xxxxxccccccccxxxxs****s****s****s
```


To get the 56 mixture trails for 6-round AES, run

`make 4round_optimal_enum`

The output is
```
Set objBound to 22
NumVar 928
NumConstr 4447
56 Feasible solutions.
0-th solution: vobj = 22
-xc---xcc--xxc------------c-xxx-ccccxxxxxxxxxxxxs*ssss*ssss**sssssssssssssss*sss
1-th solution: vobj = 22
-xc---xcc--xxc----------c-c-x-x-ccccxxxxccccxxxxs*ssss*ssss**sssssssssssssss*sss
2-th solution: vobj = 22
-xc---xcc--xxc-----------c-c-x-xxxxxccccxxxxccccsss**ssss*ssss*sssssss*sssssssss
3-th solution: vobj = 22
-xc---xcc--xxc-------------c-xxxxxxxxxxxxxxxccccs*ssss*ssss**sssssssssssssss*sss
4-th solution: vobj = 22
-xc---xcc--xxc----------c---x-xxxxxxxxxxccccxxxxs*ssss*ssss**sssssssssssssss*sss
5-th solution: vobj = 22
-xc---xcc--xxc------------cc-xx-ccccxxxxxxxxccccs*ssss*ssss**sssssssssssssss*sss
6-th solution: vobj = 22
-xc---xcc--xxc-----------c-c-x-xxxxxccccxxxxccccs*ssss*ssss**sssssssssssssss*sss
7-th solution: vobj = 22
-xc---xcc--xxc-----------c--xx-xxxxxccccxxxxxxxxs*ssss*ssss**sssssssssssssss*sss
8-th solution: vobj = 22
-xc---xcc--xxc----------cc-c---xxxxxccccccccccccs*ssss*ssss**sssssssssssssss*sss
9-th solution: vobj = 22
-xc---xcc--xxc----------cc--x--xxxxxccccccccxxxxs*ssss*ssss**sssssssssssssss*sss
10-th solution: vobj = 22
-xc---xcc--xxc----------c-cc--x-ccccxxxxccccccccs*ssss*ssss**sssssssssssssss*sss
11-th solution: vobj = 22
-xc---xcc--xxc-----------ccc-x--ccccccccxxxxccccs*ssss*ssss**sssssssssssssss*sss
12-th solution: vobj = 22
-xc---xcc--xxc-----------cc-xx--ccccccccxxxxxxxxs*ssss*ssss**sssssssssssssss*sss
13-th solution: vobj = 22
-xc---xcc--xxc----------ccc-x---ccccccccccccxxxxs*ssss*ssss**sssssssssssssss*sss
14-th solution: vobj = 22
-xc---xcc--xxc----------c-cc--x-ccccxxxxcccccccc*ssss*ssss*ssss*s*ssssssssssssss
15-th solution: vobj = 22
-xc---xcc--xxc-------------c-xxxxxxxxxxxxxxxcccc*ssss*ssss*ssss*s*ssssssssssssss
16-th solution: vobj = 22
-xc---xcc--xxc----------cc-c---xxxxxcccccccccccc*ssss*ssss*ssss*s*ssssssssssssss
17-th solution: vobj = 22
-xc---xcc--xxc------------cc-xx-ccccxxxxxxxxcccc*ssss*ssss*ssss*s*ssssssssssssss
18-th solution: vobj = 22
-xc---xcc--xxc-----------c-c-x-xxxxxccccxxxxcccc*ssss*ssss*ssss*s*ssssssssssssss
19-th solution: vobj = 22
-xc---xcc--xxc-----------ccc-x--ccccccccxxxxcccc*ssss*ssss*ssss*s*ssssssssssssss
20-th solution: vobj = 22
-xc---xcc--xxc----------c--c--xxxxxxxxxxccccccccs*ssss*ssss**sssssssssssssss*sss
21-th solution: vobj = 22
-xc---xcc--xxc----------c--c--xxxxxxxxxxccccccccsss**ssss*ssss*sssssss*sssssssss
22-th solution: vobj = 22
-xc---xcc--xxc----------c---x-xxxxxxxxxxccccxxxxsss**ssss*ssss*sssssss*sssssssss
23-th solution: vobj = 22
-xc---xcc--xxc----------cc-c---xxxxxccccccccccccsss**ssss*ssss*sssssss*sssssssss
24-th solution: vobj = 22
-xc---xcc--xxc----------ccc-x---ccccccccccccxxxxsss**ssss*ssss*sssssss*sssssssss
25-th solution: vobj = 22
-xc---xcc--xxc-------------c-xxxxxxxxxxxxxxxccccsss**ssss*ssss*sssssss*sssssssss
26-th solution: vobj = 22
-xc---xcc--xxc----------c-cc--x-ccccxxxxccccccccsss**ssss*ssss*sssssss*sssssssss
27-th solution: vobj = 22
-xc---xcc--xxc-----------ccc-x--ccccccccxxxxccccsss**ssss*ssss*sssssss*sssssssss
28-th solution: vobj = 22
-xc---xcc--xxc----------c--c--xxxxxxxxxxcccccccc*ssss*ssss*ssss*s*ssssssssssssss
29-th solution: vobj = 22
-xc---xcc--xxc----------c--c--xxxxxxxxxxccccccccss*ssss**ssss*sssssssssssss*ssss
30-th solution: vobj = 22
-xc---xcc--xxc----------c-c-x-x-ccccxxxxccccxxxxsss**ssss*ssss*sssssss*sssssssss
31-th solution: vobj = 22
-xc---xcc--xxc------------cc-xx-ccccxxxxxxxxccccsss**ssss*ssss*sssssss*sssssssss
32-th solution: vobj = 22
-xc---xcc--xxc-----------c--xx-xxxxxccccxxxxxxxxsss**ssss*ssss*sssssss*sssssssss
33-th solution: vobj = 22
-xc---xcc--xxc----------c---x-xxxxxxxxxxccccxxxx*ssss*ssss*ssss*s*ssssssssssssss
34-th solution: vobj = 22
-xc---xcc--xxc----------c-c-x-x-ccccxxxxccccxxxx*ssss*ssss*ssss*s*ssssssssssssss
35-th solution: vobj = 22
-xc---xcc--xxc----------cc--x--xxxxxccccccccxxxx*ssss*ssss*ssss*s*ssssssssssssss
36-th solution: vobj = 22
-xc---xcc--xxc------------c-xxx-ccccxxxxxxxxxxxxsss**ssss*ssss*sssssss*sssssssss
37-th solution: vobj = 22
-xc---xcc--xxc----------cc--x--xxxxxccccccccxxxxsss**ssss*ssss*sssssss*sssssssss
38-th solution: vobj = 22
-xc---xcc--xxc-----------cc-xx--ccccccccxxxxxxxxsss**ssss*ssss*sssssss*sssssssss
39-th solution: vobj = 22
-xc---xcc--xxc------------c-xxx-ccccxxxxxxxxxxxx*ssss*ssss*ssss*s*ssssssssssssss
40-th solution: vobj = 22
-xc---xcc--xxc-----------c--xx-xxxxxccccxxxxxxxx*ssss*ssss*ssss*s*ssssssssssssss
41-th solution: vobj = 22
-xc---xcc--xxc----------ccc-x---ccccccccccccxxxx*ssss*ssss*ssss*s*ssssssssssssss
42-th solution: vobj = 22
-xc---xcc--xxc-----------cc-xx--ccccccccxxxxxxxx*ssss*ssss*ssss*s*ssssssssssssss
43-th solution: vobj = 22
-xc---xcc--xxc----------cc-c---xxxxxccccccccccccss*ssss**ssss*sssssssssssss*ssss
44-th solution: vobj = 22
-xc---xcc--xxc----------cc--x--xxxxxccccccccxxxxss*ssss**ssss*sssssssssssss*ssss
45-th solution: vobj = 22
-xc---xcc--xxc----------c-cc--x-ccccxxxxccccccccss*ssss**ssss*sssssssssssss*ssss
46-th solution: vobj = 22
-xc---xcc--xxc----------c-c-x-x-ccccxxxxccccxxxxss*ssss**ssss*sssssssssssss*ssss
47-th solution: vobj = 22
-xc---xcc--xxc----------ccc-x---ccccccccccccxxxxss*ssss**ssss*sssssssssssss*ssss
48-th solution: vobj = 22
-xc---xcc--xxc-------------c-xxxxxxxxxxxxxxxccccss*ssss**ssss*sssssssssssss*ssss
49-th solution: vobj = 22
-xc---xcc--xxc------------cc-xx-ccccxxxxxxxxccccss*ssss**ssss*sssssssssssss*ssss
50-th solution: vobj = 22
-xc---xcc--xxc----------c---x-xxxxxxxxxxccccxxxxss*ssss**ssss*sssssssssssss*ssss
51-th solution: vobj = 22
-xc---xcc--xxc-----------ccc-x--ccccccccxxxxccccss*ssss**ssss*sssssssssssss*ssss
52-th solution: vobj = 22
-xc---xcc--xxc-----------cc-xx--ccccccccxxxxxxxxss*ssss**ssss*sssssssssssss*ssss
53-th solution: vobj = 22
-xc---xcc--xxc-----------c-c-x-xxxxxccccxxxxccccss*ssss**ssss*sssssssssssss*ssss
54-th solution: vobj = 22
-xc---xcc--xxc-----------c--xx-xxxxxccccxxxxxxxxss*ssss**ssss*sssssssssssss*ssss
55-th solution: vobj = 22
-xc---xcc--xxc------------c-xxx-ccccxxxxxxxxxxxxss*ssss**ssss*sssssssssssss*ssss
```

[Verification] folder contains source codes for verifying 5-round AES mixture differential distinguishers experimentally on [small scale AES]. We implemented small scale AES in lookup-table-based implementation. To test the implementation on test vectors, run

```
g++ test_smallScaleAES.cpp fast_smallAES.cpp -o test_smallScaleAES`
./test_smallScaleAES
```
to show the 10-round encryption results of small scale AES.

[MPI]: https://www.mpich.org/
To run distinguisher verification code, preinstallation is [MPI]. The PRG used is mt19937 engine. To test on $2^{24}\times 64$ quadruples by running 64 processes in parallel (each process tests $2^{24}$ quadruples) for the same randomly generated master key with random seed configured by eg. 5, run

```
make
mpirun -np 64 ./Small_5round 5
```
The output is the number of right quadruples for 5-round small scale AES and 20-round small scale AES.

To verify the mixture distinguisher on 200 randomly generated master keys, run

`
./run.sh
`

Then calculate the average probability of the distinguisher.
