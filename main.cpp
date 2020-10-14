

#include <iostream>
#include <tbb/tbb.h>
#include <pstl/algorithm>



int main()
{
	tbb::parallel_invoke(
		[]() {std::cout << "Hello1" << std::endl; },
		[]() {std::cout << "Hello2" << std::endl; }
	);
	return 0;
}