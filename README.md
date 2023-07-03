# Dnd-Character-Roller
This is a simple c++ program for dnd character roll management. The program works by editing a .txt file that records the character's modifiers and proficiencies. Random roll generation is done through a mersenne twister engine.

# Setup:

Download the .zip containing the .exe and .txt file. To configure stats and modifiers, edit the .txt file in accordance with character sheet.

# Commands:

Roll: "XdX+X", "XdX-X", "XdX\", "X"

Generate Character Stats: "stat gen"

Base Stat Check: First three letters or stat name (i.e. "str" or "strength" for strength check)

Ability Check: First 4 letters or ability name (i.e. "perc" or "perception" for perception check)

Ability Saving Throw: First 3 letters + "t" or " save" or saving throw name (i.e. "strt", "str save", or "strength saving throw" for strength saving throw)

Death Saving Throw: "deat", "death save", or "death saving throw"

Spell Attack: "spell attack" or "spell" (To choose ability other than default, add the ability name to the end, i.e. "spell attack wisdom" or "spell int")

Quit: "exit"

*Note: add " adv" or " advantage" to an ability or saving throw roll to add advantage (i.e. "perc adv" rolls perception with advantage
