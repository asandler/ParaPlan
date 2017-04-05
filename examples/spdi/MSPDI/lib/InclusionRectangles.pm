#!/usr/local/bin/perl

use MSPDI;

#########################################################################
# Usage:
#
# use MSPDI;
# use InclusionRectangles;
#
# # This function returns the left and right vector respectively
# sub differentialInclusion { my ($x,$y) = @_; return((($y, -$x), ($y, -$x)); } 
#
# rectanglesCentre ((-10,-10), (10,10), (20,20));
#                     ^          ^        ^
#                   lower      upper   number of
#                   left       right   rectangles  
#                   coord      coord   in the x and
#                                      y direction
#
# or 
#
# sub differentialInclusion { my (($x1,$y1),($x2,$y2), ($x3,$y3), ($x4, $y4)) = @_; return(...); } 
#
# rectanglesCorners((-10,-10), (10,10), (20,20));
#
#########################################################################

sub rectanglesCentre  { rectanglesGeneric("1",@_); }
sub rectanglesCorners { rectanglesGeneric("0",@_); }

# A generic rectangle grid generator
sub rectanglesGeneric {
   my ($centre,$grid_left, $grid_bottom, $grid_right, $grid_top, $xsize, $ysize) = @_;
   my ($base, $height, $ycoord, $xcoord, $xcentre, $ycentre, $x, $y, $inc_ycoord, $inc_xcoord);
   my ($inc_x, $inc_y,$vector_left_x,$vector_left_y,$vector_right_x,$vector_right_y,$inc_inc_xcoord);
   my ($y_dot, $x_dot);

   startMSPDI;

   # create grid with rectangles of size width x height
   $base   = ($grid_right - $grid_left) / $xsize;
   $height = ($grid_top - $grid_bottom) / $ysize;

   # definition of vectors
   
   # Since the above may be changed by the user, make sure that they 
   # are well defined by putting a constraint that is checked at spdi
   # generation time

   constraint ("Top of grid must be higher than bottom", $grid_top > $grid_bottom);
   constraint ("Left of grid must be smaller than right limit", $grid_left < $grid_right);
   constraint ("Grid must be at least 1x1", $xsize > 0 && $ysize > 0);

   # Generate the points, vectors, regions automatically

   $ycoord=$grid_bottom;
   for ($y=0; $y <= $ysize; $y++) {
      $xcoord=$grid_left;
      for ($x=0; $x <= $xsize; $x++) {
         newPoint("P_$x\_$y", $xcoord, $ycoord); 
         $xcoord+=$base;
      }
      $ycoord+=$height;
   }

   $ycoord = $grid_bottom;
   for ($y = 0; $y < $ysize; $y++) {
      $xcoord = $grid_left;
      for ($x = 0; $x < $xsize; $x++) {
            $inc_x = $x+1; $inc_y = $y+1; $inc_ycoord=$ycoord+$height; $inc_xcoord=$xcoord+$base;

            if ($centre=="1") {
               ($xcentre,$ycentre) = ($inc_xcoord + $base/2, $inc_ycoord + $height/2);
               (($vector_left_x, $vector_left_y),($vector_right_x, $vector_right_y)) =
                  differentialInclusion($xcentre,$ycentre);
            } else {
               (($vector_left_x, $vector_left_y),($vector_right_x, $vector_right_y)) =
                  differentialInclusion($xcoord,     $ycoord,     $inc_xcoord, $ycoord, 
                                        $inc_xcoord, $inc_ycoord, $xcoord,     $inc_ycoord);
            }

            newVector("Vl_$x\_$y", $vector_left_x,  $vector_left_y);
            newVector("Vr_$x\_$y", $vector_right_x, $vector_right_y);

            newRegionComment(" rectangle whose lower left corner is at ($xcoord, $ycoord)");
            newRegion("Vl_$x\_$y", "Vr_$x\_$y",
                      "P_$x\_$y", "P_$inc_x\_$y", "P_$inc_x\_$inc_y", "P_$x\_$inc_y", "P_$x\_$y");
 
            $xcoord = $xcoord + $base;
      }
      $ycoord = $ycoord+$height;
   }

   endMSPDI;
}

1;
