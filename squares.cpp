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
#include <algorithm>
#include <mutex>
#include <MagicSquareDB.h>
using namespace std;

int64_t assignment;
mutex mut;
MagicSquaresDB db;

namespace
{
	int64_t initialize_square_root(const int64_t increment)
	{
		return (increment % 5 == 1) ? 2 : 1;
	}

	void increase_square_root(int64_t &square_root, const int64_t increment)
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
	void increase(int64_t &n)
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
	// get next increment
	int64_t get_increment()
	{
		int64_t increment;
		mut.lock();
		increase(assignment);
		increment = assignment;
		mut.unlock();
		return increment;
	}

	bool is_perfect_square(int64_t n)
	{
		int64_t root = sqrt(n);
		return root * root == n;
	}

}

// -----------------------------------------------------------------------------
void Elaborate()
{

	int64_t increment = get_increment();

	// cout << "Elaborate: " << increment << endl;

	vector<int64_t> couples;
	int64_t distance;

	vector<array<int64_t,3>> trios;

	int count = 0;
	int64_t start = 1;									      // starting number
	int64_t squared_root = initialize_square_root(increment); // starting base
	int64_t temp = squared_root;							  // temporary variable

	while (increment > 2 * squared_root + 1)
	{
		// get square root
		squared_root = sqrt(start);

		// if start is a perfect square, count it up!
		if (start == squared_root * squared_root)
		{
			count++;
			if (count == 2)
			{
				bool found_in_trio = false;
				for (const auto &trio : trios)
				{
					for (const auto &trio_item : trio)
					{
						if (trio_item == start)
						{
							found_in_trio = true;
						}
					}
				}
				if (!found_in_trio)
				{
					couples.push_back(start);
				}
			}
			else if (count == 3)
			{
				// add the triple
				trios.push_back({start, start - increment, start - 2 * increment});
				couples.pop_back();
			}
			else if (count > 3)
			{
				cout << "Four in a row!" <<  start - 3 * increment << "," << start - 2 * increment << "," << start - increment << "," << start << endl;
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
	// cout << "Analysis: " << increment << endl;

	if ((trios.size() == 1 && couples.size() >= 2) || (trios.size() >= 2))
	{

		if (trios.size() == 1)
		{
			const auto &trio = trios.front();

			// compute distance from triples end point
			std::vector<int64_t> distances(couples.size());
			for (size_t c = 0; c < couples.size(); c++)
			{
				distances.at(c) = std::abs(couples.at(c) - trio.front());
			}

			// check if any two distances is duplicate
			for (size_t k = 1; k < couples.size(); k++)
			{

				for (size_t j = 0; j < k; j++)
				{
					distance = couples.at(k) - couples.at(j);

					bool detected = false;

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
						std::vector<int64_t> items;
						items.push_back(couples.at(j));
						items.push_back(couples.at(j) - increment);
						items.push_back(couples.at(k));
						items.push_back(couples.at(k) - increment);
						items.push_back(trio.front());
						items.push_back(trio.front() - increment);
						items.push_back(trio.front() - 2 * increment);
						std::sort(items.begin(), items.end());

						if (db.does_solution_exist(items))
						{
							continue;
						}

						// print the elements
						cout << "Elements" << endl;
						for (const auto &item : items)
						{
							cout << sqrt(item) << "^2 ";
						}
						cout << endl;
						
						cout << "Distance: " << distance << endl << endl;
					}
				}
			}
		}
		else
		{ // if there is more than one triple

			for (size_t j = 1; j < trios.size(); j++)
			{
				const auto &j_trio = trios.at(j);

				for (size_t c = 0; c < j; c++)
				{

					const auto &c_trio = trios.at(c);

					// measure the distance
					distance = j_trio.front() - c_trio.front();

					// check below if there is a square number at the same distance
					if (c_trio.front() > distance)
					{
						for (size_t k = 0; k < 3; k++)
						{
							auto possible_square = c_trio.at(k) - distance;

							if (!is_perfect_square(possible_square))
							{
								continue;
							}

							MagicSquaresDB::Solution solution;
							solution.push_back(possible_square);
							solution.push_back(c_trio.at(0));
							solution.push_back(c_trio.at(1));
							solution.push_back(c_trio.at(2));
							solution.push_back(j_trio.at(0));
							solution.push_back(j_trio.at(1));
							solution.push_back(j_trio.at(2));

							if (db.does_solution_exist(solution))
							{
								continue;
							}

							cout << "NEW (below)" << endl;
							cout << "Increment: " << increment << endl;

							std::vector<string> unknown = {"  ?  ", "  ?  ", "  ?  "};
							unknown.at(k) = to_string(static_cast<int>(sqrt(possible_square))) + "^2";

							// print a 3 x 3 grid containing the squares
							cout << "Magic square: " << endl;
							cout << unknown.at(1) << " " << sqrt(j_trio.at(2)) << "^2 " << sqrt(c_trio.at(0)) << "^2" << endl;
							cout << sqrt(j_trio.at(0)) << "^2 " << sqrt(c_trio.at(1)) << "^2 " << unknown.at(2) << endl;
							cout << sqrt(c_trio.at(2)) << "^2 " << unknown.at(0) << " " << sqrt(j_trio.at(1)) << "^2" << endl;
							cout << endl;
							
						}
					}

					// check above if there is a square number at the same distance
					for (size_t k = 0; k < 3; k++)
					{
						auto possible_square = j_trio.at(k) + distance;

						if (!is_perfect_square(possible_square))
						{
							continue;
						}

						MagicSquaresDB::Solution solution;
						solution.push_back(possible_square);
						solution.push_back(c_trio.at(0));
						solution.push_back(c_trio.at(1));
						solution.push_back(c_trio.at(2));
						solution.push_back(j_trio.at(0));
						solution.push_back(j_trio.at(1));
						solution.push_back(j_trio.at(2));

						if (db.does_solution_exist(solution))
						{
							continue;
						}

						cout << "NEW (above)" << endl;
						cout << "Increment used: " << increment << endl;

						std::vector<string> unknown = {"  ?  ", "  ?  ", "  ?  "};
						unknown.at(k) = to_string(static_cast<int>(sqrt(possible_square))) + "^2";

						// print a 3 x 3 grid containing the squares
						cout << "Magic square: " << endl;
						cout << sqrt(c_trio.at(1)) << "^2 " << unknown.at(2) << " " << sqrt(j_trio.at(0)) << "^2" << endl;
						cout << unknown.at(0) << " " << sqrt(j_trio.at(1)) << "^2 " << sqrt(c_trio.at(2)) << "^2" << endl;
						cout << sqrt(j_trio.at(2)) << "^2 " << sqrt(c_trio.at(0)) << "^2 " << unknown.at(1) << endl;
						cout << endl;
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

		// print the number of threads
		cout << "Selected number of threads: " << n_threads << endl;
	}
	else {
		// get the number of the threads as number of cores minus one
		n_threads = std::max(1, static_cast<int>(thread::hardware_concurrency()));
		n_threads = std::min(n_threads, 8);

		cout << "Using " << n_threads << " threads out of " << thread::hardware_concurrency() << " available." << endl;
	}

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
