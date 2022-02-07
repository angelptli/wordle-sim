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
    int guessCount = 0;
    int greenLetters;
    string userGuess;

    vector<string> words = getWordFileArray();
    vector<string> matchingWordVector;
    string target = getRandomTarget(words);

    cout << "Welcome to Wordle Simulator\n"
         << "---------------------------\n\n";
    while (guessCount != 6 && greenLetters != 5)
    {
        greenLetters = 0;
        cout << "Enter Guess " << guessCount + 1 << ": ";
        cin >> userGuess;
        // cout << "TARGET: " << target << '\n';        

        // Get size of vector
        int size;
        if (guessCount == 0)
            size = words.size();
        else
            size = matchingWordVector.size();

        // Get the indicies of any green letters and the amount
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
            matchingWordVector = getYellowWords(words, size, indexVector, target);
            size = matchingWordVector.size();

            if (size > 0)
                // Print vector of yellow words
                printVector(matchingWordVector, size);
            else
                cout << "No words in dictionary containing yellow letters";
            cout << "\n\n";
        }       

        // Show user's guess along with is color code (green G, yellow Y, gray -)
        cout << "Your current guess:\n"
             << userGuess << " ~ " << convertGuessToColors(target, userGuess)
             << "\n\n";
        
        // Increment guess count
        guessCount++;
    }

    if (greenLetters == 5)
        cout << "You won!\n\n";
    else if (guessCount == 6)
        cout << "You lose!\n\n";

    // Show user's final guess, its color code, and the target word
    cout << "-------------------------------------------------------------\n"
         << "FINAL GUESS:\n"
         << userGuess << " ~ " << convertGuessToColors(target, userGuess)
         << "\nTARGET WORD: " << target << '\n';
}
