#include <vector>

class QuickSort{
public:
	std::vector<int> sortFunc(std::vector<int> vec, int left, int right);
};

std::vector<int> QuickSort::sortFunc(std::vector<int> vec, int left, int right) {
	int i = left, j = right;
	int tmp;
	int pivot = vec.at( (left + right) / 2 );

	/* partition */
	while (i <= j) { 
		while (vec.at(i) < pivot)
			i++;
		while (vec.at(j) > pivot)
			j--;
		if (i <= j) {
			tmp = vec.at(i);
			vec.at(i) = vec.at(j);
			vec.at(j) = tmp;
			i++;
			j--;
		}
	}

	/* recursion */
	if (left < j)
		return sortFunc( vec, left, j );
	if (i < right)
		return sortFunc( vec, i, right );
	else
	{
		return vec;
	}
}