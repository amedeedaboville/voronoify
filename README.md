voronoify
=========

A voronoi filter for images, available for Linux in C (needs porting for osx) or a platform independent version in python.

Fast version (in C)
=========
####Dependencies 
`sudo apt-get install libmagickwand-dev`

I've included a version of libkdtree.a which works on i386 architectures. You may need to compile your own from the source [here.](https://code.google.com/p/kdtree/downloads/detail?name=kdtree-0.5.6.tar.gz&can=2&q="Title")
###Compiling 
`make vor`

###Syntax
`$./vor -s source_name -d destination-name [-n num_cells|-r random_seed| -c min_diff]

-s source_name: name of file you want to voronoify. default is lisa.png  
-d destination_name: what to save the file as. Default is $num_cells-$source_name  
-n num_cells: number of voronoi cells. Default is 500  
-r random_seed: Optional. If unspecified, a seed will be taken from /dev/random.
-c min_diff: compress: doesn't add nodes with less than $min_diff color difference. In images with a lot of uniform space, this can save a lot of time by not filling up the tree with nodes of the same color. I think a good setting is 100.

###Note: 
Probably needs tweaking for windows, though works fine on Ubuntu 12.04 and 12.10.

Platform independent version (Python)
=======
###Dependencies 
* PIL
* Scipy

both can be acquired through pip. 

###Usage  
import:

`from import python_vor import generate_voronoi
from PIL import Image`
Then create an image object with

`source = Image.open(source_filename)`  
then call  
`generate_voronoi(source, num_cells)`  
