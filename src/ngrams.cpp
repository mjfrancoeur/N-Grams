// TODO: Edit this comment

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "vector.h"
#include "queue.h"
#include "map.h"
#include "filelib.h"
#include "simpio.h"
#include "random.h"

using namespace std;

void greeting();
Vector<string> getInputFIle();
Vector<string> queueToVector(Queue<string> q);
void generateNGramsMap(ifstream & input, int n, Map<Vector<string>, Vector<string> > & map);
void populateMap(Vector<string> & key, string value, Queue<string> & queue, Map<Vector<string>, Vector<string> > & map);
ifstream getInputStream();
int getN();
int getNumberOfRandomWords();
void generateRandomText(Map<Vector<string>, Vector<string> > & map, int numWords);
string pickRandomStringFromVector(Vector<string> & v);
Vector<string> pickRandomQueueFromVector(Vector<Vector<string> > & v);
void printResultToConsole(Vector<string> & v);

int main() {
    greeting();
    
    while (true) {
        ifstream input = getInputStream();
        int n = getN();
        Map<Vector<string>,Vector<string> > nGramMap;
        generateNGramsMap(input, n, nGramMap);
        int numRWords = getNumberOfRandomWords();
        if (numRWords == 0) { // Quits program if 0 is entered.
            break;
        }
        generateRandomText(nGramMap, numRWords);
      
    }
    
    cout << "Exiting." << endl;
    return 0;
}

void greeting() {
    cout << "Welcome to CS 106B Random Writer ('N-Grams')." << endl;
    cout << "This program makes random text based on a document." << endl;
    cout << "Give me an input file and an 'N' value for groups" << endl;
    cout << "of words, and I'll create random text for you." << endl;
}

// Prompts user for filename and returns a Vector<string> of all words within that file.
ifstream getInputStream() {

    ifstream input;
    while (true) {
        string filename = getLine("Input file name? ");
        openFile(input, filename);
        if (input.is_open()) {
            break;
        } else {
            cout << "Couldn't open file. Try again." << endl;
        }
    }

    return input;
}

// Prompts user to input an integer, and returns the value.
int getN() {
    return getInteger("Value of N? ");
}

// Prompts user for an integer, the number of random words to generate.
int getNumberOfRandomWords() {
    return getInteger("# of random words to generate (0 to quit)? ");
}

// Populates the Map with n-prefixes as the key and a suffix as the value.
void generateNGramsMap(ifstream & input, int n, Map<Vector<string>, Vector<string> > & map) {

    string word;
    Queue<string> nWords;
    
    while (nWords.size() < n - 1) { // Fill queue with n words
        if (input >> word) {
            nWords.enqueue(word);
        } else {
            cout << "Not enough words in the text to complete." << endl;
        }
    }
    Vector<string> key;
    while (input >> word) {
       populateMap(key, word, nWords, map);
    }
    
    // Rewind stream to "wrap-around" the text to provide value pair for the final n strings in the input file.
    rewindStream(input);
    for (int i = 0; i < n - 1; i++) {
        input >> word;
        populateMap(key, word, nWords, map);
    }

    input.close();
}

// Places all elements in a queue into a Vector of strings
Vector<string> queueToVector(Queue<string> q) {
    Vector<string> vec;
    while (!q.isEmpty()) {
        vec.add(q.dequeue());
    }
    return vec;
}

// TODO: Finish this comment.
void populateMap(Vector<string> & key, string value, Queue<string> & queue, Map<Vector<string>, Vector<string> > & map) {
       key = queueToVector(queue);
       if (map.containsKey(key)) {
           Vector<string> currentV = map[key];
           currentV.add(value);
       } else {
           Vector<string> vec;
           vec.add(value);
           map[key] = vec;
       }
       queue.dequeue();
       queue.enqueue(value);
}

// TODO: Finish this comment.
void generateRandomText(Map<Vector<string>, Vector<string> > & map, int numWords) {
    Vector<string> result;

    // Start the random text with random set of n-1 words
    Vector<Vector<string> > keysVec = map.keys();
    Vector<string> start = pickRandomQueueFromVector(keysVec); 
    Queue<string> keyQ; // Initialize a queue to hold the key (prefix) values of size n-1.
    for (string word : start) {
        result.add(word);
        keyQ.enqueue(word);
    }

    while (result.size() < numWords) { 
        Vector<string> valueV = map[queueToVector(keyQ)];
        string word = pickRandomStringFromVector(valueV);
        result.add(word);
        keyQ.dequeue();
        keyQ.enqueue(word);
    }
    
    printResultToConsole(result);

}

// Prints result to the console.
void printResultToConsole(Vector<string> & v) {
    cout << "... ";
    for (string word : v) {
        cout << word << " ";
    }
    cout << "..." << endl;
    cout << "\n" << endl;
}

// Returns a random string from the vector argument.
string pickRandomStringFromVector(Vector<string> & v) {
    int random = randomInteger(0, v.size() - 1);
    return v[random];
}

// Returns a random vector element from the argument.
Vector<string> pickRandomVectorFromVector(Vector<Vector<string> > & v) {
    int random = randomInteger(0, v.size() - 1);
    return v[random];
}
