Based on the works of Gordon J. Pace, Gerardo Schneider and others: http://www.cs.um.edu.mt/~svrg/Tools/SPeeDI/index.html

# spdi-solver

Tool for solving reachability tasks on SPDI (polygonal differential inclusion systems)

## solver

Solves a reachability task on SPDI. Given a set *S* of start edge intervals and a set *F* of final edge intervals, tests whether there exist a trajectory starting in *S* and ending in *F*.

## generator

Generates random convex polygonal partitioning of a plane based on Voronoi diagrams. Number of center points in diagram is a parameter. After generation, some regions are pruned if some of their vertices are out of bounds (between 0 and 1000 along each coordinate).

Each edge in SPDI could be an output edge for at most one region (otherwise, there will be a Zeno behavior). Hence, to define a differential inclusion on a given partition, one can assign a set of (sequential) output edges for each region and test that the oriented angle between the pre-leftmost and the post-rightmost edges is positive. If all output edges are correctly set, one can assign two vectors to each region by taking it randomly between the pre-leftmost and post-rightmost edge vectors.

Vectors are assigned to each region as following:

```
1) Randomly iterate by all regions
2) with region R:
3)    Find all starting edges E (free edge with non-free previous neighbour)
4)    with each starting edge E:
5)        try to take sequence of free edges starting from E, so that it could define an output set for R
6)    if no output set is obtained
7)        return FAILURE
8)    else
9)        assign two random vectors between pre-leftmost and post-righmost edge to R
```

If all regions are correctly assigned, an SPDI representation is printed on output, otherwise, nothing is printed.

## draw

Draws given SPDI file on a plane, with coordinates normalization on SCREEN_HEIGHT and SCREEN_WIDTH constants (hardcoded).
