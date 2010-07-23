Readme.txt for MapTileFE.pl v1.0 - N.Kozawa (nkozawa@nkozawa.com)

MapTileFE.pl is front end processor for MapTileCacher.perl. MapTileCacher.perl has dialog style 
interface. It is good to use first time. But we need to invoke MapTileCacher multiple times to get 
different zoom levels of same regions to have a set of map tiles. MapTileFE makes it easy.

MapTileFE reads a file which has extension 'map' (map definition file). map definition file is simple 
text file describes map type, zoom levels and coordinates. A map definition file can contain multiple 
zoom levels and coordinates. So that you can create a set of map tiles by single MapTilFE 
invocation.

Perhaps, map definition files can be shared among MGMaps users.

[ Files ]

MapTileFE.zip contains following files.
> Readme.txt   - this file
> MapTileFE.pl - Front end program of MapTileCacher


[ Preparations ]

Copy MapTileFE.pl and map definition files to same folder of MapTileCacher.perl.


[ Run MapTileFE ]

Command format:
perl MapTileFE.pl [map_name]
> map_name = file name of map definition file without extension

If you do not specify map_name, MapTileFE will display map names. You can enter map name after 
that.


[ map definition file format ]

Please refer sample map definition files. You may be able to understand it easily.
- Text line started with '#' is comment line.
- First line except comment line indicates map type.
- Following lines are coordinate parameters. Format is as follows.
  Zoom_Level : Top_Left : Right_Bottom
Each parts are separated by colon (':'). Zoom_Level can be a range of zoom levels as well as 
single zoom level. Top_Left and Right_Bottom is a pair of coordinates to specify rectangle area on 
the globe. Each Top_Left or Right_Bottom is a set of latitude and longitude separated by comma.

Note to Mac user: map definition file should have LF or CRLF to end of each lines. Mac style text 
file is not suitable for this. 
