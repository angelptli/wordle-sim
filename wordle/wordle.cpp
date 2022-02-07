#include <iostream>
#include <fstream>
#include <vector>

using std::string;
using std::ifstream;
using std::vector;
using std::cout;
using std::cin;

/**
 * Fill a vector with indices that locate matching green letters.
 * 
 * Compare target and user guess letters. A match will prompt an
 * add of their shared index number to the vector. A non-match adds
 * a "404" placeholder to indicate there is no match.
 * 
 * @param target A random target word that user needs to guess.
 * @param word The user's guess taken from their input.
 * @return vector<int> a vector that stores indices of int type.
 */
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

/**
 * Determine the number of green matching letters.
 * 
 * Count the number of indices in a given vector of indices.
 * The count increments when an index is not a "404" placeholder.
 * 
 * @param indexTracker A vector of indices of int type.
 * @return int count of green matching letters.
 */
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

/**
 * Fill a vector with 5-letter words from a word text file that contain
 * a 5-letter word on each line.
 * 
 * @return vector<string> a vector that stores strings.
 */
vector<string> getWordFileArray()
{
    ifstream fin("words.txt");
    vector<string> words;
    string word;

    while (fin >> word)
        words.push_back(word);

    return words;
}

/**
 * Get a random target word for the user to guess.
 * 
 * Seed a random number generator and use this number as an index
 * to get the corresponding word from a vector filled with 5-letter
 * words.
 * 
 * @param words A vector filled with 5-letter words.
 * @return string randomly generated target word for user to guess. 
 */
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

/**
 * Return a vector filled with words that shares at least one green
 * letter with the target word.
 * 
 * A green letter indicates the letter is found in the target word and
 * is in the same index place as the target word.
 * 
 * @param words A vector filled with 5-letter words.
 * @param size The number of elements in the vector.
 * @param indexVector A vector filled with indices that represent the
 *      indices of green letters.
 * @param target A random target word that user needs to guess.
 * @return vector<string> a vector filled with words that contain at
 *      least one green letter in common with `target`.
 */
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

/**
 * Return a vector filled with words that contains at least one yellow
 * letter.
 * 
 * A yellow letter indicates the letter is found in the target word but
 * is not in the correct index place as the target word.
 * 
 * @param words A vector filled with 5-letter words.
 * @param size The number of elements in the vector.
 * @param indexVector A vector filled with indices that represent the
 *      indices of green letters.
 * @param target A random target word that user needs to guess.
 * @return vector<string> a vector filled with words that contain at
 *      least one yellow letter.
 */
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

/**
 * Print a given vector's elements onto the terminal.
 * 
 * @param strVector A vector filled with 5-letter words.
 * @param size The number of elements in the vector.
 */
void printVector(vector<string> strVector, int size)
{
    for (int i = 0; i < size; i++)
    {
        cout << '\"' << strVector[i] << '\"';

        if (i != size - 1)
            cout << ", ";
    }
}

/**
 * Get the color code of the user's guess for comparing the target
 * and user's guess.
 * 
 * The color codes are
 * G (green): same letter with same index place
 * Y (yellow): letter contained in target but not the same index place
 * - (grey): letter not found in target
 * 
 * @param target A random target word that user needs to guess.
 * @param userGuess The user's guess for one round.
 * @return string contains color code for target and guess comparison.
 */
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
    int guessCount = 0;                         // Tracks number of guesses
    int greenLetters;                           // Matching letters & same index
    string userGuess;                           // User guess from input

    vector<string> words = getWordFileArray();  // 5-letter word dictionary
    vector<string> matchingWordVector;          // Stores green or yellow words
    string target = getRandomTarget(words);     // Target to guess

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
