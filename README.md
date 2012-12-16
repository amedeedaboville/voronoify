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
`$./vor -s source_name -d destination-name -n num_cells -r random_seed`

source_name: name of file you want to voronoify. default is lisa.png  
destination_name: what to save the file as. Default is $num_cells-$source_name  
num_cells: number of voronoi cells. Default is 500  
random_seed: Optional. If unspecified, a seed will be taken from /dev/random.
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
