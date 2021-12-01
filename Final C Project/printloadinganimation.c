// C program to create loading bar
#include <stdio.h>
#include <windows.h>
  
// Function to creating loading bar
void loadingBar()
{
    // 0 - black background,
    // A - Green Foreground
    system("color 0A");
  
    // Initialize char for printing
    // loading bar
    char a = 177, b = 219;

    printf("\n\t\t\t\t\t Saving...\n\n");
    printf("\t\t\t\t\t");
  
    // Print initial loading bar
    for (int i = 0; i < 26; i++)
        printf("%c", a);
  
    // Set the cursor again starting
    // point of loading bar
    printf("\r");
    printf("\t\t\t\t\t");
  
    // Print loading bar progress
    for (int i = 0; i < 26; i++) {
        printf("%c", b);
  
        // Sleep for 1 second
        Sleep(50);
    }
    printf("\n\n\t\t\t\t\t SAVED!\n\n");
    system("color 0F");
}
  
// Driver Code
// int main()
// {
//     // Function Call
//     loadingBar();
//     return 0;
// }