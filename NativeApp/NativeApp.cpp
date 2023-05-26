// ConsoleApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <windows.h>
#include <iostream>
#include <conio.h>

typedef void(__stdcall* TNativeFunctionInNativeLib)();

int main()
{
    std::cout << "Inside NativeApp!\n";

    HINSTANCE hGetProcIDDLL = LoadLibraryA("NativeLib.dll");

    if (!hGetProcIDDLL) {
        std::cout << "could not load the NativeLib.dll" << std::endl;
        return EXIT_FAILURE;
    }
    
    auto NativeFunctionInNativeLibFunction = (TNativeFunctionInNativeLib)GetProcAddress(hGetProcIDDLL, "NativeFunctionInsideNativeLib");
    if (! NativeFunctionInNativeLibFunction) {
        std::cout << "could not locate the function NativeFunctionInsideNativeLib" << std::endl;
        return EXIT_FAILURE;
    }

    NativeFunctionInNativeLibFunction();

    std::cout << "press any key to exit...";
    _getch();
}