#include <iostream>
#include <unistd.h>

int main () {
	const int N = 3;
	int child_count = 0;
	std::cout << "main pid=" << getpid() << std::endl;

	for (int i=1, j=N; i<=N && j==N && fork()==0 ; i++ ) {
		std::cout << " i:j " << i << ":" << j << std::endl;
		for (int k=1; k<=i && j==N ; k++) {
			if ( fork() == 0) {
				j=0;
				std::cout << " k:j " << k << ":" << j << std::endl;
				std::cout << "je suis le fils"<< getpid()<< "mon pere est " << getppid()<<std::endl;
			}
			else {
			                child_count++;
			            }
		}
	}
	for (int i = 0; i < child_count; i++) {
	        wait(nullptr);
	    }

	return 0;
}//10 processus
