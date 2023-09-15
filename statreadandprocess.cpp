#include <vector>
#include <string>
#include <fstream>
#include <regex>
#include <map>
#include <unordered_map>
#include <numeric>
#include <iostream>

#include "numgen.hpp"

inline std::vector<int> statget()
{
    //vector stats: Casting Ability (0=Cha 1=Int 2=Wis), Prof, Str, Dex, Con, Int, Wis, Cha, strt, dext, cont, intt, wist, chat, 
    //              acro, anim, arca, athl, dece, hist, insi, inti, inve, medi, natu, perc, perf, pers, reli, slei, stea, surv
    std::ifstream statfile("stats.txt");
    std::vector<int> stats;
    if (statfile.is_open())
    {
        std::vector<std::string> statlines;
        std::string temp;

        while (std::getline(statfile, temp))
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
            else if (temp.find("DEFAULT SPELLCASTING ABILITY:") != std::string::npos)
            {
                std::regex re1("[Cc][Hh][Aa][Rr][Ii][Ss][Mm][Aa]");
                std::regex re2("[Ii][Nn][Tt][Ee][Ll][Ll][Ii][Gg][Ee][Nn][Cc][Ee]");
                std::regex re3("[Ww][Ii][Ss][Dd][Oo][Mm]");
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
                if (loco == std::string::npos)
                {
                    continue;
                }
                temp = temp.substr(loco, temp.size() - loco);
                if (temp.find_first_not_of("-+123456789") == std::string::npos)
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
};

inline std::pair<int, int> processStringToken(std::string token, std::vector<int>& stats)
{
    std::pair<int, int> result(0, -1);
    bool advCheck = false;
    if (token.size() > 3)
    {
        if (token.substr(token.size() - 3) == "adv")
        {
            advCheck = true;
            token = token.substr(0, token.size() - 3);
        }
        else if (token.substr(token.size() - 3) == "advantage")
        {
            advCheck = true;
            token = token.substr(0, token.size() - 8);
        }
        if (token == "spellattack" || token == "spell")
        {
            switch (stats[0])
            {
            case 0: token = "spellcha";
            case 1: token = "spellint";
            case 2: token = "spellwis";
            }
        }
    }
    std::map<std::string, std::pair<int, int>> statMap = {
    {"deathsavingthrow", {99, -1}}, {"deathsave", {-1, -1}}, {"deaththrow", {-1, -1}}, {"death", {-1, -1}}, {"deas", {-1, -1}}, {"deat", {-1, -1}},
    {"strengthsavingthrow", {2, 8}}, {"strengthsave", {2, 8}}, {"strengththrow", {2, 8}}, {"strst", {2, 8}}, {"strs", {2, 8}}, {"strt", {2, 8}},
    {"strength", {2, -1}}, {"str", {2, -1}},
    {"dexteritysavingthrow", {3, 9}}, {"dexteritysave", {3, 9}}, {"dexteritythrow", {3, 9}}, {"dexst", {3, 9}}, {"dexs", {3, 9}}, {"dext", {3, 9}},
    {"dexterity", {3, -1}}, {"dex", {3, -1}},
    {"constitutionsavingthrow", {4, 10}}, {"constitutionsave", {4, 10}}, {"constitutionthrow", {4, 10}}, {"const", {4, 10}}, {"cons", {4, 10}}, {"cont", {4, 10}},
    {"constitution", {4, -1}}, {"con", {4, -1}},
    {"intelligencesavingthrow", {5, 11}}, {"intelligencesave", {5, 11}}, {"intelligencethrow", {5, 11}}, {"intst", {5, 11}}, {"ints", {5, 11}}, {"intt", {5, 11}},
    {"intelligence", {5, -1}}, {"int", {5, -1}},
    {"wisdomsavingthrow", {6, 12}}, {"wisdomsave", {6, 12}}, {"wisdomthrow", {6, 12}}, {"wisst", {6, 12}}, {"wiss", {6, 12}}, {"wist", {6, 12}},
    {"wisdom", {6, -1}}, {"wis", {6, -1}},
    {"charismasavingthrow", {7, 13}}, {"charismasave", {7, 13}}, {"charismathrow", {7, 13}}, {"chast", {7, 13}}, {"chas", {7, 13}}, {"chat", {7, 13}},
    {"charisma", {7, -1}}, {"cha", {7, -1}},
    {"acrobatics", {3, 14}}, {"acro", {3, 14}},
    {"animalhandling", {6, 15}}, {"anim", {6, 15}},
    {"arcana", {5, 16}}, {"arca", {5, 16}},
    {"athletics", {2, 17}}, {"athl", {2, 17}},
    {"deception", {7, 18}}, {"dece", {7, 18}},
    {"history", {5, 19}}, {"hist", {5, 19}},
    {"insight", {6, 20}}, {"insi", {6, 20}},
    {"intimidation", {7, 21}}, {"inti", {7, 21}},
    {"investigation", {5, 22}}, {"inve", {5, 22}},
    {"medicine", {6, 23}}, {"medi", {6, 23}},
    {"nature", {5, 24}}, {"natu", {5, 24}},
    {"perception", {6, 25}}, {"perc", {6, 25}},
    {"performance", {7, 26}}, {"perf", {7, 26}},
    {"persuasion", {7, 27}}, {"pers", {7, 27}},
    {"religion", {5, 28}}, {"reli", {5, 28}},
    {"sleightofhand", {3, 29}}, {"slei", {3, 29}},
    {"stealth", {3, 30}}, {"stea", {3, 30}},
    {"survival", {6, 31}}, {"surv", {6, 31}},
    {"spellattackintelligence", {5, 1}}, {"spellint", {5, 1}},
    {"spellattackwisdom", {6, 1}}, {"spellwis", {6, 1}},
    {"spellattackcharisma", {7, 1}}, {"spellcha", {7, 1}},
    };

    int modifier2 = 0;
    auto it = statMap.find(token);
    if (it != statMap.end()) 
    {
        int base = stats[it->second.first];
        if (base != 99) modifier2 = base;
        int prof = it->second.second;
        if (prof != -1) if (stats[prof] == 1) modifier2 += stats[1];
        result.first = modifier2;
        if (advCheck == true) result.second = 2;
        else result.second = 1;
    }
    return result;
};

inline std::string processAltStringToken(std::string input)
{
    if (input == "8ball")
    {
        int rolled = roll(1, 20);
        std::map<int, std::string> ballresult = {
            {1, "It is certain"},
            {2, "Without a doubt"},
            {3, "You may rely on it"},
            {4, "Yes definitely"},
            {5, "It is decidedly so"},
            {6, "As I see it, yes"},
            {7, "Most likely"},
            {8, "Yes"},
            {9, "Outlook good"},
            {10, "Signs point to yes"},
            {11, "Reply hazy try again"},
            {12, "Better not tell you now"},
            {13, "Ask again later"},
            {14, "Cannot predict now"},
            {15, "Concentrate and ask again"},
            {16, "Don\'t count on it"},
            {17, "Outlook not so good"},
            {18, "My sources say no"},
            {19, "Very doubtful"},
            {20, "My reply is no"},
        };
        auto it = ballresult.find(rolled);
        return it->second;

    }
    else if (input == "coinflip")
    {
        if (roll(0, 1) == 0) return "Heads";
        else return "Tails";
    }
    else return "nomatch";
};

inline void printRolls(std::vector<std::vector<int>> rolls, std::vector<std::string> rollSigns, int modifier)
{
    int rollResult = modifier;
    int i = 0;
    std::vector<std::string> printStrings;
    std::string tempp;
    while (i < rolls.size())
    {
        tempp = "(";
        int sum = 0;
        for (int& n : rolls[i]) { sum += n; tempp = tempp + std::to_string(n) + ", "; }
        tempp = tempp.substr(0, tempp.size() - 2) + ")";
        if (rollSigns[i] == "-") rollResult = rollResult - sum;
        else rollResult = rollResult + sum;
        printStrings.push_back(tempp);
        ++i;
    }
    std::cout << "Rolled " << rollResult << std::endl;
    i = 0;
    while (i < printStrings.size())
    {
        if (i == 0 && rollSigns[i] == "+") std::cout << printStrings[i];
        else if (rollSigns[i] == "-") std::cout << " - " << printStrings[i];
        else std::cout << " - " << printStrings[i];
        ++i;
    }
    if (modifier != 0)
    {
        if (modifier < 0) std::cout << " - " << abs(modifier);
        else std::cout << " + " << modifier;
    }
    std::cout << std::endl << std::endl;
    return;
};