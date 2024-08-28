#include <iostream>
#include <vector>
#include <Windows.h>
#include "proc.h"
#include <thread>

int main()
{
	//Get ProcId of the target

	DWORD procId = GetProcId(L"ac_client.exe");

	//Get moduel base address

	uintptr_t moduleBase = GetModuleBaseAddress(procId, L"ac_client.exe");

	//get handle to process

	HANDLE hProcess = 0;
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);

	//resolve base addresss of the pointer chain

	uintptr_t dynamicPtrBaseAmmoAddr = moduleBase + 0x18AC00;
	uintptr_t dynamicPtrBaseHealthAddr = moduleBase + 0x17E0A8;
	uintptr_t dynamicPtrBaseArmorAddr = moduleBase + 0x17E0A8;

	std::cout << "dynamicPtrBaseAmmoAddr = " << "0x" << std::hex << dynamicPtrBaseAmmoAddr << std::endl;
	std::cout << "dynamicPtrBaseHealthAddr = " << "0x" << std::hex << dynamicPtrBaseHealthAddr << std::endl;
	std::cout << "dynamicPtrBaseArmorAddr = " << "0x" << std::hex << dynamicPtrBaseArmorAddr << std::endl;

	//resolve out ammo pointer chain
	
	std::vector<unsigned int> ammoOffsets = { 0x364, 0x14, 0x0 };
	uintptr_t ammoAddr = FindDMAAddy(hProcess, dynamicPtrBaseAmmoAddr, ammoOffsets);

	std::vector<unsigned int> healthOffsets = { 0xEC };
	uintptr_t healthAddr = FindDMAAddy(hProcess, dynamicPtrBaseHealthAddr, healthOffsets);

	std::vector<unsigned int> armorOffsets = { 0xF0 };
	uintptr_t armorAddr = FindDMAAddy(hProcess, dynamicPtrBaseArmorAddr, armorOffsets);

	std::cout << "ammoAddr = " << "0x" << std::hex << ammoAddr << std::endl;
	std::cout << "healthAddr = " << "0x" << std::hex << healthAddr << std::endl;
	std::cout << "armorAddr = " << "0x" << std::hex << armorAddr << std::endl;

	//read ammo value
	while (true) 
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
		int ammoValue = 0;
		int healthValue = 0;
		int armorValue = 0;

		ReadProcessMemory(hProcess, (BYTE*)ammoAddr, &ammoValue, sizeof(ammoValue), nullptr);

		std::cout << "Current ammo = " << std::dec << ammoValue << std::endl;

		ReadProcessMemory(hProcess, (BYTE*)healthAddr, &healthValue, sizeof(healthValue), nullptr);

		std::cout << "Current health = " << std::dec << healthValue << std::endl;

		ReadProcessMemory(hProcess, (BYTE*)armorAddr, &armorValue, sizeof(armorValue), nullptr);

		std::cout << "Current armor = " << std::dec << armorValue << std::endl;

		//write to it

		int newAmmo = 1337;
		int newHealth = 1337;
		int newArmor = 1337;

		WriteProcessMemory(hProcess, (BYTE*)ammoAddr, &newAmmo, sizeof(newAmmo), nullptr);
		WriteProcessMemory(hProcess, (BYTE*)healthAddr, &newHealth, sizeof(newHealth), nullptr);
		WriteProcessMemory(hProcess, (BYTE*)armorAddr, &newArmor, sizeof(newArmor), nullptr);

		//read it out again

		ReadProcessMemory(hProcess, (BYTE*)ammoAddr, &ammoValue, sizeof(ammoValue), nullptr);
		ReadProcessMemory(hProcess, (BYTE*)healthAddr, &healthValue, sizeof(healthValue), nullptr);
		ReadProcessMemory(hProcess, (BYTE*)armorAddr, &armorValue, sizeof(armorValue), nullptr);

		std::cout << "New ammo = " << std::dec << ammoValue << std::endl;
		std::cout << "New health = " << std::dec << healthValue << std::endl;
		std::cout << "New armor = " << std::dec << armorValue << std::endl;
	}

	return 0;
}


