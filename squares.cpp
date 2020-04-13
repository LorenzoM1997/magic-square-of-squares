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
  short j, k, c, len, z;
	short couples, triples, count;
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
		couples = 0;	// number of couples
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
					couples ++; // add one couple
					saved.push_back(square);
				}
				else if(count >= 3){
					trios[triples*3+2] = square*square;
					trios[triples*3+1] = trios[triples*3+2] - increment;
					trios[triples*3] = trios[triples*3+1] - increment;
					saved.pop_back();
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
		if ((triples==1 && couples>=2)||(triples>=2)){

			int numSaved = (int)saved.size();

			if (triples == 1){

				// compute distance from triples end point
				len = 0;
				double distances[triples*numSaved*2];
				for (c = 0; c < numSaved; c ++){
					for (j = 0; j < triples*3; j+=3){
						distances[len] = saved[c] * saved[c] - trios[j+2];
						distances[len + 1] = distances[len] - increment;
						if (distances[len] < 0) distances[len] *= (-1);
						if (distances[len+1]< 0) distances[len+1] *= (-1);
						len += 2;

					}
				}

				// check if any two distances is duplicate
				for (k = 1; k < numSaved; k ++){
					for (j = 0; j < k; j ++){
						distance = saved[k]*saved[k] - saved[j]*saved[j];
						for (c = 0; c < len; c ++){
							if (distance == distances[c]) detected = true;
							else if(distance - increment == distances[c]) detected = true;
							else if(distance + increment == distances[c]) detected = true;
							if (detected) {
								// print detected elements
								print_info(increment, saved[k], saved[j]);
								cout << "Triples: ";
								for (z = 0; z < triples; z ++){
									cout << trios[z*3] << " ";
									cout << trios[z*3+1] << " ";
									cout << trios[z*3+2] << endl;
								}
								cout << endl;
								cout << "Distance: " << distances[c] << endl;
								detected = false;
							}
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
									for (c = 0; c < numSaved; c++){
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
								for (c = 0; c < numSaved; c++){
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
