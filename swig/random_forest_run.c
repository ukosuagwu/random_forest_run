/* fh_random_regtreefit_big_leaves_twofeaturetypes
Builds a regression tree from the input, picking features partly at random.
*/
 
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>	// for bool
#include <math.h>

#if !defined(MAX)
#define	MAX(A, B)	((A) > (B) ? (A) : (B))
#endif

#if !defined(MIN)
#define	MIN(A, B)	((A) < (B) ? (A) : (B))
#endif

/* Useful macro to swap 2 elements, using
 * the standard trick do { } while(0) to
 * make this one statement (otherwise it
 * could be wrong to use like 'else SWAP(A,B);').
 */
#define SWAP(A,B) \
do { int tmp = A; A = B; B = tmp; } while(0)

#define D_SWAP(A,B) \
do { double tmp = A; A = B; B = tmp; } while(0)

/*=== Global variable for using built-in qsort since qsort_r can't be linked here.*/
double* ptr_to_double_array_for_qsort;
int seed;
/*/
#define RAND_MAX 2147483646
int Rand() {
    long long a=22695477LL;
    int c=1;
    return seed = (a*seed+c)%(RAND_MAX+1);
}
/*/
int Rand() {
    return rand();
}
/**/

void quick(int *a, int min, int max) {
  if (max - min > 1) {

    int i = min;
    int j = max;
    /* indices are positive */
    int pivot = a[(i+j) >> 1];
    do {
      while(a[i] < pivot) i++;
      while(a[j] > pivot) j--;
      if (i > j) break;
      SWAP(a[i], a[j]);
    } while(++i <= --j);

    /* Try to reduce bad behaviours. */
    while (min < j && a[j] == pivot) j--;
    if (min < j) quick(a, min, j);

    /* Try to reduce bad behaviours. */
    while (i < max && a[i] == pivot) i++;
    if (i < max) quick(a, i, max);

  } else if (a[min] > a[max])
    SWAP(a[min], a[max]);
}


void d_quick(double *a, int min, int max) {
  if (max - min > 1) {
    int i = min;
    int j = max;
    /* indices are positive */
    double pivot = a[(i+j) >> 1];
    do {
      while(a[i] < pivot) i++;
      while(a[j] > pivot) j--;
      if (i > j) break;
      D_SWAP(a[i], a[j]);
    } while(++i <= --j);

    /* Try to reduce bad behaviours. */
    while (min < j && a[j] == pivot) j--;
    if (min < j) d_quick(a, min, j);

    /* Try to reduce bad behaviours. */
    while (i < max && a[i] == pivot) i++;
    if (i < max) d_quick(a, i, max);
  } else if (a[min] > a[max])
    D_SWAP(a[min], a[max]);
}


void dp_quick(int *a, int min, int max) {
  if (max - min > 1) {

    int i = min;
    int j = max;
    /* indices are positive */
    double pivot = ptr_to_double_array_for_qsort[a[(i+j) >> 1]];
    do {
      while(ptr_to_double_array_for_qsort[a[i]] < pivot) i++;
      while(ptr_to_double_array_for_qsort[a[j]] > pivot) j--;
      if (i > j) break;
      SWAP(a[i], a[j]);
    } while(++i <= --j);

    /* Try to reduce bad behaviours. */
    while (min < j && ptr_to_double_array_for_qsort[a[j]] == pivot) j--;
    if (min < j) dp_quick(a, min, j);

    /* Try to reduce bad behaviours. */
    while (i < max && ptr_to_double_array_for_qsort[a[i]] == pivot) i++;
    if (i < max) dp_quick(a, i, max);

  } else if (ptr_to_double_array_for_qsort[a[min]] > ptr_to_double_array_for_qsort[a[max]]) {
    SWAP(a[min], a[max]);
  }
}

/* Quick sort for sorting an int array a.
   Example: quick_sort(a, size);
// Equivalent: qsort(a, size, sizeof(int), compare_ints);
*/
void quick_sort(int *a, int size) {
  if (size > 1) {
    quick(a, 0, size-1);
  }
}

/* Quick sort for sorting a double array b.
   Example: d_quick_sort(b, size);
// Equivalent: qsort(b, size, sizeof(double), compare_doubles);
*/
void d_quick_sort(double *a, int size) {
  if (size > 1) {
    d_quick(a, 0, size-1);
  }
}

/* Quick sort for sorting the indices a of a double array b.
   Example: (int array a, double arrays b, c)
	for(i=0; i<size; i++) a[i]=i;
	ptr_to_double_array_for_qsort = b;
    dp_quick_sort(a, size);
//	Equivalent: qsort(a, size, sizeof(int), compare_idxs_into_double_array);

    If we also want the double array sorted, get it in array c:
    for(i=0; i<size; i++) c[i]=b[a[i]];*/
void dp_quick_sort(int *a, int size) {
  if (size > 1) {
    dp_quick(a, 0, size-1);
  }
}

/* For randperm in C */
/* Arrange the N elements of ARRAY in random order.
   Only effective if N is much smaller than RAND_MAX;
   if this may not be the case, use a better random
   number generator. */
void shuffle(int *array, size_t n)
{
    if (n > 1) {
        size_t i;
		for (i = 0; i < n - 1; i++) {
			size_t j = i + Rand() / (RAND_MAX / (n - i) + 1);
			int t = array[j];
			array[j] = array[i];
			array[i] = t;
		}
    }
}



// for using qsort
int compare_ints(const void * a, const void * b)
{
	const int *ia = (const int *) a;
	const int *ib = (const int *) b;
	
	return(*ia - *ib);
}

// for using qsort
int compare_doubles (const void *a, const void *b){
	const double *da = (const double *) a;
	const double *db = (const double *) b;
 
	return(*da - *db);
}

/*=== Compare c[a] against c[b], where a and b are just indices. */
int compare_idxs_into_double_array (const void *a, const void *b){
	const int *ia = (const int *) a;
	const int *ib = (const int *) b;
	
	
	if (ptr_to_double_array_for_qsort[*ia] < ptr_to_double_array_for_qsort[*ib])
		return(-1);
	if (ptr_to_double_array_for_qsort[*ia] > ptr_to_double_array_for_qsort[*ib])
		return(1);
	return(0);
}


void printMatrixDouble(const double* X,int nRows, int nCols)
{
    int i,j;
    
    for(i = 0; i < nRows; i++) {
        printf("< ");
        for(j = 0; j < nCols; j++) {
            printf("%lf ",X[i+nRows*j]);}
        printf(">\n");}
}

void printMatrixInt(const int* X,int nRows, int nCols)
{
    int i,j;
    
    for(i = 0; i < nRows; i++) {
        printf("< ");
        for(j = 0; j < nCols; j++) {
            printf("%d ",X[i+nRows*j]);}
        printf(">\n");}
}

void Rcritval_cat(const double *x, const double *Ycum, const int* rows, int nX, int nrows, double *critval_res, int *xleft, int *xright, int *numLeftPointer, int *numRightPointer){
    /* Declare Variables */
	int i, j;
    int n = nrows + 1;
    
	/*=== Allocate Memory for Auxiliary Arrays. */
    int *t = calloc(n,sizeof(int));
    int *sorder = calloc(n,sizeof(int));
    int *diff_t = calloc(n,sizeof(int));
    int *n1 = calloc(n-1,sizeof(int));
    int *maxlocs = calloc(n-1,sizeof(int));
    
    double *B = calloc(n,sizeof(double));
	double *catmeans = calloc(n,sizeof(double));
	double *Ysplit1 = calloc(n-1,sizeof(double));
	double *allx = calloc(n,sizeof(double));
	double *mu1 = calloc(n-1,sizeof(double));
	double *mu2 = calloc(n-1,sizeof(double));
	double *ssx = calloc(n-1,sizeof(double));
    
    int maxnumlocs, maxloc;

    /*=== First get all possible split points. 
    //=== t are the changepoints + the last index   Matlab: t = [rows; size(Ycum,1)]; */
    for(i=0; i<n-1; i++){
        t[i] = rows[i]-1;
    }
    t[n-1] = nX-1;
/*
	printf("t:\n");
	printMatrixInt(t, n, 1);  
	printf("\n\n");
*/

    /*=== B contains the category sums.     Matlab: B = Ycum(t,:); B(2:end,:) = B(2:end,:) - B(1:end-1,:); */
    B[0] = Ycum[t[0]];
    for(i=1; i<n; i++){
        B[i] = Ycum[t[i]] - Ycum[t[i-1]];
    }
/*
	printMatrixDouble(B, n, 1);  
	printf("\n\n");
*/

    /*=== diff_t are the number of points in a category */
    diff_t[0] = t[0]+1;
    for(i=1; i<n; i++){
        diff_t[i] = t[i]-t[i-1]; 
    }
/*
	printMatrixInt(diff_t, n, 1);  
	printf("\n\n");
*/

	/*=== catmeans contains the means for the categories. */
	for(i=0; i<n; i++){
        catmeans[i] = B[i] / MAX(1, diff_t[i]);
    }
/*
	printMatrixDouble(catmeans, n, 1);  
	printf("\n\n");
*/
	
	/*=== Sort categories by mean response. Matlab: [smeans,sorder] = sort(catmeans); */
    for(i=0; i<n; i++){
        sorder[i] = i;
    }
	ptr_to_double_array_for_qsort = catmeans;
/*
	shuffle(sorder, n);    // shuffle to speed up qsort. 
	qsort(sorder, n, sizeof(int), compare_idxs_into_double_array);
*/	
    dp_quick_sort(sorder, n);
/*	qsort(sorder, n, sizeof(int), compare_ints);
//	qsort(ptr_to_double_array_for_qsort, n, sizeof(double), compare_doubles);
//	qsort(catmeans, n, sizeof(double), compare_doubles); */
	
/*
	printMatrixInt(sorder, n, 1);  
	printf("\n\n");
	
	printMatrixDouble(catmeans, n, 1);  
	printf("\n\n");
*/	
	
    /*=== Ysplit1 is Ycum[rows sorted by mean response]
    //=== n1(i) is the number of points going left when splitting using the ith subset*/
	for(i=0; i<n-1; i++){
        for(j=0; j<=i; j++){
            Ysplit1[i] = Ysplit1[i] + B[sorder[j]];
            n1[i] = n1[i] + diff_t[sorder[j]];
        }
    }
/*    
	printMatrixDouble(Ysplit1, n-1, 1);  
	printf("\n\n");
	printMatrixInt(n1, n-1, 1);  
	printf("\n\n");
*/	
	/*=== Take one x value from each unique set */
    for(i=0; i<n; i++){
        allx[i] = x[t[i]];
    }
/*    
	printf("\nallx:\n");
	printMatrixDouble(allx, n, 1);  
	printf("\n\n");
*/
	
	/*=== Get left/right means (same for cat/cont) */
    for(i=0; i<n-1; i++){
        mu1[i] = Ysplit1[i] / n1[i];
    }
    for(i=0; i<n-1; i++){
        mu2[i] = (Ycum[nX-1] - Ysplit1[i]) / (nX - n1[i]);
    }
/*
	printf("\nmu1:\n");
	printMatrixDouble(mu1, n-1, 1);  
	printf("\n\n");
	
	printf("\nmu2:\n");
	printMatrixDouble(mu2, n-1, 1);  
	printf("\n\n");
*/
	
	/*=== Get best split, val and location. */
    maxnumlocs = 0;
    critval_res[0] = -10000000000000.0;
    for(i=0; i<n-1; i++){
        ssx[i] = n1[i]*mu1[i]*mu1[i] + (nX-n1[i])*mu2[i]*mu2[i];
        if(ssx[i] > critval_res[0]-1e-10){
        	if(ssx[i] > critval_res[0]+1e-10){
	            critval_res[0] = ssx[i];
	            maxnumlocs = 0;
	        }
	        maxlocs[maxnumlocs] = i;
        	maxnumlocs = maxnumlocs + 1;
		}
    }
/*
	printf("\nssx:\n");
	printMatrixDouble(ssx, n-1, 1);  
	printf("\n\n");

	printf("\nmaxlocs:\n");
	printMatrixInt(maxlocs, n-1, 1);  
	printf("\n\n");
*/
	maxloc = maxlocs[Rand()%maxnumlocs];
/*
	printf("%maxnumlocs = %d\n", maxnumlocs);
	printf("maxloc %d\n", maxloc);
*/

	numLeftPointer[0] = maxloc+1;
	numRightPointer[0] = n-maxloc-1;

	/* Now we can fill the result arrays xleft and xright as usual. */
	for(i=0; i<maxloc+1; i++){
	    xleft[i] = (int) allx[sorder[i]];
	}
	for(i=maxloc+1; i<n; i++){
	    xright[i-maxloc-1] = (int) allx[sorder[i]];
	}
/*
	printf("\nxleft  inside:\n");
	printMatrixInt(xleft, maxloc+1, 1);  
	printf("\n\n");

	printf("\nxright inside:\n");
	printMatrixInt(xright, n-maxloc-1, 1);  
	printf("\n\n");
*/

	/*=== Sort outputs. */
/*
	qsort(xleft, maxloc+1, sizeof(int), compare_ints);
	qsort(xright, n-maxloc-1, sizeof(int), compare_ints);
*/
    quick_sort(xleft, maxloc+1);
    quick_sort(xright, n-maxloc-1);

/*
	xleft[0] = 1;
	xleft[1] = 2;
	
	xright[0] = 3;
	xright[1] = 4;
	xright[2] = 6;
	xright[3] = 5;
*/
	/*=== Free Memory for Auxiliary Arrays. */
	free(t);
	free(sorder);
	free(diff_t);
	free(n1);
	free(maxlocs);

	free(B);
	free(catmeans);
	free(Ysplit1);
	free(allx);
	free(mu1);
	free(mu2);
	free(ssx);
}

void Rcritval_cont(const double *x, const double *Ycum, const int* rows, int nX, int nrows, double *critval_res, double *cutval_res){
	/* Declare Variables */
	int i;
	int n = nrows + 1;

	/*=== Allocate Memory for Auxiliary Arrays. */
    int *maxlocs = calloc(n-1,sizeof(int));
    
	double *Ysplit1 = calloc(n-1,sizeof(double));
	double *mu1 = calloc(n-1,sizeof(double));
	double *mu2 = calloc(n-1,sizeof(double));
	double *ssx = calloc(n-1,sizeof(double));
    
    int maxnumlocs, maxloc, cutloc;
    double u;

    /*=== Ysplit1 is Ycum(rows sorted by mean response) */
    for(i=0; i<n-1; i++){
        Ysplit1[i] = Ycum[rows[i]-1];
    }
	
	/*=== Get left/right means (same for cat/cont) */
    for(i=0; i<n-1; i++){
        mu1[i] = Ysplit1[i] / rows[i];
    }
    for(i=0; i<n-1; i++){
        mu2[i] = (Ycum[nX-1] - Ysplit1[i]) / (nX - rows[i]);
    }
/*
	printMatrixDouble(mu1, n-1, 1);  
	printf("\n\n");
	printMatrixDouble(mu2, n-1, 1);  
	printf("\n\n");
*/
	
	/*=== Get best split, val and location. */
    maxnumlocs = 0;
    critval_res[0] = -10000000000000.0;
    for(i=0; i<n-1; i++){
        ssx[i] = rows[i]*mu1[i]*mu1[i] + (nX-rows[i])*mu2[i]*mu2[i];
        if(ssx[i] > critval_res[0]-1e-10){
        	if(ssx[i] > critval_res[0]+1e-10){
	            critval_res[0] = ssx[i];
	            maxnumlocs = 0;
	        }
	        maxlocs[maxnumlocs] = i;
        	maxnumlocs = maxnumlocs + 1;
		}
    }
/*
	printf("\nssx:\n");
	printMatrixDouble(ssx, n-1, 1);  
	printf("\n\n");

	printf("\nmaxlocs:\n");
	printMatrixInt(maxlocs, n-1, 1);  
	printf("\n\n");
*/
  
	maxloc = maxlocs[Rand()%maxnumlocs];
/*	printf("%maxnumlocs = %d\n", maxnumlocs);
//	printf("maxloc %d\n", maxloc); */
	

	/*=== Get cutval. */
	cutloc = rows[maxloc]-1;
    u = Rand()/(RAND_MAX+0.0);

/*    printf("below: %lf, above: %lf\n", x[cutloc], x[cutloc+1]);   
    printf("u = %lf\n", u); */
/*	cutval_res[0] = (x[cutloc] + x[cutloc+1])/2; */
    
    /* if points are close the just take average. If points are farthur sample randomly from lerp*/
    if(x[cutloc+1] - x[cutloc] < 1.9*1e-6){
        cutval_res[0] = (x[cutloc] + x[cutloc+1])/2;
    } else {
    	cutval_res[0] = ((1-u)*(x[cutloc]+1e-6) + u*(x[cutloc+1]-1e-6));
        if( cutval_res[0] < x[cutloc]+1e-8 || cutval_res[0] > x[cutloc+1]-1e-8 ){
            printf("below: %lf, above: %lf, u: %lf, chosen: %lf\n", x[cutloc], x[cutloc+1], u, cutval_res[0]);
            //mexErrMsgTxt("random splitpoint has to lie in between the upper and lower limit");
        }
    }
    

	/*=== Free Memory for Auxiliary Arrays. */
	free(Ysplit1);
	free(mu1);
	free(mu2);
	free(ssx);
	free(maxlocs);
}



void buildTheTree(	const double* X,				// the data points
					const double* y,				// the response values
					//const int* cens,				// related to cesoring -> removed
					const int SplitMin, 			// minimum number of points in a node to split
					const double percentageFeatures,// determines the number of features used for this particular tree
					const int* iscat, 				// encodes which features are categorical
//					const mxArray* domains_cat, 
					const int N, 					// the number of data points
					const int nvars, 				// the number of features
					const double kappa, 			// (??)
					int* nodenumber,				// stores the node number
					int* parent,					// stores the parent of each node
//					mxArray* ysub,					// (??)
					//mxArray* censsub,				// encodes censoring -> removed
					int* cutvar,					// stores the index of the variable that the split uses
					double* cutpoint,				// stores the split value for continous variables
					int* leftchildren,				// stores the left child (node number or index?)
					int* rightchildren,				// same for the right child
					double* resuberr,				// (??)
					int* nodesize,					// number of data points in the sub-tree
//					mxArray* catsplit,				// stores the values for splits on categoricals
					int* numNodes,					// (??)
					int* numNcatsplit				// (??)
					) {
	
	//int p;	// unused
	int i, j, k, dims_left[2], dims_right[2], dims[2];
    
    /*=== Allocate Memory for Auxiliary Arrays to avoid doing so inside the loops*/
    int *noderows = calloc(N,sizeof(int));
    int *assignednode = calloc(N,sizeof(int));
    double *ynode = calloc(N,sizeof(double));
    int *censnode = calloc(N,sizeof(int));
    double *xvars = calloc(N,sizeof(double));
    int *idx = calloc(N,sizeof(int));
    double *x = calloc(N,sizeof(double));
    int *rows = calloc(N,sizeof(int));
    double *ycum = calloc(N,sizeof(double));
    
    int *randomPermutation = calloc(nvars,sizeof(int));
    
    int *xleft = calloc(N,sizeof(int));
    int *xright = calloc(N,sizeof(int));
    
    int *numLeftPointer = calloc(1,sizeof(int));
	int *numRightPointer = calloc(1,sizeof(int));
    int *bestleft = calloc(N,sizeof(int));
    int *bestright = calloc(N,sizeof(int));
    double *cutvalPointer = calloc(1,sizeof(double));
    double *critvalPointer = calloc(1,sizeof(double));
    
    int *leftside = calloc(N,sizeof(int));
    int *rightside = calloc(N,sizeof(int));

/*  Whole block of variables is not used in the code
    int *period = calloc(N,sizeof(int));					
    double *period_time = calloc(N+1,sizeof(double));		
    int *N_all = calloc(N,sizeof(int));
    int *C_all = calloc(N,sizeof(int));
    int *O_all = calloc(N,sizeof(int));
    
    double *ynode_idx = (double *) calloc(N,sizeof(double));
    int *censnode_idx = (int *) calloc(N,sizeof(int));
    int *period_idx = (int *)calloc(N,sizeof(int));

*/

    int ncatsplit, tnode, nextunusednode, Nnode, bestvar, numBestLeft, numBestRight;
    int nextvar, xcat, offset, numrows, nleft, nright;
    double ybar, sst, mincost, bestcrit, bestcut;
    //bool hascens;		//unused
    bool impure, ismember;

	/*
    int num_periods, curr_period;
    double last_time, this_time;
    */ 

    int currnode, parent_node, num_compatible, num_missing_to_left, num_missing_to_right;
    double catsplit_index;
    int *compatible_values, *missing_values_for_left, *missing_values_for_right, *xleftForResult, *xrightForResult, *censsub_for_result;
    double *ysub_for_result;
    mxArray *mx_to_get_compatible_values, *mx_xleft, *mx_xright, *mx_ysub;

    /*which node the given data point is assigned to*/
    for(i=0; i<N; i++){
		assignednode[i] = 0;
	}
    nodenumber[0] = 0;
    resuberr[0] = 0;
    
    /*number of categorical splits*/
    ncatsplit = 0;
    
    
	/*=== Keep processing nodes until done.*/
    /* tnode is the id of the current node indexed from 0 in a breadth first manner*/
	tnode = 0;
    /*what to number the next node that is created*/
    nextunusednode = 1;
	while(tnode < nextunusednode){ 
        /*=== Compute mean, variance and related statistics for this node.
         * Matlab: noderows = find(assignednode==tnode); 
         * ynode = y(noderows); 
         * ybar = mean(ynode);
         */
        
        /*the number of training data points inside this node*/
		Nnode = 0;
        /*the number of uncensored " " " " " "*/
        numUncensNode = 0;
        /*average of responses from training data in this node*/
        ybar = 0;
		for(i=0; i<N; i++){
	       if( assignednode[i] == tnode ){
               /*which rows of the data are in this node*/
			   noderows[Nnode] = i;
               /*responce values in this node*/
               ynode[Nnode] = y[i];
               ybar += y[i];
			   Nnode++;
	       }
		}
        /*printf("tnode: %d, Nnode is %d (must be >0)\n", tnode, Nnode);*/
        if( Nnode == 0 ){
            mexErrMsgTxt("Nnode is 0 !!!");
        }
        ybar /= Nnode;

		/*Matlab: sst = norm(ynode-ybar)^2;   % total sum of squares at this node*/
		sst = 0;
		for(i=0; i<Nnode; i++){
			sst += (ynode[i]-ybar)*(ynode[i]-ybar);
		}

        if (Nnode > 1) {
            mincost = sqrt(sst / (Nnode-1)); /* stddev of ynode */
        } else {
            mincost = 0;
        }
        impure = (mincost > 1e-10 * resuberr[0]);

		/*=== Initialize variables before looping over possible split vars*/
		nodesize[tnode]     = Nnode;
		cutvar[tnode]       = 0;
		cutpoint[tnode]     = 0;
		leftchildren[tnode] = 0;
		rightchildren[tnode]= 0;
        resuberr[tnode]     = mincost;
        /*Critical value associated with current best found split var and point*/
        bestcrit 			= -1e12;
        
        /*=== Consider splitting this node.*/
		if ( impure && nNode >= SplitMin ){ /*split only impure nodes with more than a threshold of uncensored values*/
            bestvar = -1;
			bestcut = 0;

			/*Matlab: randomPermutation = randperm(nRandom);*/
			for(i=0; i<nvars; i++)
							randomPermutation[i] = i;
			shuffle(randomPermutation, nvars);   /*take out for debugging */
            
			/*=== Try splitting each variable at least split point and pick best split*/
            for(i=0; i < nvars; i++){
				nextvar=randomPermutation[i];
				xcat = iscat[nextvar];
                			
				/*Matlab: xvars = Xnode(:,nextvar); xvars=xvars(1:Nnode);*/
                offset = nextvar*N; 
				for(j=0; j<Nnode; j++){
                    /*index into matrix: row + column*numRows*/
					xvars[j] = X[noderows[j] + offset];  
                    /*xvars[j] contain the value of nextvar of data sample[j] in the node*/
				}
			
				/*Matlab: [x,idx] = sort(xvars);*/
				ptr_to_double_array_for_qsort = xvars;
			    for(j=0; j<Nnode; j++){
			        idx[j] = j;
			    }

                dp_quick_sort(idx, Nnode);
                for(j=0; j<Nnode; j++){
			        x[j] = xvars[idx[j]];
			    }
                
                /*=== Determine if there's anything to split along this variable. */
				if (x[Nnode-1]-x[0] < 1e-10){
					continue;
				}
			
				/* Matlab: rows = find(x(1:end-1)+maxeps < x(2:end));
                 * These are the rows we can possibly split on
				 * WATCH OUT: rows holds the indices as original in Matlab, not C style (but it itself is referenced standard C style starting at 0)
                 */
				numrows = 0;
				for (j=0; j<Nnode-1; j++){
					if (x[j]+1e-10 < x[j+1]){ 
						rows[numrows++] = j+1; /*the +1 here is to make this compatible with calling Rcritval_cat & cont from Matlab directly. */
					}
				}
				if (numrows==0){
					continue;
				}

				/*=== Do the core work: get the best split of the variable and its quality.*/
				
				/*Matlab: ycum = cumsum(ynode(idx,:) - ybar);  % centered response cum. sum*/
                ycum[0] = ynode[idx[0]] - ybar;
                for (j=1; j<Nnode; j++){
                     ycum[j] = ycum[j-1] + ynode[idx[j]] - ybar; /* % centered response cum. sum */
                 }
                 if (xcat==1){
                     Rcritval_cat(x, ycum, rows, Nnode, numrows, critvalPointer, xleft, xright, numLeftPointer, numRightPointer);
                 } else {
                     Rcritval_cont(x, ycum, rows, Nnode, numrows, critvalPointer, cutvalPointer);
                }

				/*=== Change best split if this one is best so far.*/
				if (critvalPointer[0] > bestcrit + 1e-10){
					bestcrit = critvalPointer[0];
					bestvar = nextvar;
					if (xcat == 1){
						numBestLeft = numLeftPointer[0];
						numBestRight = numRightPointer[0];
						for(j=0; j<numBestLeft; j++){
							bestleft[j] = xleft[j];
						}
						for(j=0; j<numBestRight; j++){
							bestright[j] = xright[j];
						}
					} else {
						bestcut = cutvalPointer[0];
					}
				}

                if (i >= MAX(1, (int) floor(percentageFeatures*nvars)) - 1 && bestcrit > -1e11)
                    break;
			}
            
			/*=== Best split point has been found. Split this node using the best rule found.*/
            
            /* Number of data points in the children nodes*/
            nleft = 0;
			nright = 0;
            
			if (bestvar == -1){
				/* Terminal node */
        	} else {
				for (j=0; j<Nnode; j++){
					x[j] = X[noderows[j] + bestvar*N];
				}
				
    			if (iscat[bestvar]){
					cutvar[tnode] = -(bestvar+1);          /*negative indicates cat. var. split*/
					ncatsplit++;  	   /*index into catsplit cell array*/
					cutpoint[tnode] = ncatsplit;
					                    
                    /* 1: To get all compatible values, walk up the tree, looking
                     * for a split on the same parameter. If none is found
                     * take the initial domain of that parameter.
                     */
                    currnode = tnode;
                    
                    // SF: has to come first otherwise it is always executed
                    if (currnode == 0){
                        /*printf("currnode = 0\n");*/
                        mx_to_get_compatible_values = mxGetCell(domains_cat, bestvar);
                    }
                    while (currnode > 0){
                        /*printf("currnode = %d\n", currnode);*/
                        parent_node = parent[currnode];
                        /*printf("parent_node = %d, cutvar[parent_node-1]=%d\n", parent_node, cutvar[parent_node-1]);*/
                        if (cutvar[parent_node] == -(bestvar+1)){
                            /*Take values from there, depending on whether which child we are */
                            catsplit_index = cutpoint[parent_node];
                            /*printf("catsplit_index = %d\n", catsplit_index);*/

                            if (leftchildren[parent_node] == currnode){
                                mx_to_get_compatible_values = mxGetCell(catsplit, catsplit_index-1);
                            } else {
                                if (!(rightchildren[parent_node] == currnode)){
                                    mexErrMsgTxt("currnode must either be left or right child of its parent.");
                                }
                                mx_to_get_compatible_values = mxGetCell(catsplit, catsplit_index-1+N);
                            }
                            break;
                        }
                        currnode = parent_node;
                    }

                    /*Get compatible values from mx_to_get_compatible_values.*/
                    num_compatible = mxGetNumberOfElements(mx_to_get_compatible_values);
                    compatible_values = (int*) mxGetData(mx_to_get_compatible_values);
                    
                    /* 2: For each compatible but missing value choose a side u.a.r.*/
                    missing_values_for_left = mxCalloc(num_compatible,sizeof(int));
                    missing_values_for_right = mxCalloc(num_compatible,sizeof(int));
                    num_missing_to_left = 0;
                    num_missing_to_right = 0;
                    for (i=0; i<num_compatible; i++){
                        for (j=0; j<numBestLeft; j++){
                            if (compatible_values[i] == bestleft[j]) break;
                        }
                        if (j == numBestLeft){
                            for (j=0; j<numBestRight; j++){
                                if (compatible_values[i] == bestright[j]) break;
                            }
                            if (j == numBestRight){
                                /* Missing but compatible value: choose side u.a.r.*/
                                if (Rand()%2 == 0){
                                    missing_values_for_left[num_missing_to_left++] = compatible_values[i];
                                } else {
                                    missing_values_for_right[num_missing_to_right++] = compatible_values[i];
                                }
                            }
                        }
                    }
                    
                    /* 3: Merge the determined and the randomly assigned missing values*/
                    for (i=num_missing_to_left; i<num_missing_to_left+numBestLeft; i++){
                        missing_values_for_left[i] = bestleft[i-num_missing_to_left];
                    }
                    quick_sort(missing_values_for_left, num_missing_to_left+numBestLeft);

                    for (i=num_missing_to_right; i<num_missing_to_right+numBestRight; i++){
                        missing_values_for_right[i] = bestright[i-num_missing_to_right];
                    }
                    quick_sort(missing_values_for_right, num_missing_to_right+numBestRight);

                    /* 4: Put that information into the cell array.*/
                                        
                    /*=== Set up the structures to fill the cell array output. */
					dims_left[0] = 1;
					dims_left[1] = num_missing_to_left+numBestLeft; 
					
					dims_right[0] = 1;
					dims_right[1] = num_missing_to_right+numBestRight; 
					
					mx_xleft = mxCreateNumericArray(2, dims_left, mxINT32_CLASS, mxREAL);
					mx_xright = mxCreateNumericArray(2, dims_right, mxINT32_CLASS, mxREAL);
					
					mxSetCell(catsplit, ncatsplit-1, mx_xleft);
					mxSetCell(catsplit, ncatsplit-1+N, mx_xright);
			
					xleftForResult = (int*) mxGetData(mx_xleft);
					xrightForResult = (int*) mxGetData(mx_xright);

					/*=== Copy result from our temporary arrays to the ones associated with the output.*/
                    for(i=0; i<num_missing_to_left+numBestLeft; i++){
						xleftForResult[i] = missing_values_for_left[i];
					}
					for(i=0; i<num_missing_to_right+numBestRight; i++){
						xrightForResult[i] = missing_values_for_right[i];
					}
                    mxFree(missing_values_for_left);
                    mxFree(missing_values_for_right);

					for (j=0; j<Nnode; j++){
						ismember = false;
						for (k=0; k<numBestLeft; k++){
							if (((int)floor(x[j]+0.5)) == bestleft[k]) ismember = true;
						}
						
						if (ismember){
							leftside[nleft] = j;
							nleft++;
						} else {
							rightside[nright] = j;
							nright++;
						}
					}
				} else {
					/*printf("splitting on cont %d at splitpoint %lf\n", bestvar, bestcut); */
					cutvar[tnode] = bestvar+1;
					cutpoint[tnode] = bestcut;

					for (j=0; j<Nnode; j++){
						if (x[j] <= bestcut){
							leftside[nleft] = j;
							nleft = nleft+1;
						} else {
							rightside[nright] = j;
							nright = nright+1;
						}
					}
				}
				
                /*
				printf("\nleftside:\n");
				printMatrixInt(leftside, nleft, 1);  
				printf("\n\n");

				printf("\nrightside:\n");
				printMatrixInt(rightside, nright, 1);  
				printf("\n\n");
                */

                if (nleft == 0 || nright == 0) {
                    mexErrMsgTxt("Empty side after splitting!");
                }
                
				leftchildren[tnode] = nextunusednode;
				rightchildren[tnode] = nextunusednode+1;
                parent[nextunusednode] = tnode;
				parent[nextunusednode+1] = tnode;
				for (j=0; j<nleft; j++){
					assignednode[noderows[leftside[j]]] = nextunusednode;
				}
				for (j=0; j<nright; j++){
					assignednode[noderows[rightside[j]]] = nextunusednode+1;
				}
				nodenumber[nextunusednode] = nextunusednode;
				nodenumber[nextunusednode+1] = nextunusednode+1;
				nextunusednode += 2;
			}
		} 
        if (leftchildren[tnode] == 0){
            /* Leaf => store results falling here (don't store them everywhere to avoid O(N^2) storage)*/
            /*printf("Leaf\n"); */

            /*=== Set up the structures to fill the cell array output.*/
            dims[0] = 1;
            dims[1] = Nnode;
					
            mx_ysub = mxCreateNumericArray(2, dims, mxDOUBLE_CLASS, mxREAL);
            ysub_for_result = mxGetPr(mx_ysub);
            mxSetCell(ysub, tnode, mx_ysub);
            
                        
            /* Save *runtimes*, not losses. */
            for(i=0; i<Nnode; i++){
                ysub_for_result[i] = ynode[i];
    		}
       }
       tnode++;
	}

	numNodes[0] = nextunusednode;
	numNcatsplit[0] = ncatsplit;
}



//void mexFunction( int nlhs, mxArray *plhs[],
                  //int nrhs, const mxArray *prhs[] ) {
	//double *X, *y, *cutpoint, *resuberr, percentageFeatures, kappa;
	//int *cens, *iscat, *nodenumber, *parent, *cutvar, *leftchildren, *rightchildren, *nodesize, *numNodes, *numNcatsplit, *seeds_for_result;
	//int N, nvars, Splitmin;
    
    //double *tmp_double_ptr;
    //int *tmp_int_ptr, i, j, mrows, ncols, dim[1], dims[2];
    //int domains_cat_idx, ysub_idx, cat_split_idx, censsub_idx;
    
  ///* Check for proper number of arguments. */
  //if(nrhs!=9 || nlhs != 13) {
    //mexErrMsgTxt("USAGE: [nodenumber, parent, ysub, censsub, cutvar, cutpoint, leftchildren, rightchildren, resuberr, nodesize, catsplit, numNodes, ncatsplit] = fh_random_regtreefit_big_leaves_twofeaturetypes_dist(X, y, cens, Splitmin, percentageFeatures, iscat, kappa, domains_cat, seed).");
  //}
  
  ///* Check each argument for proper form and dimensions. */
  //i = 0; 
    
  //N = mxGetM(prhs[i]);
  //nvars = mxGetN(prhs[i]);
  //if( !mxIsDouble(prhs[i]) || mxIsComplex(prhs[i]) ) {
    //mexErrMsgTxt("X must be a noncomplex double matrix.");
  //}
  //X = mxGetPr(prhs[i]);

  //i++;
  //mrows = mxGetM(prhs[i]);
  //ncols = mxGetN(prhs[i]);
  //if( !mxIsDouble(prhs[i]) || mxIsComplex(prhs[i]) || !(mrows==N) || !(ncols==1) ) {
    //mexErrMsgTxt("y must be a noncomplex double column vector of the same length as size(X,1).");
  //}
  //y = mxGetPr(prhs[i]);

  //i++;
  //mrows = mxGetM(prhs[i]);
  //ncols = mxGetN(prhs[i]);
  //if( !mxIsInt32(prhs[i]) || mxIsComplex(prhs[i]) || !(mrows==N) || !(ncols==1) ) {
    //mexErrMsgTxt("cens must be a noncomplex int column vector of the same length as size(X,1).");
  //}
  //cens = (int*) mxGetData(prhs[i]);
  
  //i++;
  //mrows = mxGetM(prhs[i]);
  //ncols = mxGetN(prhs[i]);
  //if( !mxIsInt32(prhs[i]) || mxIsComplex(prhs[i]) || !(mrows==1) || !(ncols==1) ) {
    //mexErrMsgTxt("Splitmin must be a noncomplex int scalar (cast it to int!).");
  //}
  //tmp_int_ptr = (int*) mxGetPr(prhs[i]);
  //Splitmin = tmp_int_ptr[0];
  
  //i++;
  //mrows = mxGetM(prhs[i]);
  //ncols = mxGetN(prhs[i]);
  //if( !mxIsDouble(prhs[i]) || mxIsComplex(prhs[i]) || !(mrows==1) || !(ncols==1) ) {
    //mexErrMsgTxt("percentageFeatures must be a noncomplex double scalar.");
  //}
  //tmp_double_ptr = mxGetPr(prhs[i]);
  //percentageFeatures = tmp_double_ptr[0];
  
  //i++;
  //mrows = mxGetM(prhs[i]);
  //ncols = mxGetN(prhs[i]);
  //if( !mxIsInt32(prhs[i]) || mxIsComplex(prhs[i]) || !(mrows==nvars) || !(ncols==1) ) {
    //mexErrMsgTxt("iscat must be a noncomplex int column vector of the same length as size(X,2).");
  //}
  //iscat = (int*) mxGetPr(prhs[i]);
  
  //i++;
  //mrows = mxGetM(prhs[i]);
  //ncols = mxGetN(prhs[i]);
  //if( !mxIsDouble(prhs[i]) || mxIsComplex(prhs[i]) || !(mrows==1) || !(ncols==1) ) {
    //mexErrMsgTxt("kappa must be a noncomplex double scalar.");
  //}
  //tmp_double_ptr = mxGetPr(prhs[i]);
  //kappa = tmp_double_ptr[0];

  //i++;
  //mrows = mxGetM(prhs[i]);
  //ncols = mxGetN(prhs[i]);
  //if( !mxIsCell(prhs[i]) || !(ncols==1) || (!mrows==nvars) ) {
    //mexErrMsgTxt("domain_sizes_cat must be a nvars x 1 cell array (empty entries for cont. dimensions)");
  //}
  //domains_cat_idx = i;
  
  //i++;
  //mrows = mxGetM(prhs[i]);
  //ncols = mxGetN(prhs[i]);
  //if( !mxIsInt32(prhs[i]) || mxIsComplex(prhs[i]) || !(mrows==1) || !(ncols==1) ) {
    //mexErrMsgTxt("seed must be a noncomplex int scalar.");
  //}
  //tmp_int_ptr = (int*) mxGetData(prhs[i]);
  //seed = tmp_int_ptr[0];
  //srand ( seed );
  
  ///* Create vectors for return arguments and assign pointers. */
  ///* These have to be of size 2*N since the number of nodes can be that big (well, 2N-1) */
  //dim[0] = 2*N;

  //i=0;
  //plhs[i] = mxCreateNumericArray(1, dim, mxINT32_CLASS, mxREAL);
  //nodenumber = (int*) mxGetData(plhs[i]); 

  //i++;
  //plhs[i] = mxCreateNumericArray(1, dim, mxINT32_CLASS, mxREAL);
  //parent = (int*) mxGetPr(plhs[i]);

  //i++;
  //plhs[i] = mxCreateCellArray(1, dim); /*ysub*/
  //ysub_idx = i;
  
  //i++;
  //plhs[i] = mxCreateCellArray(1, dim); /*censsub*/
  //censsub_idx = i;

  //i++;
  //plhs[i] = mxCreateNumericArray(1, dim, mxINT32_CLASS, mxREAL);
  //cutvar = (int*) mxGetData(plhs[i]);

  //i++;
  //plhs[i] = mxCreateNumericArray(1, dim, mxDOUBLE_CLASS, mxREAL);
  //cutpoint = mxGetPr(plhs[i]);

  //i++;
  //plhs[i] = mxCreateNumericArray(1, dim, mxINT32_CLASS, mxREAL);
  //leftchildren = (int*) mxGetData(plhs[i]);

  //i++;
  //plhs[i] = mxCreateNumericArray(1, dim, mxINT32_CLASS, mxREAL);
  //rightchildren = (int*) mxGetData(plhs[i]);
  
  //i++;
  //plhs[i] = mxCreateNumericArray(1, dim, mxDOUBLE_CLASS, mxREAL);
  //resuberr = mxGetPr(plhs[i]);
  
  //i++;
  //plhs[i] = mxCreateNumericArray(1, dim, mxINT32_CLASS, mxREAL);
  //nodesize = (int*) mxGetData(plhs[i]);

  //i++;
  //dims[0] = N;
  //dims[1] = 2;
  //plhs[i] = mxCreateCellArray(2, dims); /* catsplit */
  //cat_split_idx = i;
  
  //i++;
  //dim[0] = 1;
  //plhs[i] = mxCreateNumericArray(1, dim, mxINT32_CLASS, mxREAL);
  //numNodes = (int*) mxGetData(plhs[i]);

  //i++;
  //plhs[i] = mxCreateNumericArray(1, dim, mxINT32_CLASS, mxREAL);
  //numNcatsplit = (int*) mxGetData(plhs[i]);
  
  //buildTheTree(X, y, cens, Splitmin, percentageFeatures, iscat, prhs[domains_cat_idx], N, nvars, kappa, nodenumber, parent, plhs[ysub_idx], plhs[censsub_idx], cutvar, cutpoint, leftchildren, rightchildren, resuberr, nodesize, plhs[cat_split_idx], numNodes, numNcatsplit);  
//}
