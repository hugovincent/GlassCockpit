#!/usr/bin/perl
# Front-End program for MapTileCacher - v1.23
# Written by N.Kozawa
# Updated by Cristian Streng

use File::Basename;

$systemDirDivider = "\/";
print "Detected OS: $^O\n";
if($^O ge "cygwin") 
{
	$systemDirDivider = "\/";
}
elsif($^O ge "MSWin")
{
	$systemDirDivider = "\\";
}

$cacher = dirname($0).$systemDirDivider.'MapTileCacher.perl';

if (0 > $#ARGV) {
	opendir(DIR, ".");
	@dir = readdir(DIR);
	closedir(DIR);
	print "Following map files are available.\n";
	foreach $d (@dir) {
		if ($d =~ /\.map$/) {
			print substr($d, 0, length($d) - 4).", ";
		}
	}
	print "\nChoose one of Map File: ";
	chomp($MapFile = <>);
} else {
	$MapFile = $ARGV[0];
}

# Check if the cache folder already exists
$dir_TopLevel = 'MGMapsCache';
$file_CacheConf = 'cache.conf';

if ((-d $dir_TopLevel == FALSE) || (-f $dir_TopLevel.$systemDirDivider.$file_CacheConf == FALSE)) {

  $tiles_per_file=32;
  $hash_size=1;
  
  do {
    print "Map tiles per file (a power of two, 1-4096, default $tiles_per_file): ";
    chomp($tpf = <>);
    if ($tpf eq "") {
      $tpf = $tiles_per_file;
      break;
    }
    $tpf = int($tpf);
    if ($tpf == 0) {
      $tpf = $tiles_per_file;
    }
    # power of two
  } while ($tpf < 1 || $tpf > 4096 || ($tpf & -$tpf) != $tpf);
  
  $tiles_per_file = $tpf;
  
  if ($tpf == 1) {
    do {
      print "Hash size (examples: 11, 23, 47, 97 or 1 to disable hashing, default $hash_size): ";
      chomp($hs = <>);
      if ($hs eq "") {
        $hs = $hash_size;
        break;
      }
      $hs = int($hs);
      if ($hs == 0) {
        $hs = $hash_size;
      }
    } while ($hs < 1 || $hs >= 100);
   
    $hash_size = $hs;
  }
}  

open(MF, $MapFile.".map");
$MapType = "";
while(<MF>) {
	s/[\n\r]//g;
	if (/^#/) { next; }
	if ("" eq $MapType) {
		$MapType = $_;
	} elsif (/\d+/) {
		($ZoomLevel, $TopLeft, $BottomRight) = split(":");
		($Z1, $Z2) = split("[-,]", $ZoomLevel);
		if ($Z2 !~ /\d/) { $Z2 = $Z1; }
		for ($Z = $Z1; $Z <= $Z2; $Z++) {
			open(CMD, "|perl $cacher");
			print CMD "$tiles_per_file\n$hash_size\n$MapType\n$Z\n$TopLeft\n$BottomRight\n\n";
			close(CMD);
		}
	}
}
close(MF);
