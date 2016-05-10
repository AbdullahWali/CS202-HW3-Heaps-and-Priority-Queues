/*
Abdullah Wali
21402793
Section 3
*/
#ifndef SIMULATION
#define SIMULATION

#include <string>


//Patient class
class Patient {

public:
	Patient() : id(-1), priority(-1), arrivalTime(-1), duration(-1) {};
	Patient(int id, int priority, int arrivalTime, int duration) : id(id), priority(priority), arrivalTime(arrivalTime), duration(duration){ };
	bool operator>(const Patient& other) { return this->compareTo(other) > 0; };
	bool operator<(const Patient& other) { return this->compareTo(other) < 0; };
	bool operator==(const Patient& other) { return this->compareTo(other) == 0; };
	
	
	int getId() { return id; };
	int getPriority(){ return priority; };
	int getArrivalTime() { return arrivalTime; };
	int getDuration() { return duration; };
	

private:
	int compareTo(const Patient& other);

	int id;
	int priority;
	int arrivalTime;
	int duration;
};


class Doctor {
	static int idCount;

public:
	void takePatient(int time, Patient temp) { freeAt = time + temp.getDuration(); };
	Doctor() : id(idCount++), freeAt(-1) {};
	bool isFree(int time);

private:
	int id;
	int freeAt; //the minute that the doctor will be free at if currently busy, it defaults to -1 if doctor is free

};



//Heap Definitions
const int MAX_HEAP = 200;
typedef Patient HeapItemType;

class Heap {
public:
	Heap();				// default constructor
						// copy constructor and destructor are supplied by the compiler

	bool heapIsEmpty() const;
	void heapInsert(const HeapItemType& newItem);
	void heapDelete(HeapItemType& rootItem);

protected:
	void heapRebuild(int root);		// Converts the semiheap rooted at 
									// index root into a heap
private:
	HeapItemType items[MAX_HEAP];	// array of heap items
	int          size;            	// number of heap items
};


typedef HeapItemType PQItemType;
class PriorityQueue {
public:
	// default constructor, copy constructor, and destructor 
	// are supplied by the compiler

	// priority-queue operations:
	bool pqIsEmpty() const;
	void pqInsert(const PQItemType& newItem);
	void pqDelete(PQItemType& priorityItem);

private:
	Heap h;
};


void Simulation( std::string filename, int avgwaitingtime);

#endif // !SIMULATION
