#include <random>
#include <vector>

inline int roll(int x, int y)
{
    if (x > y)
    {
        return 0;
    }
    else
    {
        std::random_device rd;
        std::mt19937 engine{ rd() };
        std::uniform_int_distribution<> randomDie(x, y);
        return randomDie(engine);
    }

}

inline std::vector<std::vector<int>> diceset(std::vector<int> number, std::vector<int> value)
{
    std::vector<std::vector<int>> results;
    std::vector<int> temp;
    results.clear();
    if (number.size() != value.size()) return results;
    for (int i = 0; i < value.size(); i++)
    {
        temp.clear();
        for (int ii = 0; ii < number[i]; ii++)
        {
            temp.push_back(roll(1, value[i]));
        }
        results.push_back(temp);

    }
    return results;
}
