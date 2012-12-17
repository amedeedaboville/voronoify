voronoify: vor.c
	gcc -g -o vor vor.c `MagickWand-config --cflags --cppflags` `MagickWand-config --ldflags --libs` libkdtree.a
