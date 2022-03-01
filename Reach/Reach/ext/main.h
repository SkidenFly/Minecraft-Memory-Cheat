#include <iostream>
#include <vector>
#include <algorithm>
#include <thread>
#include <chrono>

#include <Windows.h>


namespace ext
{
	extern void init();

	inline HANDLE mc_handle;

	inline bool other_client;
}

namespace memory
{
	extern void scanner_thread();
	extern void reach_thread();

	inline size_t calc_min_address;
	inline size_t calc_max_address;

	inline size_t min_address;
	inline size_t max_address;

	namespace reach
	{
		inline double break_default = 4.5;
		inline float reach_default = 3.f;

		inline double new_break = 7.5;
		inline float new_reach = 6.f;

		inline std::vector<size_t> address_break;
		inline std::vector<size_t> address_double;
		inline std::vector<size_t> address_float;
	}
}

namespace utils
{
	extern void get_handle();
	extern void client_check();
	extern bool minecraft_running();

	template<class val>
	extern void write_memory(std::vector<size_t> address_list, val default_value, val new_value);
}