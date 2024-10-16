#include "Banque.h"

#include <random>
#include <iostream>
using namespace std;

const int NB_THREAD = 10;

int threadSafeRand(int min,int max){

	static thread_local std::mt19937 gen;
	std::uniform_int_distribution<int> dist(min,max);
	return dist(gen);


}

void work(pr::Banque & b){
	size_t i,j;
	unsigned int m;
	for(int k = 0; k<1000;k++){
		i = threadSafeRand(0,99);
		m = threadSafeRand(1,100);
		j = threadSafeRand(0,99);
		 if (i != j) {
		            b.transfert(i, j, m);
		        }

	std::this_thread::sleep_for(std::chrono::milliseconds(threadSafeRand(0, 20)));

	}



}
int main () {
	vector<thread> threads;
	pr::Banque b(100,50);
	// TODO : creer des threads qui font ce qui est demandé
	for (int i = 0;i <NB_THREAD;i++){
		threads.emplace_back(&work,std::ref(b));


	}


	for (auto & t : threads) {
		t.join();
	}

b.comptabiliser(50*100);
	// TODO : tester solde = NB_THREAD * JP
	return 0;
}
