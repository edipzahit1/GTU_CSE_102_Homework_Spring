#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#define SUIT_COUNT 4
#define FACE_COUNT 13
#define DECK_SIZE (SUIT_COUNT * FACE_COUNT)

typedef struct
{
    const char *face;
    const char *suit;
} Card;

void initializeDeck(Card *deck);
void shuffle(Card *deck);
void printDeck(Card *deck);
void freeDeck(Card *deck);

void initializeDeck(Card *deck)
{
    char suits[SUIT_COUNT][10] = { "Hearts", "Diamonds", "Clubs", "Spades" };
    char faces[FACE_COUNT][6] = { "Ace", "Deuce", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten", "Jack", "Quenn", "King"};
    
    int i, j, deckIndex = 0;
    for (i = 0; i < SUIT_COUNT; i++)
    {
        for (j = 0; j < FACE_COUNT; j++)
        {
            deck[deckIndex].suit = (char *) malloc(sizeof(char) * strlen(suits[i]));
            deck[deckIndex].face = (char *) malloc(sizeof(char) * strlen(faces[j])); 
            strcpy((char *) deck[deckIndex].suit, suits[i]);
            strcpy((char *) deck[deckIndex].face, faces[j]);
            deckIndex++;
        }
    }
}

void shuffle(Card *deck)
{
    int i, j;
    int shuffledIndexes[DECK_SIZE];
    int alreadyPut[DECK_SIZE];

    for (i = 0; i < DECK_SIZE; ++i) alreadyPut[i] = 0;

    for (i = 0; i < DECK_SIZE; ++i)
    {
        int randIndex;
        do
        {
            randIndex = rand() % DECK_SIZE;
        } while (alreadyPut[randIndex]);
        shuffledIndexes[i] = randIndex;
        alreadyPut[randIndex] = 1;
    }
    Card shuffledDeck[DECK_SIZE];
    for (i = 0; i < DECK_SIZE; ++i)
        shuffledDeck[i] = deck[shuffledIndexes[i]];

    for (i = 0; i < DECK_SIZE; ++i)
        deck[i] = shuffledDeck[i];    
}

void printDeck(Card *deck)
{
    int i, j;
    for (i = 0; i < DECK_SIZE; ++i)
    {
        printf("%10s of %-10s", deck[i].face, deck[i].suit);
        ++i;
        printf("%10s of %-10s\n", deck[i].face, deck[i].suit);
    }
}

void freeDeck(Card *deck)
{
    int i;
    for (i = 0; i < 52; i++)
    {
        free((char *)deck[i].face);
        free((char *)deck[i].suit);
    }
}

int main()
{
    srand(time(NULL));
    Card deck[52];
    initializeDeck(deck);
    shuffle(deck);
    printDeck(deck);
    freeDeck(deck);   
}
