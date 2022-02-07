#include <iostream>
#include <fstream>
#include <vector>

using std::string;
using std::ifstream;
using std::vector;
using std::cout;
using std::cin;

vector<int> getGreenIndices(string target, string word)
{
    vector<int> indexTracker;
    for (int i = 0; i < 5; i++)
    {
        if (target[i] == word[i])
            indexTracker.push_back(i);
        else
            indexTracker.push_back(404);
    }

    return indexTracker;
}

vector<int> getYellowIndices(string target, string word)
{
    vector<int> indexTracker;
    for (int i = 0; i < 5; i++)
    {
        if (target[i] == word[i])
            indexTracker.push_back(i);
        else
            indexTracker.push_back(404);
    }

    return indexTracker;
}

int getNumOfGreen(vector<int> indexTracker)
{
    int matchCount = 0;
    for (int i = 0; i < 5; i++)
    {
        if (indexTracker[i] != 404)
            matchCount++;
    }

    return matchCount;
}

vector<string> getWordFileArray()
{
    ifstream fin("words.txt");
    vector<string> words;
    string word;

    while (fin >> word)
        words.push_back(word);

    return words;
}

string getRandomTarget(vector<string> words)
{
    const int MIN_VALUE = 1;
    const int MAX_VALUE = words.size();
    unsigned seed = time(0);  // system time
    srand(seed);              // seed random number generator
    int randNum = (rand() % (MAX_VALUE - MIN_VALUE + 1)) + MIN_VALUE;
    string target = words[randNum - 1];
    return target;
}

vector<string> getGreenWords(vector<string> words, int size, vector<int> indexVector, string target)
{
    vector<string> greenWords;
    string word;
    int idx;

    for (int i = 0; i < size; i++)
    {
        word = words[i];

        for (int j = 0; j < 5; j++)
        {
            idx = indexVector[j];

            // Add word if word has green matching letter with target
            if ((idx != 404) && (word[idx] == target[idx]))
            {
                greenWords.push_back(word);
                break;
            }
        }
    }

    return greenWords;
}

vector<string> getYellowWords(vector<string> words, int size, vector<int> indexVector, string target)
{
    vector<string> yellowWords;
    string word;

    for (int i = 0; i < size; i++)
    {
        word = words[i];

        for (int j = 0; j < 5; j++)
        {
            if (target.find(word[j]))
            {
                yellowWords.push_back(word);
                break;
            }
        }        
    }
    
    return yellowWords;
}

void printVector(vector<string> strVector, int size)
{
    for (int i = 0; i < size; i++)
    {
        cout << '\"' << strVector[i] << '\"';

        if (i != size - 1)
            cout << ", ";
    }
}

string convertGuessToColors(string target, string userGuess)
{
    string colorString;

    for (int i = 0; i < 5; i++)
    {
        if (target[i] == userGuess[i])
            colorString += "G";
        else if (target.find(userGuess[i]) != string::npos)
            colorString += "Y";
        else
            colorString += "-";
    }

    return colorString;
}

int main()
{
    vector<string> words = getWordFileArray();
    string target = getRandomTarget(words);
    string userGuess;
    int guessCount = 0;
    int greenLetters;
    vector<string> matchingWordVector;

    cout << "Welcome to Walmart Wordle\n"
         << "-------------------------\n\n";
    while (guessCount != 6 && greenLetters != 5)
    {
        greenLetters = 0;
        cout << "Enter Guess " << guessCount + 1 << ": ";
        cin >> userGuess;
        // cout << "TARGET: " << target << '\n';        

        // Get size of array or vector
        int size;
        if (guessCount == 0)
            size = words.size();
        else
            size = matchingWordVector.size();

        int vectorCounter = 0;

        // --------------------------- FIRST GUESS ---------------------------
        // Get green letters
        vector<int> indexVector = getGreenIndices(target, userGuess);
        greenLetters = getNumOfGreen(indexVector);

        if (greenLetters > 0)
        {
            // Add words to vector that contain at least one green letter
            cout << "Words with at least one green letter:\n";
            matchingWordVector = getGreenWords(words, size, indexVector, target);
            size = matchingWordVector.size();

            if (size > 0)
                // Print vector of green words
                printVector(matchingWordVector, size);
            else
                cout << "No words in dictionary containing green letters";

            cout << "\n\n";
        }
        else
        {
            // Add words to vector that contain at least one yellow letter
            cout << "Words with at least one yellow letter\n";
            
            // Ask if user wants to see words with yellow 
            matchingWordVector = getYellowWords(words, size, indexVector, target);
            size = matchingWordVector.size();

            if (size > 0)
                // Print vector of green words
                printVector(matchingWordVector, size);
            else
                cout << "No words in dictionary containing yellow letters";

            cout << "\n\n";
        }       

        // Show green, yellow, gray and user's guess
        cout << "Your current guess:\n"
             << userGuess << " - " << convertGuessToColors(target, userGuess)
             << "\n\n";
        
        // Reset green letter count
        guessCount++;
    }

    if (greenLetters == 5)
        cout << "You won!\n\n";
    else if (guessCount == 6)
        cout << "You lose!\n\n";

    // Show green, yellow, gray and user's guess
    cout << "---------------------------\n"
         << "FINAL GUESS:\n"
         << userGuess << " - " << convertGuessToColors(target, userGuess)
         << "\nTARGET WORD: " << target << '\n';
}

/* SAMPLE OUTPUT
$ g++ -o wordle wordle.cpp
$ ./wordle
Welcome to Walmart Wordle
-------------------------

Enter Guess 1: ouija
Words with at least one yellow letter
"dream", "hobos", "those", "steam", "grass", "agile", "abbey", "bacon", "cache", "dance", "eater", "feeds"

Your current guess:
ouija - Y---Y

Enter Guess 2: bacon
Words with at least one green letter:
"hobos", "bacon", "cache", "dance", "eater"

Your current guess:
bacon - GGGGG

You won!

---------------------------FINAL GUESS:
bacon - GGGGG
TARGET WORD: bacon

*/