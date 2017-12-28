#!/usr/local/bin/perl 

use strict;

my $warnings      = 0;
my $spdi_header   = "";
my $spdi_points   = "";
my $spdi_vectors  = "";
my $spdi_regions  = "";
my @mspdi_vectors = ();
my @mspdi_points  = ();
my $mspdi_infile  = "";
my $mspdi_outfile = "";

sub constraint { 
   my ($name, $condition) = @_;

   if (!$condition) {
      error("contraint \"$name\" not satisfied");
   }
}

sub warning {
   my ($message) = @_;
   print("Warning: $message\n");
   $warnings++;
}

sub error {
   my ($message) = @_;
   print("Error: $message\n");
   exit;
}

sub remark {
   my ($message) = @_;
   print("$message\n");
}

sub newRegion {
   my ($vector_left, $vector_right, @pointnames) = @_;
   my $newregion = "@pointnames";

   $newregion =~ s/ / : /g; 
   $spdi_regions = "$spdi_regions$newregion, $vector_left, $vector_right\n";
}

sub newPoint {
   my ($newname, $newx, $newy) = @_;
   my ($name);

   foreach $name (@mspdi_points) {
     if ($name eq $newname) { error("redefinition of point named $name") }
   }

   foreach $name (@mspdi_vectors) {
     if ($name eq $newname) { error("defining point with name $name which is already used as a vector name") }
   }


   $spdi_points = sprintf("%s%s. %f, %f\n",$spdi_points,$newname,$newx,$newy);
   #$spdi_points = "$spdi_points$newname. $newx, $newy\n"
}

sub newVector {
   my ($newname, $newx, $newy) = @_;
   my ($name);
   
   foreach $name (@mspdi_vectors) {
     if ($name eq $newname) { error("redefinition of vector named $name") }
   }

   foreach $name (@mspdi_points) {
     if ($name eq $newname) { error("defining vector with name $name which is already used as a point name") }
   }

   $spdi_vectors = sprintf("%s%s. %f, %f\n",$spdi_vectors,$newname,$newx,$newy);
   #$spdi_vectors = "$spdi_vectors$newname. $newx, $newy\n"
}

sub newHeaderComment {
   my ($comment) = @_;
   $spdi_header = "$spdi_header* $comment\n";
}

sub newRegionComment {
   my ($comment) = @_;
   $spdi_regions = "$spdi_regions* $comment\n";
}

sub newPointComment {
   my ($comment) = @_;
   $spdi_points = "$spdi_points* $comment\n";
}

sub newVectorComment {
   my ($comment) = @_;
   $spdi_vectors = "$spdi_vectors* $comment\n";
}

sub startMSPDI {
   # register arguments
   $mspdi_infile  = $0;
   if ( @ARGV[0] eq "" ) {
      $_ = $mspdi_infile; s/\.mspdi$//;
      $mspdi_outfile = "$_.spdi";
   } else {
      $mspdi_outfile = @ARGV[0];
   }  

   # the file signature
   $spdi_header = "* Created by the perl mspdi library from $mspdi_infile\n\n";

   # the spdi file accumulated text
   $spdi_vectors = "\nVectors:\n\n";
   $spdi_regions = "\nRegions:\n\n";
   $spdi_points  = "\nPoints:\n\n";

   # the vector and point names defined
   @mspdi_vectors = ();
   @mspdi_points  = ();

   # number of warnings issued
   $warnings = 0;
}

sub endMSPDI {
   my $FILE;

   print "MSPDI Perl Library v0.1\n\n";
   print "Generating file $mspdi_outfile from $mspdi_infile\n";
   open (FILE, ">$mspdi_outfile");
   print FILE "$spdi_header$spdi_points$spdi_vectors$spdi_regions\n* End of spdifile\n";
   close (FILE);

   print "Successfully generated spdi file\n";

   if ($warnings > 0) {
      if ($warnings == 1) {
         print "\n1 warning found\n";
      } else {
         print "\n$warnings warnings found\n";
      }
   } 
}  

1;
