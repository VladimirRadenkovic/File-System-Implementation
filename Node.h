#ifndef NODE_H
#define NODE_H

#include"Exeptions.h"
#include <fstream>
#include <iostream>
#include <list>
#include <stack>
#include <string>
using namespace std;

 /* Objekti polimorfne klase Node su elementi stabla. Ova klasa je zaduzena za rad sa elementima stabla. Svaki cvor ima proizvoljan broj podcvorova.
 * Svaki element sadrzi ime i listu children_ ciji elementi predstavljaju podcvorove datog cvora.
 */
 class Node {
public:
	Node(const string& name = "");
	~Node();

	string getName() const;

	void addElement(const string& line); // Funkcija je zaduzena da pronadje mesto u stablu gde ce biti inicijalizovan novi element.

	virtual void EXE(Node*& current, fstream& outputFile, stack<Node*>& parents, const string& command = "", const string& path =""); /* Virtuelna metoda koja je zaduzena
	da pravilno izvrsi zadatu komandu.*/

protected:
	string name_;
	list <Node*> children_;

	void addNode(const string& name); // Funkcija je zaduzena da pronadje mesto u listi children_ gde ce biti inicijalizovan novi element.
	Node* findElem(const string& name) const;
	Node* newElem(const string& name); // Funkcija kreira i vraca novi cvor.

	void LS(fstream& outputFile, const string& prevname = "", bool firstCall = true) const; /*Funkcija se poziva rekurzivno, parametar firstCall govori da li se radi o
	prvom pozivu funckije. Parametar prevname sluzi za pamcenje putanje za ispis datog elementa.*/
	Node* CD(stack <Node*>& parents, string element); // Funkcija koristi stek za pamcenje putanje od korena do tekuceg cvora.
	void NEW(const string& element);
	void DEL(const string& input = "");

	char getCharPosition(char i) const;
};

 /* Klase ExeFile i TxtFile su izvedene klase iz osnovne klase Node. Ove klase predstavljaju vrste fajlova koji mogu da se pojavljuju u sistemu. 
  Klase sadrze element tipa string koji predstavlja sadrzaj fajla.
 */
class ExeFile : public Node {
public:
	ExeFile(const string& name, const string& commands);
	string getCommands() const;

	void EXE(Node*& current, fstream& outputFile, stack<Node*>& parents, const string& command = "", const string& path = "") override; /* Polimorfna metoda zaduzena da 
	izvrsi komande iz .exe fajla.*/																														
private:
	string commands_;
};

class TxtFile : public Node {
public:
	TxtFile(const string& name, const string& text);
	string getText() const;
private:
	string text_;
};

#endif