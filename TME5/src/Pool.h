#pragma once

#include "Queue.h"
#include "Job.h"
#include <vector>
#include <thread>

namespace pr {

class Pool {
	Queue<Job> queue;
	std::vector<std::thread> threads;
public:
	Pool(int qsize):queue(qsize){


	}
	void start (int nbthread){
		for(int i = 0; i<nbthread;++i){
			threads.emplace_back(&Pool::worker,this,ref(queue));
		}
	}
	void worker(Queue<Job>& queue){
		while(true){
			Job* job = queue.pop();
			if (job == nullptr) return;
			job->run();
			delete job;
		}

	}
	void submit (Job * job){
		queue.push(job);
	}
	void stop(){
		queue.setBlocking(false);
		for(thread&t: threads){
			t.join();
		}
		threads.clear();

	}

	~Pool(){}
};

}
