// MILESTONE 3 - main only

#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

// SYMBOLIC CONSTANTS

#define  MAX_LENGTH 20  // of SKU name
#define  LINEAR      1
#define  FORM        0

// global - struct Item

struct Item 
{
   double price;
   int    sku;
   int    isTaxed;
   int    quantity;
   int    minQuantity;
   char   name[MAX_LENGTH+1]; // 1 for delimiter
};

// mile 1 functions

void   printWelcome     ( void           );
void   printTitle       ( void           );
void   printFooter      ( double         );
void   clearKeyboard    ( void           );
void   pause            ( void           );
int    getInt           ( void           );
double getDouble        ( void           );
int    getIntLimited    ( int, int       );
double getDoubleLimited ( double, double );

// mile 2 functions

bool   getYesOrNo         ( void );
int    getMenuChoice      ( void );
void   runInventorySystem ( void );

// mile 3 functions

double      totalAfterTax ( struct Item                         );
bool        isQuantityLow ( struct Item *                       );
struct Item enterItem     ( int                                 );
void        displayItem   ( struct Item *, int                  );
void        listItems     ( struct Item [], int                 );
bool        locateItem    ( struct Item item[], int, int, int * );

// global - TAX rate

//const double TAX = 0.13;

// ==================  MAIN FUNCTION ===============

int main( void ) 
{
   // hard-coded inventory; 21 items - room for 1 more

   struct Item inventory[21] = 
   {
     // price  sku  txd qty min  name
 
      {  4.40, 275, 0,  10,  2, "Royal Gala Apples"   }, 
      {  5.99, 386, 0,  20,  4, "Honeydew Melon"      },
      {  3.99, 240, 0,  30,  5, "Blueberries"         },
      { 10.56, 916, 0,  20,  3, "Seedless Grapes"     },
      {  2.50, 385, 0,   5,  2, "Pomegranate"         },
      {  0.44, 495, 0, 100, 30, "Banana"              },
      {  0.50, 316, 0, 123, 10, "Kiwifruit"           },
      {  4.49, 355, 1,  20,  5, "Chicken Alfredo"     },
      {  5.49, 846, 1,   3,  5, "Veal Parmigiana"     },
      {  5.29, 359, 1,  40,  5, "Beefsteak Pie"       },
      {  4.79, 127, 1,  30,  3, "Curry Chicken"       },
      { 16.99, 238, 1,  10,  2, "Tide Detergent"      },
      { 10.49, 324, 1,  40,  5, "Tide Liq. Pods"      },
      { 10.98, 491, 1,  50,  5, "Tide Powder Det."    },
      {  3.69, 538, 1,   1,  5, "Lays Chips S&V"      },
      {  3.29, 649, 1,  15,  5, "Joe Org Chips"       },
      {  1.79, 731, 1, 100, 10, "Allen's Apple Juice" },
      {  6.69, 984, 1,  30,  3, "Coke 12 Pack"        },
      {  7.29, 350, 1,  50,  5, "Nestea 12 Pack"      },
      {  6.49, 835, 1,  20,  2, "7up 12 Pack"         }  
   };

   // --- test totalAfterTax

   printf( "=== TEST ITEM INPUT/OUTPUT ===\n" );
  
   double val = totalAfterTax( inventory[0] );
   printf( "SKU 275: Total After Tax: %.2lf\n"
           "Expected              --> 44.00\n", val );

   val = totalAfterTax( inventory[7] );
   printf( "SKU 355: Total After Tax: %.2lf\n"
           "Expected              --> 101.47\n", val );

   // --- test isQuantityLow

   int ival = isQuantityLow( &inventory[1] );
   printf( "SKU 386: Is Quantity Low: %d\n"
           "Expected              --> 0\n", ival );

   ival = isQuantityLow( &inventory[14] );
   printf( "SKU 538: Is Quantity Low: %d\n"
           "Expected              --> 1\n", ival );

   // --- test enterItem 

   pause( );
   printf( "Enter the following values:\n" );
   printf( "        SKU: 999\n"
           "       Name: Red Apples\n"
           "      Price: 4.54\n"
           "   Quantity: 50\n"
           "Minimum Qty: 5\n"
           "   Is Taxed: n\n");

   printf( "Enter the values:\n" );

   inventory[20] = enterItem( 999 );

   // --- test displayItem of last entered item: linear & form

   printf( "Display Item, Linear:\n" );

   displayItem( &inventory[20], LINEAR );

   printf( "Expected -->\n" );

   printf( "|999|Red Apples          "
           "|    4.54|   No"
           "|  50 |   5 |      227.00|\n" );

   printf( "Display Item, Form:\n" );

   displayItem( &inventory[20], FORM );

   printf( "Expected -->\n" );
   printf( "        SKU: 999\n"
           "       Name: Red Apples\n"
           "      Price: 4.54\n"
           "   Quantity: 50\n"
           "Minimum Qty: 5\n"
           "   Is Taxed: No\n" );

   // change last item: qty = 2, is taxed

   inventory[20].quantity = 2;
   inventory[20].isTaxed  = true;
   pause( );

   printf( "Display Item, Linear with low value and taxed:\n" );
   displayItem( &inventory[20], LINEAR );
   printf( "Expected -->\n" );
   printf( "|999|Red Apples          |    4.54|  Yes"
           "|   2 |   5 |       10.26|***\n" );

   printf( "Display Item, Form with low value:\n" );
   displayItem( &inventory[20], FORM );
   printf( "Expected -->\n" );
   printf( "        SKU: 999\n"
           "       Name: Red Apples\n"
           "      Price: 4.54\n"
           "   Quantity: 2\n"
           "Minimum Qty: 5\n"
           "   Is Taxed: Yes\n"
           "WARNING: Quantity low, please order ASAP!!!\n" );

  // --- test listItems

   pause( );
   printf( "List Items: \n" );

   listItems( inventory, 21 );

   printf( "Expected                               " );
   printf( "--> Grand Total: |     3478.59\n" );

   pause( );

   // --- test locateItem - success

   printf( "Locate Item, successful search:\n" );
  
   int searchIndex;

   if ( locateItem( inventory, 21, 999, &searchIndex) )
      printf( "SKU 999 found at index: %d\n", searchIndex ); 
   else
      printf( "SKU 999 not found in the inventory!\n" );

   printf( "Expected -->\n" );
   printf( "SKU 999 found at index: 20\n" );

   // --- test locateItem - fail

   printf( "Locate Item, unsuccessful search:\n" );
   
   if ( locateItem( inventory, 21, 732, &searchIndex ) )
      printf( "SKU 732 found at index: %d\n", searchIndex );
   else
      printf( "SKU 732 not found in the inventory!\n" );

   printf( "Expected -->\n" );
   printf( "SKU 732 not found in the inventory!\n" );

   printf( "=== END TEST ===\n" );
 
   return 0;
}

