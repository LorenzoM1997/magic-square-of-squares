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
  short i, j, k, c, len;
	short couples, triples, count;
	double distance;
	int r;

	double trios[9];

	for (increment = 40000 ;increment < 10e7; increment++){

		// numbers ending in 2,3,7,8 can't lead to a group of 3 squares
		r = (int)increment % 10;
		if (r == 7 || r == 2)  {
			increment ++;
			continue;
		}

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
					trios[triples+2] = square*square;
					trios[triples+1] = trios[triples+2] - increment;
					trios[triples] = trios[triples+1] - increment;
					i --;
					temp = square;
					couples --; // remove one couple
					triples ++; // add one triple
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

			// find triples and take them out from pool
			for (c = 0; c < i; c++){
				printf("%.0lf ", saved[c]);
			}

			// compute distance from triples end point
			len = 0;
			double distances[triples*i*2];
			for (c = 0; c < i; c ++){
				for (j = 0; j < triples*3; j+=3){
					distances[len] = saved[c] * saved[c] - trios[j+2];
					distances[len + 1] = distances[len] - increment;
					if (distances[len] < 0) distances[len] *= (-1);
					if (distances[len+1]< 0) distances[len+1] *= (-1);
					len += 2;

				}
			}

			// check if any two distances is duplicate
			for (k = 1; k < i; k ++){
				for (j = 0; j < k; j ++){
					distance = saved[k]*saved[k] - saved[j]*saved[j];
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
