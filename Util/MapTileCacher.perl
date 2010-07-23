#!/usr/bin/perl
# version 1.35
# created by MichaelAtUVa
# updated by Cristian Streng based on suggestions by n95_rit, Kozawa, denilsonsa, and others
# modified by Luca Bisti

#Externals
use Math::Trig;
use POSIX;

#Proxy Variables
$use_proxy = 0;
# uncomment to use proxy for Google Satellite Download
# $use_proxy = 1;
@proxy_list = (
   'http://edwarf.com/cgi-bin/nph-proxy.cgi/000000A/',
   'http://hostingdi.com/cgiproxy/nph-proxy.pl/000000A/',
   'http://www.alexmattoni.info/scgi-bin/nph-proxy.cgi/000000A/'
);

#Caching Variables
$dir_TopLevel = "MGMapsCache";
$file_CacheConf = "cache.conf";
$cache_Version = 3;
$tiles_per_file = 1;
$hash_size = 1;
$center_found = 0;

#Cache Source Variables
$cache_source = 0;
$dir_TopLevel_src = "MGMapsCache_src";
$file_CacheConf_src = "cache.conf";
$cache_Version_src = 3;
$tiles_per_file_src = 1;
$hash_size_src = 1;
$center_found_src = 0;

# Sleep
$sleepTime = 0;
$sleepEvery = 10000;

print <<EOF;

*** MGMaps Tile Cacher ***

EOF

#OS Specific Variables
$pid = $$;
$wget = "wget -Y on -nv -O TempTile.$pid ";
$linuxDirDivider = "\/"; $linuxMoveCommand = "mv"; $linuxDeleteCommand = "rm"; $linuxCopyCommand = "cp";
$windowsDirDivider = "\\"; $windowsMoveCommand = "move"; $windowsDeleteCommand = "del"; $windowsCopyCommand = "copy";
print "Detected OS: $^O\n";
if($^O ge "linux" || $^O ge "cygwin" )
{
  $systemDirDivider = $linuxDirDivider;
  $systemMoveCommand = $linuxMoveCommand;
  $systemCopyCommand = $linuxCopyCommand;
  $systemDeleteCommand = $linuxDeleteCommand;
  $systemRefererParam = "--referer=http://maps.google.com/ ";
}
elsif($^O ge "MSWin")
{
  $systemDirDivider = $windowsDirDivider;
  $systemMoveCommand = $windowsMoveCommand;
  $systemCopyCommand = $windowsCopyCommand;
  $systemDeleteCommand = $windowsDeleteCommand;
  $systemRefererParam = "--referer=http://maps.google.com/ ";
}
if($^O eq "darwin")
{
  $systemDirDivider = $linuxDirDivider;
  $systemMoveCommand = $linuxMoveCommand;
  $systemDeleteCommand = $linuxDeleteCommand;
  $wget = "curl -# -o TempTile.$pid ";
  $systemRefererParam = "-e http://maps.google.com/ ";
}

$file_CacheConf_src = $dir_TopLevel_src.$systemDirDivider.$file_CacheConf;
$file_CacheConf = $dir_TopLevel.$systemDirDivider.$file_CacheConf;

# Main


# Check Cache Source configuration
if (-d $dir_TopLevel_src == FALSE) {
  $cache_source = 0;
}
else
{
if (-f $file_CacheConf_src != FALSE) {
  open(F, "<".$file_CacheConf_src) or die("Cannot read cache source config file: $file_CacheConf_src");
  $cache_source = 1;
  $line = <F>;
  $hash_size_src = 97;
  next if ($line =~ /^#/);
  @splits = split(/\s*=\s*/, $line);
  if (($splits[0] eq 'version') && int($splits[1]) > 2) {
    while ($line = <F>) {
      @splits = split('=', $line);
      if ($splits[0] eq 'tiles_per_file') {
        $tiles_per_file_src = int($splits[1]);
      }
      elsif ($splits[0] eq 'hash_size') {
        $hash_size_src = int($splits[1]);
      }
      elsif ($splits[0] eq 'center') {
        $center_found_src = 1;
      }
    }
  }
  else {
    print "Older version of MGMapsCache found for source cache, please (re)move it.\n";
    $cache_source = 0;
    exit;
  }
  close(F);
}
if ($tiles_per_file_src > 1) {
  $hash_size_src = 1;
}
print "Cached source: $tiles_per_file_src tiles per file, hash_size = $hash_size_src\n";
}

# Read and/or write configuration
if (-d $dir_TopLevel == FALSE) {
  system("mkdir $dir_TopLevel");
}
# Search for version number
$configured = 0;
if (-f $file_CacheConf != FALSE) {
  open(F, "<".$file_CacheConf) or die("Cannot read cache config file");
  $line = <F>;
  $hash_size = 97;
  next if ($line =~ /^#/);
  @splits = split(/\s*=\s*/, $line);
  if (($splits[0] eq 'version') && int($splits[1]) > 2) {
    $configured = 1;
    while ($line = <F>) {
      @splits = split('=', $line);
      if ($splits[0] eq 'tiles_per_file') {
        $tiles_per_file = int($splits[1]);
      }
      elsif ($splits[0] eq 'hash_size') {
        $hash_size = int($splits[1]);
      }
      elsif ($splits[0] eq 'center') {
        $center_found = 1;
      }
    }
  }
  else {
    print "Older version of MGMapsCache found, please (re)move it.\n";
    exit;
  }
  close(F);
}

if ($tiles_per_file > 1) {
  $hash_size = 1;
}



# Ask for configuration
if ($configured == 0) {
  # read configuration
  print "Cache configuration file does not exist, configuring...\n\n";
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

  else {
    print "Hashing is disabled.\n";
    $hash_size = 1;
  }

  open(F, ">".$file_CacheConf) or die("Cannot write cache config file");
  print F "version=".$cache_Version."\n";
  print F "tiles_per_file=".$tiles_per_file."\n";
  print F "hash_size=".$hash_size."\n";
  close(F);
}
else {
  print "Storing $tiles_per_file tiles per file.\n";
  if ($hash_size > 1) {
    print "Hash size is $hash_size.\n";
  }
  else {
    print "Hashing is disabled.\n";
  }
  print "\n";
}

# Cache output computations
$tiles_per_file_log = 0;
$tpf = $tiles_per_file;
while ($tpf > 1) {
  $tiles_per_file_log++;
  $tpf>>=1;
}
$tpfy = $tiles_per_file_log>>1;
$tpfx = 1<<($tpfy + ($tiles_per_file_log&1));
$tpfy = 1<<$tpfy;

# Cache Source computations
$tiles_per_file_log = 0;
$tpf = $tiles_per_file_src;
while ($tpf > 1) {
  $tiles_per_file_log++;
  $tpf>>=1;
}
$tpfy_src = $tiles_per_file_log>>1;
$tpfx_src = 1<<($tpfy_src + ($tiles_per_file_log&1));
$tpfy_src = 1<<$tpfy_src;


my @mapTypeOrder = (
  'GoogleMap', 'GoogleSat', 'GoogleHyb', 'GoogleTer', 'GoogleChina',
  'MicrosoftMap', 'MicrosoftSat', 'MicrosoftHyb', 'MicrosoftTer', 'MicrosoftBrMap',
  'YahooMap', 'YahooSat', 'YahooHyb', 'YahooInMap', 'YahooInHyb',
  'OpenStreetMap', 'Osmarender', 'OpenAerialMap', 'OpenCycleMap'
);

my %mapTypes = (

GoogleMap => 'Google Road Maps',
GoogleSat => 'Google Satellite Images',
GoogleHyb => 'Google Satellite Images with Road Maps Overlayed',
GoogleTer => 'Google Terrain Maps',
GoogleChina => "Google Road Maps (China)\n",

MicrosoftMap => 'Microsoft Live Road Maps',
MicrosoftSat => 'Microsoft Live Aerial Images',
MicrosoftHyb => 'Microsoft Live Aerial Images with Road Maps Overlayed',
MicrosoftTer => 'Microsoft Live Road Maps with Terrain Shading',
MicrosoftBrMap => "Microsoft Live Road Maps (Brasil)\n",

YahooMap => 'Yahoo Road Maps',
YahooSat => 'Yahoo Satellite Images',
YahooHyb => 'Yahoo Satellite Images with Road Maps Overlayed',
YahooInMap => 'Yahoo Road Maps (India)',
YahooInHyb => "Yahoo Satellite Images with Road Maps Overlayed (India)\n",

OpenStreetMap => 'OpenStreetMap.org Maps (Mapnik Renderer)',
Osmarender => "OpenStreetMap.org Maps (Osmarender)",
OpenAerialMap => 'OpenAerialMap.org imagery',
OpenCycleMap => 'Open Cycle Map imagery\n'
);

print <<EOF;
Please choose map type by entering the associated keyword (case sensitive)...

EOF

foreach $k (@mapTypeOrder) {
  print "$k - $mapTypes{$k}\n";
}

print "Choice: ";
do {
  $MapType = <>;
  $MapType =~ s/^\s+//;
  $MapType =~ s/\s+$//;

  $found = 0;
  while ( my ($k, $v) = each (%mapTypes) ) {
    if (lc($k) eq lc($MapType)) {
      $found = 1;
      $MapType = $k;
      last;
    }
  }

  if ($found == 0) {
    print "Invalid map type. Please try again: ";
  }
} while (($MapType eq "") || ($found == 0));

print "Enter Zoom Level (1-22): ";
chomp($zoomLevel = <>);
$zoomLevel = int($zoomLevel);

print "\nEnter coordinates in decimal degrees format... 38.031078, -78.481529 \n";
print "Top left (latitude, longitude):  ";
@topLeftCoords = split(/,/,<>,2);
($topLeftTileX, $topLeftTileY) = convertCoordindates2Tiles(@topLeftCoords, $zoomLevel);

print "Bottom right (latitude, longitude):  ";
@bottomRightCoords = split(/,/,<>,2);
($bottomRightTileX, $bottomRightTileY) = convertCoordindates2Tiles(@bottomRightCoords, $zoomLevel);

my $bound = 1<<$zoomLevel;
# swap coords if not in correct order
if ($topLeftTileX > $bottomRightTileX) { $t = $topLeftTileX; $topLeftTileX = $bottomRightTileX; $bottomRightTileX = $t; }
if ($topLeftTileY > $bottomRightTileY) { $t = $topLeftTileY; $topLeftTileY = $bottomRightTileY; $bottomRightTileY = $t; }
# out of map?
if ($topLeftTileX < 0) { $topLeftTileX = 0; }
if ($bottomRightTileX >= $bound) { $bottomRightTileX = $bound-1; }
if ($topLeftTileY < 0) { $topLeftTileY = 0; }
if ($bottomRightTileY >= $bound) { $bottomRightTileY = $bound-1; }
# still swapped coords? (we're entirely out of the map)
if ($topLeftTileX > $bottomRightTileX or $topLeftTileY > $bottomRightTileY) {
  print "\n*** No map tiles to download ***\n";
  exit;
}

$numTiles = (abs($topLeftTileX-$bottomRightTileX)+1)*(abs($topLeftTileY-$bottomRightTileY)+1);
$numTiles *= (index($MapType,"Hyb") != -1 && ($MapType ne "MicrosoftHyb")) ? 2 : 1;

if ($center_found == 0) {
  open(F, ">>".$file_CacheConf) or die("Cannot write cache config file");
  print F "center=".(($topLeftCoords[0]+$bottomRightCoords[0])/2).",".(($topLeftCoords[1]+$bottomRightCoords[1])/2).",".$zoomLevel.",".$MapType."\n";
  close(F);
}
print <<EOF;

Ready to cache $numTiles tiles...

Type: $MapType
Zoom: $zoomLevel

Press enter to continue.
EOF

$_ = <>;

print "*** Caching Started ***\n\n";
my $tileCount = 0;
my $actualTileCount = 0;
downloadTiles($MapType, $topLeftTileX, $topLeftTileY, $bottomRightTileX, $bottomRightTileY, 1);
print "\n*** Caching Complete ***\n";

exit;

#MapTileCacher Subroutines

sub convertCoordindates2Tiles
{
    my $lat = shift;
    my $long = shift;
    my $z = shift;

    my $pixelsPerTile = 256;

    my $mapsize = 2**($z+(log($pixelsPerTile)/log(2)));
    my $origin = $mapsize / 2;

    my $longdeg = abs(-180 - $long);
    my $longppd = $mapsize / 360;
    my $longppdrad = $mapsize/(2*pi);
    my $pixelx = $longdeg * $longppd;
    my $longtiles = $pixelx  / $pixelsPerTile;

    my $tilex = cast2Integer($longtiles);

    my $e = sin($lat*(1/180.*pi));
    if ($e>0.9999) { $e=0.9999; }
    if ($e<-0.9999) { $e=-0.9999; }

    my $pixely = $origin + 0.5*log((1+$e)/(1-$e)) * (-$longppdrad);
    my $tiley = cast2Integer($pixely / $pixelsPerTile);

    return ($tilex, $tiley);
}

sub downloadTiles
{
  my $MapType = shift;
  my $topLeftTileX = shift;
  my $topLeftTileY = shift;
  my $bottomRightTileX = shift;
  my $bottomRightTileY = shift;
  my $rec = shift;

  if(index($MapType,"Hyb") != -1 && ($MapType ne "MicrosoftHyb") && ($rec == 1))
  {
      my $MapSource = substr($MapType,0,index($MapType,"Hyb"));
      if (index($MapType,"Ter") != -1)
      {
        downloadTiles($MapSource, $topLeftTileX, $topLeftTileY, $bottomRightTileX, $bottomRightTileY, 0);
      }
      else
      {
        downloadTiles($MapSource . "Sat", $topLeftTileX, $topLeftTileY, $bottomRightTileX, $bottomRightTileY, 0);
      }
      downloadTiles($MapSource . "Hyb", $topLeftTileX, $topLeftTileY, $bottomRightTileX, $bottomRightTileY, 0);
  }
  else
  {
    for ($y = $topLeftTileY; $y <= $bottomRightTileY; $y++)
    {
      for ($x = $topLeftTileX; $x <= $bottomRightTileX; $x++)
      {
        $tileCount++;

        # avoid sleeping for #0
        if ((($actualTileCount+1) % $sleepEvery) == 0 && index($MapType,"Sat") != -1)
        {
          print "* Sleeping $sleepTime seconds\n";
          sleep($sleepTime);
        }

        my $localFileName = $dir_TopLevel . $systemDirDivider . $MapType . "_" .$zoomLevel;
        if ($hash_size > 1) {
          $localFileName .= $systemDirDivider . (($x*256+$y)%$hash_size) . $systemDirDivider . "$x\_$y";
        }
        else {
          $localFileName .= $systemDirDivider . int($x/$tpfx) . "_" . int($y/$tpfy);
        }
        $localFileName .= ".mgm";
        $dx = $x % $tpfx;
        $dy = $y % $tpfy;

        # Cache Source
        my $localFileName_src = $dir_TopLevel_src . $systemDirDivider . $MapType . "_" .$zoomLevel;
        if ($hash_size_src > 1) {
          $localFileName_src .= $systemDirDivider . (($x*256+$y)%$hash_size_src) . $systemDirDivider . "$x\_$y";
        }
        else {
          $localFileName_src .= $systemDirDivider . int($x/$tpfx_src) . "_" . int($y/$tpfy_src);
        }
        $localFileName_src .= ".mgm";
        $dx_src = $x % $tpfx_src;
        $dy_src = $y % $tpfy_src;

        my $url = getTileURL($MapType,$x,$y);

        $found = checkCached($localFileName,$dx,$dy);

        if($found == 0)
        {
          if($cache_source == 1)
          {
            print "* Reading tile $tileCount of $numTiles from cached file $localFileName_src\n";
            $found = getCached($localFileName_src,$dx_src,$dy_src);
          }
          if($found == 0) {
            my $referer = (index($MapType,"Google") != -1) ? $systemRefererParam : "";
            $wgetFileCommand = $wget . $referer. "\"" . $url . "\"" ;
            print "* Downloading tile $tileCount of $numTiles from $url\n";
            if (index($MapType,"Sat") != -1) {$actualTileCount++;}
            system($wgetFileCommand);
          }
          cacheDownloadedTile("TempTile.$pid",$localFileName,$dx,$dy);
        }

        else {print "* Skipping tile $tileCount of $numTiles from $url (already cached).\n";}
      }
    }
  }
}

# check if the map tile is already cached in $localFilename as ($dx,$dy)
sub checkCached
{
  my $localFileName = shift;
  my $dx = shift;
  my $dy = shift;
  local $buf = '';

  if (-f $localFileName == FALSE) {
    return 0;
  }

  # if the file exists and we have 1 tile/file, don't verify anything else
  elsif ($tiles_per_file == 1) {
    return 1;
  }

  sysopen(FD, $localFileName, O_RDONLY);
  binmode(FD);
  sysread(FD, $buf, 6*$tiles_per_file+2);
  $num = (ord(substr($buf,0,1))<<8)+ord(substr($buf,1,1));
  for (my $i=0; $i<$num; $i++) {
    if (ord(substr($buf,2+$i*6,1)) == $dx && ord(substr($buf,2+$i*6+1,1)) == $dy) {
      close(FD);
      return 1;
    }
  }
  close(FD);
  return 0;
}

# get the map tile already cached in $localFilename as ($dx,$dy)
sub getCached
{
  my $localFileName = shift;
  my $dx = shift;
  my $dy = shift;
  local $buf = '';

  my $ofs_prev = 0;
  my $ofs_next = 2+6*$tiles_per_file_src;

  # print "   getCached($localFileName $dx $dy)\n";

  if (-f $localFileName == FALSE) {
    return 0;
  }

  # if the file exists and we have 1 tile/file, just copy the source file
  elsif ($tiles_per_file_src == 1) {
    $copyCommand = "$systemCopyCommand $localFileName TempTile.$pid";
    # print "  executing: $copyCommand";
    system($copyCommand);
    return 1;
  }

  sysopen(FD, $localFileName, O_RDONLY);
  binmode(FD);
  sysread(FD, $buf, 6*$tiles_per_file_src+2);
  $num = (ord(substr($buf,0,1))<<8)+ord(substr($buf,1,1));
  for (my $i=0; $i<$num; $i++) {
    $ofs_prev = $ofs_next;
    $ofs_next = (ord(substr($buf,2+$i*6+2,1))<<24)+
              (ord(substr($buf,2+$i*6+3,1))<<16)+
              (ord(substr($buf,2+$i*6+4,1))<<8)+
               ord(substr($buf,2+$i*6+5,1));
    if (ord(substr($buf,2+$i*6,1)) == $dx && ord(substr($buf,2+$i*6+1,1)) == $dy) {
    # read image at proper offset and write to TempTile.$pid
    sysseek(FD, $ofs_prev, 0);
    sysread(FD, $buf, $ofs_next - $ofs_prev);
      close(FD);
      sysopen(FD, "TempTile.$pid", O_WRONLY|O_CREAT);
    binmode(FD);
    syswrite(FD, $buf, $ofs_next - $ofs_prev);
    close(FD);
      return 1;
    }
  }
  close(FD);
  return 0;
}

sub cacheDownloadedTile
{
  my $tempTile = shift;
  my $localFileName = shift;
  my $dx = shift;
  my $dy = shift;
  local $buf = '';

  if(-s $tempTile)
  {
    my $dir_TopLevel;
    my $dir_TypeZoomLevel;
    my $dir_HashLevel;
    if ($hash_size > 1) {
      ($dir_TopLevel, $dir_TypeZoomLevel, $dir_HashLevel)
      = split(($systemDirDivider eq "\\") ? "$systemDirDivider$systemDirDivider" : "$systemDirDivider",$localFileName);
    }

    else {
      ($dir_TopLevel, $dir_TypeZoomLevel)
      = split(($systemDirDivider eq "\\") ? "$systemDirDivider$systemDirDivider" : "$systemDirDivider",$localFileName);
    }

    $dir_TypeZoomLevel = $dir_TopLevel . $systemDirDivider . $dir_TypeZoomLevel;
    if(-d $dir_TypeZoomLevel == FALSE){system("mkdir $dir_TypeZoomLevel");}

    if ($tiles_per_file == 1) {
      if ($hash_size > 1) {
        $dir_HashLevel = $dir_TypeZoomLevel . $systemDirDivider . $dir_HashLevel;
        if(-d $dir_HashLevel == FALSE){system("mkdir $dir_HashLevel");}
      }
      system("$systemMoveCommand $tempTile $localFileName");
      print "\tTile cached at $localFileName\n";
      return;
    }

    # append to the file, write info and offset to header
    my $num = 0;
    my $ofs = $tiles_per_file*6+2;
    $existed = ((-f $localFileName != FALSE) ? 1 : 0);
    sysopen($fh, $localFileName, O_RDWR|O_CREAT);
    binmode($fh);
    if ($existed == 1) {
      sysread($fh, $buf, $ofs);
      $num = (ord(substr($buf,0,1))<<8)+ord(substr($buf,1,1));
      $ofs = (ord(substr($buf,2+$num*6-6+2,1))<<24)+
             (ord(substr($buf,2+$num*6-6+3,1))<<16)+
             (ord(substr($buf,2+$num*6-6+4,1))<<8)+
             ord(substr($buf,2+$num*6-6+5,1));
    }
    else {
      $buf = chr(0) x $ofs;
      syswrite($fh, $buf, $ofs);
    }
    my $totalsize = 0;
    sysseek($fh, $ofs, 0);
    sysopen(F2, $tempTile, O_RDONLY);
    binmode(F2);
    while ((my $read = sysread(F2, $buf, 4096)) > 0) {
      $totalsize += $read;
      syswrite($fh, $buf, $read);
    }
    close(F2);
    # new offset
    $totalsize += $ofs;

    # update header - number of tiles
    $buf = "" . chr(($num+1)>>8) . chr(($num+1)&255);
    sysseek($fh, 0, 0);
    syswrite($fh, $buf, 2);

    # update header - new tile data
    $buf = "" . chr($dx) . chr($dy) . chr($totalsize>>24) . chr(($totalsize>>16)&255) . chr(($totalsize>>8)&255) . chr($totalsize&255);
    sysseek($fh, 6*$num+2, 0);
    syswrite($fh, $buf, 6);
    close($fh);

    # delete temp tile
    system("$systemDeleteCommand $tempTile");

    # system("$systemMoveCommand $tempTile $localFileName");

    print "\tTile cached at $localFileName\n"
  }
  else {system("$systemDeleteCommand $tempTile"); print "\tERROR: Requested tile did not exist.\n"}
}

sub getTileURL
{
  my $MapType = shift;
  my $x = shift;
  my $y = shift;

  my $url = "";

  if($MapType eq "GoogleMap" || $MapType eq "GoogleHyb" || $MapType eq "GoogleTer")
  {
    $url = "http://mt" . (($x+$y)&3) . ".google.com/mt/v=";
    $url .= ($MapType eq "GoogleMap") ? "w2.89" :
            ($MapType eq "GoogleHyb") ? "w2t.88" : "w2p.87";
    $url .= "&hl=en&x=$x&y=$y&z=$zoomLevel";
  }
  elsif($MapType eq "GoogleChina")
  {
    $url = "http://mt" . (($x+$y)&3) . ".google.cn/mt/v=cn1.6&hl=en&x=$x&y=$y&z=$zoomLevel";
  }
  elsif($MapType eq "GoogleSat")
  {
    if ($use_proxy)
    {
      $url = $proxy_list[int(rand(@proxy_list))] . "http/khm";
    }
    else
    {
      $url = "http://khm";
    }

    $url .= (($x+$y)&3) . ".google.com/kh/v=36&hl=en&x=$x&y=$y&z=$zoomLevel";
  }
  elsif(index($MapType,"YahooIn") != -1)
  {
    $url = ($MapType eq "YahooInMap") ? "http://maps.yimg.com/hw/tile?locale=en&imgtype=png&yimgv=1.2&v=4.1" :
           "http://maps.yimg.com/hw/tile?imgtype=png&yimgv=0.95&t=p";
    $url .= "&x=$x&y=" . (((1 << $zoomLevel) >> 1)-1-$y) . "&z=" . (17-$zoomLevel+1);
  }
  elsif(index($MapType,"Yahoo") != -1)
  {
    $url = ($MapType eq "YahooMap") ? "http://us.tile.maps.yimg.com/tl?v=4.2" :
       ($MapType eq "YahooSat") ? "http://aerial.maps.yimg.com/ximg?t=a&v=1.9&s=256" :
       "http://us.tile.maps.yimg.com/tl?v=4.2&t=h";
    $url .= "&x=$x&y=" . (((1 << $zoomLevel) >> 1)-1-$y) . "&z=" . ($zoomLevel+1) . "&r=1";
  }
  elsif(index($MapType,"Microsoft") != -1)
  {
    $digit = ((($y & 1) << 1) + ($x & 1));
    if ($MapType eq "MicrosoftBrMap") {
      $digit++;
      $url = "http://imak" . $digit . ".maplink3.com.br/maps.ashx?v=";
    }
    else
    {
      $url  = ($MapType eq "MicrosoftHyb") ?  "http://h" : ($MapType eq "MicrosoftSat") ?  "http://a" : "http://r";
      $url .= $digit . ".ortho.tiles.virtualearth.net/tiles/";
      $url .= ($MapType eq "MicrosoftHyb") ?  "h" : ($MapType eq "MicrosoftSat") ?  "a" : "r";
    }
    for($i=$zoomLevel-1; $i >= 0; $i--)
    {
              $url = $url . ((((($y >> $i) & 1) << 1) + (($x >> $i) & 1)));
    }
    if ($MapType eq "MicrosoftBrMap")
    {
      $url .= "|t&call=2.2.3";
    }
    else
    {
      $url .= ($MapType eq "MicrosoftMap" || $MapType eq "MicrosoftTer") ?  ".png" : ".jpeg";
      $url .= "?g=244&mkt=en-us";
      if ($MapType eq "MicrosoftTer")
      {
        $url .= "&shading=hill";
      }
    }
  }
  elsif(index($MapType,"OpenStreetMap") != -1)
  {
    $url  = "http://tile.openstreetmap.org/" . $zoomLevel . "/" . $x . "/" . $y . ".png";
  }
  elsif(index($MapType,"Osmarender") != -1)
  {
    $url = "http://tah.openstreetmap.org/Tiles/tile/" . $zoomLevel . "/" . $x . "/" . $y . ".png";
  }
  elsif(index($MapType,"OpenAerialMap") != -1)
  {
    $url = "http://tile.openaerialmap.org/tiles/?v=mgm&layer=openaerialmap-900913&x=$x&y=$y&zoom=" . (17-$zoomLevel);
  }
  elsif(index($MapType,"OpenCycleMap") != -1)
  {
    $url = "http://andy.sandbox.cloudmade.com/tiles/cycle/" . $zoomLevel . "/" . $x . "/" . $y . ".png";
  }

  return $url;
}

#Utility Subroutines

sub cast2Integer
{
    my $f = shift;
    if ($f < 0) {return ceil($f);}
    else {return floor($f);}
}
