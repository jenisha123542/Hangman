#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

// Function to perform bubble sort on the array
void bubbleSort(char* arr, int length) {
    int i, j; // Move the variable declarations here
    for (i = 0; i < length - 1; ++i) {
        for (j = 0; j < length - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                char temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

// Function to perform binary search in a sorted array
bool binarySearch(char letter, const char* sortedWord, int length) {
    int left = 0;
    int right = length - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (sortedWord[mid] == letter) {
            return true;
        } else if (sortedWord[mid] < letter) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return false;
}

// Function to check if the guessed letter is in the sorted secret word
bool isLetterInWord(char letter, const char* secretWord) {
    int length = strlen(secretWord);
    char* sortedWord = (char*)malloc(length * sizeof(char));
    if (sortedWord == NULL) {
        printf("Memory allocation error!");
        return false;
    }

    strcpy(sortedWord, secretWord);
    bubbleSort(sortedWord, length);

    bool result = binarySearch(letter, sortedWord, length);
    free(sortedWord);
    return result;
}

// Function to update the display of the secret word with correctly guessed letters
void updateDisplay(char letter, const char* secretWord, char* displayWord) {
    int length = strlen(secretWord);
    int i; // Move the variable declaration here
    for (i = 0; i < length; ++i) {
        if (secretWord[i] == letter) {
            displayWord[i] = letter;
        }
    }
}

// Function to display the hangman figure
void displayHangman(int incorrectGuesses) {
    printf("\n");

    switch (incorrectGuesses) {
        case 0:
            printf("  +---+\n");
            printf("  |   |\n");
            printf("      |\n");
            printf("      |\n");
            printf("      |\n");
            printf("      |\n");
            printf("=========\n");
            break;
        case 1:
            printf("  +---+\n");
            printf("  |   |\n");
            printf("  O   |\n");
            printf("      |\n");
            printf("      |\n");
            printf("      |\n");
            printf("=========\n");
            break;
        case 2:
            printf("  +---+\n");
            printf("  |   |\n");
            printf("  O   |\n");
            printf("  |   |\n");
            printf("      |\n");
            printf("      |\n");
            printf("=========\n");
            break;
        case 3:
            printf("  +---+\n");
            printf("  |   |\n");
            printf("  O   |\n");
            printf(" /|   |\n");
            printf("      |\n");
            printf("      |\n");
            printf("=========\n");
            break;
        case 4:
            printf("  +---+\n");
            printf("  |   |\n");
            printf("  O   |\n");
            printf(" /|\\  |\n");
            printf("      |\n");
            printf("      |\n");
            printf("=========\n");
            break;
        case 5:
            printf("  +---+\n");
            printf("  |   |\n");
            printf("  O   |\n");
            printf(" /|\\  |\n");
            printf(" /    |\n");
            printf("      |\n");
            printf("=========\n");
            break;
        case 6:
            printf("  +---+\n");
            printf("  |   |\n");
            printf("  O   |\n");
            printf(" /|\\  |\n");
            printf(" / \\  |\n");
            printf("      |\n");
            printf("=========\n");
            break;
    }
}

// Function to play a level and return the score
int playLevel(const char* secretWord) {
    const int MAX_TRIES = 6;
    char displayWord[20];
    char guessHistory[MAX_TRIES];
    int wordLength = strlen(secretWord);
    strcpy(displayWord, "");
    int i;
    for (i = 0; i < wordLength; i++) {
        strcat(displayWord, "_");
    }

    int tries = 0;
    char guess;
    int score = 0;
    int incorrectGuesses = 0;

    while (incorrectGuesses < MAX_TRIES) {
        printf("\nWord: %s\n", displayWord);
        printf("Tries left: %d\n", MAX_TRIES - incorrectGuesses);
        printf("Guess History: ");
        int i;
        for (i = 0; i < tries; i++) {
            printf("%c ", guessHistory[i]);
        }
        printf("\nEnter a letter: ");
        scanf(" %c", &guess);

        if (isLetterInWord(guess, secretWord)) {
            printf("Correct guess!\n");
            updateDisplay(guess, secretWord, displayWord);
            score += 10; // Increase the score for correct guesses
        } else {
            printf("Incorrect guess!\n");
            incorrectGuesses++; // Increase the count of incorrect guesses
        }

        guessHistory[tries] = guess;
        tries++;

        displayHangman(incorrectGuesses);

        if (strcmp(secretWord, displayWord) == 0) {
            printf("\nCongratulations! You guessed the word.\n");
            printf("Your score for this level is: %d\n", score);
            return score;
        }
    }

    if (incorrectGuesses == MAX_TRIES) {
        printf("\nOut of tries! The word was not revealed.\n");
        printf("Your score for this level is: %d\n", score);
        exit(0); // Exit the game if the player fails to guess the word
    }

    return score;
}

int main() {
    const char* filename = "secret_words.txt"; // File containing secret words
    FILE* file = fopen(filename, "r");

    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    const int MAX_WORD_LENGTH = 20;
    char levels[100][MAX_WORD_LENGTH]; // Assuming a maximum of 100 words

    int numLevels = 0;
    char word[MAX_WORD_LENGTH];

    // Read secret words from the file
    while (fgets(word, MAX_WORD_LENGTH, file) != NULL) {
        word[strcspn(word, "\n")] = '\0'; // Remove the newline character
        strcpy(levels[numLevels], word);
        numLevels++;
    }

    fclose(file);

    printf("Welcome to Hangman!\n");

    int totalScore = 0;
    int level;
    for (level = 0; level < numLevels; level++) {
        printf("\nLevel %d\n", level + 1);
        totalScore += playLevel(levels[level]);
    }

    // Save the total score to a file
    const char* scoreFilename = "scores.txt";
    FILE* scoreFile = fopen(scoreFilename, "a");

    if (scoreFile == NULL) {
        perror("Error opening score file");
        return 1;
    }

    fprintf(scoreFile, "Total Score: %d\n", totalScore);
    fclose(scoreFile);

    printf("\nCongratulations! You've completed all levels.\n");
    printf("Your total score for all levels is: %d\n", totalScore);

    return 0;
}

