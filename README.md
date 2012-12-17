voronoify
=========

A voronoi filter for images

Fast version (in C)
=========
####Dependencies 
`$sudo apt-get install libmagickwand-dev`

###Compiling 
`$make vor`

###Syntax
`$./vor -s source_name -d destination-name [-n num_cells|-r random_seed| -c min_diff]

-s source_name: name of file you want to voronoify. default is lisa.png  
-d destination_name: what to save the file as. Default is $num_cells-$source_name  
-n num_cells: number of voronoi cells. Default is 500  
-r random_seed: Optional. If unspecified, a seed will be taken from /dev/random.
-c min_diff: compress: doesn't add nodes with less than $min_diff color difference. 
  In images with a lot of uniform space, this can save a lot of time by not filling up the tree with nodes of the same color.

###Note: 
Probably needs tweaking for windows, though works fine on Ubuntu 12.04 and 12.10.
Python version
=======
###Dependencies 
* PIL
* Scipy
both can be acquired through pip. 
###Usage  
create an image object with  
`source = Image.open(source_filename)`  
then call  
`generate_voronoi_diagram(source, num_cells)`  
