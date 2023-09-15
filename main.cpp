#include <algorithm>
#include <string>
#include <iostream>
#include <vector>
#include <regex>
#include <unordered_set>

#include "numgen.hpp"
#include "statreadandprocess.hpp"

int main()
{

	std::vector<int> stats = statget();
	std::vector<int> value;
	std::vector<int> number;
	std::vector<std::string> rollSigns;
	std::vector<std::vector<int>> rolls;
	int modifier;
	std::regex tokenRegex("([+-]?)([^+-]+)");
	std::regex whitespaceRegex("\\s+");
	std::regex dicecheck("\\d*d\\d+");
	std::vector<std::pair<std::string, std::string>> tokens;
	std::cout << "Commands:\nRoll: \"XdX+X\", \"X\", \"Coinflip\"\nBase Stat Check: First three letters or name (i.e. \"str\" or \"strength\" for strength check)\n"
		"Ability Check: First 4 letters or ability name (i.e. \"perc\" or \"perception\" for perception check)\n"
		"Ability Saving Throw: First 3 letters + \"t\" or \" save\" or saving throw name (i.e. \"strt\", \"str save\", or \"strength saving throw\" for strength saving throw)\n"
		"Death Saving Throw: \"deat\", \"death save\", or \"death saving throw\"\n"
		"Spell Attack: \"spell attack\" or \"spell\" (To choose ability other than default, add the ability name to the end, i.e. \"spell attack wisdom\" or \"spell int\")\n"
		"Quit: \"exit\"\n"
		"*Note: add \" adv\" or \" advantage\" to an ability or saving throw roll to add advantage (i.e. \"perc adv\" rolls perception with advantage)\n\n";
	do 
	{
		//reset values
		value.clear();
		number.clear();
		tokens.clear();
		rolls.clear();
		rollSigns.clear();
		modifier = 0;
		//take and format input
		std::cout << "Enter Roll Prompt\n";
		std::string input;
		std::getline(std::cin, input);
		transform(input.begin(), input.end(), input.begin(), ::tolower);
		input = std::regex_replace(input, whitespaceRegex, "");
		if (input.size() == 0) { std::cout << "No Entry\n"; continue; }
		//checks if input is a valid string entry
		if (input == "exit") break;
		std::string check = processAltStringToken(input);
		if (check != "nomatch") { std::cout << check << std::endl << std::endl; continue; }
		std::pair<int, int> keywordCheck = processStringToken(input, stats);
		if (keywordCheck.second == 1)
		{
			int roll1 = roll(1, 20);
			std::cout << "Rolled " << (roll1 + keywordCheck.first) << std::endl << "(" << roll1 << ") ";
			if (keywordCheck.first < 0) std::cout << "- " << keywordCheck.first << std::endl << std::endl;
			else std::cout << "+ " << keywordCheck.first << std::endl << std::endl;
			continue;
		}
		else if (keywordCheck.second == 2)
		{
			rolls.push_back(std::vector<int>());
			int roll1 = roll(1, 20);
			int roll2 = roll(1, 20);
			if (roll1 > roll2) std::cout << "Rolled " << (roll1 + keywordCheck.first) << std::endl;
			else std::cout << "Rolled " << (roll2 + keywordCheck.first) << std::endl;
			std::cout << "(" << roll1 << ", " << roll2 << ") ";
			if (keywordCheck.first < 0) std::cout << "- " << keywordCheck.first << std::endl << std::endl;
			else std::cout << "+ " << keywordCheck.first << std::endl << std::endl;
			continue;
		}
		//tokenize into entries and signs
		std::sregex_iterator tokenIterator(input.begin(), input.end(), tokenRegex);
		std::sregex_iterator endIterator;
		while (tokenIterator != endIterator) 
		{
			std::smatch match = *tokenIterator;
			std::string sign = match[1].str();
			std::string token = match[2].str();
			tokens.emplace_back(sign, token);
			++tokenIterator;
		}
		if (tokens[0].second.find_first_not_of("0123456789") == std::string::npos) tokens[0].second.insert(tokens[0].second.begin(), 'd');
		//process entries
		bool failure = false;
		int num;
		int val;
		for (const auto& tokenPair : tokens) 
		{
			if (std::regex_match(tokenPair.second, dicecheck) == true)
			{
				size_t pos = tokenPair.second.find_first_of("d");
				if (pos == 0) { num = 1; val = stoi(tokenPair.second.substr(1, tokenPair.second.size() - 1));  }
				else { num = stoi(tokenPair.second.substr(0, pos)); val = stoi(tokenPair.second.substr(pos + 1, tokenPair.second.size() - pos - 1)); }
				value.push_back(val); number.push_back(num);
				if (tokenPair.first == "-") rollSigns.push_back("-");
				else rollSigns.push_back("+");
			}
			else if (tokenPair.second.find_first_not_of("0123456789") == std::string::npos)
			{
				if (tokenPair.first == "-") { modifier = modifier - stoi(tokenPair.second); }
				else { modifier = modifier + stoi(tokenPair.second); }
			}
			else
			{
				std::cout << "Invalid Entry\n";
				failure = true;
				break;
			}
		}
		if (failure == true) continue;
		else
		{
			rolls = diceset(number, value);
			printRolls(rolls, rollSigns, modifier);
		}

	} while (true);
}
