from PIL import Image
import random
import math
from scipy import spatial

def generate_voronoi_diagram(source, num_cells):
    image = Image.new("RGB", source.size)
    imgx, imgy = source.size
    points = [(random.randrange(imgx),random.randrange(imgy))  for i in range(num_cells)]#Here we choose our random points
    tree = spatial.KDTree(points)#Then put them in the KD tree
    neigh_index = tree.query([(x,y) for y in range(imgy) for x in range(imgx)]) #We then query the nearest neighbour for every pixel in the image
    neigh = tree.data[neigh_index[1]]# The second value of the result the array of positions we want
    neigh_coords = map(lambda i: (int(i[0]),int(i[1])), neigh) # we have to convert them to integers before accessing 
    pixels = source.load()
    values = [pixels[i] for i in neigh_coords]
    image.putdata(values)
    return image
