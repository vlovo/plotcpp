/**
 * Copyright (c) 2017 Markus Leitz
 * Licensed under MIT License
 *  
 */

#pragma once
#include <iostream>
#include <tuple>
#include <functional>
#include <numeric>
#include <algorithm>
#include <vector>
#include <sstream>
#include <fstream>

#include "plot_types.h"
#include "plot_defines.h"
 
namespace plotcxx
{
	
	std::string gpStr(const std::string &in)
	{
		 

		return("\'" + in + "\'");
	}


	class plot_gnuplot
	{
	public:
	

		template< class InputIt, class Extractor>
		void plot2d(const InputIt first, const InputIt last, Extractor extract)
		{
			typedef  typename  std::iterator_traits<InputIt>::value_type value_type;

			Point2DContainerType dataPoints;

			std::transform(first, last, std::back_inserter(dataPoints), [&](value_type item)
			{
				return extract(item);
			});


			std::ofstream file;
			std::string tmpname = generateFilename();
			file.open(tmpname);

			std::for_each(begin(dataPoints), end(dataPoints), [&](Point2DType item) {

				file << std::get<0>(item) << " " << std::get<1>(item) << "\n";

			});

			flush(file);

			std::string style = "linespoint";
			buffer << gpStr(tmpname) << " using 1:2 with "<<style<<", ";
			executeCmd("plot " + buffer.str());


			return;
		};


		template< class InputIt, class Extractor>
		void plot3d(const InputIt first, const InputIt last, Extractor extract)
		{
			typedef  typename  std::iterator_traits<InputIt>::value_type value_type;

			Point3DContainerType dataPoints;

			std::transform(first, last, std::back_inserter(dataPoints), [&](value_type item)
			{
				return extract(item);
			});



			std::ofstream file;
			std::string tmpname = generateFilename();
			file.open(tmpname);

			std::for_each(begin(dataPoints), end(dataPoints), [&](Point3DType item)
			{

				file << std::get<0>(item) << " " << std::get<1>(item) << " " << std::get<2>(item) << "\n";

			});

			flush(file);


			buffer << gpStr(tmpname) << " using 1:2:3 with linespoint, ";
			executeCmd("splot " + buffer.str());


			return;
		};


		inline plot_gnuplot& operator<<(const std::string &cmdstr)
		{
			executeCmd(cmdstr);
			executeCmd("replot");
			return(*this);
		}

		plot_gnuplot()
		{
			open();
		}
		~plot_gnuplot()
		{
			close();
		}

		void reset()
		{
			buffer.str(std::string(""));
			
			std::for_each(begin(listOfFilename),end(listOfFilename),[](std::string item)
			{ 
				std::remove(item.c_str());
			});
			listOfFilename.clear();
		}

		  
	private:

		std::stringstream buffer;
		FILE    *gnucmd;
		std::vector<std::string> listOfFilename;


		std::string generateFilename()
		{
			static int counter;
			std::stringstream s;
			s << counter;
			listOfFilename.push_back(std::string("gp").append(s.str()));
			counter++;
			return(listOfFilename.back());

		}
		void open()
		{
			 
			#ifdef WIN32
						gnucmd = _popen(gpath.c_str(), "w");
			#else
						gnucmd = popen(gpath.c_str(), "w");
			#endif


		}
		void close()
		{
			
			#ifdef WIN32
						_pclose(gnucmd);
			#else
						pclose(gnucmd);
			#endif

			std::for_each(begin(listOfFilename),end(listOfFilename),[](std::string item)
			{
				std::remove(item.c_str());
			});
		}
		void executeCmd(const std::string &cmd)
		{
			fputs((cmd + "\n").c_str(), gnucmd);
			fflush(gnucmd);

		}

	};


}// end namespace 