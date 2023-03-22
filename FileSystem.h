#ifndef FILE_SYSTEM_
#define FILE_SYSTEM_

#include "Exeptions.h"
#include "Node.h"
#include <fstream>
#include <iostream>
#include <list>
#include <stack>
#include <string>
using namespace std;

/* Klasa FileSystem sadrzi stablo sa proizvoljnim brojem cvorova koje imaju proizvoljan broj podgrana.
 Zaduzena je za inicijalizaciju stabla, brisanje stabla i za rad sa ulaznim i izlaznim fajlovima.
*/
class FileSystem {
public:
	FileSystem();
	~FileSystem();

	void loadHierarchy(const string& fs_filepath, const string& log_filepath);
	void execute(const string& filepath); 

private:
	Node* root_;
	Node* current_;
	string output_;
};

#endif
