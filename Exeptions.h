#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H
#include "Node.h"
#include <exception>
#include <string>
using namespace std;

class SameNameInFolder : public exception {
public:
	SameNameInFolder(const string& s) : exception("Error: Dva elementa sa istim imenom: "), s_(s){}
	string getName() const {
		return s_;
	}
private: 
	string s_;
};

class NodeNotFound : public exception {
public:
	NodeNotFound(const string& p) : exception("Error: CD "), p_(p){}
	string getPath() const {
		return p_;
	}
private:
	string p_;
};

class FalseCommand : public exception {
public:
	FalseCommand(const string& s) : exception("Error: "), s_(s){}
		string getCommand() const {
		return s_;
	}
private:
	string s_;
};

class InRoot : public exception {
public:
	InRoot() : exception("Error: CD.."){}
};

class AddSameName : public exception {
public:
	AddSameName(const string& s) : exception("Error: NEW "), s_(s) {}
	string getName() const {
		return s_;
	}
private: 
	string s_;
};

class DelNotFound : public exception {
public:
	DelNotFound(const string& d) : exception("Error: DEL "), d_(d) {}
	string getName() const {
		return d_;
	}
private:
	string d_;
};

class InfExe : public exception {
public:
	InfExe(const string & e) : exception("Error: EXE "), e_(e) {}
	string getName() const {
		return e_;
		}
private:
	string e_;
};

#endif
