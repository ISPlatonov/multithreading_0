#include <thread>
#include <mutex>
#include <iostream>
#include <string>
#include <chrono>
#include <vector>

using namespace std;

vector<string> v(1, "_begining");

mutex mtx;

template <typename T>
void output_vector(vector<T> v, unsigned int stoptime_ms) {
	for (string s : v) {
		cout << s << ' ';
		this_thread::sleep_for(chrono::milliseconds(stoptime_ms));
	}
	cout << endl;
}

void read(vector<string> &v, int id) {
	this_thread::sleep_for(chrono::milliseconds(2000));
	mtx.lock();
	cout << id << " read the vector: ";
	output_vector(v, 0);
	cout << "-----" << endl;
	mtx.unlock();
	this_thread::sleep_for(chrono::milliseconds(2000));
}

void write(vector<string> &v, string s, int id) {
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

//This structure attends about starting and ending a thread
struct Attention {
	int id;

	Attention(int id) {
		this->id = id;
		lock_guard<mutex> guard(mtx);
		cout << "Thread " << this->id << " starts worling" << endl << "-----" << endl;
	}

	~Attention(){
		lock_guard<mutex> guard(mtx);
		cout << "Thread " << this->id << " stops worling" << endl << "-----" << endl;
	}
};

void f1(vector<string> &v, int id) {
	Attention attention(id);
	read(v, id);
	read(v, id);
	write(v, "_str", id);
	read(v, id);
}
void f2(vector<string> &v, int id) {
	Attention attention(id);
	write(v, "_str", id);
	read(v, id);
	write(v, "_str", id);
	read(v, id);
}
void f3(vector<string> &v, int id) {
	Attention attention(id);
	read(v, id);
	write(v, "_str", id);
	read(v, id);
	write(v, "_str", id);
}
void f4(vector<string> &v, int id) {
	Attention attention(id);
	read(v, id);
	write(v, "_str", id);
	write(v, "_str", id);
	write(v, "_str", id);
}

int main() {
	thread t1(f1, ref(v), 1);
	thread t2(f2, ref(v), 2);
	thread t3(f3, ref(v), 3);
	thread t4(f4, ref(v), 4);
	thread t5(f1, ref(v), 5);
	thread t6(f2, ref(v), 6);
	thread t7(f3, ref(v), 7);
	thread t8(f4, ref(v), 8);
	thread t9(f1, ref(v), 9);
	thread t10(f2, ref(v), 10);
	thread t11(f3, ref(v), 11);
	thread t12(f4, ref(v), 12);
	thread t13(f1, ref(v), 13);
	thread t14(f2, ref(v), 14);
	thread t15(f3, ref(v), 15);
	thread t16(f4, ref(v), 16);

	auto begin = chrono::steady_clock::now();
	t1.join(), t2.join(), t3.join(), t4.join(), 
	t5.join(), t6.join(), t7.join(), t8.join(), 
	t9.join(), t10.join(), t11.join(), t12.join(), 
	t13.join(), t14.join(), t15.join(), t16.join();
	auto end = chrono::steady_clock::now();

	auto elapsed_ms = chrono::duration_cast<chrono::milliseconds>(end - begin);
	cout << "The program was working for " << elapsed_ms.count() << " milliseconds" << endl;
	cout << "Final string: ";
	output_vector(v, 0);

	return 0;
}