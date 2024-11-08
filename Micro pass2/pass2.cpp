#include<iostream>
#include<vector>
#include<sstream>
#include<fstream>
using namespace std;
class Pass2
{
	vector<pair<string, string>>SymTab;
	ifstream sym, fin;
	ofstream fout;
public:
	Pass2()
	{
		fin.open("output.txt");
		sym.open("sym1.txt");
		fout.open("machine1.txt");
		if (fin.fail() || sym.fail() || fout.fail())
		{
			cout << "\nError in opening file";
			exit(1);
		}
		cout << "\nFiles opened successfully!!";
		GetSymTab();
	}
	void GetSymTab()
	{
		string line,label,symbol,location;
		while (getline(sym, line))
		{
			stringstream ss(line);
			ss >> label >> symbol >> location;
			SymTab.push_back({ symbol,location });
		}
	}
	void genearate()
	{
		fout.seekp(0, ios::beg);
		string line,location,opcode,word;
		while (getline(fin, line))
		{
			fout << "\n";
			stringstream ss(line);
			ss >> location>>opcode;
			if (opcode == "(IS,")
			{
				ss >> word;
				fout << location << " " << word.substr(0, word.length() - 1);
				while (ss >> word)
				{
					if (word == "(S,")
					{
						ss >> word;
						int index = stoi(word.substr(0, word.length() - 1))-1;
						fout << " " << SymTab[index].second;

					}
					else if (word == "(C,")
					{
                        ss>>word;
                        fout<<word.substr(0,word.length()-1);
					}
					else
					{
						fout << " "<<word;
					}
				}
			}
			else if (opcode == "(DL,")
			{
				ss >> word;
				if (word == "02)")
				{
					fout << location << " 00 00 00 ";
				}
				else if (word == "01)")
				{
					fout << location << " 00 00 ";
					ss >> word;
					ss >> word;
					fout << word.substr(0, word.length() - 1);
				}
			}
		}
	}
};
int main()
{
	Pass2 obj;
	obj.genearate();
	return 0;
}
