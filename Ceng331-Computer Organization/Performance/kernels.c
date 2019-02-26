/********************************************************
 * Kernels to be optimized for the CS:APP Performance Lab
 ********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "defs.h"
/* 
 * Please fill in the following student struct 
 */
team_t team = {
    "e2171395",              /* Student ID */
    "Beste Burhan",     /* full name */
    "e2171395@ceng.metu.edu.tr",  /* email address */
    "",                   /* leave blank */
    ""                    /* leave blank */
};


/***************
 * CONVOLUTION KERNEL
 ***************/

/******************************************************
 * Your different versions of the convolution functions  go here
 ******************************************************/

/* 
 * naive_conv - The naive baseline version of convolution 
 */
char naive_conv_descr[] = "naive_conv: Naive baseline implementation";
void naive_conv(int dim,int *src, int *ker,int *dst) {
    int i,j,k,l;

    for(i = 0; i < dim-8+1; i++)
        for(j = 0; j < dim-8+1; j++) {
            dst[j*dim+i] = 0;
            for(k = 0; k < 8; k++)
                for(l = 0; l < 8; l++) {
                    dst[j*dim+i] = dst[j*dim+i] +src[(j+l)*dim+(i+k)]*ker[l*dim+k];
                }
        }
            
        
}

/* 
 * convolution - Your current working version of convolution
 * IMPORTANT: This is the version you will be graded on
 */
char convolution_descr[] = "Dot product: Current working version";
void convolution(int dim,int *src, int *ker,int *dst) 
{
	int i,j,k,l,indx1,indx2;
	int *tmp1,*tmp2;
	int acc;
    for(j = 0; j < dim-7; j++)
        for(i = 0; i < dim-7; i++) {
            acc=0;
            indx1=j*dim+i;
            indx2=0;
            	tmp1=&src[indx1];
            	tmp2=&ker[indx2];
                    acc +=(*tmp1) *(*tmp2);
                    tmp1+=dim;tmp2+=dim;
                    acc +=(*tmp1) *(*tmp2);
                    tmp1+=dim;tmp2+=dim;
                    acc +=(*tmp1) *(*tmp2);
                    tmp1+=dim;tmp2+=dim;
                    acc +=(*tmp1) *(*tmp2);
                    tmp1+=dim;tmp2+=dim;
                    acc +=(*tmp1) *(*tmp2);
                    tmp1+=dim;tmp2+=dim;
                    acc +=(*tmp1) *(*tmp2);
                    tmp1+=dim;tmp2+=dim;
                    acc +=(*tmp1) *(*tmp2);
                    tmp1+=dim;tmp2+=dim;
                    acc +=(*tmp1) *(*tmp2); 
                tmp1=&src[indx1+1];
            	tmp2=&ker[indx2+1];
                    acc +=(*tmp1) *(*tmp2);
                    tmp1+=dim;tmp2+=dim;
                    acc +=(*tmp1) *(*tmp2);
                    tmp1+=dim;tmp2+=dim;
                    acc +=(*tmp1) *(*tmp2);
                    tmp1+=dim;tmp2+=dim;
                    acc +=(*tmp1) *(*tmp2);
                    tmp1+=dim;tmp2+=dim;
                    acc +=(*tmp1) *(*tmp2);
                    tmp1+=dim;tmp2+=dim;
                    acc +=(*tmp1) *(*tmp2);
                    tmp1+=dim;tmp2+=dim;
                    acc +=(*tmp1) *(*tmp2);
                    tmp1+=dim;tmp2+=dim;
                    acc +=(*tmp1) *(*tmp2);
                tmp1=&src[indx1+2];
            	tmp2=&ker[indx2+2];
                    acc +=(*tmp1) *(*tmp2);
                    tmp1+=dim;tmp2+=dim;
                    acc +=(*tmp1) *(*tmp2);
                    tmp1+=dim;tmp2+=dim;
                    acc +=(*tmp1) *(*tmp2);
                    tmp1+=dim;tmp2+=dim;
                    acc +=(*tmp1) *(*tmp2);
                    tmp1+=dim;tmp2+=dim;
                    acc +=(*tmp1) *(*tmp2);
                    tmp1+=dim;tmp2+=dim;
                    acc+=(*tmp1) *(*tmp2);
                    tmp1+=dim;tmp2+=dim;
                    acc +=(*tmp1) *(*tmp2);
                    tmp1+=dim;tmp2+=dim;
                    acc +=(*tmp1) *(*tmp2);
                tmp1=&src[indx1+3];
            	tmp2=&ker[indx2+3];
                    acc +=(*tmp1) *(*tmp2);
                    tmp1+=dim;tmp2+=dim;
                    acc +=(*tmp1) *(*tmp2);
                    tmp1+=dim;tmp2+=dim;
                    acc +=(*tmp1) *(*tmp2);
                    tmp1+=dim;tmp2+=dim;
                    acc +=(*tmp1) *(*tmp2);
                    tmp1+=dim;tmp2+=dim;
                    acc +=(*tmp1) *(*tmp2);
                    tmp1+=dim;tmp2+=dim;
                    acc +=(*tmp1) *(*tmp2);
                    tmp1+=dim;tmp2+=dim;
                    acc +=(*tmp1) *(*tmp2);
                    tmp1+=dim;tmp2+=dim;
                    acc +=(*tmp1) *(*tmp2);
                tmp1=&src[indx1+4];
            	tmp2=&ker[indx2+4];
                    acc +=(*tmp1) *(*tmp2);
                    tmp1+=dim;tmp2+=dim;
                    acc +=(*tmp1) *(*tmp2);
                    tmp1+=dim;tmp2+=dim;
                    acc +=(*tmp1) *(*tmp2);
                    tmp1+=dim;tmp2+=dim;
                    acc +=(*tmp1) *(*tmp2);
                    tmp1+=dim;tmp2+=dim;
                    acc +=(*tmp1) *(*tmp2);
                    tmp1+=dim;tmp2+=dim;
                    acc +=(*tmp1) *(*tmp2);
                    tmp1+=dim;tmp2+=dim;
                    acc+=(*tmp1) *(*tmp2);
                    tmp1+=dim;tmp2+=dim;
                    acc +=(*tmp1) *(*tmp2);
                tmp1=&src[indx1+5];
            	tmp2=&ker[indx2+5];
                    acc +=(*tmp1) *(*tmp2);
                    tmp1+=dim;tmp2+=dim;
                    acc +=(*tmp1) *(*tmp2);
                    tmp1+=dim;tmp2+=dim;
                    acc +=(*tmp1) *(*tmp2);
                    tmp1+=dim;tmp2+=dim;
                    acc +=(*tmp1) *(*tmp2);
                    tmp1+=dim;tmp2+=dim;
                    acc +=(*tmp1) *(*tmp2);
                    tmp1+=dim;tmp2+=dim;
                    acc +=(*tmp1) *(*tmp2);
                    tmp1+=dim;tmp2+=dim;
                    acc +=(*tmp1) *(*tmp2);
                    tmp1+=dim;tmp2+=dim;
                    acc +=(*tmp1) *(*tmp2);
                tmp1=&src[indx1+6];
            	tmp2=&ker[indx2+6];
                    acc +=(*tmp1) *(*tmp2);
                    tmp1+=dim;tmp2+=dim;
                    acc +=(*tmp1) *(*tmp2);
                    tmp1+=dim;tmp2+=dim;
                    acc +=(*tmp1) *(*tmp2);
                    tmp1+=dim;tmp2+=dim;
                    acc +=(*tmp1) *(*tmp2);
                    tmp1+=dim;tmp2+=dim;
                    acc +=(*tmp1) *(*tmp2);
                    tmp1+=dim;tmp2+=dim;
                    acc +=(*tmp1) *(*tmp2);
                    tmp1+=dim;tmp2+=dim;
                    acc +=(*tmp1) *(*tmp2);
                    tmp1+=dim;tmp2+=dim;
                    acc +=(*tmp1) *(*tmp2);
                tmp1=&src[indx1+7];
            	tmp2=&ker[indx2+7];
                    acc +=(*tmp1) *(*tmp2);
                    tmp1+=dim;tmp2+=dim;
                    acc +=(*tmp1) *(*tmp2);
                    tmp1+=dim;tmp2+=dim;
                    acc+=(*tmp1) *(*tmp2);
                    tmp1+=dim;tmp2+=dim;
                    acc+=(*tmp1) *(*tmp2);
                    tmp1+=dim;tmp2+=dim;
                    acc +=(*tmp1) *(*tmp2);
                    tmp1+=dim;tmp2+=dim;
                    acc +=(*tmp1) *(*tmp2);
                    tmp1+=dim;tmp2+=dim;
                    acc +=(*tmp1) *(*tmp2);
                    tmp1+=dim;tmp2+=dim;
                    acc +=(*tmp1) *(*tmp2);
                dst[j*dim+i] = acc; 
                
            }
}

/*********************************************************************
 * register_conv_functions - Register all of your different versions
 *     of the convolution functions  with the driver by calling the
 *     add_conv_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_conv_functions() {
    add_conv_function(&naive_conv, naive_conv_descr);   
    add_conv_function(&convolution, convolution_descr);   
    /* ... Register additional test functions here */
}




/***************
 * MATRIX MULTIP KERNEL
 ***************/

/******************************************************
 * Your different versions of the matrix multiplications  go here
 ******************************************************/

/* 
 * naive_matrix_multiplication - The naive baseline version of matrix multiplication 
 */
char naive_matrix_multiplication_descr[] = "Naive_matrix_multiplication: Naive baseline implementation";
void naive_matrix_multiplication(int dim,int *src, int *src2,int *dst) {
    int i,j,k;

    for(j = 0; j < dim; j++)
        for(i = 0; i < dim; i++) {
            dst[j*dim+i]=0;
            for(k = 0; k < dim; k++) 
                dst[j*dim+i] = dst[j*dim+i] + 
            	 	src[j*dim+k]*src2[i+k*dim]  ;        
            	 }    
}



/* 
 * matrix_multiplication - Your current working version of matrix_multiplication
 * IMPORTANT: This is the version you will be graded on
 */
char matrix_multiplication_descr[] = "Matrix multiplications: Current working version";
void matrix_multiplication(int dim,int *src, int *src2,int *dst) 
{
	
	int i,j,k,i1,i1xdim,kxdim,kxdim_d1,kxdim_d2,kxdim_d3,kxdim_d4,kxdim_d5,kxdim_d6,kxdim_d7;
	int *temp;
	for(i=0;i<dim*dim;i++) dst[i]=0;
	
	for(i = 0; i < dim; i+=8) {
        for(j = 0; j < dim; j+=8) {
            for(k = 0; k < dim; k+=8) {
		    kxdim=k*dim+j;
		    kxdim_d1=kxdim+dim;kxdim_d2=kxdim_d1+dim;kxdim_d3=kxdim_d2+dim;
		    kxdim_d4=kxdim_d3+dim;kxdim_d5=kxdim_d4+dim;kxdim_d6=kxdim_d5+dim;
		    kxdim_d7=kxdim_d6+dim;
            	for(i1=i,i1xdim=i*dim; i1 < i+8; i1++,i1xdim+=dim){
            			temp = &src[i1xdim+k];
            			dst[i1xdim+j] += ((*temp++)*src2[kxdim]
            						 + (*temp++)*src2[kxdim_d1]
            						 + (*temp++)*src2[kxdim_d2]
            						+ (*temp++)*src2[kxdim_d3]
            						+ (*temp++)*src2[kxdim_d4]
            						+ (*temp++)*src2[kxdim_d5]
            						 + (*temp++)*src2[kxdim_d6]
            						 + (*temp)*src2[kxdim_d7]);
						temp = &src[i1xdim+k];
            			dst[i1xdim+j+1] += ((*temp++)*src2[kxdim+1]
            						 + (*temp++)*src2[kxdim_d1+1]
            						 + (*temp++)*src2[kxdim_d2+1]
            						+ (*temp++)*src2[kxdim_d3+1]
            						+ (*temp++)*src2[kxdim_d4+1]
            						+ (*temp++)*src2[kxdim_d5+1]
            						 + (*temp++)*src2[kxdim_d6+1]
            						 + (*temp)*src2[kxdim_d7+1]);
            			temp = &src[i1xdim+k];
            			dst[i1xdim+j+2] += ((*temp++)*src2[kxdim+2]
            						 + (*temp++)*src2[kxdim_d1+2]
            						 + (*temp++)*src2[kxdim_d2+2]
            						+ (*temp++)*src2[kxdim_d3+2]
            						+ (*temp++)*src2[kxdim_d4+2]
            						+ (*temp++)*src2[kxdim_d5+2]
            						 + (*temp++)*src2[kxdim_d6+2]
            						 + (*temp)*src2[kxdim_d7+2]);
            			temp = &src[i1xdim+k];
            			dst[i1xdim+j+3] += ((*temp++)*src2[kxdim+3]
            						 + (*temp++)*src2[kxdim_d1+3]
            						 + (*temp++)*src2[kxdim_d2+3]
            						+ (*temp++)*src2[kxdim_d3+3]
            						+ (*temp++)*src2[kxdim_d4+3]
            						+ (*temp++)*src2[kxdim_d5+3]
            						 + (*temp++)*src2[kxdim_d6+3]
            						 + (*temp)*src2[kxdim_d7+3]);
            			temp = &src[i1xdim+k];
            			dst[i1xdim+j+4] += ((*temp++)*src2[kxdim+4]
            						 + (*temp++)*src2[kxdim_d1+4]
            						 + (*temp++)*src2[kxdim_d2+4]
            						+ (*temp++)*src2[kxdim_d3+4]
            						+ (*temp++)*src2[kxdim_d4+4]
            						+ (*temp++)*src2[kxdim_d5+4]
            						 + (*temp++)*src2[kxdim_d6+4]
            						 + (*temp)*src2[kxdim_d7+4]);
            			temp = &src[i1xdim+k];
            			dst[i1xdim+j+5] += ((*temp++)*src2[kxdim+5]
            						 + (*temp++)*src2[kxdim_d1+5]
            						 + (*temp++)*src2[kxdim_d2+5]
            						+ (*temp++)*src2[kxdim_d3+5]
            						+ (*temp++)*src2[kxdim_d4+5]
            						+ (*temp++)*src2[kxdim_d5+5]
            						 + (*temp++)*src2[kxdim_d6+5]
            						 + (*temp)*src2[kxdim_d7+5]);
            			temp = &src[i1xdim+k];
            			dst[i1xdim+j+6] += ((*temp++)*src2[kxdim+6]
            						 + (*temp++)*src2[kxdim_d1+6]
            						 + (*temp++)*src2[kxdim_d2+6]
            						+ (*temp++)*src2[kxdim_d3+6]
            						+ (*temp++)*src2[kxdim_d4+6]
            						+ (*temp++)*src2[kxdim_d5+6]
            						 + (*temp++)*src2[kxdim_d6+6]
            						 + (*temp)*src2[kxdim_d7+6]);
            			temp = &src[i1xdim+k];
            			dst[i1xdim+j+7] += ((*temp++)*src2[kxdim+7]
            						 + (*temp++)*src2[kxdim_d1+7]
            						 + (*temp++)*src2[kxdim_d2+7]
            						+ (*temp++)*src2[kxdim_d3+7]
            						+ (*temp++)*src2[kxdim_d4+7]
            						+ (*temp++)*src2[kxdim_d5+7]
            						 + (*temp++)*src2[kxdim_d6+7]
            						 + (*temp)*src2[kxdim_d7+7]);
            		
            	}
            }
            
        }
    }
	

}

/*********************************************************************
 * register_matrix_multiplication_functions - Register all of your different versions
 *     of the matrix multiplication  with the driver by calling the
 *     add_matrix_multiplication_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 *********************************************************************/

void register_matrix_multiplication_functions() {
    add_matrix_multiplication_function(&naive_matrix_multiplication, naive_matrix_multiplication_descr);   
    add_matrix_multiplication_function(&matrix_multiplication, matrix_multiplication_descr);   
    /* ... Register additional test functions here */
}

