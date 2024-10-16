#include "Banque.h"

#include <iostream>

using namespace std;

namespace pr {

void Banque::transfert(size_t deb, size_t cred, unsigned int val) {
	Compte & debiteur = comptes[deb];
	Compte & crediteur = comptes[cred];

	std::recursive_mutex &mutexDeb = debiteur.getMutex();
	    std::recursive_mutex &mutexCred = crediteur.getMutex();


	    std::lock(mutexDeb, mutexCred);


	    std::unique_lock<std::recursive_mutex> lockDeb(mutexDeb, std::adopt_lock);
	    std::unique_lock<std::recursive_mutex> lockCred(mutexCred, std::adopt_lock);
	if (debiteur.debiter(val)) {

		crediteur.crediter(val);
		std::cout << "Transfer of " << val << " from account " << deb
		           << " to account " << cred << " was successful.\n";

	}
	else {

	        std::cout << "Transfer of " << val << " from account " << deb
	                  << " to account " << cred << " failed (insufficient balance).\n";
	    }

}
size_t Banque::size() const {
	return comptes.size();
}
bool Banque::comptabiliser (int attendu) const {
	int bilan = 0;
	int id = 0;
	for ( auto  compte : comptes) {
		std::recursive_mutex &mutexD = compte.getMutex();
		std::unique_lock<std::recursive_mutex> g(mutexD);
		if (compte.getSolde() < 0) {
			cout << "Compte " << id << " en négatif : " << compte.getSolde() << endl;
		}
		bilan += compte.getSolde();
		id++;
	}
	if (bilan != attendu) {
		cout << "Bilan comptable faux : attendu " << attendu << " obtenu : " << bilan << endl;
	}
	return bilan == attendu;
}
}
