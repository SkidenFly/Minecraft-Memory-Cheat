#include "../ext/main.h"


void memory::reach_thread() {

	while (true) {

		std::this_thread::sleep_for(std::chrono::milliseconds(500));

		utils::write_memory(reach::address_break, reach::break_default, reach::new_break);
		utils::write_memory(reach::address_float, reach::reach_default, reach::new_reach);
		utils::write_memory(reach::address_double, (double) reach::reach_default, (double) reach::new_reach);
	}
}

template<class val>
void utils::write_memory(std::vector<size_t> address_list, val default_value, val new_value) {

	for (size_t address : address_list) {
		val buffer;

		if (!ReadProcessMemory(ext::mc_handle, (LPCVOID) address, &buffer, sizeof(val), NULL)) continue;

		if (buffer != default_value && buffer != new_value) continue;

		WriteProcessMemory(ext::mc_handle, (LPVOID)address, &new_value, sizeof(val), NULL);
	}

}

