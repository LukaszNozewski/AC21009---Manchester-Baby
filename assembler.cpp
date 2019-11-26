#include <iostream>
#include <string>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include "assembler.h"
#include <vector>

using namespace std;


int currentLineNumber=0;
int totalLines = 0;
string fileName = "default.txt";
bool displayProgressBar=true;

/**
 * symbolTable
 * stores any symbol from the assembly code as 2 variables
 */
class symbolTable
{
	public:
		string label;
		int lineNumber;

		symbolTable();
		~symbolTable();
};
symbolTable::symbolTable()
{
	//cout << " Symbol Table Created" << endl;
}
symbolTable::~symbolTable()
{
	//cout << " Symbol Table Destroyed." << endl;
}

/**
 * Assembler
 * Creates the assembler and all necessary classes - stores the temporaryMemory before it is written.
 */
class Assembler
{
	public:

		string tempStore[38][32];
		string insertFileName();

		Assembler();
	  ~Assembler();

		void getInstructions();
		void writeProgram();
		void importfile(string selection);
		void interpretateLine(string sourceCodeLine);
		void addSymbol(string symbolLabel, int symbolLineNumber);
		void insertInstruction(string label);

		string convertToBinary(int decimalNumber);
		string convertOpcode(string opcode);

};

Assembler::Assembler()
{
	//cout << " Assembler Created" << endl;
}

Assembler::~Assembler()
{
	//cout << "\n Assembler Destroyed." << endl;
}


/**
 * main
 * creates a new instance of assembler
 * imports header from assembler.h
 * calls import file to make the first pass at the source code
 * calls import file again ignoring specific blocks of code to make a 2nd pass
 * displays progress bar whilst running through code.
 * writes to program
 */

int main()
{
	Assembler Assembler;

	// Displaying the header at the top of the program
	header();

	// Making first pass at source code
	Assembler.importfile("1st");

	// Making second pass at source code
	Assembler.importfile("2nd");

	// Writing the program to a text file
	Assembler.writeProgram();
	return 0;
}

string Assembler::insertFileName(){
  string fileName;
  bool fileValid = false;

  while(fileValid != true){
    cout << "\n Filename: " << endl;
    cin >> fileName;

    string validExtension = ".txt";
		int stringLength = fileName.size();
    if(fileName.substr(stringLength-3) == validExtension){
      fileValid = true;
    }
    else{
      fileValid = false;
    }

    return fileName;
  }
}

void Assembler::importfile(string selection){
	int linesLoaded = 0;
	string readLine;
	string previousLine;
	ifstream programFile;
	bool fileExists = false;
	int counter = 0;

	while(!fileExists){
		ifstream ifile(fileName.c_str());
		if(selection == "first"){
			cout << "Enter the name of the file to convert (binary): " << endl;
			fileName = insertFileName();
			programFile.open(fileName.c_str());
		}
		else{
			programFile.open(fileName.c_str());
		}

		while(readLine != ""){
			if(previousLine == readLine){
				readLine = "";
			}
			getline(programFile,readLine);
			previousLine = readLine;

			if(selection == "first"){
				cout << readLine << endl;
			}
			counter++;
		}
		programFile.close();
		totalLines = counter - 1;

		if(ifile){
			fileExists = true;
			programFile.open(fileName.c_str());

			if(selection == "first"){
				cout << "\n Opening " << fileName << endl;
				cout << "---------------------------------------------" << endl;
				cout << "\n Converting " << totalLines << " lines to binary.\n" << endl;
			}

			for(int i = 0; i < totalLines; i++){
				getline(programFile, readLine);
				linesLoaded++;

				if(selection == "first"){
					cout << " Converting line: " << readLine << endl;
					interpretateLine(readLine);
				}
			}


		}
	}
}
