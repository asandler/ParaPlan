# spdi-solver

Tool for solving reachability tasks on SPDI (polygonal differential inclusion systems)

Based on the papers and previous work of Gordon J. Pace, Gerardo Schneider and others: http://www.cs.um.edu.mt/~svrg/Tools/SPeeDI/index.html

How to make it: install SFML for drawer and just type "make" in each directory.

## solver

Solves a reachability task on SPDI. Given a set *S* of start edge intervals and a set *F* of final edge intervals, tests whether there exist a trajectory starting in *S* and ending in *F*.

parameters: .spdi file, .task file

## generator

Generates random SPDI by convex polygonal partitioning of a plane based on Voronoi diagrams. Number of center points in diagram is a parameter. After generation, some regions are pruned if some of their vertices are out of bounds (between 0 and 1000 along each coordinate).

Each edge in SPDI could be an output edge for at most one region (otherwise, there will be a Zeno behavior). Hence, to define a differential inclusion on a given partition, one can assign a set of (sequential) output edges for each region and test that the oriented angle between the pre-leftmost and the post-rightmost edges is positive. If all output edges are correctly set, one can assign two vectors to each region by taking it randomly between the pre-leftmost and post-rightmost edge vectors.

Vectors are assigned to each region as following:

```
Randomly iterate by all regions
with region R:
   find all starting edges E (free edge with non-free previous neighbour)
   
   if E is empty
       if no free edges in R
           return FAILURE
       else // all edges are free, no edge with non-free neighbour
           E = {random edge from R}

   with each starting edge E:
       try to take sequence of free edges starting from E, so that it could define an output set for R
   if no output set is obtained
       return FAILURE
   else
       assign two random vectors between pre-leftmost and post-righmost edge to R
```

If all regions are correctly assigned, an SPDI representation is printed on output, otherwise, nothing is printed.

parameters: number of initial region centers in Voronoi diagram (output SPDI could contain less number of regions, but not more)

## task_generator

Takes one random interval of one random edge as a starting set and one random interval of one random edge as a final set.

parameters: .spdi file

## draw

Draws given SPDI file on a plane, with coordinates normalization on SCREEN_HEIGHT and SCREEN_WIDTH constants (hardcoded).

paramters: .spdi file
