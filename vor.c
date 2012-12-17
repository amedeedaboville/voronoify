#include <stdlib.h>
#include <stdio.h>  //file io
#include <unistd.h> //for getopt(), which makes parameters easy
#include <string.h> //filename manipulation
#include <math.h>   //random generator
#include "kdtree.h" //the basis of our program
#include <ImageMagick/wand/MagickWand.h> //egh


//This is a super-uniform function, much better than (rand() % max) +min
int random_in_range (unsigned int min, unsigned int max)
{
    int base_random = rand(); /* in [0, RAND_MAX] */
    if (RAND_MAX == base_random) return random_in_range(min, max);          /* now guaranteed to be in [0, RAND_MAX) */
    unsigned int range   = max - min,
                 remainder= RAND_MAX % range, // These are range buckets, plus one smaller interval
                 bucket   = RAND_MAX / range; // within remainder of RAND_MAX */
    if (base_random < RAND_MAX - remainder) 
        return min + base_random/bucket;
    else 
        return random_in_range (min, max);
}
int pixel_compare_NN(double epsilon,void* source, void *kd,  double* pixel_position, PixelWand* color, PixelWand* neigh_color)
{
    struct kdres *neigh;
    double neigh_position[2]= {0,0};

    neigh = kd_nearest(kd,pixel_position);
    if(neigh == NULL)
        return 1;
    kd_res_item(neigh, neigh_position);
    kd_res_free(neigh);//need to free the memory used for the query
    MagickGetImagePixelColor(source,neigh_position[0],neigh_position[1],neigh_color); 
    MagickGetImagePixelColor(source,pixel_position[0],pixel_position[1],color); 
    double color_diff =0;
    color_diff += pow(PixelGetRed(color) - PixelGetRed(neigh_color),2);
    color_diff += pow(PixelGetGreen(color) - PixelGetGreen(neigh_color),2);
    color_diff += pow(PixelGetBlue(color) - PixelGetBlue(neigh_color),2);
    if(color_diff < epsilon)
        return 0;
    return 1;

}
int main(int argc,char **argv) {
    unsigned int width, height,seed;
    double pos[2] = {0,0},target_pos[2], diff[3],min_col_diff; 
    struct kdres *neigh; void *kd; //kd-tree
    int i,x,y,c,num_cells=500,random;
    size_t row_size;//imagemagick wants this
    char source_filename[50] = "lisa.png";
    char dest_filename[59];//I set to 59 so user could have 49 chars + 10 in the default case of NNNNNN-sourcefilename
    int oflag=0,sflag=0,vflag=0,cflag=0;

    //imagemagick stuff
    MagickWand *source,*dest;
    PixelWand *neigh_color,*color,**pmw;
    PixelIterator *imw;

    while((c = getopt(argc,argv,"vc:n:s:r:d:")) != -1) {
        switch (c) {
            case 'v':
                vflag=1;
                break;
            case 'n':
                num_cells=atoi(optarg);
                break;
            case 's':
                strcpy(source_filename,optarg);
                break; 
            case 'd':
                oflag=1;
                strcpy(dest_filename,optarg);
                break;  
            case 'r':
                if((seed=atoi(optarg))!=0)
                    sflag=1;
                break;
            case 'c':
                cflag=1;
                min_col_diff = atof(optarg);
                break;
            default:
                printf("Option %s not recognized and ignored.\n",c);
        }
    }

    if(!oflag) sprintf(dest_filename,"%d-%s",num_cells,source_filename);

    MagickWandGenesis();
    source=NewMagickWand();
    dest = NewMagickWand();

    color = NewPixelWand();
    neigh_color = NewPixelWand();
    pmw = NewPixelWand();
    MagickReadImage(source,source_filename);
    if (source==MagickFalse)  {
        printf("Error reading file. Usage: vor filename\n");
        return 1;
    }

    width = MagickGetImageWidth(source);
    height = MagickGetImageHeight(source);
    printf("File has width %d and height %d\n", width, height);
    if(!sflag) { //seed the algorithm with /dev/random if a seed wasn't specified
        random = open("/dev/random", 'r');
        read(random, &seed, sizeof (seed));
        close(random);
    }
    if(vflag) printf("seed : %d\n",seed);
    srand(seed);

    kd = kd_create(2);
    if(cflag)
    {
        for(i = 0; i < num_cells; i++) {
            pos[0]= (double)random_in_range(0,width);
            pos[1]= (double)random_in_range(0,height);
            if(pixel_compare_NN(min_col_diff,source,kd,pos,color,neigh_color)) 
                kd_insert(kd,pos,0);
        }
    }
    else {
        for(i = 0; i < num_cells; i++) {
            pos[0]= (double)random_in_range(0,width);
            pos[1]= (double)random_in_range(0,height);
            kd_insert(kd,pos,0);
        }
    }
    MagickSetSize(dest,width,height);
    MagickReadImage(dest,"xc:none");
    imw = NewPixelIterator(dest);

    for (y=0; y < height; y++) {
        pos[1] = y;
        pmw = PixelGetNextIteratorRow(imw, &row_size); //we iterate through the rows, grabbing one at a time 
        for (x=0; x < (long) width; x++) {
            pos[0] =x;
            neigh = kd_nearest(kd,pos);//this is the query
            kd_res_item(neigh, target_pos);//then we pull out the result into target_pos
            kd_res_free(neigh);//need to free the memory used for the query

            MagickGetImagePixelColor(source,target_pos[0],target_pos[1],color);
            PixelSetColorFromWand(pmw[x],color);
        }
        PixelSyncIterator(imw);//this will write to the image (MagickWand)
    }
    if(vflag)printf("Writing to file %s.\n",dest_filename);
    if(MagickWriteImage(dest,dest_filename)==MagickFalse)
    {
        printf("Error writing to file %s.\n",dest_filename);
    }
    source=DestroyMagickWand(source);
    dest=DestroyMagickWand(dest);
    MagickWandTerminus();
    kd_free(kd);
    return 0;
}
