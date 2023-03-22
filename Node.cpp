#include "Node.h"

Node::Node(const string& name) : name_(name) {}

Node::~Node() {
	for (auto t : children_) { //Destruktor rekurzivno unistava elemente liste children_.
		delete t;
	}
}

string Node::getName() const {
	return name_;
}

void Node::addElement(const string& line) {
	// Funkcija rekurzijom pronalazi cvor u cijoj ce listi children_ biti ucitan novi cvor. String line sadrzi ucitan red teksta iz ulaznog fajla.
	// Naredni blok koda pronalazi putanju do cvora.
	int s = 1;
	if (line[0] != '\\') s = 0;
	int i = line.find("\\", s); // Potrebno je pronaci karakter '\' koji razdvaja roditeljske cvorove od novog cvora
	if (line[i + 1] == 'n') i = line.find("\\", i + 1);
	if (i != -1) { // Ukoliko je pronadjen roditeljski cvor proverava se da li se u listi children_ ovog cvora dodaje element novim pozivom funkcije addElement.
		findElem(line.substr(1, i - 1))->addElement(line.substr(i));
		return;
	}
	else {
		if (findElem(line.substr(s)) != nullptr) throw new SameNameInFolder(line.substr(s));
		addNode(line.substr(s)); // Dodavanje elementa u listu children_ tekuceg cvora.
		return;
	}
}

void Node::EXE(Node*& current, fstream& outputFile, stack<Node*>& parents, const string& command, const string& path) {
	// Na osnovu stringa command izvrsava se jedna od mogucih komandi.
	if (command == "LS") {
		LS(outputFile);
		return;
	}
	if (command == "CD") {
		// Ukoliko je komandom CD zadato postavljanje na podcvor trenutnog cvora potrebno je na stek roditeljskih cvorova postaviti trenutni cvor.
		Node* current2 = current;
		if (path != "..") {
			parents.push(current2);
		}
		current = current->CD(parents, path);
		return;
	}
	if (command == "NEW") {
		NEW(path);
		return;
	}
	if (command == "DEL") {
		DEL(path);
		return;
	}
	if (command == "EXE") {
		if (path == name_) throw new InfExe(path);
		findElem(path)->EXE(current, outputFile, parents);
		return;
	}
	throw new FalseCommand(command);
	return;
}

void Node::LS(fstream& outputFile, const string& prevname, bool firstCall) const {
	if(firstCall && outputFile.tellg() != 0) outputFile << endl; // Ukoliko dolazi do upisa u fajl koji vec ima sadrzaj potrevno je upisati novi red pre pocetka rada funckije LS.
	for (auto t : children_) {
		if (!(firstCall == 1 && t == children_.front())) /*Ukoliko trenutni poziv funkcije LS nije prvi poziv u rekurziji i ukoliko pokazivac t ne pokazuje na prvi cvor
		u listi children_, prelazi se u novi red u fajlu u kom se ispisuje putanja*/
			outputFile << endl;
		if (!prevname.empty()) 
			outputFile << prevname;
		outputFile << '\\' << t->name_; //ispisuje se u fajl ime elementa sa celom putanjom od cvora nad kojim je pozvana funkcija LS.
		if (!t->children_.empty()) { /* Ukoliko lista podcvorova nije prazna prelazi se na ispis elemenata liste children_. U string-u prevname se pamti putanja do 
		elemenata liste*/
			t->LS(outputFile, prevname + "\\" + t->name_, 0);
		}
	}
}

Node* Node::CD(stack<Node*>& parents, string element){
	if (element == "..") { /*Ukoliko je u komandi zadato vracanje na roditeljski cvor potrebno je sa steka na kom se nalazi putanja do datog elementa skinuti roditeljski cvor*/
		if (parents.empty()) {
			throw new InRoot();
		}
		Node* elem = parents.top();
		parents.pop();
		return elem;
	}
	Node* ret = findElem(element);
	if (ret == nullptr) throw new NodeNotFound(element);
	return ret;
}

void Node::NEW(const string& element){
	if (findElem(element) != nullptr) throw new AddSameName(element);
	addNode(element);
}

void Node::DEL(const string& input){
	for (auto it = children_.begin(); it != children_.end(); it++) {
		if (input == (*it)->name_) {
			delete* it;
			children_.erase(it);
			return;
		}
	}
	throw new DelNotFound(input); // Ukoliko nije pronadjen element za brisanje vraca se izuzetak.
}

void Node::addNode(const string& name) {
	Node* elem = newElem(name); // Funkcija newElem je zaduzena da inicijalizuje novi element u zavisnosti od imena.
	if (children_.empty()) { // Ukoliko je lista prazna dodaje se element na pocetak liste.
		children_.push_front(elem);
		return;
	}
	//Petlja u kojoj se dodaje element u sortiranu listu.
	for (auto t = children_.begin(); t != children_.end(); t++) {
		for (int j = 0; j < name.length(); j++) {
			char a = getCharPosition(name[j]);
			char b = getCharPosition((*t)->name_[j]);
			if (a < b) {
				children_.insert(t, 1, elem);
				return;
			}
			else if (a > b) break;
		}
	}
	children_.push_back(elem);
}

Node* Node::findElem(const string& name)const {
	for (auto t : children_) {
		if (name == t->getName()) {
			return t;
		}
	}
	return nullptr;
}

Node* Node::newElem(const string& name) {
	// Funkcija stvara novi cvor.
	int i = name.find(".") + 1;
	if (i - 1 != -1) { // Ukoliko je pronadjen char '.' stvara se jedan od mogucih fajlova na osnovu ekstenzije.
		int j = name.find(" ", i);
		string ext = name.substr(i, j - i);
		if (ext == "txt") return new TxtFile(name.substr(0, j), name.substr(j + 1));
		if (ext == "exe") {
			return new ExeFile(name.substr(0, j), name.substr(j + 1));
		}
	}
	else
		return new Node(name);
}

char Node::getCharPosition(char i) const {
	if (i >= 'A' && i <= 'Z') return i - 'A';
	if (i >= 'a' && i <= 'z') return i - 'a';
	return i;
}

ExeFile::ExeFile(const string& name, const string& commands) : Node(name), commands_(commands){
}

string ExeFile::getCommands() const{
	return commands_;
}

void ExeFile::EXE(Node*& current, fstream& outputFile, stack<Node*>& parents, const string& command, const string& path){ 
	// Virutelna funkcija koja izdvaja komande zadate .exe fajlom i izvrsava komande nad tekucim cvorom pozivajuci funkciju EXE() osnovne klase Node.
	int i = 0;
	while (i < commands_.size()) {
		int j = commands_.find("\\n", i);
		string line = commands_.substr(i, j - i);
		int a = line.find(" ");
		string com = line.substr(0, a);
		string path = ( com == "LS") ? "" : line.substr(a + 1);
		current->Node::EXE(current, outputFile, parents, com, path);
		if (j == -1) break;
		else i = j + 2;
	}
}

TxtFile::TxtFile(const string& name, const string& text) : Node(name), text_(text) {}

string TxtFile::getText() const{
	return text_;
}
