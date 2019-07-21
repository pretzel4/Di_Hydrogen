#pragma once
#include <vector>
#include "GF.h"
namespace random2
{
	//This is the seed of the random number generator.
	extern unsigned int g_seed;
	//This will take a high number and a low number.
	//And will return a random integer between the two numbers.
	int randomrange(unsigned int lownum, unsigned int highnum);
	//This will take two numbers that can be negative.
	//This is a little slower then calling just randomrange();
	int randomrange_complex(int lownum, int highnum);
	//This is used to set the seed of the generator.
	void fast_srand(int seed);
	//This will is the bare geneterator used to find random numbers.
	//This should only be used if you know what you are doing.
	//Use randomrange and randomrange_complex for easier usage.
	int fastrand();
	//This will take a range and decide if the number of one of those numbers.
	float range(float range_between, float given_num, float low_num, float high_num);
	//This will take a range and decide if the number of one of those numbers.
	int range_complex(unsigned int range_between, int given_num, int low_num, int high_num);
	//This gets a number between a range given section divider and the given number.
	int range(unsigned int section_divide, int given_num);
	//This gets a number between a range given section divider and the given number.
	int range_complex(unsigned int section_divide, int given_num);
	//This gets a random number between 0 and 1.
	bool getBool();
	//This will return a random number in the form a float.
	//This does two random calls, making it twice as slow as randomrange.
	float randomrange_float(float lownum, float highnum);
	//This class will create a specified number of random numbers that will be put in an an array to be used later.
	//This should be used if you want quick access to random numbers.
	//This will be looped through, so it will not create new numbers as you use them.
	class RandomStorage_int {
		
		private:
			//This tells you what number index you are on.
			int index = -1;
			//This is a vector full of random numbers that can be used later.
			std::vector<int> random_numbers;
			//This is the max number that it can return.
			int _max_num = 0;
			//This is the minimal number the it can return.
			int _min_num = 0;
		public:
			//This takes a lot of time to do.
			//So make sure that you call this function when nothing exciting is happening.
			//This can be called more then once if you want the numbers to be refreshed.
			void setup(unsigned int how_many_numbers, int lownum, int highnum){
				_max_num = highnum;
				_min_num = lownum;
				random_numbers.clear();
				for (unsigned int i = 0; i < how_many_numbers; i++) {
					//random2::g_seed = fastrand();
					random_numbers.push_back(lownum + (fastrand() % (highnum - lownum)));
				}
				index = -1;
			};

			//This will get you the next number in the cache.
			int RandomStorage_int::getNum() {
				index++;
				if (index >= random_numbers.size()) {
					index = 0;
				}
				return random_numbers[index];
			};
			
			//This will get you the minimal number you have allowed it to allocate.
			//Will return zero if never setup.
			int RandomStorage_int::getMinNum() {
				return _min_num;
			};
			//This will get you the maximal number you have allowed it to allocate.
			//Will return zero if never setup.
			int RandomStorage_int::getMaxNum() {
				return _max_num;
			};
	};

	//This class will create a specified number of random numbers that will be put in an an array to be used later.
	//This should be used if you want quick access to random numbers.
	//This will be looped through, so it will not create new numbers as you use them.
	class RandomStorage_float {

	private:
		//This tells you what number index you are on.
		int index = -1;
		//This is a vector full of random numbers that can be used later.
		std::vector<float> random_numbers;
		//This is the max number that it can return.
		float _max_num = 0;
		//This is the minimal number the it can return.
		float _min_num = 0;
	public:
		//This takes a lot of time to do.
		//So make sure that you call this function when nothing exciting is happening.
		//This can be called more then once if you want the numbers to be refreshed.
		void setup(unsigned int how_many_numbers, int lownum, int highnum, int divisor) {
			_max_num = highnum / divisor;
			_min_num = lownum / divisor;
			random_numbers.clear();
			for (unsigned int i = 0; i < how_many_numbers; i++) {
				random2::g_seed = fastrand();
				float num = (lownum + (fastrand() % (highnum - lownum)));
				random_numbers.push_back(num / divisor);
			}
			index = -1;
		};

		//This will get you the next number in the cache.
		float RandomStorage_float::getNum() {
			index++;
			if (index >= random_numbers.size()) {
				index = 0;
			}
			return random_numbers[index];
		};

		//This will get you the minimal number you have allowed it to allocate.
		//Will return zero if never setup.
		float RandomStorage_float::getMinNum() {
			return _min_num;
		};
		//This will get you the maximal number you have allowed it to allocate.
		//Will return zero if never setup.
		float RandomStorage_float::getMaxNum() {
			return _max_num;
		};
	};
	//This class will create a specified number of random numbers that will be put in an an array to be used later.
	//This should be used if you want quick access to random numbers.
	//This will be looped through, so it will not create new numbers as you use them.
	class RandomStorage_bool {

	private:
		//This tells you what number index you are on.
		int index = -1;
		//This is a vector full of random numbers that can be used later.
		std::vector<bool> random_numbers;
	public:
		//This takes a lot of time to do.
		//So make sure that you call this function when nothing exciting is happening.
		//This can be called more then once if you want the numbers to be refreshed.
		void setup(unsigned int how_many_numbers) {
			random_numbers.clear();
			for (unsigned int i = 0; i < how_many_numbers; i++) {
				random_numbers.push_back(getBool());
			}
			index = -1;
		}

		bool getNum() {
			index++;
			if (index >= random_numbers.size()) {
				index = 0;
			}
			return random_numbers[index];
		}
	};

	
}

