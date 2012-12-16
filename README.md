voronoify
=========

A voronoi filter for images

Dependencies
$sudo apt-get install libmagickwand-dev

Compiling:
$make vor

Syntax:
$./vor -s source_name -d destination-name -n num_cells -r random_seed

source_name: name of file you want to voronoify. default is lisa.png
destination_name: what to save the file as. Default is "$num_cells-$source_name"
num_cells: number of voronoi cells. Default is 500
random_seed: Optional. If unspecified, a seed will be taken from /dev/random.

Amedee d'Aboville.
