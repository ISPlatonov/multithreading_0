#include <thread>
#include <mutex>
#include <iostream>
#include <string>
#include <chrono>
#include <vector>

#define READERS_NUM 100
#define WRITERS_NUM 50

using namespace std;

mutex mtx;

//This structure attends about starting and ending a thread
struct Attention {
	int id;

	Attention(int id) {
		this->id = id;
		lock_guard<mutex> guard(mtx);
		cout << "Thread " << this->id << " starts worling" << endl << "-----" << endl;
	}

	~Attention() {
		lock_guard<mutex> guard(mtx);
		cout << "Thread " << this->id << " stops worling" << endl << "-----" << endl;
	}
};

void output_vector(vector<string> v, unsigned int stoptime_ms) {
	for (string s : v) {
		cout << s << ' ';
		this_thread::sleep_for(chrono::milliseconds(stoptime_ms));
	}
	cout << endl;
}

void read(vector<string> &v, int id) {
	Attention atntn(id);
	this_thread::sleep_for(chrono::milliseconds(2000));
	mtx.lock();
	cout << id << " read the vector: ";
	output_vector(v, 0);
	cout << "-----" << endl;
	mtx.unlock();
	this_thread::sleep_for(chrono::milliseconds(2000));
}

void write(vector<string> &v, string s, int id) {
	Attention atntn(id);
	this_thread::sleep_for(chrono::milliseconds(2000));
	mtx.lock();
	v.push_back(s + to_string(id));
	cout << id << " pushed the string " << s << id << " into the vector" << endl;
	cout << "The vector: ";
	output_vector(v, 20);
	cout << "-----" << endl;
	mtx.unlock();
	this_thread::sleep_for(chrono::milliseconds(2000));
}

void joinThreads(vector<thread*>& trds)
{
	for (auto trd : trds)
	{
		trd->join();
	}
}

void createThreads(vector<thread*>& trds, vector<string>& v)
{
	for (uint16_t i = 1, j = 1; i <= READERS_NUM || j <= WRITERS_NUM; ++i, ++j)
	{
		if (i <= READERS_NUM)
			trds.push_back(new thread(read, ref(v), i));
		if (j <= WRITERS_NUM)
			trds.push_back(new thread(write, ref(v), "_str", j + READERS_NUM));
	}
}

int main() {
	vector<string> v(1, "_begining");

	vector<thread*> trds;
	//const vector<void*> funcs = { &f1, &f2, &f3, &f4 };
	
	createThreads(trds, v);
	/*for (int i = 1; i <= 16; ++i)
	{
		trds.push_back(new thread(f1, ref(v), i));
	}*/

	auto begin = chrono::steady_clock::now();
	/*t1.join(), t2.join(), t3.join(), t4.join() ,
	t5.join(), t6.join(), t7.join(), t8.join(), 
	t9.join(), t10.join(), t11.join(), t12.join(), 
	t13.join(), t14.join(), t15.join(), t16.join();*/
	joinThreads(trds);
	/*for (auto trd : trds)
	{
		trd->join();
	}*/
	auto end = chrono::steady_clock::now();

	auto elapsed_ms = chrono::duration_cast<chrono::milliseconds>(end - begin);
	cout << "The program was working for " << elapsed_ms.count() << " milliseconds" << endl;
	cout << "Final string: ";
	output_vector(v, 0);

	return 0;
}