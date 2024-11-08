#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <tuple>
#include <algorithm>

using namespace std;

class MacroProcessor
{
private:
    vector<tuple<string, int, int, int, int>> MNTAB; // Macro Name Table
    vector<tuple<string, string>> KDTAB;             // Keyword Definition Table
    vector<vector<string>> MDT;                      // Macro Definition Table
    vector<vector<string>> PNTAB;                    // Parameter Name Table
    vector<vector<string>> myToken;                  // Tokenized input lines

    void displayToken();
    void displayAll();
    void displayAPTAB(string& macroName, vector<string>& APTAB);
    int deal(string& token, int index);
    int findInMNT(string& name);
    string getValueFromKDTAB(string& key, int KDTP);
    int getKeywordIndex(string& keyword, int macroIndex);
    int getMacroLength(string& macName);
    int storeDefinition(int lineNum);
    void expandMacro(vector<string>& currSentence, ofstream& opFile);

public:
    MacroProcessor();
    void pass1();
    void pass2();
};

MacroProcessor::MacroProcessor()
{
    ifstream ipFile("macro_input_2.txt");
    if (!ipFile) {
        cerr << "Error opening file" << endl;
        exit(1);
    }

    string buffer;
    while (getline(ipFile, buffer))
    {
        stringstream ss(buffer);
        vector<string> token;
        string word;
        while (ss >> word)
        {
            if (word != ",")
                token.push_back(word);
        }
        myToken.push_back(token);
    }
    displayToken();
}

void MacroProcessor::displayToken()
{
    for (int i = 0; i < myToken.size(); i++)
    {
        cout << i << " :";
        for (string word : myToken[i])
        {
            cout << word << "\t";
        }
        cout << endl;
    }
}

void MacroProcessor::displayAll()
{
    cout << "\n---------MNT-----------\n";
    cout << "Name\t#PP\t#KP\t#MDTP\t#KDTP\n";
    for (tuple<string, int, int, int, int>entry : MNTAB)
    {
        cout << get<0>(entry) << "\t" << get<1>(entry) << "\t" << get<2>(entry) << "\t" << get<3>(entry) << "\t" << get<4>(entry) << "\n";
    }
    cout << "-------------------------\n";

    cout << "\n---------MDT-----------\n";
    for (size_t i = 0; i < MDT.size(); i++)
    {
        cout << "Line " << i + 1 << ": ";
        for (auto& word : MDT[i])
        {
            cout << word << " ";
        }
        cout << endl;
    }
    cout << "-------------------------\n";

    cout << "\n--------KDTAB----------\n";
    cout << "Keyword\tValue\n";
    for (tuple<string, string> entry : KDTAB)
    {
        cout << get<0>(entry) << "\t" << get<1>(entry) << endl;
    }
    cout << "-------------------------\n";
    cout << "\n--------PNTAB----------\n";
    for (vector<string>line : PNTAB)
    {
        for (string para : line)
        {
            cout << para << "\t";
        }
        cout << endl;
    }
    cout << "-----------------------\n";
}

void MacroProcessor::displayAPTAB(string& macroName, vector<string>& APTAB)
{
    cout << "\n--------APTAB[" << macroName << "]------------\n";
    for (size_t k = 0; k < APTAB.size(); k++)
    {
        cout << k << " :" << APTAB[k] << endl;
    }
    cout << "\n-------------------------------------------\n";
}

int MacroProcessor::deal(string& token, int index)
{
    if (index < 0 || index >= PNTAB.size()) return -1;

    vector<string>parameters = PNTAB[index];
    auto it = find(parameters.begin(), parameters.end(), token);
    if (it != parameters.end())
        return distance(parameters.begin(), it) + 1;
    return -1;
}

int MacroProcessor::findInMNT(string& name)
{
    for (size_t i = 0; i < MNTAB.size(); i++)
    {
        if (get<0>(MNTAB[i]) == name)
            return static_cast<int>(i);
    }
    return -1;
}

string MacroProcessor::getValueFromKDTAB(string& key, int KDTP)
{
    if (KDTP < 0 || KDTP >= static_cast<int>(KDTAB.size())) return "Not defined"; // Prevent out-of-bounds access

    for (int i = KDTP; i < static_cast<int>(KDTAB.size()); i++)
    {
        if (get<0>(KDTAB[i]) == key)
            return get<1>(KDTAB[i]);
    }
    return "Not defined";
}

int MacroProcessor::getKeywordIndex(string& keyword, int macroIndex)
{
    if (macroIndex < 0 || macroIndex >= static_cast<int>(PNTAB.size())) return -1; // Prevent out-of-bounds access

    auto& currParameters = PNTAB[macroIndex];
    auto it = find(currParameters.begin(), currParameters.end(), keyword);
    if (it != currParameters.end())
        return distance(currParameters.begin(), it);
    return -1;
}

int MacroProcessor::getMacroLength(string& macName)
{
    int index = findInMNT(macName);
    if (index == -1) return 0;

    int MDTP = get<3>(MNTAB[index]);
    for (int k = MDTP; k < static_cast<int>(MDT.size()); k++)
    {
        if (MDT[k][0] == "MEND")
        {
            return k + 1 - MDTP;
            return k + 1 - MDTP;
        }
    }
    return 0;
}

int MacroProcessor::storeDefinition(int lineNum)
{
    if (lineNum < 0 || lineNum >= myToken.size())return lineNum; // Prevent out-of-bounds access

    string name;
    vector<string> tempPNTAB;
    int PP = 0, KP = 0, KDTP, MDTP, j;
    KDTP = KDTAB.size();
    MDTP = MDT.size();
    name = myToken[lineNum][0];

    for (int i = 1; i < myToken[lineNum].size(); i++)
    {
        string currToken = myToken[lineNum][i];
        if (currToken.find('=') != string::npos)
        {
            KP++;
            int index = currToken.find('=');
            string value = currToken.substr(index + 1);
            string keyword = currToken.substr(0, index);
            KDTAB.emplace_back(keyword, value);
            tempPNTAB.push_back(keyword);
        }
        else
        {
            tempPNTAB.push_back(currToken);
        }
    }
    PP = tempPNTAB.size() - KP;
    PNTAB.push_back(tempPNTAB);

    for (j = lineNum + 1; j < myToken.size() && myToken[j][0] != "MEND"; j++)
    {
        vector<string> V1;
        for (string token : myToken[j])
        {
            int index = deal(token, PNTAB.size() - 1);
            if (index != -1)
            {
                V1.push_back(" (P," + to_string(index) + ")");
            }
            else
            {
                V1.push_back(token);
            }
        }
        MDT.push_back(V1);
    }
    MDT.push_back({ "MEND" });
    MNTAB.emplace_back(name, PP, KP, MDTP, KDTP);
    return j;
}

void MacroProcessor::expandMacro(vector<string>& currSentence, ofstream& opFile)
{
    int index = findInMNT(currSentence[0]);
    if (index == -1) return;

    int numPP = get<1>(MNTAB[index]);
    int numKP = get<2>(MNTAB[index]);
    int MDTP = get<3>(MNTAB[index]);
    int KDTP = get<4>(MNTAB[index]);

    vector<string> APTAB(numKP + numPP, "NULL");
    int i;
    for (i = 1; i <= numPP; i++)
    {
        APTAB[i - 1] = currSentence[i];
    }

    for (; i < static_cast<int>(currSentence.size()); i++)
    {
        auto loc = currSentence[i].find('=');
        if (loc != string::npos)
        {
            string value = currSentence[i].substr(loc + 1);
            string keyword = currSentence[i].substr(0, loc);
            APTAB[getKeywordIndex(keyword, index)] = value;
        }
    }

    for (int i = 0; i < static_cast<int>(APTAB.size()); i++)
    {
        if (APTAB[i] == "NULL")
        {
            string key = PNTAB[index][i];
            APTAB[i] = getValueFromKDTAB(key, KDTP);
        }
    }
    displayAPTAB(get<0>(MNTAB[index]), APTAB);

    for (int i = MDTP; i < static_cast<int>(MDT.size()) && MDT[i][0] != "MEND"; i++)
    {
        for (auto& instruct : MDT[i])
        {
            if (instruct.find('(') != string::npos)
            {
                int temp = stoi(instruct.substr(4, 1));
                opFile << APTAB[temp - 1] << " ";
            }
            else
            {
                opFile << instruct << " ";
            }
        }
        opFile << endl;
    }
}

void MacroProcessor::pass1()
{
    for (int i = 0; i < myToken.size(); i++)
    {
        vector<string>curr = myToken[i];
        if (find(curr.begin(), curr.end(), "MACRO") != curr.end())
        {
            i = storeDefinition(i + 1);
        }
    }
}

void MacroProcessor::pass2()
{
    ofstream opFile("expanded_Code.txt");
    if (!opFile)
    {
        cerr << "Error opening output file" << endl;
        exit(1);
    }
    pass1();
    displayAll();
    for (vector<string>tokens : myToken)
    {
        if (find(tokens.begin(), tokens.end(), "MACRO") == tokens.end())
        {
            expandMacro(tokens, opFile);
        }
    }
    opFile.close();
}

int main()
{
    MacroProcessor mp;
    mp.pass2();
    return 0;
}

