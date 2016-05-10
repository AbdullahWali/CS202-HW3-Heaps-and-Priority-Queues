/*
Abdullah Wali
21402793
Section 3
*/
#include <iostream>
#include <stdlib.h>
#include "simulation.h"
using namespace std;


int main(int argc, char *argv[])
{
	string filename = argv[1];
	int avgWaitingTime = atoi(argv[2]);
	Simulation(filename, avgWaitingTime);
}

