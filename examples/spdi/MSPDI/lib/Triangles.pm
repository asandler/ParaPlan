#!/usr/local/bin/perl

use MSPDI;

#########################################################################
# Usage:
#
# use MSPDI;
# use Triangles;
#
# sub xdot { my ($x,$y) = @_; return($x-$y); } 
# sub ydot { my ($x,$y) = @_; return($x+$y); } 
#
# trianglesCentre ((-10,-10), (10,10), (20,20));
#                     ^          ^        ^
#                   lower      upper   number of
#                   left       right   triangles  
#                   coord      coord   in the x and
#                                      y direction
#
# or 
#
# sub ydot { my (($x1,$y1),($x2,$y2), ($x3,$y3)) = @_; return(...); } 
# sub xdot { my (($x1,$y1),($x2,$y2), ($x3,$y3)) = @_; return(...); } 
#
# trianglesCorners((-10,-10), (10,10), (20,20));
#
#########################################################################

sub trianglesCentre  { trianglesGeneric("1",@_); }
sub trianglesCorners { trianglesGeneric("0",@_); }

# A generic triangle grid generator
sub trianglesGeneric {
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

   $ycoord = $grid_bottom;
   for ($y = 0; $y <= $ysize; $y+=2) {
      $xcoord = $grid_left;
      for ($x = 0; $x <= $xsize; $x++) {
         newPoint("P_$x\_$y", $xcoord, $ycoord); 
         $xcoord = $xcoord + $base;
      }
      $ycoord = $ycoord+2*$height;
   }

   $ycoord = $grid_bottom+$height;
   for ($y = 1; $y <= $ysize; $y+=2) {
      $xcoord = $grid_left-$base/2;
      for ($x = 0; $x <= $xsize; $x++) {
         newPoint("P_$x\_$y", $xcoord, $ycoord);
         $xcoord = $xcoord + $base;
      }
      $ycoord = $ycoord+2*$height;
   }
 
   $ycoord = $grid_bottom;
   for ($y = 0; $y < $ysize; $y+=2) {
      $xcoord = $grid_left;
      for ($x = 0; $x < $xsize; $x++) {
            $inc_x = $x+1; $inc_y = $y+1; $inc_ycoord=$ycoord+$height; $inc_xcoord=$xcoord+$base/2;

            if ($centre=="1") {
               ($xcentre,$ycentre) = ($inc_xcoord + $base/2, $inc_ycoord - $height/2);
               $x_dot = xdot($xcentre,$ycentre); $y_dot = ydot($xcentre,$ycentre);
            } else {
               $x_dot = xdot( $inc_xcoord, $inc_ycoord
                            , $inc_xcoord+$base, $inc_ycoord
                            , $inc_xcoord+$base/2, $inc_ycoord+$height);
               $y_dot = ydot( $inc_xcoord, $inc_ycoord
                            , $inc_xcoord+$base, $inc_ycoord
                            , $inc_xcoord+$base/2, $inc_ycoord-$height);
            }
            if ($x_dot == 0) {
              ($vector_left_x, $vector_left_y) = (0, $y_dot);
            } else {
              ($vector_left_x, $vector_left_y) = (1, $y_dot / $x_dot);
            }

            ($vector_right_x, $vector_right_y) = ($vector_left_x, $vector_left_y);

            newVector("Vl_$x\_$y\_up", $vector_left_x,  $vector_left_y);
            newVector("Vr_$x\_$y\_up", $vector_right_x, $vector_right_y);

            newRegionComment("triangle pointing up whose top corner is at ($inc_xcoord, $inc_ycoord)");
            newRegion("Vl_$x\_$y\_up", "Vr_$x\_$y\_up",
                      "P_$inc_x\_$inc_y", "P_$x\_$y",
                      "P_$x\_$inc_y", "P_$inc_x\_$inc_y");
            
            if ($centre=="1") {
               ($xcentre,$ycentre) = ($inc_xcoord + $base/2, $inc_ycoord + $height/2);
               $x_dot = xdot($xcentre,$ycentre); $y_dot = ydot($xcentre,$ycentre);
            } else {
               $x_dot = xdot( $inc_xcoord, $inc_ycoord
                            , $inc_xcoord+$base, $inc_ycoord
                            , $inc_xcoord+$base/2, $inc_ycoord+$height);
               $y_dot = ydot( $inc_xcoord, $inc_ycoord
                            , $inc_xcoord+$base, $inc_ycoord
                            , $inc_xcoord+$base/2, $inc_ycoord+$height);
            }
            if ($x_dot == 0) {
              ($vector_left_x, $vector_left_y) = (0, $y_dot);
            } else {
              ($vector_left_x, $vector_left_y) = (1, $y_dot / $x_dot);
            }
            ($vector_right_x, $vector_right_y) = ($vector_left_x, $vector_left_y);


            newVector("Vl_$x\_$y\_down", $vector_left_x,  $vector_left_y);
            newVector("Vr_$x\_$y\_down", $vector_right_x, $vector_right_y);

            newRegionComment("triangle pointing down whose top corner is at ($xcoord, $ycoord)");
            newRegion("Vl_$x\_$y\_up", "Vr_$x\_$y\_up",
                      "P_$x\_$y", "P_$inc_x\_$inc_y",
                      "P_$inc_x\_$y", "P_$x\_$y");

            $xcoord = $xcoord + $base;
      }
      $ycoord = $ycoord+2*$height;
   }

   $ycoord = $grid_bottom+$height;
   for ($y = 1; $y < $ysize; $y+=2) {
      $xcoord = $grid_left-$base/2;
      for ($x = 0; $x < $xsize; $x++) {
            $inc_x = $x+1; $inc_y = $y+1; $inc_ycoord=$ycoord+$height;
            $inc_xcoord=$xcoord+$base/2; $inc_inc_xcoord=$xcoord+$base;

            if ($centre=="1") {
               ($xcentre,$ycentre) = ($inc_xcoord + $base/2, $inc_ycoord - $height/2);
               $x_dot = xdot($xcentre,$ycentre); $y_dot = ydot($xcentre,$ycentre);
            } else {
               $x_dot = xdot( $inc_xcoord, $inc_ycoord
                            , $inc_xcoord+$base, $inc_ycoord
                            , $inc_xcoord+$base/2, $inc_ycoord-$height);
               $y_dot = ydot( $inc_xcoord, $inc_ycoord
                            , $inc_xcoord+$base, $inc_ycoord
                            , $inc_xcoord+$base/2, $inc_ycoord-$height);
            }
            if ($x_dot == 0) {
              ($vector_left_x, $vector_left_y) = (0, $y_dot);
            } else {
              ($vector_left_x, $vector_left_y) = (1, $y_dot / $x_dot);
            }
            ($vector_right_x, $vector_right_y) = ($vector_left_x, $vector_left_y);

            newVector("Vl_$x\_$y\_up", $vector_left_x,  $vector_left_y);
            newVector("Vr_$x\_$y\_up", $vector_right_x, $vector_right_y);

            newRegionComment("triangle pointing up whose top corner is at ($inc_xcoord, $inc_ycoord)");
            newRegion("Vl_$x\_$y\_up", "Vr_$x\_$y\_up",
                      "P_$x\_$inc_y", "P_$inc_x\_$y",
                      "P_$x\_$y", "P_$x\_$inc_y");

            if ($centre=="1") {
               ($xcentre,$ycentre) = ($inc_xcoord + $base/2, $inc_ycoord + $height/2);
               $x_dot = xdot($xcentre,$ycentre); $y_dot = ydot($xcentre,$ycentre);
            } else {
               $x_dot = xdot( $inc_xcoord, $inc_ycoord
                            , $inc_xcoord+$base, $inc_ycoord
                            , $inc_xcoord+$base/2, $inc_ycoord+$height);
               $y_dot = ydot( $inc_xcoord, $inc_ycoord
                            , $inc_xcoord+$base, $inc_ycoord
                            , $inc_xcoord+$base/2, $inc_ycoord+$height);
            }
            if ($x_dot == 0) {
              ($vector_left_x, $vector_left_y) = (0, $y_dot);
            } else {
              ($vector_left_x, $vector_left_y) = (1, $y_dot / $x_dot);
            }
            ($vector_right_x, $vector_right_y) = ($vector_left_x, $vector_left_y);
 
            newVector("Vl_$x\_$y\_down", $vector_left_x,  $vector_left_y);
            newVector("Vr_$x\_$y\_down", $vector_right_x, $vector_right_y);

            newRegionComment("triangle pointing down whose top corner is at ($inc_inc_xcoord, $ycoord)");
            newRegion("Vl_$x\_$y\_up", "Vr_$x\_$y\_up",
                      "P_$inc_x\_$inc_y", "P_$inc_x\_$y",
                      "P_$x\_$inc_y", "P_$inc_x\_$inc_y");

            $xcoord = $xcoord + $base;
      }
      $ycoord = $ycoord+2*$height;
   }


   endMSPDI;
}

1;
