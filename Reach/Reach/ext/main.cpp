#include "main.h"


void ext::init() {

    for (; !ext::mc_handle; utils::get_handle())
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

    utils::client_check();

    std::vector<HANDLE> handles = {
        CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)&memory::scanner_thread, nullptr, 0, nullptr),
        CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)&memory::reach_thread, nullptr, 0, nullptr)
    };

    for (HANDLE handle : handles) {
        CloseHandle(handle);
    }

    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(100));
    }
}


void utils::get_handle() {

	HWND mc_window = FindWindow("LWJGL", NULL);

	DWORD pid;
	GetWindowThreadProcessId(mc_window, &pid);

	ext::mc_handle = OpenProcess(PROCESS_ALL_ACCESS, false, pid);
}

bool utils::minecraft_running()
{
    DWORD ret = WaitForSingleObject(ext::mc_handle, 0);
    return ret == WAIT_TIMEOUT;
}

void utils::client_check() {
    std::vector<std::string> strings = { "https://www.lunarclient.com/" }; //You can add your own strings for others clients

    size_t start_addr;
    size_t stop_addr;

    bool found = false;

    MEMORY_BASIC_INFORMATION mbi;
    for (size_t address = 0x0; VirtualQueryEx(ext::mc_handle, (LPVOID)address, &mbi, sizeof(mbi)); address += mbi.RegionSize) {

        if (mbi.State == MEM_COMMIT && mbi.AllocationProtect == PAGE_READWRITE && mbi.Protect == PAGE_READWRITE) {

            start_addr = (size_t)mbi.BaseAddress;
            stop_addr = start_addr + mbi.RegionSize;

            break;
        }
    }

    for (size_t address = start_addr; address < stop_addr; address += 100000) {
        std::vector<std::string> regions;

        regions.resize(regions.size() + 1);
        regions[regions.size() - 1].resize(100000, 0);

        if (!ReadProcessMemory(ext::mc_handle, (void*)address, &regions[regions.size() - 1][0], 100000, nullptr))
            continue;

        for (std::string string : strings) {
            
            if (regions[0].find(string) != std::string::npos) {
                found = true;
		ext::other_client = true;
		   
                memory::calc_min_address = 0x10000000000;
                memory::calc_max_address = 0xF0000000000;
            }
        }
    }

    if (!found) {
        memory::calc_min_address = 0x1000000;
        memory::calc_max_address = 0x10000000;
    }
}
