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
				else if(selection == "second"){
					string label;
					for(int j = 11; j < 16; j++){
						if(readLine.at(j) == 32 || readLine.at(j) == 58){
							j = 15;
						}
						else{
							label = label + readLine.at(j);
						}
					}
				}
				for(int h = 0; h < Symbols.size(); h++){
					if(Symnols.at(h).label == label){
						string binaryLineno = convertToBinary(Symbols.at(h).lineNumber);

						for(int k = 0; k < 5; k++){
							tempStore[k+5][linesLoaded] = binaryLineno.at(k);
						}
						h = Symbols.size();
					}
				}
			}
		}
	}
}

string Assembler::convertOpcode (string code){
	string convertCode="Null";

	/*
	- S = contents of that store location
	- CI = contents of control instruction
	- A = contents of accumulator
	*/
	if (code == "JMP") // CI = S
	{
		convertCode="000";
	}

	else if (code == "JRP") // CI = CI + S ()
	{
		convertCode="100";
	}

	else if (code == "LDN") // A = -S
	{
		convertCode="010";
	}

	else if (code == "STO") // S = A
	{
		convertCode="110";
	}

	else if (code == "SUB") // A = A - S
	{
		convertCode="001";
	}

	else if (code == "NUM") // Num
	{
		convertCode="101";
	}

	else if (code == "CMP") // if A<O then CI = CI + 1
	{
		convertCode="011";
	}

	else if (code == "STP") //to stop
	{
		convertCode="111";
	}

	return covertCode;
}

section	.text
   global _start     ;must be declared for linker (ld)

_start:	            ;tells linker entry point
   mov	edx,len     ;message length
   mov	ecx,msg     ;message to write
   mov	ebx,1       ;file descriptor (stdout)
   mov	eax,4       ;system call number (sys_write)
   int	0x80        ;call kernel

   mov	eax,1       ;system call number (sys_exit)
   int	0x80        ;call kernel

section	.data
msg db 'Hello, world!', 0xa  ;string to be printed
len equ $ - msg     ;length of the string


void Assembler::insertInstruction(string label)
{
	// Inserting Start
	if(label=="START" && currentLineNumber==0)
	{
		string line0 =  "0000:00000000000000000000000000000000";
		for(int i =0; i < 37; i++)
		{
					tempStore[i][currentLineNumber]=line0.at(i);
		}

		currentLineNumber++;
	}

	if(label=="STOP")
	{
		string lineNo;
		stringstream out;
		out << currentLineNumber;
		lineNo = out.str();

		if(currentLineNumber<10)
		{
			string linex =  "000" + lineNo + ":00000000000001110000000000000000";
			for(int i =0; i < 37; i++)
			{
				tempStore[i][currentLineNumber]=linex.at(i);
			}
		}
		else
		{
			string linex =  "00" + lineNo + ":00000000000001110000000000000000";
			for(int i =0; i < 37; i++)
			{
				tempStore[i][currentLineNumber]=linex.at(i);
			}
		}
	}
	if(displayProgressBar==true){
		displayProgress();
	}
}
