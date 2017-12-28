#!/usr/local/bin/perl

###############################
#
# Usage: start file with:
#
# use MSPDI;
# use Exact;
#
# Then either:
#
# sub xdot { my ($x,$y) = @_; return(...); } 
# sub ydot { my ($x,$y) = @_; return(...); } 
#
# and any one of:
#   rectanglesCentre, halfrectanglesCentre, trianglesCentre,
#   rectanglesCylinderCentre, halfrectanglesCylinderCentre
#
# or define:
#
# sub ydot { my (($x1,$y1),($x2,$y2), ($x3,$y3), ($x4,$y4)) = @_; return(...); } 
# sub xdot { my (($x1,$y1),($x2,$y2), ($x3,$y3), ($x4,$y4)) = @_; return(...); } 
# (leave out ($x4,$y4) for triangles)
#
# and any one of:
#   rectanglesCorners, halfrectanglesCorners, trianglesCorners,
#   rectanglesCylinderCorners, halfrectanglesCylinderCorners.
#
# All these constructor functions take parameters of the form:
#    ((x_left, y_bottom), (x_right, y_top), (count_x, count_y))
# The first four parameters refer to the coordinates of the grid
# extremes, while the last two give the number of shapes along
# and down the grid.
#
##########################

use MSPDI;
use Rectangles;
use HalfRectangles;
use Triangles;

1;
