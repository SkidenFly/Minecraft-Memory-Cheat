#include "../ext/main.h"

void address_calculator();

template<class val>
std::vector<int> locations(std::vector<val> values_list, val value);

void memory::scanner_thread() {

    while (utils::minecraft_running()) {

        address_calculator();

        for (size_t address = memory::min_address; address < memory::max_address; address += 100000) {

            std::vector<double> memory(100000);

            if (!ReadProcessMemory(ext::mc_handle, (LPCVOID) address, &memory[0], 100000, NULL)) continue;

            if (!std::count(memory.begin(), memory.end(), reach::break_default)) continue;
            
            for (int location : locations(memory, reach::break_default)) {
                    
                size_t addr = address + location * sizeof(double);

                std::vector<float> reachf(200);
                std::vector<double> reachd(200);

                if (!ReadProcessMemory(ext::mc_handle, (LPCVOID)(addr - 100), &reachf[0], 200, NULL) && 
                    !ReadProcessMemory(ext::mc_handle, (LPCVOID)(addr - 100), &reachd[0], 200, NULL)) 
                    continue;
                    
                if (std::count(reachf.begin(), reachf.end(), memory::reach::reach_default)) {

                    for (int location : locations(reachf, memory::reach::reach_default)) {

                        memory::reach::address_break.push_back(addr);
                        memory::reach::address_float.push_back((addr - 100) + location * sizeof(float));

                        memory::reach::address_float.erase(
                            unique(memory::reach::address_float.begin(), memory::reach::address_float.end()), memory::reach::address_float.end());
                    }

                }

                if (std::count(reachd.begin(), reachd.end(), (double) memory::reach::reach_default)) {

                    for (int location : locations(reachd, (double) memory::reach::reach_default)) {

                        memory::reach::address_break.push_back(addr);
                        memory::reach::address_double.push_back((addr - 100) + location * sizeof(double));

                        memory::reach::address_double.erase(
                            unique(memory::reach::address_double.begin(), memory::reach::address_double.end()), memory::reach::address_double.end());
                    }
                }
            }
            memory::reach::address_break.erase(
                unique(memory::reach::address_break.begin(), memory::reach::address_break.end()), memory::reach::address_break.end());
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    exit(0);
}

template<class val>
std::vector<int> locations(std::vector<val> values_list, val value) {
    std::vector<int> locations;

    for (int i = 0; i < values_list.size(); i++) {

        if (values_list[i] != value) continue;

        locations.push_back(i);
    }

    return locations;
}

void address_calculator() {
	
    size_t address = memory::calc_min_address;

    MEMORY_BASIC_INFORMATION mbi;

    for (
        size_t address = memory::calc_min_address;
        VirtualQueryEx(ext::mc_handle, (LPVOID)address, &mbi, sizeof(mbi)) && address < memory::calc_max_address;
        address += mbi.RegionSize
        ) {

        if (mbi.State == MEM_COMMIT && mbi.Type == MEM_PRIVATE && 
            mbi.Protect == PAGE_EXECUTE_READWRITE && mbi.AllocationProtect == PAGE_READWRITE) {

            memory::min_address = address;

            if (ext::other_client) memory::max_address = address + 251658240; //Pretty stupid but works :P
            
            else memory::max_address = address + mbi.RegionSize;
        }

    }
}