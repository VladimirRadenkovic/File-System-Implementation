#include "FileSystem.h"
FileSystem::FileSystem() :root_(new Node("Root")), output_(""), current_(root_) {}

FileSystem::~FileSystem() {
	// Destruktor klase FileSystem je zaduzen da obrise sve elemente stabla ciji je koren element ove klase.
	delete root_; 
	current_  = root_ = nullptr;
}

void FileSystem::loadHierarchy(const string& fs_filepath, const string& log_filepath) {
	fstream inputFile(fs_filepath, ios::in);
	output_ = log_filepath; //Ime fajla za ispis.
	string element;
	try {
		while (inputFile.peek() != EOF) {
			getline(inputFile, element);
			root_->addElement(element);  // funkcija addElement() ucitava ucitani red teksta iz ulazne datoteke u stablo.
		}
	}
	catch (SameNameInFolder* s) { // Izuzetak: dva fajla ili foldera sa istim imenom u jednom folderu.
		delete root_;
		inputFile.close();
		cout << s->what() << s->getName()<< endl;
		exit(0);
	}
	inputFile.close();
}
void FileSystem::execute(const string& filepath){
	stack <Node*> parents; 
	/* Stek cija je uloga da pamti putanju od korena do tekuceg elementa na koji pokazuje pokazivac current_. 
	Na steku se cuvaju cvorovi u kojima se nalazi tekuci element.*/
	fstream input_file(filepath, ios::in);
	// Naredne 2 komande kreiraju fstream objekat fajla za ispis i brisu njegov sadrzaj ukoliko je tekst bio upisan u fajlu pre otvaranja.
	fstream outputFile(output_, ios::out);
	outputFile.close();
	outputFile.open(output_, ios::app); // Ponovno otvaranje fajla za ispis u rezimu app za upis na kraj datoteke.
	string line;
	/* Petlja koja prolazi kroz sadrzaj fajla u kome su zadate komande.*/
	while (input_file.peek() != EOF) {
		try {
			getline(input_file, line);
			int i = line.find(" ");
			string command = line.substr(0, i); // U string-u command se nalazi komanda zadata u jednom redu teksta ulaznog fajla.
			string path = (command == "LS") ? "" : line.substr(i + 1); // U stringu path se nalazi element nad kojim treba da bude izvrsena komanda.
			current_->EXE(current_, outputFile, parents, command, path);
			/* Svaki cvor stabla je zaduzen da izvrsi komandu, funckiji EXE() se prosledjuje referenca na pokazivac current_,
			ime fajla za ispis, stek sa trenutnom putanjom, komanda i ime elementa nad kojim se izvrsava komanda*/
		}
		catch(FalseCommand* f){ // Izuzetak: nepostojeca komanda.
			cout << f->what() << f->getCommand() << endl;
		}
		catch (InRoot* i) { // Izuzetak: Tekuci cvor je koreni cvor, ne postoji cvor za vracanje nazad.
			cout << i->what() << endl;
		}
		catch (NodeNotFound* p) { // Izuzetak: Nije pronadjen folder sa trazenim imenom.
			cout << p->what() << p->getPath() << endl;
		}
		catch (AddSameName* a) { // Izuzetak: Pokusaj dodavanja foldera ili fajla u folder sa vec postojecim folderom ili fajlom sa datim imenom.
		cout << a->what() << a->getName() << endl;
		}
		catch (DelNotFound* d) { // Izuzetak: nije pronadjen folder sa trazenim imenom za brisanje.
			cout << d->what() << d->getName() << endl;
		}
		catch (InfExe* e) { // Izuzetak:: rekurzivno pozivanje EXE komande nad istim fajlom.
			cout << e->what() << e->getName() << endl;
			exit(0);
		}
	}
	outputFile.close();
}
