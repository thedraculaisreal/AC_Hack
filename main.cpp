#include <iostream>
#include <vector>
#include <Windows.h>
#include "proc.h"

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
	uintptr_t dynamicPtrBaseHealthAddr = moduleBase + 0x17E254;

	std::cout << "dynamicPtrBaseAmmoAddr = " << "0x" << std::hex << dynamicPtrBaseAmmoAddr << std::endl;
	std::cout << "dynamicPtrBaseHealthAddr = " << "0x" << std::hex << dynamicPtrBaseHealthAddr << std::endl;

	//resolve out ammo pointer chain
	
	std::vector<unsigned int> ammoOffsets = { 0x364, 0x14, 0x0 };
	uintptr_t ammoAddr = FindDMAAddy(hProcess, dynamicPtrBaseAmmoAddr, ammoOffsets);

	std::vector<unsigned int> healthOffsets = { 0xEC };
	uintptr_t healthAddr = FindDMAAddy(hProcess, dynamicPtrBaseHealthAddr, healthOffsets);

	std::cout << "ammoAddr = " << "0x" << std::hex << ammoAddr << std::endl;
	std::cout << "healthAddr = " << "0x" << std::hex << healthAddr << std::endl;
	//read ammo value

	int ammoValue = 0;
	int healthValue = 0;

	ReadProcessMemory(hProcess, (BYTE*)ammoAddr, &ammoValue, sizeof(ammoValue), nullptr);

	std::cout << "Current ammo = " << std::dec << ammoValue << std::endl;

	ReadProcessMemory(hProcess, (BYTE*)healthAddr, &healthValue, sizeof(healthValue), nullptr);

	std::cout << "Current health = " << std::dec << healthValue << std::endl;

	//write to it

	int newAmmo;
	int newHealth;

	std::cout << "What do you want your health to be: ";
	std::cin >> newHealth;

	std::cout << "What do you want your ammo to be: ";
	std::cin >> newAmmo;

	WriteProcessMemory(hProcess, (BYTE*)ammoAddr, &newAmmo, sizeof(newAmmo), nullptr);
	WriteProcessMemory(hProcess, (BYTE*)healthAddr, &newHealth, sizeof(newHealth), nullptr);

	//read it out again

	ReadProcessMemory(hProcess, (BYTE*)ammoAddr, &ammoValue, sizeof(ammoValue), nullptr);
	ReadProcessMemory(hProcess, (BYTE*)healthAddr, &healthValue, sizeof(healthValue), nullptr);

	std::cout << "New ammo = " << std::dec << ammoValue << std::endl;
	std::cout << "New health = " << std::dec << healthValue << std::endl;


	getchar();

	return 0;
}


