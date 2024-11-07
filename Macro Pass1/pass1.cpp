#include<iostream>
#include<vector>
#include<sstream>
#include<fstream>
#include<tuple>
using namespace std;
class Macro
{
	vector<vector<string>>MyToken;
	vector<vector<string>>MDT;
	vector<pair<string, string>>KPDT;
	vector<tuple<string, int, int, int, int>>MNT;
	vector<vector<string>>PNTAB;
public:
	Macro()
	{
		string line,word;
		ifstream fin;
		fin.open("input.txt");
		if (fin.fail())
		{
			cout << "\nError in opening file";
			exit(1);
		}
		while (getline(fin, line))
		{
			stringstream ss(line);
			vector<string>token;
			while (ss >> word)
			{
				token.push_back(word);
			}
			MyToken.push_back(token);
		}
	}
	void displayToken()
	{
		for (int i = 0; i < MyToken.size(); i++)
		{
			for (int j = 0; j < MyToken[i].size(); j++)
			{
				cout << MyToken[i][j] << " ";
			}
			cout << endl;
		}
	}
	int deal(string token, int size)
	{
		if (find(PNTAB[size].begin(), PNTAB[size].end(), token)!=PNTAB[size].end())
		{
			return (find(PNTAB[size].begin(), PNTAB[size].end(), token) - PNTAB[size].begin()) + 1;
		}
		return -1;
	}
	int storeDef(int i)
	{
		int kp = 0, pp = 0;
		int kpdtp = KPDT.size();
		int mdtp = MDT.size();
		string name = MyToken[i][0];
		vector<string>temppntab;
		for (int j= 1; j< MyToken[i].size(); j++)
		{
			string curr = MyToken[i][j];
			if (curr.find("=") != string::npos)
			{
				int index = curr.find("=");
				kp++;
				string keyword = curr.substr(0,index);
				string value=curr.substr(index+1,curr.length()-index-2);
				KPDT.push_back({ keyword,value });
				temppntab.push_back(keyword);
			}
			else
			{
				temppntab.push_back(curr.substr(0, curr.length() - 1));
				pp++;
			}
		}
	
		MNT.emplace_back(name, pp, kp, mdtp, kpdtp);
		PNTAB.push_back(temppntab);
		int j;
		for (j = i + 1; j < MyToken.size() && MyToken[j][0] != "MEND";j++)
		{
			vector<string>v1;
			v1.push_back(MyToken[j][0]);
			for (int k = 1; k < MyToken[j].size(); k++)
			{
				int index = deal(MyToken[j][k].substr(0,MyToken[j][k].length()-1), PNTAB.size() - 1);
				if (index == -1)
				{
					v1.push_back(MyToken[j][k]);
				}
				else
				{
					v1.push_back("(p," + to_string(index) + ")");
				}
			}
			MDT.push_back(v1);
		}
		return j;
	}
	void displayMNT()
	{
		cout << "\nDisplaying MNT:";
		for (tuple<string, int, int, int, int>t : MNT)
		{
			cout <<"\n"<< get<0>(t) << " " << get<1>(t) << " " << get<2>(t) << " " << get<3>(t) << " " << get<4>(t);
		}
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
	void displayKPDT()
	{
		cout << "\nDisplaying KPDT:";
		for (pair<string, string>p : KPDT)
		{
			cout << "\n" << p.first << " " << p.second;
		}
	}
	void Tokenize()
	{
		for (int i = 0; i < MyToken.size(); i++)
		{
			if (MyToken[i][0] == "MACRO")
			{
				i=storeDef(i + 1);
			}
			
		}
	}
};
int main()
{
	Macro m1;
	m1.displayToken();
	m1.Tokenize();
	m1.displayMNT();
	m1.displayPNTAB();
	m1.displayKPDT();
	m1.displayMDT();
	return 0;
}
