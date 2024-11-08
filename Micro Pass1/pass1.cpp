#include<iostream>
#include<vector>
#include<map>
#include<fstream>
#include<sstream>
using namespace std;
class Pass1
{
	int litindex = 0;
	map<string, pair<int,int>>SymTab;
	ifstream fin;
	fstream fout;
	int LC = -1;
	map<string, pair<string, string>>opcode;
	vector<pair<string, int>>LitTab;
	vector<string>pooltab;

public:
	void initialize()
	{
		opcode = {
			{"STOP",{"IS","00"}},
			{"ADD",{"IS","01"}},
			{"SUB",{"IS","02"}},
			{"MULT",{"IS","03"}},
			{"MOVER",{"IS","04"}},
			{"MOVEM",{"IS","05"}},
			{"COMP",{"IS","06"}},
			{"BC",{"IS","07"}},
			{"DIV",{"IS","08"}},
			{"READ",{"IS","09"}},
			{"PRINT",{"IS","04"}},
			{"START",{"AD","01"}},
			{"END",{"AD","02"}},
			{"ORIGIN",{"AD","03"}},
			{"EQU",{"AD","04"}},
			{"LTORG",{"AD","05"}},
			{"DC",{"DL","01"}},
			{"DS",{"DL","02"}},
			{"AREG",{"1",""}},
			{"BREG",{"2",""}},
			{"CREG",{"3",""}},
			{"DREG",{"4",""}},
			{"LT",{"1",""}},
			{"LE",{"2",""}},
			{"EQ",{"3",""}},
			{"GT",{"4",""}},
			{"GE",{"5",""}},
			{"ANY",{"6",""}}
		};
	}
	void Tokenize()
	{
		fout.open("output.txt");
		fout.seekp(0, ios::beg);
		fin.open("input.txt");
		string line,word,label;
		while (getline(fin, line))
		{
			fout << endl;
			stringstream ss(line);
			ss >> word;
			if (opcode.count(word) == 0)
			{
				if (SymTab.count(word) == 0)
				{
				
					SymTab[word] = {LC,SymTab.size()+1};
				}
				else
				{
					SymTab[word].first = LC;
				}
				label = word;
				ss >> word;
			}
			string operation = word;
			if (operation == "START")
			{
				ss >> word;
				LC = stoi(word);
			}
			else if (operation == "END")
			{
				fout <<" ("<< opcode[operation].first << "," << opcode[operation].second << ")";
				pooltab.push_back("#" + to_string(litindex + 1));
				while (litindex < LitTab.size())
				{
					LitTab[litindex].second = LC;
					litindex++;
					LC++;
				}
				return;
			}
			else if (operation == "DC")
			{
				fout <<LC <<" ("<<opcode[operation].first<<","<< opcode[operation].second <<") " << "  (C,";
				ss >> word;
				fout << word << ")";

			}
			else if (operation == "DS")
			{
				fout << LC <<" (" << opcode[operation].first << "," << opcode[operation].second << ") "  " " << "(C,";
				ss >> word;
				fout << word << ")";
				LC =LC+stoi(word);

			}
			else if (operation=="ORIGIN")
			{
				ss >> word;
				int ind = -1;
				char op;
				for (int i = 0; i < word.length(); i++)
				{
					if (word[i] == '+' || word[i] == '-')
					{
						op = word[i];
						ind = i;
						break;
					}
				}
				string beforeind;
				string afterind="";
				if (ind != -1)
				{
					beforeind = word.substr(0, ind);
					afterind = word.substr(ind + 1);
				}
				else
				{
					beforeind = word.substr(0);
				}
				LC = SymTab[beforeind].first;
	
				if (afterind != "")
				{
					if (op =='+')
					{
						LC += stoi(afterind);
					}
					else
					{
						LC -= stoi(afterind);
					}
				}
			}
			else if (operation == "EQU")
			{
				ss >> word;
				int ind = -1;
				char op ;
				for (int i = 0; i < word.length(); i++)
				{
					if (word[i] == '+' || word[i] == '-')
					{
						ind = i;
						op = word[i];
						break;
					}
				}
				string beforeind;
				string afterind = "";
				if (ind != -1)
				{
					beforeind = word.substr(0, ind);
					afterind = word.substr(ind + 1);
				}
				else
				{
					beforeind = word.substr(0);
				}
				int temp = SymTab[beforeind].first;
		
				if (afterind != "")
				{
					if (op == '+')
					{
						temp += stoi(afterind);
					}
					else
					{
						temp -= stoi(afterind);
					}
				}
			
				SymTab[label].first = temp;
			}
			else if (operation == "LTORG")
			{
				pooltab.push_back("#" + to_string(litindex + 1));
				while(litindex< LitTab.size())
				{
					LitTab[litindex].second = LC;
					LC++;
					litindex++;
				}

			}
			else
			{
				if (opcode.count(word) > 0)
				{
					fout << endl << LC << "(" << opcode[word].first << "," << opcode[word].second << ")";
					while (ss >> word)
					{
						if (opcode.count(word) > 0)
						{
							fout << "(" << opcode[word].first << "," << opcode[word].second << ")";

						}
						else if (word.find("=") !=string::npos)
						{
							LitTab.push_back({ word,LC });
							fout << "(L," << LitTab.size()<<")";
						}
						else
						{
							if (SymTab.count(word) == 0)
							{
								SymTab[word] = { LC,SymTab.size() + 1 };
							}

							fout << "(S," << SymTab[word].second << ")";
						}
					}
				}
				LC++;
			}
				
		} 
	}
	void readOutput()
	{
		fout.seekg(0, ios::beg);
		string line;
		while (!fout.eof())
		{
			getline(fout, line);
			cout << "\n" << line;
		}
	}
	void displayPoolTab()
	{
		cout << "\nDisplaying pooltab:\n";
		for (int i = 0; i < pooltab.size(); i++)
		{
			cout <<"\n"<< pooltab[i];
		}
	}
	void readSymTab()
	{
		cout << "\nDisplaying symbol Table:";
		for (auto it = SymTab.begin(); it != SymTab.end(); it++)
		{
			cout<< "\n"<<it->second.second <<it->first << " " << it->second.first ;
		}
	}
	void displayLitTab()
	{
		cout << "\nDisplaying Literal Table\n";
		for (auto it = LitTab.begin(); it != LitTab.end(); it++)
		{
			cout << endl<<it->first << " " << it->second;
		}
	}
};
int main()
{
	Pass1 p1;
	p1.initialize();
	p1.Tokenize();
	void readoutput();
	p1.readSymTab();
	p1.displayLitTab();
	p1.displayPoolTab();
	return 0;
}
