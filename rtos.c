#include <stdio.h>
#include <stdlib.h>

int savitzy_golay_filter(int *data,int index) {
    int approximated_value=0;
    // approximated_value=(1.0/35.0)*(-3.0*data[index-2] + 12.0*data[index-1]
                        // + 17.0*data[index] + 12.0*data[index+1] - 3.0*data[index+2]);
    approximated_value=(1.0/21.0)*(-2*data[index-3]+3*data[index-2]+
            6*data[index-1]+7*data[index]+6*data[index+1]+3*data[index+2]-2*data[index+3]);
    return approximated_value;
}

int main(int argc, char *argv[]) {
    int size_of_data=0, temp=0, i=0, j=0;
    FILE *in;
    FILE *out;
    in=fopen(argv[1],"rw");
    
    // count number of entries in a file
    
    while(!feof(in)) {
        fscanf(in,"%d\n",&temp);
        size_of_data=size_of_data+1;
    }
    fclose(in);

    // dynamic allocation of memory to hold data; 

    int * array_data=(int *)malloc(sizeof(int)*(size_of_data+6));

    // read data from file and store it in array_data
    i=3;
    in=fopen(argv[1],"rw");
    while(!feof(in)) {
        fscanf(in,"%d\n",&array_data[i]);
        // printf("%d\n",array_data[i]);
        i=i+1;
    }
    fclose(in);

    /* 
    adding data before and after actual data for calculating approximate value 
    using at corners of signal while using savitzy_golay_filter
    */

    // array_data[0]=array_data[3];
    // array_data[0]=array_data[2];
    // array_data[1]=array_data[1];
    // array_data[size_of_data]=array_data[size_of_data-2];
    // array_data[size_of_data+1]=array_data[size_of_data-3];
    // array_data[size_of_data+2]=array_data[size_of_data-4];

    array_data[0]=array_data[3];
    array_data[1]=array_data[2];
    array_data[2]=array_data[1];
    array_data[size_of_data]=array_data[size_of_data-2];
    array_data[size_of_data+1]=array_data[size_of_data-3];
    array_data[size_of_data+2]=array_data[size_of_data-4];

    // data smoothening

    int * array_data_smooth=(int *)malloc(sizeof(int)*size_of_data);

    for(j=0;j<size_of_data;j++) {
        // j+2 because actual data starts after two positions
        array_data_smooth[j]=savitzy_golay_filter(array_data,j+3);
        // printf("%d\n",array_data_smooth[j]);
    }

    // apply median filter
    
    int * array_data_smooth_median=(int *)malloc(sizeof(int)*size_of_data);

    for(j=0;j<size_of_data;j++) {
        if(j==0) {
            if(array_data_smooth[j]>array_data_smooth[j+1]) {
                array_data_smooth_median[j]=array_data_smooth[j];
            }
            else {
                array_data_smooth_median[j]=array_data_smooth[j+1];
            }
        }
        else {
            if(j==size_of_data-1) {
                if(array_data_smooth[j]>array_data_smooth[j-1]) {
                    array_data_smooth_median[j]=array_data_smooth[j];
                }
                else {
                    array_data_smooth_median[j]=array_data_smooth[j-1];
                }
            }
            else {
                temp=(array_data_smooth[j-1]>array_data_smooth[j]) ? array_data_smooth[j-1] : array_data_smooth[j];
                array_data_smooth_median[j]=(temp<array_data_smooth[j+1]) ? temp : array_data_smooth[j+1]; 
            }
        }
    }

    // apply average filter

    int * array_data_smooth_average=(int *)malloc(sizeof(int)*size_of_data);
    for(j=0;j<size_of_data;j++) {
        if(j==0) {
            array_data_smooth_average[j]=(array_data[j]+array_data_smooth[j+1])/2.0;
        }
        else {
            if(j==size_of_data-1) {
                array_data_smooth_average[j]=(array_data_smooth[j-1]+array_data_smooth[j])/2.0;
            }
            else {
                array_data_smooth_average[j]=(array_data_smooth[j-1]+array_data_smooth[j]+array_data_smooth[j+1])/3.0;
            }
        }
    }

    // smoothening moving average data

    for(j=0;j<size_of_data;j++) {
        array_data[j+3]=array_data_smooth[j];
    }

    array_data[0]=array_data[3];
    array_data[1]=array_data[2];
    array_data[2]=array_data[1];
    array_data[size_of_data]=array_data[size_of_data-2];
    array_data[size_of_data+1]=array_data[size_of_data-3];
    array_data[size_of_data+2]=array_data[size_of_data-4];

    int * array_data_smooth_average_smooth=(int *)malloc(sizeof(int)*size_of_data);

    for(j=0;j<size_of_data;j++) {
        // j+2 because actual data starts after two positions
        array_data_smooth_average_smooth[j]=savitzy_golay_filter(array_data,j+3);
        // printf("%d\n",array_data_smooth[j]);
    }

    // writing smoothed data to a file

    out=fopen("data_smooth.csv","w");
    for(j=0;j<size_of_data;j++) {
        fprintf(out,"%d\n",array_data_smooth[j]);
        // printf("%d\n",array_data_smooth[j]);
    }
    fclose(out);

    out=fopen("data_smooth_median.csv","w");
    for(j=0;j<size_of_data;j++) {
        fprintf(out,"%d\n",array_data_smooth_median[j]);
        // printf("%d\n",array_data_smooth[j]);
    }
    fclose(out);

    out=fopen("data_smooth_average.csv","w");
    for(j=0;j<size_of_data;j++) {
        fprintf(out,"%d\n",array_data_smooth_average[j]);
        // printf("%d\n",array_data_smooth[j]);
    }
    fclose(out);

    out=fopen("data_smooth_average_smooth.csv","w");
    for(j=0;j<size_of_data;j++) {
        fprintf(out,"%d\n",array_data_smooth_average_smooth[j]);
        // printf("%d\n",array_data_smooth[j]);
    }
    fclose(out);

    return 0;
}