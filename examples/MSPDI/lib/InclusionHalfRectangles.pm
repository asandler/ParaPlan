#!/usr/local/bin/perl

use MSPDI;

#########################################################################
# Usage:
#
# use MSPDI;
# use InclusionHalfRectangles;
#
# # This function returns the left and right vector respectively
# sub differentialInclusion { my ($x,$y) = @_; return(($y, -$x), ($y, -$x)); } 
#
# halfrectanglesCylinderCentre ((-10,-10), (10,10), (20,20)); or
# halfrectanglesCentre ((-10,-10), (10,10), (20,20));
#                          ^          ^        ^
#                       lower      upper   number of
#                       left       right   halfrectangles  
#                       coord      coord   in the x and
#                                          y direction
#
# or 
#
# sub differentialInclusion { my (($x1,$y1),($x2,$y2), ($x3,$y3)) = @_; return(...); } 
#
# halfrectanglesCylinderCorners((-10,-10), (10,10), (20,20));
# halfrectanglesCorners((-10,-10), (10,10), (20,20));
#
#########################################################################

sub halfrectanglesCentre  { halfrectanglesGeneric("0","1",@_); }
sub halfrectanglesCorners { halfrectanglesGeneric("0","0",@_); }
sub halfrectanglesCylinderCentre  { halfrectanglesGeneric("1","1",@_); }
sub halfrectanglesCylinderCorners { halfrectanglesGeneric("1","0",@_); }

# A generic halfrectangle grid generator
sub halfrectanglesGeneric {
   my ($cylinder,$centre,$grid_left, $grid_bottom, $grid_right, $grid_top, $xsize, $ysize) = @_;
   my ($base, $height, $ycoord, $xcoord, $xcentre, $ycentre, $x, $y, $inc_ycoord, $inc_xcoord);
   my ($inc_x, $inc_y,$vector_left_x,$vector_left_y,$vector_right_x,$vector_right_y,$inc_inc_xcoord);
   my ($y_dot, $x_dot, $y_top, $y_bottom, $nw,$ne,$sw,$se,$e);

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
      if ($cylinder == "1") {
        # Loop coordinates
        $y_top = $grid_bottom-($y+1)*$height; $y_bottom = $y_top - $height;
        newPoint("L1_$y", $grid_left-($y+1)*$height*2, $y_top);
        newPoint("L2_$y", $grid_left-($y+1)*$height*2, $y_bottom);
        newPoint("L3_$y", $grid_left-($y+1)*$height*2+$height, $y_top);
        newPoint("L4_$y", $grid_left-($y+1)*$height*2+$height, $y_bottom);
  
        newPoint("L7_$y", $grid_right+($y+1)*$height*2, $y_top);
        newPoint("L8_$y", $grid_right+($y+1)*$height*2, $y_bottom);
        newPoint("L5_$y", $grid_right+($y+1)*$height*2-$height, $y_top);
        newPoint("L6_$y", $grid_right+($y+1)*$height*2-$height, $y_bottom);
      }
 
      for ($x=0; $x <= $xsize; $x++) {
         newPoint("P_$x\_$y", $xcoord, $ycoord); 
         $xcoord+=$base;
      }
      $ycoord+=$height;
   }
   if ($cylinder=="1") {
     newVector("NW",-1,1); newVector("SW",-1,-1); newVector("NE",1,1);
     newVector("SE",1,-1); newVector("East", 1,0); newVector("West",-1,0);
   }

   $ycoord = $grid_bottom;
   for ($y = 0; $y < $ysize; $y++) {
      $xcoord = $grid_left;
      for ($x = 0; $x < $xsize; $x++) {
            $inc_x = $x+1; $inc_y = $y+1; $inc_ycoord=$ycoord+$height; $inc_xcoord=$xcoord+$base;

            if ($centre=="1") {
               ($xcentre,$ycentre) = ($inc_xcoord + $base/4, $inc_ycoord + $height/4);
               (($vector_left_x, $vector_left_y),($vector_right_x, $vector_right_y)) =
                  differentialInclusion($xcentre,$ycentre); 
            } else {
               (($vector_left_x, $vector_left_y),($vector_right_x, $vector_right_y)) =
                  differentialInclusion($xcoord, $ycoord, $inc_xcoord, $ycoord, $xcoord, $inc_ycoord);
            }

            newVector("Vl_$x\_$y\_up", $vector_left_x,  $vector_left_y);
            newVector("Vr_$x\_$y\_up", $vector_right_x, $vector_right_y);

            newRegionComment("half rectangle pointing up whose lower left corner is at ($xcoord, $ycoord)");
            newRegion("Vl_$x\_$y\_up", "Vr_$x\_$y\_up",
                      "P_$x\_$y", "P_$inc_x\_$y",
                      "P_$x\_$inc_y", "P_$x\_$y");
 
            if ($centre=="1") {
               ($xcentre,$ycentre) = ($inc_xcoord + 3*$base/4, $inc_ycoord + 3*$height/4);
               (($vector_left_x, $vector_left_y),($vector_right_x, $vector_right_y)) =
                  differentialInclusion($xcentre,$ycentre); 
            } else {
               (($vector_left_x, $vector_left_y),($vector_right_x, $vector_right_y)) =
                  differentialInclusion($inc_xcoord, $inc_ycoord, $inc_xcoord, $ycoord, $xcoord, $inc_ycoord);
            }

            newVector("Vl_$x\_$y\_down", $vector_left_x,  $vector_left_y);
            newVector("Vr_$x\_$y\_down", $vector_right_x, $vector_right_y);

            newRegionComment("half rectangle pointing down whose top right corner is at ($inc_xcoord, $inc_ycoord)");
            newRegion("Vl_$x\_$y\_up", "Vr_$x\_$y\_up",
                      "P_$inc_x\_$inc_y", "P_$x\_$inc_y",
                      "P_$inc_x\_$y", "P_$inc_x\_$inc_y");

            $xcoord = $xcoord + $base;
      }

      if ($cylinder == "1") {
        newRegionComment("looping this strip");
        if ($vector_left_x > 0 || $vector_right_x > 0) {
           ($sw,$se,$e,$ne,$nw) = ("NE","NW","West","SW","SE");
        } else {
           ($sw,$se,$e,$ne,$nw) = ("SW","SE","East","NE","NW");
        }
        newRegion($sw,$sw,"P_0_$y", "P_0_$inc_y", "L1_$y", "L3_$y", "P_0_$y");
        newRegion($se,$se,"L1_$y", "L2_$y", "L4_$y", "L3_$y", "L1_$y");
        newRegion($e, $e, "L3_$y", "L4_$y", "L6_$y", "L5_$y", "L3_$y");
        newRegion($ne,$ne,"L5_$y", "L6_$y", "L8_$y", "L7_$y", "L5_$y");
        newRegion($nw,$nw,"L5_$y", "L7_$y", "P_$inc_x\_$inc_y", "P_$inc_x\_$y", "L5_$y");
      }

      $ycoord = $ycoord+$height;
   }

   endMSPDI;
}

1;
