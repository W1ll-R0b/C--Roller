#include <functional>
#include <iostream>
#include <fstream>
#include <numeric>
#include <random>
#include <string>
#include <vector>
#include <regex>
#include <algorithm>
using namespace std;

int roll(int x) 
{
    if (x == 0)
    {
        return 0;
    }
    else
    {
        random_device rd;
        mt19937 engine{rd()};
        uniform_int_distribution<> randomDie(1, x);
        return randomDie(engine);
    }

}

void abilitychecks(string input);
void print(vector<int> Rolled, string input, int diceno, int value, int modifier);

vector<int> statget()
{
ifstream statfile("stats.txt");
vector<int> stats;
if(statfile.is_open())
{
    vector<string> statlines;
    string temp;

    while(getline(statfile, temp))
    {
        if (temp == "" || temp[0] == '*')
        {
            continue;
        }
        else if (temp[0] == '{')
        {
                if (temp[1] == 'X')
                {
                    stats.push_back(1);
                }
                else
                {
                    stats.push_back(0);
                }
        }
        else if (temp.find("DEFAULT SPELLCASTING ABILITY:") != string::npos)
        {
            regex re1 ("[Cc][Hh][Aa][Rr][Ii][Ss][Mm][Aa]");
            regex re2 ("[Ii][Nn][Tt][Ee][Ll][Ll][Ii][Gg][Ee][Nn][Cc][Ee]");
            regex re3 ("[Ww][Ii][Ss][Dd][Oo][Mm]");
            if (regex_search(temp, re1) == true)
            {
                stats.push_back(0);
            }
            else if (regex_search(temp, re2) == true)
            {
                stats.push_back(1);
            }
            else if (regex_search(temp, re3) == true)
            {
                stats.push_back(2);
            }
            else
            {
                stats.push_back(3);
            }
            

        }
        else
        {
            size_t loco = temp.find_first_of("-+123456789");
            if (loco == string::npos)
            {
                continue;
            }
            temp = temp.substr(loco, temp.size() - loco);
            if (temp.find_first_not_of("-+123456789") == string::npos)
            {
                stats.push_back(stoi(temp));
            }
            else
            {
                stats.push_back(0);
            }
        }
    }
}
return stats;
}

int main() {
    cout << "Commands:\nRoll: \"XdX+X\", \"XdX-X\", \"XdX\", \"X\"\nGenerate Character Stats: \"stat gen\"\nBase Stat Check: First three letters or stat name (i.e. \"str\" or \"strength\" for strength check)\n"
    "Ability Check: First 4 letters or ability name (i.e. \"perc\" or \"perception\" for perception check)\n"
    "Ability Saving Throw: First 3 letters + \"t\" or \" save\" or saving throw name (i.e. \"strt\", \"str save\", or \"strength saving throw\" for strength saving throw)\n"
    "Death Saving Throw: \"deat\", \"death save\", or \"death saving throw\"\n"
    "Spell Attack: \"spell attack\" or \"spell\" (To choose ability other than default, add the ability name to the end, i.e. \"spell attack wisdom\" or \"spell int\")\n"
    "Quit: \"exit\"\n"
    "*Note: add \" adv\" or \" advantage\" to an ability or saving throw roll to add advantage (i.e. \"perc adv\" rolls perception with advantage\n\n";
    regex reg1 ("\\d*[dD]\\d{1,100}[+-]\\d{1,100}");
    regex reg2 ("\\d*[dD]\\d{1,100}");
    regex reg3 ("\\d{1,100}[+-]\\d{1,100}");
    regex reg4 ("[Ss][Tt][Aa][Tt][-\\s][Gg][Ee][Nn]");
    vector<int> Rolled;
    int diceno = 1;
    int modifier = 0;
    int value = 0;
    while (true) {
        Rolled.clear();
        diceno = 1;
        modifier = 0;
        cout << "\nEnter dice value or ability to roll, \"stat gen\" for character stat "
                "generation, or \"exit\" to quit\n\n";
        string input;
        getline(cin, input);
        if (regex_match(input, reg4) == true) {
            vector<int> stats;
            vector<int> sums;
            sums.clear();
            int iii = 0;
            while (iii < 6) {
                stats.clear();
                int iiii = 0;
                while (iiii < 4) {
                stats.push_back(roll(6));
                iiii++;
                }
                sort(stats.begin(), stats.end());
                int sum = stats[1] + stats[2] + stats[3];
                sums.push_back(sum);
                cout << "\nRolled " << sum << endl
                    << "(" << stats[3] << ", " << stats[2] << ", " << stats[1] << ", *"
                    << stats[0] << "*)\n";
                iii++;
            }
            float average = (accumulate(sums.begin(), sums.end(), 0) / 6.00);
            cout << "Average: " << average << endl;
            continue;
            }
        else if(regex_match(input, reg1) == true)
        {
            size_t dplace = input.find_first_of("dD");
            size_t modplace = input.find_first_of("+-");
            if (input[0] != 'd' && input[0] != 'D')
            {
                diceno = stoi(input.substr(0, dplace));

            }
            value = stoi(input.substr(dplace + 1, (modplace - dplace)));
            modifier = stoi(input.substr(modplace + 1, (input.size() - modplace)));
        }
        else if(regex_match(input, reg2) == true)
        {

            size_t dplace = input.find_first_of("dD");
            if (input[0] != 'd' && input[0] != 'D')
            {
                diceno = stoi(input.substr(0, dplace));
            }
            value = stoi(input.substr(dplace + 1, (input.size() - dplace)));
        }
        else if(regex_match(input, reg3) == true)
        {
            size_t modplace = input.find_first_of("+-");
            value = stoi(input.substr(0, modplace));
            modifier = stoi(input.substr(modplace + 1, (input.size() - modplace)));
        }
        else if(input.find_first_not_of("0123456789") == string::npos)
        {
            value = stoi(input);
        }
        else if(input == "exit")
        {
            break; 
        }
        else
        {
            abilitychecks(input);
            continue;
        }
        print(Rolled, input, diceno, value, modifier);
    }



}

void abilitychecks(string input)
{
    vector<int> stats = statget();
    bool adv = false;
    int modifier = 0;
    regex regx ("\\s[Aa][Dd][Vv]$|\\s[Aa][Dd][Vv][Aa][Nn][Tt][Aa][Gg][Ee]$");
    smatch m;
    regex_search(input, m, regx);
    if (m.empty() == false)
    {
        input = input.substr(0, m.position());
        adv = true;
    }
    transform(input.begin(), input.end(), input.begin(), ::tolower);
    if (((input == "strength saving throw" | input == "strength save") | input == "strength throw") | (input == "strt" | input == "str save"))
    {
        modifier = stats[2];
        if (stats[8] == 1)
        {
            modifier = modifier + stats[1]; 
        }
    }
    else if (((input == "dexterity saving throw" | input == "dexterity save") | input == "dexterity throw") | (input == "dext" | input == "dex save"))
    {
        modifier = stats[3];
        if (stats[9] == 1)
        {
            modifier = modifier + stats[1]; 
        }
    }
    else if (((input == "constitution saving throw" | input == "constitution save") | input == "constitution throw") | (input == "cont" | input == "con save"))
    {
        modifier = stats[4];
        if (stats[10] == 1)
        {
            modifier = modifier + stats[1]; 
        }
    }
    else if (((input == "intelligence saving throw" | input == "intelligence save") | input == "intelligence throw") | (input == "intt" | input == "int save"))
    {
        modifier = stats[5];
        if (stats[11] == 1)
        {
            modifier = modifier + stats[1]; 
        }
    }
    else if (((input == "wisdom saving throw" | input == "wisdom save") | input == "wisdom throw") | (input == "wist" | input == "wis save"))
    {
        modifier = stats[6];
        if (stats[12] == 1)
        {
            modifier = modifier + stats[1]; 
        }
    }
    else if (((input == "charisma saving throw" | input == "charisma save") | input == "charisma throw") | (input == "chat" | input == "cha save"))
    {
        modifier = stats[7];
        if (stats[13] == 1)
        {
            modifier = modifier + stats[1]; 
        }
    }
    else if (input == "strength" | input == "str")
    {
        modifier = stats[2];
    }
    else if (input == "dexterity" | input == "dex")
    {
        modifier = stats[3];
    }
    else if (input == "constitution" | input == "con")
    {
        modifier = stats[4];
    }
    else if (input == "intelligence" | input == "int")
    {
        modifier = stats[5];
    }
    else if (input == "wisdom" | input == "wis")
    {
        modifier = stats[6];
    }
    else if (input == "charisma" | input == "cha")
    {
        modifier = stats[7];
    }
    else if (input == "acrobatics" | input == "acro")
    {
        modifier = stats[3];
        if (stats[14] == 1)
        {
            modifier = modifier + stats[1]; 
        }
    }
    else if (input == "animal handling" | input == "anim")
    {
        modifier = stats[6];
        if (stats[15] == 1)
        {
            modifier = modifier + stats[1]; 
        }
    }
    else if (input == "arcana" | input == "arca")
    {
        modifier = stats[5];
        if (stats[16] == 1)
        {
            modifier = modifier + stats[1]; 
        }
    }
    else if (input == "athletics" | input == "athl")
    {
        modifier = stats[2];
        if (stats[17] == 1)
        {
            modifier = modifier + stats[1]; 
        }
    }
    else if (input == "deception" | input == "dece")
    {
        modifier = stats[7];
        if (stats[18] == 1)
        {
            modifier = modifier + stats[1]; 
        }
    }
    else if (input == "history" | input == "hist")
    {
        modifier = stats[5];
        if (stats[19] == 1)
        {
            modifier = modifier + stats[1]; 
        }
    }
    else if (input == "insight" | input == "insi")
    {
        modifier = stats[6];
        if (stats[20] == 1)
        {
            modifier = modifier + stats[1]; 
        }
    }
    else if (input == "intimidation" | input == "inti")
    {
        modifier = stats[7];
        if (stats[21] == 1)
        {
            modifier = modifier + stats[1]; 
        }
    }
    else if (input == "investigation" | input == "inve")
    {
        modifier = stats[5];
        if (stats[22] == 1)
        {
            modifier = modifier + stats[1]; 
        }
    }
    else if (input == "medicine" | input == "medi")
    {
        modifier = stats[6];
        if (stats[23] == 1)
        {
            modifier = modifier + stats[1]; 
        }
    }
    else if (input == "nature" | input == "natu")
    {
        modifier = stats[5];
        if (stats[24] == 1)
        {
            modifier = modifier + stats[1]; 
        }
    }
    else if (input == "perception" | input == "perc")
    {
        modifier = stats[6];
        if (stats[25] == 1)
        {
            modifier = modifier + stats[1]; 
        }
    }
    else if (input == "performance" | input == "perf")
    {
        modifier = stats[7];
        if (stats[26] == 1)
        {
            modifier = modifier + stats[1]; 
        }
    }
    else if (input == "persuasion" | input == "pers")
    {
        modifier = stats[7];
        if (stats[27] == 1)
        {
            modifier = modifier + stats[1]; 
        }
    }
    else if (input == "religion" | input == "reli")
    {
        modifier = stats[5];
        if (stats[28] == 1)
        {
            modifier = modifier + stats[1]; 
        }
    }
    else if (input == "sleight of hand" | input == "slei")
    {
        modifier = stats[3];
        if (stats[29] == 1)
        {
            modifier = modifier + stats[1]; 
        }
    }
    else if (input == "stealth" | input == "stea")
    {
        modifier = stats[3];
        if (stats[30] == 1)
        {
            modifier = modifier + stats[1]; 
        }
    }
    else if (input == "survival" | input == "surv")
    {
        modifier = stats[6];
        if (stats[31] == 1)
        {
            modifier = modifier + stats[1]; 
        }
    }
    else if (input == "death saving throw" | (input == "deat" | input == "death save"))
    {
        modifier = 0;
    }
    else if (input == "spell attack" | input == "spell")
    {
        if (stats[0] == 0)
        {
            modifier = stats[7] + stats[1]; 
        }
        else if (stats[0] == 1)
        {
            modifier = stats[5] + stats[1]; 
        }
        else if (stats[0] == 2)
        {
            modifier = stats[6] + stats[1]; 
        }
        else
        {
            cout << "\nNo or invalid ability set as default\n";
            return;
        }
    }
    else if ((input == "spell attack cha" | input == "spell cha") | (input == "spell attack charisma" | input == "spell charisma"))
    {

        modifier = stats[7] + stats[1]; 

    }
    else if ((input == "spell attack int" | input == "spell int") | (input == "spell attack intelligence" | input == "spell intelligence"))
    {

        modifier = stats[5] + stats[1]; 

    }
    else if ((input == "spell attack wis" | input == "spell wis") | (input == "spell attack wisdom" | input == "spell wisdom"))
    {

        modifier = stats[6] + stats[1]; 

    }
    else
    {
        cout << "\nInvalid Output\n";
        return;
    }
    if (adv == true)
    {
        int r1 = roll(20);
        int r2 = roll(20);
        int sum;
        if (r1 > r2)
        {
            sum = r1;
            r1 = r2;
            r2 = sum;
            sum = sum + modifier;
        }
        else
        {
            sum = r2 + modifier;
        }
        cout << "\nRolled " << sum <<"\n(" << r2 << ", *" << r1 << "*) ";
        if (modifier < 0)
        {
            cout << "- " << abs(modifier) << endl;
        }
        else if (modifier == 0)
        {
            cout << endl;
        }
        else
        {
            cout << "+ " << modifier << endl;
        }
    }
    else
    {
        int r1 = roll(20);
        int sum = r1 + modifier;
        cout << "\nRolled " << sum <<"\n(" << r1 << ") ";
        if (modifier < 0)
        {
            cout << modifier << endl;
        }
        else if (modifier == 0)
        {
            cout << endl;
        }
        else
        {
            cout << "+ " << modifier << endl;
        }
    }
}

void print(vector<int> Rolled, string input, int diceno, int value, int modifier)
{
    int check = 0;
    for (int i = 0; i < diceno; i++) 
    {
        Rolled.push_back(roll(value));
    }

    int sum = accumulate(Rolled.begin(), Rolled.end(), 0);
    if (modifier != 0) {
    if (input.find("+") != string::npos) 
    {
       sum = sum + modifier;
            check = 1;
    }
    else if (input.find("-") != string::npos) 
    {
        sum = sum - modifier;
        check = 2;
    }
    }
    cout << "\nRolled " << sum;
    int ii = 0;
    cout << "\n(";
    while (ii < Rolled.size()) 
    {
        cout << Rolled[ii];
        if (ii < (Rolled.size() - 1)) 
        {
            cout << " + ";
        }
        ii++;
    }
    cout << ")";
    if (check == 1)
    {
        cout << " + " << modifier << endl;
    }
    else if (check == 2)
    {
        cout << " - " << modifier << endl;
    }
    else
    {
        cout << endl;
    }
}