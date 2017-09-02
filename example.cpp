/**
 * Copyright (c) 2017 Markus Leitz
 * Licensed under MIT License
 *  
 */


#include "plot_gnuplot.h"

#include <random>
#include <thread>

using namespace plotcpp;

int main()
{


	std::vector<int> data(69, 0);
	 
	static std::random_device rnd_device;
	 
	static std::mt19937 mersenne_engine(rnd_device());
	std::uniform_int_distribution<int> dist(-12, 5);



	plotcpp::plot_gnuplot plt;
	size_t index = 0;

	plt << "set title " + gpStr("Test plotcpp");
	plt << "set xlabel " + gpStr("Time [ms]") << "set ylabel " + gpStr("Dist");


	for (int i=0; /*i<4*/ ;++i)
	{

		std::generate(begin(data), end(data), [&] {  return dist(mersenne_engine); });


		plt.plot2d(begin(data), end(data), [=](const auto element) mutable
		{
	 		return std::make_tuple(++index, element); 
		}
		);
	
	
		auto s = std::accumulate(begin(data), end(data), 0.0) / data.size();
		plt.plot2d(begin(data), end(data), [=](const auto element) mutable
		{
			 return std::make_tuple(++index, s);
		}
		);

		std::this_thread::sleep_for(std::chrono::milliseconds(499));

		plt.reset();
	

	}

	return 0;
}
