// for DEBUG
#include <bits/stdc++.h>
#define Debug(func) \
	clock_t start, end; \
	start = clock(); \
	func; \
	end = clock(); \
	double time_taken = double(end - start) / double(CLOCKS_PER_SEC) * 1000; \
	std::cout << "Time taken by func is : " << std::fixed << time_taken << std::setprecision(5) << " ms " << std::endl;
