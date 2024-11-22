#include "Stack.h"
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <vector>
#include <sys/mman.h>

using namespace std;
using namespace pr;

void producteur (Stack<char> * stack) {
	char c ;
	while (cin.get(c)) {
		stack->push(c);
	}
}
bool done = false;
void handler(int sig){
	done = true;
}
void consomateur (Stack<char> * stack) {
	while (!done) {
		char c = stack->pop();
		cout << c << flush ;
	}
}


int main () {
	signal(SIGINT,&handler);

	const char* shm_name = "/shared_stack";

	    // Créer un segment de mémoire partagée
	    int shm_fd = shm_open(shm_name, O_CREAT | O_RDWR, 0666);
	    if (shm_fd == -1) {
	        perror("shm_open");
	        return 1;
	    }
	    if (ftruncate(shm_fd, sizeof(Stack<char>)) == -1) {
	           perror("ftruncate");
	           shm_unlink(shm_name);
	           return 1;
	       }
	    void* shared_mem = mmap(nullptr, sizeof(Stack<char>), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
	        if (shared_mem == MAP_FAILED) {
	            perror("mmap");
	            shm_unlink(shm_name);
	            return 1;
	        }
	//Stack<char> * s = new Stack<char>();


	    pr::Stack<char>* s = new (shared_mem) pr::Stack<char>();
	    std::vector<pid_t> producers, consumers;

	        // Créer plusieurs producteurs
	        for (int i = 0; i < 3; ++i) {
	            pid_t p = fork();
	            if (p == 0) {
	                producteur(s);
	                munmap(shared_mem, sizeof(pr::Stack<char>));
	                return 0;
	            }
	            producers.push_back(p);
	        }



	        for (int i = 0; i < 2; ++i) {
	                pid_t p = fork();
	                if (p == 0) {
	                    consomateur(s);
	                    munmap(shared_mem, sizeof(pr::Stack<char>));
	                    return 0;
	                }
	                consumers.push_back(p);
	            }

	        for (pid_t p : producers) waitpid(p, nullptr, 0);
	            for (pid_t c : consumers) waitpid(c, nullptr, 0);

	s->~Stack();
	munmap(shared_mem, sizeof(pr::Stack<char>));
	shm_unlink(shm_name);

	//delete s;
	return 0;
}

