#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <tuple>
#include <algorithm>
using namespace std;
class Macro
{
	vector<vector<string>>MyToken;
	vector<vector<string>>MDT;
	vector<pair<string, string>>KPDT;
	vector<tuple<string, int, int, int, int>>MNT;
	vector<vector<string>>PNTAB;
	ifstream a, b, c, d,fin;
	ofstream fout;
public:
	Macro()
	{
		fout.open("expanded_code.txt");
		a.open("MDT.txt");
		b.open("PNTAB.txt");
		c.open("KPDT.txt");
		d.open("MNT.txt");
		fin.open("input.txt");
		if (fin.fail() || a.fail() || b.fail() || c.fail() || d.fail())
		{
			cout << "\nError in opening file";
			exit(1);
		}
		string line,word;
		while (getline(fin, line))
		{
			stringstream ss(line);
			vector<string>v1;
			while (ss >> word)
			{
				v1.push_back(word);
			}
			MyToken.push_back(v1);
		}
		readMDT();
		readPNTAB();
		readKPDT();
		readMNT();
	}
	void readMDT()
	{
		string line,word;
		while (getline(a, line))
		{
			vector<string>v1;
			stringstream ss(line);
			while (ss >> word)
			{
				v1.push_back(word);
			}
			MDT.push_back(v1);
		}
		displayMDT();

	}
	void displayMDT()
	{
		cout << "\nDisplaying MDT:\n";
		for (vector<string>s : MDT)
		{
			for (int i = 0; i < s.size(); i++)
			{
				cout << s[i] << " ";
			}
			cout << endl;
		}
	}
	void readPNTAB()
	{
		string line, word;
		while (getline(b, line))
		{
			vector<string>v1;
			stringstream ss(line);
			while (ss >> word)
			{
				v1.push_back(word);
			}
			PNTAB.push_back(v1);
		}
		displayPNTAB();
	}
	void displayPNTAB()
	{
		cout << "\nDisplaying PNTAB:";
		for (vector<string>s : PNTAB)
		{
			for (int i = 0; i < s.size(); i++)
			{
				cout << "\n" << s[i];
			}
		}
	}
	void readKPDT()
	{
		string line;
		while (getline(c, line))
		{
			stringstream ss(line);
			string keyword, value;
			ss >> keyword;
			ss >> value;
			KPDT.push_back({ keyword,value });
		}
		displayKPDT();
	}
	void displayKPDT()
	{
		cout << "\nDisplaying KPDT:";
		for (pair<string, string>p : KPDT)
		{
			cout << "\n" << p.first << " " << p.second;
		}
	}
	void readMNT()
	{
		string line, word;
		while (getline(d, line))
		{
			stringstream ss(line);
			tuple<string, int, int, int, int>t1;
			ss >> get<0>(t1) >> get<1>(t1) >> get<2>(t1) >> get<3>(t1) >> get<4>(t1);
			MNT.push_back(t1);
		}
		displayMNT();
	}
	void displayMNT()
	{
		cout << "\nDisplaying MNT:";
		for (tuple<string, int, int, int, int>t : MNT)
		{
			cout << "\n" << get<0>(t) << " " << get<1>(t) << " " << get<2>(t) << " " << get<3>(t) << " " << get<4>(t);
		}
	}
	void expandMacro(vector<string>token)
	{
		string macro_name = token[0];
		cout << "\nExpanding macro " << macro_name;
		fout<< "\nExpanding macro " << macro_name;
		int index = getMNTIndex(macro_name);
		if (index == -1)
		{
			cout << "\nINvalid macro call";
			return;
		}
		int pp = get<1>(MNT[index]);
		int kp = get<2>(MNT[index]);
		int MDTP = get<3>(MNT[index]);
		int KPDTP = get<4>(MNT[index]);
		vector<string>APTAB(pp + kp, "NULL");
		int i;
		for (i = 0; i < pp; i++)
		{
			APTAB[i] = token[i + 1];
		}
		for (int j = i + 1; j < token.size(); j++)
		{
			if (token[j].find("=") != string::npos)
			{
				int ind = token[j].find("=");
				string keyword = token[j].substr(0, ind);
				string value = token[j].substr(ind + 1);
				int pntindex = getPNTABIndex(index, keyword);
				if (pntindex == -1)
				{
					cout << "\n" << keyword << " is not prsent in PNTAB";
					return;
				}
				APTAB[pntindex] = value;
			}
		}
		for (int i = 0; i < APTAB.size(); i++)
		{
			if (APTAB[i] == "NULL")
			{
				string temp = PNTAB[index][i];
				for (int j = KPDTP; j < KPDT.size(); j++)
				{
					if (KPDT[j].first == temp)
					{
						APTAB[i] = KPDT[j].second;
					}
				}
			}
		}
		displayAPTAB(macro_name, APTAB);
		//Expanding MDT
		int limit;
		if (index + 1 < MNT.size())
		{
			limit = get<3>(MNT[index + 1]);
		}
		else
		{
			limit = MDT.size();
		}
		for (int i = MDTP; i < limit; i++)
		{
			fout << endl;
			for (int j = 0; j < MDT[i].size(); j++)
			{
				if (MDT[i][j].find("(") != string::npos)
				{
					int aptabind = stoi(MDT[i][j].substr(3, 1)) - 1;
					fout << " " << APTAB[aptabind];
				}
				else
				{
					fout << MDT[i][j] << " ";
				}
			}
		}
	}
	int getMNTIndex(string name)
	{
		for (int i = 0; i < MNT.size(); i++)
		{
			if (get<0>(MNT[i]) == name)
			{
				return i;
			}
		}
		return -1;
	}
	int getPNTABIndex(int index, string key)
	{
		for (int i = 0; i < PNTAB[index].size(); i++)
		{
			if (PNTAB[index][i] == key)
			{
				return i;
			}
		}
		return -1;
	}
	void displayAPTAB(string macro, vector<string>APTAB)
	{
		cout << "\nDisplaying APTAB for :" << macro;
		for (int i = 0; i < APTAB.size(); i++)
		{
			cout << "\n" << APTAB[i];
		}
	}
	void pass2()
	{
		for (int i =0; i < MyToken.size(); i++)
		{
			cout << endl;
			expandMacro(MyToken[i]);

		}
	}
};
int main()
{
	Macro m1;
	m1.pass2();
}
