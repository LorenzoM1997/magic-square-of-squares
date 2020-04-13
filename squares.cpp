// Developed by Lorenzo Mambretti, 2019
//
// Code for searching candidates of 3x3 magic square of squares
// with six or more square numbers. The squares are guaranteed
// to have the same sum for each column, row, and diagonal.
#include <iostream>
#include <vector>
using namespace std;


void print_info(double increment, double first, double second){
	cout << "Increment: " << increment << endl;
	cout << "Couples: ";

	cout << "[" << sqrt((first*first) - increment) << "," << first << "]";
	cout << "[" << sqrt((second*second) - increment) << "," << second << "]";
	cout << endl;
	return;
}

int main(){

	double start, increment, square;
	vector<double> saved;
	double temp;
  short j, k, c, z;
	short triples, count;
	double distance;
	bool detected = false;
	int r;

	double trios[9];

	for (increment = 1 ;increment < 10e7; increment++){

		// numbers ending in 2,3,7,8 can't lead to a group of 3 squares
		r = (int)increment % 10;
		if (r == 7 || r == 2)  {
			increment ++;
			continue;
		}

		saved.clear();
		triples = 0;	// number of triples
		count = 0;
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
					saved.push_back(square);
				}
				else if(count == 3){
					trios[triples*3+2] = square*square;
					trios[triples*3+1] = trios[triples*3+2] - increment;
					trios[triples*3] = trios[triples*3+1] - increment;
					saved.pop_back();
					temp = square;
					triples ++; // add one triple
				}
				else {
					cout << "Four in a row!" << endl;
				}
			}
			else{
				count = 0;
				temp ++;
				square = temp;
				start = square * square;
			}
		}

		// Analysis

		int numCouples = (int)saved.size();

		if ((triples==1 && numCouples>=2)||(triples>=2)){

			if (triples == 1){

				// compute distance from triples end point
				double distances[numCouples];
				for (c = 0; c < numCouples; c ++){
						distances[c] = saved[c] * saved[c] - trios[2];
						if (distances[c] < 0) distances[c] *= (-1);
				}

				// check if any two distances is duplicate
				for (k = 1; k < numCouples; k ++){
					for (j = 0; j < k; j ++){
						distance = saved[k]*saved[k] - saved[j]*saved[j];

						if (distance == distances[k]) detected = true;
						else if(distance - increment == distances[k]) detected = true;
						else if(distance + increment == distances[k]) detected = true;

						if (distance == distances[j]) detected = true;
						else if(distance - increment == distances[j]) detected = true;
						else if(distance + increment == distances[j]) detected = true;

						if (detected) {
							// print detected elements
							print_info(increment, saved[j], saved[k]);
							cout << "Triples: ";
							for (z = 0; z < triples; z ++){
								cout << trios[z*3] << " ";
								cout << trios[z*3+1] << " ";
								cout << trios[z*3+2] << endl;
							}
							cout << "Distance: " << distance << endl << endl;
							detected = false;
						}
					}
				}
			}
			else{  // if there is more than one triple

				for (j = 1; j < triples;j++){
					for(c = 0; c< j; c ++){
						// measure the distance
						distance = trios[j*3] - trios[c*3];

						// check below if there is a square number at the same distance
						square = 1;
						if (trios[c*3] - distance > 0){
							for (k = 0; k < 3; k++){
								while(trios[c*3 + k] - distance > square * square) square ++;
								if (trios[c*3 + k] - distance == square * square){
									cout << "Increment: " << increment << endl;
									// print detected elements
									for (c = 0; c < numCouples; c++){
										cout << saved[c] << " ";
									}
									cout << endl;
									cout << "NEW (below)" << endl;
									cout << trios[c*3] << trios[j*3];
								}
							}
						}

						// check above if there is a square number at the same distance
						for (k = 0; k < 3; k++){
							while(trios[j*3 + k] + distance > square*square) square++;
							if (trios[j*3 + k] + distance == square * square){
								cout << "Increment: " << increment << endl;
								// print detected elements
								for (c = 0; c < numCouples; c++){
									cout << saved[c] << " ";
								}
								cout << endl;
								cout << "NEW (above)" << endl;
								cout << trios[c*3] << trios[j*3] << endl;
							}
						}

					}
				}



			}
		}
	}
	return 0;
}
