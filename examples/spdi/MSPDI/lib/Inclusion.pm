#!/usr/local/bin/perl

###############################
#
# Usage: start file with:
#
# use MSPDI;
# use Inclusion;
#
# Then either:
#
# sub differentialInclusion { 
#    my ($x,$y) = @_; 
#    return((left_vector_x,  left_vector_y )
#          ,(right_vector_x, right_vector_y)
#          ); 
# } 
#
# and any one of:
#   rectanglesCentre, halfrectanglesCentre, trianglesCentre,
#   rectanglesCylinderCentre, halfrectanglesCylinderCentre
#
# or define:
#
# sub differentialInclusion { 
#    my (($x1,$y1),($x2,$y2), ($x3,$y3), ($x4,$y4)); for rectangles
# or my (($x1,$y1),($x2,$y2), ($x3,$y3)); for triangles
#    return((left_vector_x,  left_vector_y )
#          ,(right_vector_x, right_vector_y)
#          ); 
# } 
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
use InclusionRectangles;
use InclusionHalfRectangles;
use InclusionTriangles;

1;


