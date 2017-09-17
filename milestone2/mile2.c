#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

int menu(void);  // ms2
void welcome(void);
void runInventorySystem(void);  // ms2
int getYesOrNo(void);  // ms2
int getMenuChoice(void); 
int getIntLimited(int lowerLimit, int upperLimit);
void clrKyb(void);
void pause(void);

int main(void) {
	   runInventorySystem();

           return 0;
}

// implmement MS2 fucntions here

int menu(void){
	int decision;
	printf("1- List all items\n2- Search by SKU\n3- Checkout an item\n4- Stock an item\n5- Add new item or update item\n6- Delete item\n7- Search by name\n0- Exit program\n> ");
	decision = getMenuChoice();
	
	return decision;
}

void welcome(void){
	printf("---=== Grocery Inventory System ===---\n\n");
}

void runInventorySystem(void){
	int input;
	welcome();

	do{
		input = menu();
                if(input == 0){
			int decision;
			printf("Exit the program? (Y)es/(N)o: ");
                        decision = getYesOrNo();

			if(decision == 1){
				input = 0;
			}	
			else{
				input = (-1);
			}
               }
	       else if (input == 7){
		       printf("Search: NOT IMPLEMENTED\n");
		       pause();
	       }
               else{
	               printf("List: NOT IMPLEMENTED\n");
	               pause();
	       }
	}while(input != 0 || input == (-1));
}

int getYesOrNo(void){
	char input[10];
	int result = 0;

	do{
		scanf(" %s",input);
                getchar();
                if ((strcmp(input, "y") != 0 && (strcmp(input, "Y") != 0)) && ((strcmp(input, "n") != 0) && (strcmp(input, "N") != 0))){
			printf("Only (Y)es or (N)o are acceptable: ");
		}

	}while(((strcmp(input, "y") != 0 && (strcmp(input, "Y") != 0))) && ((strcmp(input, "n") != 0) && (strcmp(input, "N") != 0)));

        if(strcmp(input, "y") == 0 || strcmp(input, "Y") == 0){
		result = 1;
	}
	else if (strcmp(input, "n") == 0 || strcmp(input, "N") == 0){
		result = 0;
	}
	return result;
}

// copy MS1 functions here

int getMenuChoice(void){
	int choice;
	choice = getIntLimited(0, 7);

	return choice;
}

int getIntLimited(int lowerLimit, int upperLimit){
        int value;
        do{
		scanf("%d", &value);
	        if (value < lowerLimit || value > upperLimit){
			printf("Invalid value, %d <= value <= %d: ", lowerLimit, upperLimit);
	       	}
	}while (value < lowerLimit || value > upperLimit);

	return value;
}

void pause(void){
	printf("Press <ENTER> to continue...\n");
	clrKyb();
}

void clrKyb(void){
	char enter = 0;
	while (enter != '\n'){
		enter = getchar();
       	}
}


//
//
//
// /*
// ---=== Grocery Inventory System ===---
// 1- List all items
// 2- Search by SKU
// 3- Checkout an item
// 4- Stock an item
// 5- Add new item or update item
// 6- delete item
// 7- Search by name
// 0- Exit program
// > 8
// Invalid value, 0 < value < 7: 1
// List Items!
// Press <ENTER> to continue...
// 1- List all items
// 2- Search by SKU
// 3- Checkout an item
// 4- Stock an item
// 5- Add new item or update item
// 6- delete item
// 7- Search by name
// 0- Exit program
// > 2
// Search Items!
// Press <ENTER> to continue...
// 1- List all items
// 2- Search by SKU
// 3- Checkout an item
// 4- Stock an item
// 5- Add new item or update item
// 6- delete item
// 7- Search by name
// 0- Exit program
// > 3
// Checkout Item!
// Press <ENTER> to continue...
// 1- List all items
// 2- Search by SKU
// 3- Checkout an item
// 4- Stock an item
// 5- Add new item or update item
// 6- delete item
// 7- Search by name
// 0- Exit program
// > 4
// Stock Item!
// Press <ENTER> to continue...
// 1- List all items
// 2- Search by SKU
// 3- Checkout an item
// 4- Stock an item
// 5- Add new item or update item
// 6- delete item
// 7- Search by name
// 0- Exit program
// > 5
// Add/Update Item!
// Press <ENTER> to continue...
// 1- List all items
// 2- Search by SKU
// 3- Checkout an item
// 4- Stock an item
// 5- Add new item or update item
// 6- delete item
// 7- Search by name
// 0- Exit program
// > 6
// Delete Item!
// Press <ENTER> to continue...
// 1- List all items
// 2- Search by SKU
// 3- Checkout an item
// 4- Stock an item
// 5- Add new item or update item
// 6- delete item
// 7- Search by name
// 0- Exit program
// > 7
// Search by name!
// Press <ENTER> to continue...
// 1- List all items
// 2- Search by SKU
// 3- Checkout an item
// 4- Stock an item
// 5- Add new item or update item
// 6- delete item
// 7- Search by name
// 0- Exit program
// > 0
// Exit the program? (Y)es/(N)o : x
// Only (Y)es or (N)o are acceptable: n
// 1- List all items
// 2- Search by SKU
// 3- Checkout an item
// 4- Stock an item
// 5- Add new item or update item
// 6- delete item
// 7- Search by name
// 0- Exit program
// > 0
// Exit the program? (Y)es/(N)o : y
// */
