#include <iostream>
#include <fstream>
#include <stdio.h>
#include <sstream>
#include <string>
#include <vector>

//#include "Assembler.h"

using namespace std;





int lineNum=0;

int Lines = 0;

string fileName = "BabyTest1-Assembler.txt";

int currentlineNum = 0;

int totalLines =0;

string lineNo;





class symbolTable

{
	public:

		string label;
		int lineNum;


};


vector <symbolTable> Symbol;

class Assembler

{

	public:

		string tempStore[38][32];

		void importfile();
		void getInstructions();

		void writeProgram();

		void insertInstruction(string label);
		void readLine(string codeLine);

		string convertToBinary(int decimalNum);
		string convertCode(string code);

		void addSymbol(string symbolLabel, int symbolLineNumber);

};




int main()

{

	Assembler Assembler;

	Assembler.importfile();

	Assembler.writeProgram();

	return 0;
}




void Assembler::importfile()

{

	int linesLoaded = 0;
	string line;
	string previousLine;
	ifstream file;
	int counter =0;

			file.open ("BabyTest1-Assembler.txt");

			for(int i=0; i< totalLines; i++){

				// Get line
				getline(file,line);

				// Increment linesloaded counter to move to next line

				linesLoaded++;
				string label;
				readLine(line);

					for(int i =11; i <16; i++)

					{
							if(line.at(i)==32 || line.at(i)==58 )
							{

								i=15;

							}
							else
							{

								label = label + line.at(i);
							}

					}

				for(int i = 0; i <Symbol.size(); i++)
				{

						if(Symbol.at(i).label==label)

						{

							string binarylineno = convertToBinary(Symbol.at(i).lineNum);

							for(int j=0; j<5;j++)
							{

								tempStore[j+5][linesLoaded]=binarylineno.at(j);

							}

							i=Symbol.size();

						}

					}

				}

			file.close();


}




void Assembler::insertInstruction(string label)
{



	if(label=="START" && currentlineNum==0)

	{

		string line0 =  "0000:00000000000000000000000000000000";

		for(int i =0; i < 37; i++)
		{

					tempStore[i][currentlineNum]=line0.at(i);
		}

		currentlineNum++;
	}



	if(label=="STOP")

	{


		string lineNo;
		stringstream out;
		out << currentlineNum;
		lineNo = out.str();


		if(currentlineNum<10)
		{
			string linex =  "000" + lineNo + ":00000000000001110000000000000000";
			for(int i =0; i < 37; i++)
			{
				tempStore[i][currentlineNum]=linex.at(i);
			}
		}
		else
		{
			string linex =  "00" + lineNo + ":00000000000001110000000000000000";
			for(int i =0; i < 37; i++)
			{
				tempStore[i][currentlineNum]=linex.at(i);
			}
		}

	}

}




void Assembler::writeProgram()
{

	 ofstream outdata;
	 outdata.open("output.txt");


	cout <<  totalLines << endl;
	outdata << totalLines << endl;

	for(int j = 1; j<=totalLines; j++)
	{

	  for (int i=0; i<37; i++)

	  {

		  outdata << tempStore[i][j];
		  cout << tempStore[i][j];

	  }
	  outdata<< endl;
	 cout << endl;
	}


	outdata.close();

}




void Assembler::readLine(string codeLine)

{

	string label = NULL;
	string code = NULL ;
	string value = NULL;
	string writeToFile= NULL;

	string lineNum;
	stringstream out;
	string labelval;
	int labelValue =0;




	for(int i =0; i <6; i++){

		if(codeLine.at(i)==32 || codeLine.at(i)==58 ){

			i=6;

		}
		else{

			label = label + codeLine.at(i);

		}
	}


	// gets instruction based upon label

	insertInstruction(label);

	if(label=="")
	{

		label=" no";

	}


	cout << "\n Label = " << label << endl;


	if(label!="START" || label !="STOP")
	{
		for(int i = 0; i < 8; i++)

		{

			value=value + codeLine.at(i+7);

		}

		istringstream ( value ) >> labelValue;

		cout << " Label Value = " << labelValue << endl;

	}



	// Add symbol to symbol table
	if(label!=" no")
	{
		addSymbol(label,currentlineNum);
	}



	if(label!="STOP")
	{

		for(int index=0; index <3; index++)
		{
			code = code + codeLine.at(index+7);
		}

		if(code.at(0) < 65)
		{
			code=" no";
		}

		cout << " Opcode = " << code << endl;
		string convert = convertCode(code);
		cout << " Binary Opcode = " << convert << endl;



		if(convert=="Null")

		{
			convert="000";
		}

		out << currentlineNum;
		lineNo = out.str();



		if(labelValue>0)
		{
			labelval = convertToBinary(labelValue);
		}



		if(currentlineNum<10)
		{

						if(labelValue>0)

						{

							writeToFile = "000" + lineNo + ":" + labelval + "";

							cout << " " << writeToFile << endl;

						}
						else
						{

							writeToFile = "000" + lineNo + ":0000000000000" + convert.at(0) + convert.at(1)  + convert.at(2) + "0000000000000000";

							cout << " " << writeToFile << endl;

						}

				}

				else
				{

					if(labelValue>0)

					{

						writeToFile = "00" + lineNo + ":" + labelval + "";
						string convert = convertCode(code);
						cout << " " << writeToFile << endl;

					}

					else

					{
						string convert = convertCode(code);

						writeToFile = "00" + lineNo + ":0000000000000" + convert.at(0) + convert.at(1)  + convert.at(2) + "0000000000000000";

						cout << " " << writeToFile << endl;

					}

				}


				for(int i =0; i < 37; i++)

				{

					tempStore[i][currentlineNum]=writeToFile.at(i);

				}

	}

	currentlineNum++;

	cout << endl;

}




string Assembler::convertToBinary(int decimalNum)

{

	unsigned int powerOf2 = 2147483648;

	string binaryNumber = "";

	string bits= "00000000000000000000000000000000";

	int counter =0;



	do
	{

		if(decimalNum >= powerOf2)
		{

			binaryNumber = binaryNumber+"1";
			decimalNum=decimalNum-powerOf2;
		}
		else
		{
			binaryNumber = binaryNumber+"0";
		}
		if(decimalNum!=0)
		{
			powerOf2 = powerOf2/2;
		}
		counter++;

	}while(counter!=32);

	for(int i = 0; i < 32; i++)	{

		bits.at(i) = binaryNumber.at(31-i);

	}
	return bits;

}





string Assembler::convertCode(string mycode)
{
	string convertCode="";

	if(mycode=="JMP")
	{

		convertCode="000";
	}else if(mycode=="JRP")
	{

		convertCode="100";
    }else if(mycode=="SUB")
    {

		convertCode="001";
	}else if(mycode=="LDN")
    {

		convertCode="010";
    }else if(mycode=="CMP")
	{

		convertCode="011";
	}else if(mycode=="NUM")
	{

		convertCode="101";
	}else if(mycode=="STO")
	{

		convertCode="110";
	}else if(mycode=="STP")
	{

		convertCode="111";
	}

	return convertCode;

}


void Assembler::addSymbol(string symbolLabel, int symbolNumber)
{
	symbolTable s;
	s.label= symbolLabel;
	s.lineNum = symbolNumber;
	int size = Symbol.size();

	Symbol.resize(size+1);
	Symbol.at(size)=s;
	cout << " Label Value = " << Symbol.at(size).lineNum << endl;



}
