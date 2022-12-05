// Developed by Lorenzo Mambretti, 2019
//
// Code for searching candidates of 3x3 magic square of squares
// with six or more square numbers. The squares are guaranteed
// to have the same sum for each column, row, and diagonal.
#include <iostream>
#include <vector>
#include <cmath>
#include <cstdint>
#include <thread>
#include <mutex>
using namespace std;

int assignment;
mutex mut;

namespace
{
	uint64_t initialize_square_root(const int increment)
	{
		if (increment % 5 == 1)
		{
			return 2;
		}
		else
		{
			return 1;
		}
	}

	void increase_square_root(uint64_t &square_root, const int increment)
	{
		char d = increment % 5;
		if (d == 0)
		{
			square_root++;
		}
		else
		{
			char r = square_root % 5;
			if (d == 1)
			{

				if (r == 2)
				{
					square_root++;
				}
				else
				{
					square_root += 4;
				}
			}
			else
			{
				if (r == 1)
				{
					square_root += 3;
				}
				else
				{
					square_root += 2;
				}
			}
		}
	}

	// given a point n, get next valid point greater than n
	void increase(int &n)
	{
		// numbers ending in 2,3,7,8 can't lead to a group of 3 squares
		if (n % 5 == 1)
		{
			n += 3;
		}
		else
		{
			n += 1;
		}
	}

	// -----------------------------------------------------------------------------
	// print info
	void print_info(uint64_t increment, uint64_t first, uint64_t second)
	{
		cout << "Increment: " << increment << endl;
		cout << "Couples: ";

		cout << "[" << sqrt((first * first) - increment) << "," << first << "]";
		cout << "[" << sqrt((second * second) - increment) << "," << second << "]";
		cout << endl;
		return;
	}

	// -----------------------------------------------------------------------------
	// get next increment
	int get_increment()
	{
		int increment;
		mut.lock();
		increase(assignment);
		increment = assignment;
		mut.unlock();
		return increment;
	}

}

// -----------------------------------------------------------------------------
void Elaborate()
{

	int increment = get_increment();

	// std::cout << "Elaborate: " << increment << std::endl;

	vector<uint64_t> saved;
	short j, k, c, z;
	uint64_t distance;
	bool detected = false;

	vector<uint64_t> trios(9);

	int triples = 0; // number of triples
	int count = 0;
	uint64_t start = 1;									 // starting number
	uint64_t square = initialize_square_root(increment); // starting base
	uint64_t temp = square;								 // temporary variable

	while ((uint64_t)increment > 2 * square + 1)
	{
		// get square root
		square = sqrt(start);

		// if start is a perfect square, count it up!
		if (start == square * square)
		{
			count++;
			if (count == 2)
			{
				// check if it's already in a triple
				bool found_in_triple = false;
				for (auto &&triple : trios)
				{
					if (start == triple)
					{
						found_in_triple = true;
					}
				}

				// if it's not in the trios, then add it to the couple
				if (!found_in_triple)
				{
					saved.push_back(square);
				}
			}
			else if (count == 3)
			{
				trios.at(triples * 3 + 2) = square * square;
				trios.at(triples * 3 + 1) = trios.at(triples * 3 + 2) - increment;
				trios.at(triples * 3) = trios.at(triples * 3 + 1) - increment;
				saved.pop_back();
				triples++; // add one triple
			}
			else if (count > 3)
			{
				cout << "Four in a row!" << endl;
			}
		}
		else
		{
			// get new start
			increase_square_root(temp, increment);
			start = (temp * temp);

			// reset the counter to 1 (the current square start)
			count = 1;
		}

		start += increment;
	}

	// Analysis

	int numCouples = (int)saved.size();

	if ((triples == 1 && numCouples >= 2) || (triples >= 2))
	{

		if (triples == 1)
		{

			// compute distance from triples end point
			std::vector<uint64_t> distances(numCouples);
			for (c = 0; c < numCouples; c++)
			{
				distances.at(c) = saved.at(c) * saved.at(c) - trios[2];
				if (distances.at(c) < 0)
					distances.at(c) *= (-1);
			}

			// check if any two distances is duplicate
			for (k = 1; k < numCouples; k++)
			{
				for (j = 0; j < k; j++)
				{
					distance = saved.at(k) * saved.at(k) - saved.at(j) * saved.at(j);

					if (distance == distances.at(k))
						detected = true;
					else if (distance - increment == distances.at(k))
						detected = true;
					else if (distance + increment == distances.at(k))
						detected = true;

					if (distance == distances.at(j))
						detected = true;
					else if (distance - increment == distances.at(j))
						detected = true;
					else if (distance + increment == distances.at(j))
						detected = true;

					if (detected)
					{
						// print detected elements
						print_info(increment, saved.at(j), saved.at(k));
						cout << "Triples: ";
						for (z = 0; z < triples; z++)
						{
							cout << trios.at(z * 3) << " ";
							cout << trios.at(z * 3 + 1) << " ";
							cout << trios.at(z * 3 + 2) << endl;
						}
						cout << "Distance: " << distance << endl
							 << endl;
						detected = false;
					}
				}
			}
		}
		else
		{ // if there is more than one triple

			for (j = 1; j < triples; j++)
			{
				for (c = 0; c < j; c++)
				{
					// measure the distance
					distance = trios.at(j * 3) - trios.at(c * 3);

					// check below if there is a square number at the same distance
					square = 1;
					if (trios.at(c * 3) - distance > 0)
					{
						for (k = 0; k < 3; k++)
						{

							if (distance >= trios.at(c * 3 + k))
							{
								continue;
							}

							square = sqrt(trios.at(c * 3 + k) - distance);

							if (trios.at(c * 3 + k) == (square * square) + distance)
							{
								cout << "Increment: " << increment << endl;
								// print detected elements
								for (c = 0; c < numCouples; c++)
								{
									cout << saved.at(c) << " ";
								}
								cout << endl;
								cout << "NEW (below)" << endl;
								cout << trios.at(c * 3) << trios.at(j * 3);
							}
						}
					}

					// check above if there is a square number at the same distance
					for (k = 0; k < 3; k++)
					{
						while (trios.at(j * 3 + k) + distance > square * square)
							square++;
						if (trios[j * 3 + k] + distance == square * square)
						{
							cout << "Increment: " << increment << endl;
							// print detected elements
							for (c = 0; c < numCouples; c++)
							{
								cout << saved.at(c) << " ";
							}
							cout << endl;
							cout << "NEW (above)" << endl;
							cout << trios[c * 3] << trios[j * 3] << endl;
						}
					}
				}
			}
		}
	}
}

// main, takes an optional argument about the number of threads to use
int main(int argc, char *argv[])
{

	// the default number of threads is 8
	int n_threads = 8;

	if (argc > 1)
	{
		n_threads = atoi(argv[1]);
	}
	else {
		// get the number of the threads as number of cores minus one
		n_threads = thread::hardware_concurrency() - 1;

		// make sure that the number of threads is at least 1
		if (n_threads < 1)
			n_threads = 1;
	}

	// print the number of threads
	cout << "Number of threads: " << n_threads << endl;

	assignment = 1;
	
	// numbers ending in 2,3,7,8 can't lead to a group of 3 squares
	while (assignment < 10e7)
	{
		// initialize a vector that will contain the threads
		vector<thread> threads;

		// generate a number of threads equal to n_threads
		for (int i = 0; i < n_threads; i++)
		{
			thread th1(Elaborate);
			threads.push_back(std::move(th1));
		}

		// join all the threads
		for (int i = 0; i < n_threads; i++)
		{
			threads.at(i).join();
		}
	}

	return 0;
}
