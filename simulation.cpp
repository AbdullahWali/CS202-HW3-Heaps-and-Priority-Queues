/*
Abdullah Wali
21402793
Section 3
*/

#include "simulation.h"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

int Patient::compareTo(const Patient & other)
{
	if (priority > other.priority) return 1;
	else if (priority < other.priority) return -1;
	else if (arrivalTime < other.arrivalTime) return 1;
	else if (arrivalTime > other.arrivalTime) return -1;
	else return 0;
}

bool Doctor::isFree(int time)
{
	if (freeAt == -1) return true;
	else return time >= freeAt;
}
int Doctor::idCount = 0;


//Heap Implementation

// Default constructor 
Heap::Heap() : size(0) {

}

bool Heap::heapIsEmpty() const {
	return (size == 0);
}

void Heap::heapInsert(const HeapItemType& newItem)  {

	// Place the new item at the end of the heap
	items[size] = newItem;

	// Trickle new item up to its proper position
	int place = size;
	int parent = (place - 1) / 2;
	while ((place > 0) && (items[place] > items[parent])) {
		HeapItemType temp = items[parent];
		items[parent] = items[place];
		items[place] = temp;

		place = parent;
		parent = (place - 1) / 2;
	}
	++size;
}

void Heap::heapDelete(HeapItemType& rootItem) {
	if (heapIsEmpty())
		std::cout << "Heap Empty, can't delete" << std::endl;
	else {
		rootItem = items[0];
		items[0] = items[--size];
		heapRebuild(0);
	}
}

void Heap::heapRebuild(int root) {
	int child = 2 * root + 1; 	// index of root's left child, if any
	if (child < size) {
		// root is not a leaf so that it has a left child
		int rightChild = child + 1; 	// index of a right child, if any
										// If root has right child, find larger child
		if ((rightChild < size) &&
			(items[rightChild] >items[child]))
			child = rightChild; 	// index of larger child

									// If root’s item is smaller than larger child, swap values
		if (items[root] < items[child]) {
			HeapItemType temp = items[root];
			items[root] = items[child];
			items[child] = temp;

			// transform the new subtree into a heap
			heapRebuild(child);
		}
	}
}


//Priority Queue
bool PriorityQueue::pqIsEmpty() const {
	return h.heapIsEmpty();
}

void PriorityQueue::pqInsert(const PQItemType& newItem) {
		h.heapInsert(newItem);
	}

void PriorityQueue::pqDelete(PQItemType& priorityItem) {
	h.heapDelete(priorityItem);
}

void Simulation(std::string filename, int avgwaitingtime)
{
	ifstream infile;
	infile.open(filename.c_str());
	int patientCount = 0;


	//Repeat the simulation while avgWaited > avgWaitingTime
	stringstream buffer;
	int numDoctors = 0;
	double avgWaited = 0;
	do {
		infile >> patientCount; // Take the patient count from file
		//Clear past results
		buffer.str("");
		buffer.clear();
		//Increment num of doctors
		numDoctors++;
		int totalWaited = 0; //Will use to calculate avgWaited
	
		//Create list of doctors
		Doctor* doctors = new Doctor[numDoctors];

		//Start Time at 1 and start taking patients
		int time = 1;
		PriorityQueue patientQueue;

		//In this stage, Patients are being treated and entered to system at same time
		int patientsEntered = 0;
		while (patientsEntered < patientCount) {

			//Read Data from line
			int id, priority, arrivalTime, duration;
			infile >> id >> priority >> arrivalTime >> duration;

			//If the time is still the same, add patient to data, and
			//read more patients until all patients at that time are added to the system.
			if (arrivalTime == time) {
				patientQueue.pqInsert(Patient(id, priority, arrivalTime, duration));
				patientsEntered++;
			}
		
			//Else, Start processing the patients --> update time --> Add the new patient
			else  {
				//Process the existing patients
				while (time < arrivalTime) { //Process existing Patients until the arrival time of the new patient
					while (!patientQueue.pqIsEmpty()) {
						int doctorIndex = -1; // index of free doctor
						for (int j = 0; j < numDoctors; j++) {
							if (doctors[j].isFree(time)) {
								doctorIndex = j; break;
							} //Find free doctor at this time
						}
						if (doctorIndex == -1) break; //if No doctor is free, break to next step
						else {
							Patient temp;
							patientQueue.pqDelete(temp);//temp now holds the removed patient
							doctors[doctorIndex].takePatient(time, temp);
							buffer << "Doctor " << doctorIndex << " takes patient " << temp.getId() <<
								" at minute " << time << " (wait: " << (time - temp.getArrivalTime()) << " mins)" << endl;
							totalWaited += time - temp.getArrivalTime();
						}
					}
					time++;
				}

				//add patient 
				patientQueue.pqInsert(Patient(id, priority, arrivalTime, duration));
				patientsEntered++;
			}
		}


		//Process Patients that are left after all new patients are entered
		while ((!patientQueue.pqIsEmpty())) {
			while (!patientQueue.pqIsEmpty()) {
				int doctorIndex = -1; // index of free doctor
				for (int j = 0; j < numDoctors; j++) {
					if (doctors[j].isFree(time)) {
						doctorIndex = j; break;
					} //Find free doctor at this time
				}
				if (doctorIndex == -1) break; //if No doctor is free, break to next step
				else {
					Patient temp;
					patientQueue.pqDelete(temp);//temp now holds the removed patient
					doctors[doctorIndex].takePatient(time, temp);
					buffer << "Doctor " << doctorIndex << " takes patient " << temp.getId() <<
						" at minute " << time << " (wait: " << (time - temp.getArrivalTime()) << " mins)" << endl;
					totalWaited += time - temp.getArrivalTime();
				}
			}
			time++;
		}




		//Calculate avgWaitingTime
		avgWaited = totalWaited / (double) patientCount;

		//Delete Doctors array
		delete[] doctors;
		//Rewind the pointer for the ifstream
		infile.clear();
		infile.seekg(0);
	} while (avgWaited > avgwaitingtime);

	cout << "Minimum number of Doctors required: " << numDoctors << endl<<endl;
	cout << "Simulation with " << numDoctors << " doctors: " << endl << endl;
	cout << buffer.str() << endl<<endl;
	cout << "Average waiting time: " << avgWaited << endl;
}
