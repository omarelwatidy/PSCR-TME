#include <iostream>
#include <csignal>
#include <unistd.h>
#include <cstdlib>
#include <ctime>
#include <sys/types.h>
#include <sys/wait.h>
#include "rsleep.h"
#include <cerrno>

int vies = 3;

void handler(int sig) {
    vies--;
    std::cout << "Points de vie restants pour"  << getpid()<<": " << vies << std::endl;
    if (vies <= 0) {
        std::cout << "Le processus meurt." << std::endl;
        exit(1);
    }

}
int wait_till_pid(pid_t pid) {
	pid_t p;
	while(true){

		p = wait(NULL);
		if (p == -1){
			if (errno == ECHILD) {
			                std::cerr << "Erreur: aucun processus enfant." << std::endl;
			                return -1;
			            } else {
			                std::cerr << "Erreur dans wait: " << strerror(errno) << std::endl;
			                return -1;
			            }
		}
		if (p == pid){
			return p;
		}

}
}


int child_fin = 0;

pid_t p;

void handler_SIGCHLD(int sig) {

    pid_t pid = wait(NULL);
    if (pid == p) {
        child_fin = 1;
    }
}

void handler_SIGALRM(int sig) {
    std::cout << "Temps écoulé sans que le processus " <<p << " ne termine." << std::endl;
    child_fin = -1;
}

int wait_till_pid1(pid_t pid, int sec){
    p = pid;
    struct sigaction sa_chld, sa_alarm;


    sa_chld.sa_handler = handler_SIGCHLD;
    sigemptyset(&sa_chld.sa_mask);
    sa_chld.sa_flags = 0;
    sigaction(SIGCHLD, &sa_chld, nullptr);


    sa_alarm.sa_handler = handler_SIGALRM;
    sigemptyset(&sa_alarm.sa_mask);
    sa_alarm.sa_flags = 0;
    sigaction(SIGALRM, &sa_alarm, nullptr);


    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGCHLD);
    sigaddset(&mask, SIGALRM);
    sigprocmask(SIG_BLOCK, &mask, nullptr);


    alarm(sec);

    sigset_t setneg;
        sigfillset(&setneg);
        sigdelset(&setneg, SIGCHLD);
        sigdelset(&setneg, SIGALRM);
    sigsuspend(&setneg);


    if (child_fin == 1) {

        alarm(0);
        return p;
    }


    if (child_fin == -1) {
        return 0;
    }

    return -1;
}



    void attaque(pid_t adversaire,pid_t luke_pid) {

        struct sigaction action;
        action.sa_handler = handler;
        sigemptyset(&action.sa_mask);
        action.sa_flags = 0;
        sigaction(SIGINT, &action, nullptr);
        int status;
        if (getpid() != luke_pid){
            pid_t child_status = waitpid(adversaire, &status, WNOHANG);  // Non-blocking check

            if (child_status > 0) {
                std::cerr << "Adversary process is dead (zombie). Exiting." << std::endl;
                exit(0);
            }
        }
        std::cout << "hil !" << std::endl;
        std::cout << "Process " << getpid() << " attacking adversary " << adversaire << std::endl;

        if (kill(adversaire, SIGINT) == -1) {
        	std::cout << "hi !" << std::endl;
        	if (errno == ESRCH) {
            std::cerr << "L'adversaire est déjà mort !" << std::endl;
            exit(0);
        	}
        }



        randsleep();
        randsleep();

}
    void handlerLuke(int sig) {
        std::cout << "Luke: Coup paré !" << std::endl;
    }
    void defenseLuke() {
        struct sigaction action;
        action.sa_handler = handlerLuke;
        sigemptyset(&action.sa_mask);
        action.sa_flags = 0;
        sigaction(SIGINT, &action, nullptr);


        sigset_t mask;
        sigemptyset(&mask);
        sigaddset(&mask, SIGINT);
        sigprocmask(SIG_BLOCK, &mask, nullptr);


        randsleep();
        sigset_t set;
                sigfillset(&set);
                sigdelset(&set, SIGINT);

        sigsuspend(&set);


    }

    void defense() {
        // Ignorer le signal SIGINT

    	signal(SIGINT,SIG_IGN);

        randsleep();


    }

    void combat(pid_t adversaire,pid_t luke_pid) {
        while (true) {
        	if(getpid() == luke_pid){
            defenseLuke();
        	}
        	else{
        		defense();
        	}

            attaque(adversaire,luke_pid);
        }
    }
    int main() {
    	randsleep();

        pid_t luke_pid = fork();

        if (luke_pid == -1) {
            std::cerr << "Erreur lors de la création du processus fils." << std::endl;
            return 1;
        }

        if (luke_pid == 0) {

            std::cout << "Luke (pid=" << getpid() << ") entre dans le combat !" << std::endl;
            combat(getppid(),luke_pid);
        } else {


            std::cout << "Vador (pid=" << getpid() << ") entre dans le combat !" << std::endl;
            randsleep();

            combat(luke_pid,luke_pid);



        }

        return 0;
    }


