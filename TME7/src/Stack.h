#pragma once
#include <semaphore.h>
#include <cstring> // size_t,memset
#include <errno.h>      // For errno and EINTR
#include <stdexcept>
namespace pr {

#define STACKSIZE 100

template<typename T>
class Stack {
	T tab [STACKSIZE];
	size_t sz;
	sem_t mutex;
	    sem_t empty;
	    sem_t full;
public :
	Stack () : sz(0) { memset(tab,0,sizeof tab) ;

	        sem_init(&mutex, 1, 1);
	        sem_init(&empty, 1, STACKSIZE);
	        sem_init(&full, 1, 0);}

	T pop () {
		// bloquer si vide

		if (sem_wait(&full) == -1) {
		        if (errno == EINTR) {
		            throw std::runtime_error("Semaphore wait interrupted");
		        }
		        throw std::runtime_error("Error waiting on semaphore");
		}
		if (sem_wait(&mutex) == -1) {
				        if (errno == EINTR) {
				            throw std::runtime_error("Semaphore wait interrupted");
				        }
				        throw std::runtime_error("Error waiting on semaphore");
				}

		T toret = tab[--sz];
		sem_post(&mutex);      // Libère le mutex
		sem_post(&empty);
		return toret;
	}

	void push(T elt) {
		sem_wait(&empty);
		sem_wait(&mutex);
		//bloquer si plein
		tab[sz++] = elt;
		sem_post(&mutex);
		sem_post(&full);
	}
	~Stack(){
		sem_destroy(&mutex);
		        sem_destroy(&empty);
		        sem_destroy(&full);

	}
};

}
