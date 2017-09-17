/*
 * Name: Geoffrey Wu
 * Date: Dec. 6, 2016
 * Professor: Lewis Baxter
 * IPC144M
 * Final Project
*/
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#define MAX_ITEM_NO 21
#define MAX_QTY  999
#define SKU_MAX 999
#define SKU_MIN 100
#define LINEAR 1
#define FORM 0

// global - struct Item

struct Item
{
   double price;
   int    sku;
   int    isTaxed;
   int    quantity;
   int    minQuantity;
   char   name[MAX_ITEM_NO]; // 1 for delimiter
};

void search      (struct Item[], int);
void updateItem  (struct Item*);
int  loadItems   (struct Item[], char[], int);
void printGoodbye(void);

// mile 1 functions

void      printWelcome  ( void           );
void      printTitle    ( void           );
void      printFooter   ( double         );
void      clearKeyboard ( void           );
void      pause         ( void           );
int       getInt        ( void           );
double    getDouble     ( void           );
int       getIntLimited ( int, int       );
double getDoubleLimited ( double, double );

// mile 2 functions

int    menu               ( void );
bool   getYesOrNo         ( void );
int    getMenuChoice      ( void );
void   runInventorySystem ( void );

// mile 3 functions

double      totalAfterTax ( struct Item                         );
bool        isQuantityLow ( struct Item *                       );
struct Item enterItem     ( int                                 );
void        displayItem   ( struct Item *, int                  );
void        listItems     ( struct Item [], int                 );
bool        locateItem    ( struct Item item[], int, int, int*  );

// global - TAX rate

const double TAX = 0.13;

//

/*
 * Function: main
 * -----------------
 * starts the program
 * parameters: none
 *
 * variables: none
 *
 * returns 0
 *
 * */
int main(void){
        printWelcome();
        runInventorySystem();
        printGoodbye();

	return 0;
}

/*
 * Function: runInventorySystem
 * ----------------------------
 *  starts the inventory system
 *  parameters: none
 *
 *  variables:
 *  open: integer flag that determines whether the file is successfully opened
 *  	  returns 0 if cannot open file
 *  input: user input regarding the options
 *  skuInput: sku enter by the user in option 5 (Update item)
 *  index: index of the desired item in option 5 (Update item)
 *
 *  leave: boolean flag that determines if the user wants to leave
 *  	   returns false if the user typed 'n' (case-insensitive)
 *  isFound: boolean flag that determines if the item is found
 *  	     returns false if the item cannot be found, and true if the item is found
 *
 *  fileName: name of the file that the fucntion is opening
 *  inventory[MAX_ITEM_NO]: an Item array that would storage all the product's information
 *
 *  returns: none
 */
void runInventorySystem(void){
        int open; char fileName[] = "inventory.dat";
        struct Item inventory[MAX_ITEM_NO];//declare an item array called inventory

        printf("Loading inventory data...\n");
        open = loadItems(inventory, fileName, MAX_ITEM_NO);//open the file, then load the data into inventory

        if(open == 0){//if the program can't open the file
                printf("Cannot open file <%s>\n", fileName);
        }
        else{//if the program can open the file
                int input;
                pause();
                do{
                        input = menu();//getting user choices
                        if(input == 0){
                                bool leave;
                                printf("Exit the program? (Y)es/(N)o: ");//asks the user if they want to exit or not
                                leave = getYesOrNo();//gets the user's decision
                                if(leave == true){
                                        input = 0; //stays in the program
                                }
                                else{
                                        input = (-1);//exits the program
                                }
                        }
                        else if (input == 1){
                                listItems(inventory, MAX_ITEM_NO-1); //list all items
                        }
                        else if (input == 2){
                                search(inventory, MAX_ITEM_NO);//search an item
                        }
                        else if (input == 5){
                                int skuInput, index;
                                printf("Please enter the SKU: ");
                                skuInput = getIntLimited(SKU_MIN, SKU_MAX);//scan for a sku, which has to 
									   //within the range of 100-999 (inclusive)
                                bool isFound = locateItem(inventory, MAX_ITEM_NO, skuInput, &index);//locate 
                                                                                                    //item from theinventory array
                                if (isFound == true){//if such item exists
                                        printf("Current data:\n");
                                        displayItem(&inventory[index], FORM);//display the found item in FORM format
                                        updateItem(&inventory[index]);//prompt the user to update the item

                                }
                                else{//if no such item exists
                                        printf("Item not found!\n");
                                }
                        }
                        else{
                                printf("NOT IMPLEMENTED\n");
                                pause();
                        }
             }while(input != 0 || input == (-1));//repeats the loop until input equals to 0 or 1
      }
}

/*
 * Function: loadItems
 * -----------------
 *  load items into the Item array
 *
 *  parameters: 
 *  item[]: the Item array that would be loaded with data from the file
 *  fileName: the name of the file that fopen will open
 *  noOfRecsPtr: the number of records
 *
 *  variables:
 *  dataFile: holding all informations in the file
 *
 *  returns: 0 if fopen fails to open the file
 *           1 if fopen successfully opens the file
 */
int loadItems(struct Item item[], char fileName[], int noOfRecsPtr){
        FILE* dataFile;
        dataFile = fopen(fileName, "r");//open the file

        if (dataFile == NULL){ //if the function cannot find the file
                return 0;
        }
        else{ //if the function can find the file
                int i;
                for(i = 0; i < noOfRecsPtr; i++){
                        fscanf(dataFile, "%d,%[^,],%lf,%d,%d,%d",
                        &item[i].sku, item[i].name, &item[i].price,
                        &item[i].isTaxed, &item[i].quantity, &item[i].minQuantity);/*retriving sku, name, price, tax status (0 means not taxed,
										    1 means taxed), quantity and minimum quantity into the array*/
                }
        }

        fclose(dataFile);//close the file

        return 1;
}

/* 
 * Function: search
 * -------------------
 *  searching for an item using the sku
 *
 *  parameters:
 *  item[]: the Item array that would be searched
 *  noOfRecs: number of records\
 *
 *  variables:
 *  skuInput: sku entered by the user
 *  isFound: boolean flag that determines if the item is found
 *  	     returns false if the item doesn't exist, and true if it does exists
 *
 *  retruns: none
 *  */
void search(struct Item item[], int noOfRecs){
        int skuInput, index;
        printf("Please enter the SKU: ");//prompt the user to enter a sku
        skuInput = getIntLimited(SKU_MIN, SKU_MAX);//getting user input
        bool isFound = locateItem(item, noOfRecs, skuInput, &index);//search for the item

        if (isFound == true){//if the item exists
                displayItem(&item[index], FORM);//display details of the item in FORM format
        }
        else{//if the item doesn't exists
                printf("Item not found!\n");//tell the user no such item exists
        }
}

/*
 * Function: printGoodbye
 * --------------
 *  print goodbye message
 *
 *  parameters: none
 *
 *  variables: none
 *
 *  returns: none*/
void printGoodbye(void){
        printf("Thank you for using the system. By Geoffrey Wu\n");//print goodbye message
}

/*
 * Function: updateItem
 * -----------------
 *  undate an item
 *  parameters:
 *  itemptr: an item pointer that points to the user-selected item
 *
 *  variables:
 *  temp: a temperory Item that storages the new data
 *  update: a boolean flag that determines if the user wants to update the item
 *	    returns false if the user doesn't want to update, and true if the user wants to
 *
 *  returns: none
 */
void updateItem(struct Item* itemptr){
        printf("Enter new data:\n");
        struct Item temp = enterItem((*itemptr).sku);//storage the user's update on a new struct Item called temp

        printf("Overwrite old data? (Y)es/(N)o: ");//ask the user if they want to overwrite the old data
        bool update = getYesOrNo();//get the user input
        if (update == true){//if the user want to overwrite
                printf("--== Updated! ==--\n");
                *itemptr = temp;//overwrite the original data with the updated version
        }
        else{//if the user doesn't want to overwrite
                printf("--== Aborted! ==--\n");//abort the overwrite
        }
}

/*
 * Function: printWelcome
 * ------------------
 *  prints welcome message
 *
 *  parameters: none
 *
 *  variables: none
 *
 *  returns: none
 */
void printWelcome(void){
        printf("---=== Geoffrey's Grocery Inventory System ===---\n\n");//print welcome message
}

/* 
 * Function: printWelcome
 * ------------------
 *  prints the title
 *
 *  parameters: none
 *
 *  variables: none
 *
 *  returns: none
 */
void printTitle(void){
        printf("Row |SKU| Name               | Price  |Taxed| Qty | Min |   Total    |Atn"
        "\n----+---+--------------------+--------+-----+-----+-----+------------|---\n");//print the title
}


/* 
 * Function: printFooter
 * ------------------
 *  prints the footer
 *
 *  parameters: 
 *  gTotal: grand total of all items
 *
 *  variables: none
 *
 *  returns: none
 */
void printFooter(double gTotal){
        printf("--------------------------------------------------------+----------------\n");//print the footer
        if (gTotal > 0){//if the grand total is bigger than 0
                printf("                                           Grand Total: |     %.2lf\n", gTotal);//print the grand total
        }
}

/*
 * Function: pause
 * --------------------
 *  instruct the user to press enter to continue
 *
 *  parameters: none
 *
 *  variables: none
 *
 *  returns: none
 */
void pause(void){
        printf("Press <ENTER> to continue...");
        clearKeyboard();
}

/*
 * Function: clearKeyboard
 * -----------------------
 *  clear the keyboard
 *
 *  parameters: none
 *
 *  variables:
 *  enter: storage user input
 *
 *  returns: none
 */
void clearKeyboard(void){
        char enter = 0;
        while (enter != '\n'){//repeats the loop unitl the user press enter
                enter = getchar();//get new user input
        }
}

/*
 * Function: getInt
 * ----------------------------
 *  get an integer
 *
 *  parameters: none
 *
 *  variables:
 *  value: user input
 *  NL: new line 
 *
 *  returns: the user input if it's an integer
 */
int getInt(void){
        int value;
        char NL = 'x';
        while (NL != '\n'){;//repeats the loop until receive a vaild input
                scanf("%d%c", &value, &NL);//scan for user input
                if (NL != '\n'){//if the input isn't a number
                        clearKeyboard();
                        printf("Invalid integer, please try again: ");//advice the user
                }
        }
        return value;//return the user input

}

/*
 * Function: getIntLimited
 * ----------------------------
 *  get an integer within the range
 *
 *  parameters: 
 *  lowerlimited: self-explanatory
 *  upperLimit:  self-explanatory
 *
 *  variables:
 *  value: user input
 *
 *  returns the user input if it's an integer that's within the range
 */
int getIntLimited(int lowerLimit, int upperLimit){
        int value;
        do{
                value = getInt();//scan for user input
                if (value < lowerLimit || value > upperLimit){//if the input isn't within the range
                        printf("Invalid value, %d <= value <= %d: ", lowerLimit, upperLimit);//advice the user
                }
        }while (value < lowerLimit || value > upperLimit);//repeats the loop until receive a vaild input

        return value;//return the user input

}

/* 
 * Function: getDouble
 * ----------------------------
 *  get a number
 *
 *  parameters: none
 *
 *  variables:
 *  value: user input
 *  NL: new line 
 *
 *  returns: the user input if it's a number
 */
double getDouble(void){
        double value;
        char NL = 'x';
        while (NL != '\n'){;//repeats the loop until receive a vaild input
                scanf("%lf%c", &value, &NL);//scan for user input
                if (NL != '\n'){//if the input isn't a number
                        clearKeyboard();
                        printf("Invalid number, please try again: ");//advice the user
                }
        }

        return value;//return the user input
}

/* 
 * Function: getDoubleLimited
 * ----------------------------
 *  get a number within the range
 *
 *  parameters: 
 *  lowerlimited: self-explanatory
 *  upperLimit:  self-explanatory
 *
 *  variables:
 *  value: user input
 *
 *  returns the user input if it's a number that's within the range
 */
double getDoubleLimited(double lowerLimit, double upperLimit){
        double value;
        do{
                value = getDouble();//scan for user input
                if (value < lowerLimit || value > upperLimit){//if the input isn't within the range
                        printf("Invalid value, %.6lf <= value <= %.6lf: ", lowerLimit, upperLimit);//advice the user
                }
        }while (value < lowerLimit || value > upperLimit);//repeat the loop until receive a vaild input

        return value;//return the user input
}

/*
 * Function: getYesOrNo
 * ----------------------
 *  get the user's choice on yes/no question
 *
 *  parameters: none
 *
 *  variables:
 *  input: user input
 *  result: boolean flag
 *
 *  returns true when user enters "y", and false if the user enters "n" (Note: it's not case-sensitive)
 */
bool getYesOrNo(void){
        char input[MAX_ITEM_NO+1];
        bool result = false;

        do{
                scanf(" %s",input);//scans for user input

                if ((strcmp(input, "y") != 0 && (strcmp(input, "Y") != 0)) &&
                   ((strcmp(input, "n") != 0) && (strcmp(input, "N") != 0))){//if the user enters anything other than "y" or "n" (Note: it's not case-sensitive)
                        printf("Only (Y)es or (N)o are acceptable: ");
                }
        }while(((strcmp(input, "y") != 0 && (strcmp(input, "Y") != 0))) &&
              ((strcmp(input, "n") != 0) && (strcmp(input, "N") != 0)));//repeats the loop until receive a vaild input

        if(strcmp(input, "y") == 0 || strcmp(input, "Y") == 0){//if the user enters "y" (case-insensitive)
                result = true;
        }
        else if (strcmp(input, "n") == 0 || strcmp(input, "N") == 0){//if the user enters "n" (case-insensitive)
                result = false;
        }

        return result;//return the final result
}

/*
 * Function: getMenuChoice
 * ----------------------
 *  get menu choice
 *
 *  parameters: none
 *
 *  variables: 
 *  value: user input
 *
 *  returns the user input*/
int getMenuChoice(void){
        int choice;
        choice = getIntLimited(0, 7);//gets the user's choice, which is between 0-7 (inclusive)

        return choice;//return the choice
}

/*
 * Function: menu
 * ------------------------
 *  display all the options and receive user's decision
 *
 *  parameters: none
 *
 *  variables:
 *  decision: user input
 *
 *  returns user input
 */
int menu(void){
        int decision;
        printf("1- List all items\n2- Search by SKU\n3- Checkout an item\n4- Stock an item\n"
               "5- Update Item\n6- Delete item\n7- Search by name\n0- Exit program\n> ");//list all available options
        decision = getMenuChoice();//get the user's selection

        return decision;//return the selection
}


// mile3
/*
 * Function: totalAfterTax
 * ----------------------------
 *  calculate the taxed total price
 *
 *  parameters: 
 *  input: item selected for calculation
 *
 *  variables:
 *  result: toal price of a product
 *
 *  returns the taxed price
 */
double totalAfterTax(struct Item input){
        double result = input.quantity * input.price;//gets the product's overall price

        if(input.isTaxed == 1){//if the product is taxed
                result += result * TAX;//include the tax into the price
        }

        return result;
}

/*
 * Function: isQuantityLow
 * ---------------------
 *  determines if a item's quantity is too low
 *
 *  parameters:
 *  input: pointer to the item
 *
 *  variables:
 *  result: final result
 *
 *  returns true if the quantity is low, and false if the quantity isn't low
 */
bool isQuantityLow(struct Item *input){
        bool result;
        if((*input).quantity < (*input).minQuantity){//if the quantity is lower than the minimum quantity
                result = true;
        }
        else{//if the quantity is not lower than the minimum quantity
                result = false;
        }

        return result;//return the result
}

/*
 * Function: enterItem
 * ------------------
 *  enter new information to an item
 *
 *  parameters:
 *  skuInput: sku entered by the user
 *  newItem: new item to be created
 *  yesorNo: if the product is taxed or not
 *
 *  returns the newly created item
 */
struct Item enterItem(int skuInput){
        bool yesOrNo;
        struct Item newItem;

        newItem.sku = skuInput;
        printf("        SKU: %d\n", newItem.sku);
        printf("       Name: ");
        scanf("%20[^\n]s", newItem.name);//gets the new name
        clearKeyboard();
        printf("      Price: ");
        newItem.price = getDouble();//gets the new price
        printf("   Quantity: ");
        newItem.quantity = getInt();//gets the new quantity
        printf("Minimum Qty: ");
        newItem.minQuantity = getInt();//gets the new minimum quantity
        printf("   Is Taxed: ");
        yesOrNo = getYesOrNo();//get the new taxation status

        if (yesOrNo == true){//if the product is taxed
                newItem.isTaxed = 1;
        }
        else{//if the product is not taxed
                newItem.isTaxed = 0;
        }

        return newItem;//return the new item
}

/*
 * Function: displayItem
 * -------------
 *  display the item in a selected format
 *
 *  parameters: 
 *  *input: pointer to the selected item
 *  linear: format selcted
 *
 *  variables:
 *  taxed[]: taxation status
 *  lowQty: boolean flag that determines is the item's quantity is low
 *
 *  returns: none*/
void displayItem(struct Item *input, int linear){
        char *taxed[] = {"No", "Yes"};
        bool lowQty = isQuantityLow(input);

        if(linear == FORM){//if the FORM format is selected
                //print data in a form format
                printf("        SKU: %d\n", (*input).sku);
                printf("       Name: %s\n", (*input).name);
                printf("      Price: %.2lf\n", (*input).price);
                printf("   Quantity: %d\n", (*input).quantity);
                printf("Minimum Qty: %d\n", (*input).minQuantity);
                printf("   Is Taxed: %s\n", taxed[(*input).isTaxed]);

                if(lowQty == true && (*input).isTaxed == 1){//if the quantity is low
                        printf("WARNING: Quantity low, please order ASAP!!!\n");
                }
        }
        else{//if the LINEAR format is selected
                char *temp;

                if(lowQty == true){//if the quantity is low
                        temp = "***";
                }
                else{//if the quantity is not low
                        temp = "";
                }

                printf("|%d|%-20s|%8.2lf|%5s|%4d |%4d |%12.2lf|%s\n", (*input).sku, (*input).name,(*input).price,
		        taxed[(*input).isTaxed], (*input).quantity, (*input).minQuantity, totalAfterTax(*input), temp);//print data in a linear format
        }
}

/*
 * Function: listItems
 * ------------------------
 *  list all items
 *
 *  parameters:
 *  item[]: item array being displayed
 *  NoOfItems: number of items
 *
 *  variables:
 *  grandTotal: grand total of all items
 *  i: counter of the for loop
 *
 *  returns: none
 */
void listItems(struct Item item[], int NoOfItems){
        double grandTotal = 0;
        int i;

        printTitle();//print the title
        for(i = 0; i < NoOfItems; i++){
                printf("%-4d", (i+1));//print the row number
                displayItem(&item[i], LINEAR);//display the item details in LINEAR format
                grandTotal += totalAfterTax(item[i]);//add the product's total (after tax) into grandTotal
        }
        printFooter(grandTotal);//print the footer
}

/*
 * Function: locateItem
 * --------------
 *  locate an item
 *
 *  parameters:
 *  item[]: item array being displayed
 *  NoOfRecs: number of records
 *  sku: sku being searched
 *  *index: pointer to the found item's index
 *
 *  variables:
 *  i: counter of the for loop
 *
 *  returns true if the item is found, and false if the item is not found 
 */
bool locateItem(struct Item item[], int NoOfRecs, int sku, int *index){
        int i;
        for(i = 0; i < NoOfRecs; i++){//using a for loop to search the product
                if(item[i].sku == sku){//if the sku matches
                        *index = i;//assign the index to the pointer
                        return true;
                }
        }

        return false;
}

