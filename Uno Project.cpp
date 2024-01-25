#define _CRT_SECURE_NO_WARNINGS

//Samantha Fisher and MiLee Vogel

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

typedef struct card_s {

	char color[7];
	int value;
	char action[15];
	struct card_s* next;

} card;

//universal variables
bool p1CanPlay = true;
bool p2CanPlay = true;

int dealCount = 1;


//all the functions used
void printCard(card* number);
void printDiscard(card deck[]);
void readDeck(card arr[]);
void createOwnDeck(card arr[]);
void swapCards(card deck[], int i, int j);
void shuffleDeck(card deck[]);
void dealCards(card** head, card** tail, card deck[], int i);
void drawFromDeck(card** tail, card deck[], int dealCount);
void removeCard(card** head, int index);
void firstDiscard(card deck[]);
void discardPile(card deck[108], card** head, int index);
bool checkCard(card** head, card** tail, int index, card deck[], int playerNum);




int main(void) {

	srand(time(NULL));

	int i;

	//variable to store if the card is valid and can be played
	bool checkResult = false;

	//deck things
	card deck[108]; //initialize size of deck

	//shuffle things
	int shuffleChoice; //if the user wants to shuffle
	char loadedFile[25]; //variable to hold the string of what the user's entered file is called

	//player info and other things
	card* pH1; //player head 1
	card* pH2; //player head 2
	card* pT1; //player tail 1
	card* pT2; //player tail 2
	pH1 = (card*)malloc(sizeof(card));
	pH2 = (card*)malloc(sizeof(card));
	pT1 = (card*)malloc(sizeof(card));
	pT2 = (card*)malloc(sizeof(card));
	card* temp1;
	card* temp2;
	pH1 = NULL;
	pH2 = NULL;
	temp1 = pH1;
	temp2 = pH2;
	//bool p1CanPlay = true;
	//bool p2CanPlay = true; 

	//variable to store either 'y' or 'n' to determine if the player wants to continue playing the game after someone has won
	char playAgain = 'y';

	//will continue running until user enters 'n' to state they do not want to play uno anymore
	while (playAgain == 'y') {

		//turn things initialize at start of game
		int turnChoice;
		int turnCount = 1;
		dealCount = 1;

		//each player gets 7 cards, keep track of their hand
		int p1Count = 7;
		int p2Count = 7;

		//player things to initialize
		pH1 = NULL;
		pH2 = NULL;
		p1CanPlay = true;
		p2CanPlay = true;

		//start the game
		printf("LET'S PLAY A GAME OF UNO!\n\n");

		//ask user what they want to do before the start of the game
		printf("PRESS [1] TO SHUFFLE THE UNO DECK OR [2] TO LOAD A DECK FROM A FILE: "); //shuffle vs load deck
		scanf(" %d", &shuffleChoice);

		//if they don't select the numbers asked
		while (shuffleChoice != 1 && shuffleChoice != 2) {
			printf("PLEASE SELECT [1] OR [2]. ");
			scanf(" %d", &shuffleChoice);
		}

		//if the selected choice is 1
		if (shuffleChoice == 1) {
			readDeck(deck);
			swapCards(deck, 0, 107);
			shuffleDeck(deck);
			printf("\nTHE DECK IS SHUFFLED.\n");

			//alternating between the two players when dealing cards
			for (i = 0; i < 14; i++) {

				dealCards(&pH1, &pT1, deck, i);
				++dealCount;

				i++;

				dealCards(&pH2, &pT2, deck, i);
				++dealCount;

			}

			//initial deal of the cards to the two players
			while (temp1 != NULL) {

				if (dealCount % 2 != 0) {
					temp1 = temp1->next;
				}

				else {
					temp2 = temp2->next;
				}
				++dealCount;

			}
			temp1 = pH1;
			temp2 = pH2;
		}

		//if the selected choice is 2
		else if (shuffleChoice == 2) {

			printf("\nPLEASE ENTER THE NAME OF FILE(load.txt): "); //add your deck to the load.txt file beforehand, it is empty for you to use
			scanf(" %s", &loadedFile);

			createOwnDeck(deck);
			swapCards(deck, 0, 107);
			shuffleDeck(deck);
			printf("\nTHE DECK IS SHUFFLED.\n");
		}

		//where game is played, as long as neither player has 0 cards continue to loop game
		while (p1Count > 0 && p2Count > 0) {

			//player 1 turn

			//make sure p1 is not skipped and p2 hasn't already won
			if (p1CanPlay == true) {

				p2CanPlay = true;

				printf("\n");
				printf("\n-----TURN %d-----\n", turnCount);

				printf("\nPLAYER 1 HAND:\n");
				printCard(temp1);
				printf("\n");

				printDiscard(deck);


				printf("\nSELECT [1]-[%d] TO PLAY A CARD OR SELECT [0] TO DRAW A CARD FROM THE DECK: ", p1Count);
				scanf(" %d", &turnChoice);

				if (turnChoice >= 1 && turnChoice <= p1Count) {

					checkResult = checkCard(&temp1, &pT2, turnChoice, deck, 1);

					if (checkResult == true) {

						discardPile(deck, &temp1, turnChoice);
						removeCard(&temp1, turnChoice);
					}

					//discardPile(deck, &temp1, turnChoice);
					//removeCard(&temp1, turnChoice);

					printf("\nPLAYER 1 HAND: ");
					printCard(temp1);
					printf("\n");

					--p1Count;

					if (p1Count == 1) {
						printf("\nPLAYER 1 HAS UNO");
					}

					if (p1Count == 0) {
						//p1 won the game, so set p2CanPlay to false
						p2CanPlay = false;
					}
				}

				//runs if player wants to draw a card from the deck rather than play a card
				else if (turnChoice == 0) {

					dealCount++;
					drawFromDeck(&pT1, deck, dealCount);


					printf("\nPLAYER 1 HAND: ");
					printCard(temp1);
					printf("\n");

					++p1Count;
				}
			}



			++turnCount;

			//player 2 turn

			//make sure p2 is not skipped and p1 hasn't already won
			if (p2CanPlay == true) {

				p1CanPlay = true;

				printf("\n");
				printf("\n-----TURN %d-----\n", turnCount);

				printDiscard(deck);

				printf("\nPLAYER 2 HAND:\n");
				printCard(temp2);
				printf("\n");

				printf("\nSELECT [1]-[%d] TO PLAY A CARD OR SELECT [0] TO DRAW A CARD FROM THE DECK: ", p2Count);
				scanf(" %d", &turnChoice);

				if (turnChoice >= 1 && turnChoice <= p2Count) {

					checkResult = checkCard(&temp2, &pT1, turnChoice, deck, 2);

					if (checkResult == true) {

						discardPile(deck, &temp2, turnChoice);
						removeCard(&temp2, turnChoice);
					}

					//discardPile(deck, &temp2, turnChoice);
					//removeCard(&temp2, turnChoice);
					--p2Count;

					printf("\nPLAYER 2 HAND:\n");
					printCard(temp2);
					printf("\n");

					//check if the player has uno
					if (p2Count == 1) {
						printf("\nPLAYER 2 HAS UNO");
					}

					if (p2Count == 0) {
						//p2 won the game, so set p1CanPlay to false
						p1CanPlay = false;
					}
				}

				//runs if player wants to draw a card from the deck rather than play a card
				else if (turnChoice == 0) {

					dealCount++;
					drawFromDeck(&pT2, deck, dealCount);


					printf("\nPLAYER 2 HAND: ");
					printCard(temp2);
					printf("\n");

					++p2Count;
				}

			}

			++turnCount;
		}

		//when the game is over say who won
		printf("\nTHE GAME IS OVER\n");

		if (p1Count == 0) {
			printf("\nPLAYER 1 HAS WON!\n");
		}
		if (p2Count == 0) {
			printf("\nPLAYER 2 HAS WON!\n");
		}

		//ask user after game if they would like to play again
		printf("\nWOULD YOU LIKE TO PLAY AGAIN (y/n): ");
		scanf(" %c", &playAgain);
		printf("\n");

		//if they don't want to play again, user will enter 'n' and then exit the while loop and end
		if (playAgain == 'n') {
			printf("GOODBYE\n");
		}
	}




	return 0;
}

//print each card and be able to call the function for printing a players hand
void printCard(card* number) {

	card* temp;
	temp = number;
	int count = 1;

	while (temp != NULL) {
		printf("\n%d.) ", count);
		count++;

		//print the number cards
		if ((temp)->value >= 0 && (temp)->value <= 9) {
			printf("%d %s ", (temp)->value, temp->color);
		}
		//print the special cards
		else if (((temp)->value == -1) || ((temp)->value == -2) || ((temp)->value == -3) || ((temp)->value == -4)) {
			printf("%s %s ", (temp)->color, (temp)->action);
		}
		temp = temp->next;
	}
}


//function to print out the first card in the discard pile
void printDiscard(card deck[]) {
	if (deck[107].value >= 0 && deck[107].value <= 9) {
		printf("\nDISCARD PILE: %d %s \n", deck[107].value, deck[107].color);
	}
	//print the special cards
	else if ((deck[107].value == -1) || (deck[107].value == -2) || (deck[107].value == -3) || (deck[107].value == -4)) {
		printf("\nDISCARD PILE: %s %s \n", deck[107].color, deck[107].action);
	}
}

//read the deck of 108 cards, predetermined deck for selection 1
void readDeck(card arr[]) {

	FILE* unoDeck;
	unoDeck = fopen("deck.txt", "r");

	int i;

	if (unoDeck == NULL) {
		printf("FILE CANNOT BE FOUND.\n");
	}

	//read the file containing the deck
	else {
		for (i = 0; i < 108; ++i) {
			fscanf(unoDeck, "%d", &arr[i].value);
			//printf("%d", arr[i].value);
			fscanf(unoDeck, "%*c");
			fscanf(unoDeck, "%s", arr[i].color);
			//printf("%s", arr[i].color);
			fscanf(unoDeck, "%*c");
			fscanf(unoDeck, "%s", arr[i].action);
			//printf("%s", arr[i].action);
		}
	}
}

//read the deck of 108 crads, user inputed deck for selection 2
void createOwnDeck(card arr[]) {

	FILE* userFile; //name file
	userFile = fopen("load.txt", "r"); //open text in file

	int i;

	if (userFile == NULL) { //incase the file can't be found 
		printf("FILE CANNOT BE FOUND.\n");
	}

	else {
		for (i = 0; i < 108; ++i) {
			fscanf(userFile, "%d", &arr[i].value);
			//printf("%d", arr[i].value);
			fscanf(userFile, "%*c");
			fscanf(userFile, "%s", arr[i].color);
			//printf("%s", arr[i].color);
			fscanf(userFile, "%*c");
			fscanf(userFile, "%s", arr[i].action);
			//printf("%s", arr[i].action);
		}
	}
}

//swap the cards for the shuffle function
void swapCards(card deck[], int i, int j) { //swap function

	card temp;

	temp = deck[i];
	deck[i] = deck[j];
	deck[j] = temp;

}

//shuffles the deck for the user
void shuffleDeck(card deck[]) { //shuffles the deck 

	int i;
	int round1, round2;

	for (i = 0; i < 10000; ++i) {
		round1 = rand() % 108;
		round2 = rand() % 108;
		swapCards(deck, round1, round2);
	}
}

//deal the cards to the player at the beginning of the game
void dealCards(card** head, card** tail, card deck[], int i) {

	card* temp;
	temp = (card*)malloc(sizeof(card));

	temp->value = deck[i].value;
	strcpy(temp->color, deck[i].color);
	strcpy(temp->action, deck[i].action);

	if (*head == NULL) {
		*head = temp;
	}
	else {
		(*tail)->next = temp;
	}

	(*tail) = temp;
	(*tail)->next = NULL;
}

//allow a specific player to draw a card from the deck and add it to their hand
void drawFromDeck(card** tail, card deck[], int index) { //copies the card from the pile to the player hand

	card* temp;
	temp = (card*)malloc(sizeof(card));

	temp->value = deck[index].value;
	strcpy(temp->color, deck[index].color);
	strcpy(temp->action, deck[index].action);

	temp->next = NULL;
	(*tail)->next = temp;
	*tail = (*tail)->next;

}

//initial discard
void firstDiscard(card deck[]) {
	int i;
	card temp[1];

	temp[0] = deck[0];

	for (i = 0; i < 108; i++) {
		if (deck[i].value != 0) {
			deck[i] = deck[i + 1];
		}
		else if ((deck[i].value == 0) || (deck[i].value == '\0')) {
			deck[i].value = 0;
			break;
		}
	}
	deck[107] = temp[0];
}

//discard a card from a specific player
void discardPile(card deck[108], card** head, int index) {
	//initialize variable
	card* temp;
	int i;

	temp = *head;
	for (i = 1; i < index; i++) {
		temp = temp->next;
	}

	for (i = 107; i > 0; i--) {
		if (temp->value != 0) {
			deck[i - 1] = deck[i];
		}
		else {
			break;
		}
	}
	//temp = NULL;
	deck[107].value = temp->value;
	strcpy(deck[107].color, temp->color);
	strcpy(deck[107].action, temp->action);
	temp = NULL;
}

//function to remove a card from a specific players hand
void removeCard(card** head, int index) {

	// assumes first card = 1 
	card* temp = *head;

	//previous card
	card* prev = NULL;
	int counter = 0;

	//while loop to sort through our player's hand
	while (temp != NULL) {
		counter++;
		if (counter == index) {
			break;
		}
		prev = temp;
		temp = temp->next;
	}

	if (prev == NULL) {
		// removing first card 
		*head = (*head)->next;
	}
	else {
		//removing any other card thats not first
		prev->next = temp->next;
		// free temp stuff check in notes
		free(temp);
	}
}

//function to check if the user's card is valid and can be played on the card on the top of the discard pile
bool checkCard(card** head, card** tail, int index, card deck[], int playerNum) {
	//initialize variables
	bool check;
	card* temp;
	int i;
	check = false;
	temp = *head;

	for (i = 1; i < index; ++i) {
		temp = temp->next;
	}
	if (strcmp(temp->color, deck[107].color) == 0) {
		check = true;
	}
	else if (temp->value == deck[107].value) {
		check = true;
	}

	//handle the reverse and skips
	if (temp->value == -2) {
		if (playerNum == 1) {
			printf("\nPLAYER 2 - ");
			p2CanPlay = false;
		}

		else if (playerNum == 2) {
			printf("\nPLAYER 1 - ");
			p1CanPlay = false;
		}
		printf("YOU HAVE BEEN COCONUT MALLED (skipped)\n");
	}

	//handle the draw two's
	if (temp->value == -3) {
		if (playerNum == 1) {
			printf("\nPLAYER 2 - ");
			p2CanPlay = false;

			dealCount++;
			drawFromDeck(tail, deck, dealCount);
			dealCount++;
			drawFromDeck(tail, deck, dealCount);
		}

		else if (playerNum == 2) {
			printf("\nPLAYER 1 - ");
			p1CanPlay = false;

			dealCount++;
			drawFromDeck(tail, deck, dealCount);
			dealCount++;
			drawFromDeck(tail, deck, dealCount);

		}
		printf("DRAW 2 CARDS\n");
	}

	return check;
}

