// Developed by Lorenzo Mambretti, 2019
//
// Code for searching candidates of 3x3 magic square of squares
// with six or more square numbers. The squares are guaranteed
// to have the same sum for each column, row, and diagonal.

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int main(){
	
	double start, increment, square;
	double saved[128];
	double temp;
	int i, j, c, len;
	short couples, triples, count;
	
	for (increment = 40000; increment < 10e6; increment++){
			
		couples = 0;	// number of couples
		triples = 0;	// number of triples
		count = 0;
		i = 0;			// index for saved numbers
		start = 1.0;	// starting number
		square = 1.0;	// starting base
		temp = square;	// temporary variable
			
		while (increment > 2*square + 1){
			// raise the square to reach the number desired
			while (start > square * square) square ++;
			if (start == square * square){
				count ++;
				start += increment;
				if (count == 2){
					couples ++; // add one couple
					saved[i] = square;
					i ++;
					temp = square;
				}
				else if(count >= 3){
					couples --; // remove one couple
					triples ++; // add one triple
					saved[i] = square;
					i ++;
					temp = square;
				}
			}
			else{
				count = 0;
				temp ++;
				square = temp;
				start = square * square;
			}		
		}
		if (couples + triples >= 3 && triples >= 1){
			printf("increment: %.0lf, couples: %d, triples: %d\n", increment, couples, triples);
			
			// compute distance
			double distance;
			double trios[triples*3];
			int trio_ix = 0; // index for trios
			short new_i = 1;
			double new_saved[i - triples];
			bool isTriple;
			
			printf("%.0lf ", saved[0]);
			new_saved[0] = saved[0];
			
			// find triples and take them out from pool
			for (c = 1; c < i; c++){
				isTriple = false;
				for (j = 0; j < c; j ++){
					distance = saved[c]*saved[c] - saved[j]*saved[j];
					if (distance == increment){
						trios[trio_ix+1] = saved[j]*saved[j];
						trios[trio_ix] = trios[trio_ix+1] - increment;
						trios[trio_ix+2] = trios[trio_ix+1] + increment;
						trio_ix += 3;
						isTriple = true;
					}
				}
				printf("%.0lf ", saved[c]);
				if (!isTriple){
					new_saved[new_i] = saved[c];
					new_i ++;
				}
				else{
					new_i --; 
				}
			}
			
			// compute distance from triples end point
			len = 0;
			double distances[triples*i*2];
			for (c = 0; c < new_i; c ++){
				for (j = 0; j < trio_ix; j+=3){
					distances[len] = new_saved[c] * new_saved[c] - trios[j+2];
					distances[len + 1] = distances[len] - increment;
					if (distances[len] < 0) distances[len] *= (-1);
					if (distances[len+1]< 0) distances[len+1] *= (-1);
					len += 2;
					
				}
			}
			
			// check if any two distances is duplicate
			for (i = 1; i < new_i; i ++){
				for (j = 0; j < i; j ++){
					distance = new_saved[i]*new_saved[i] - new_saved[j]*new_saved[j];
					for (c = 0; c < len; c ++){
						if (distance == distances[c]){
							printf("%.0lf ***\n", distances[c]);
						}
						else if(distance - increment == distances[c]){
							printf("%.0lf ***\n", distance - increment);
						}
					}
				}
			}
			printf("\n");
		}
	}
	return 0;
}
