#include <vector>
#include <mutex>
#include <algorithm>
#include <cstdint>
using namespace std;

class MagicSquaresDB {
	public:
	using Solution = std::vector<int64_t>;

	MagicSquaresDB() {}

	bool does_solution_exist(Solution solution)
	{
		// make sure the solution is sorted
		std::sort(solution.begin(), solution.end());

		lock_guard<mutex> lock(m_mutex);
		for (const auto &s : m_solutions)
		{
			if (s.size() != solution.size())
				continue;

			bool found = true;
			for (size_t i = 0; i < s.size(); i++)
			{
				if (s.at(i) != solution.at(i))
				{
					found = false;
					break;
				}
			}

			if (found)
				return true;
		}

		// if the solution is not found, add it to the list
		m_solutions.push_back(solution);

		return false;
	}

	private:
	std::vector<Solution> m_solutions;
	mutex m_mutex;
};