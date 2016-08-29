#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char* argv[])
{
    
    // Open raw file
    FILE *masterFile = fopen("card.raw", "r");
    if (masterFile == NULL)
    {
        printf("Could not open card.raw\n");
        return 1;
    } else {
        printf("Awesome, it's open!\n");
    }
    
    
    // Declare JPG signatures
    char jpgSignatureA[4] = {0xff, 0xd8, 0xff, 0xe0};
    char jpgSignatureB[4] = {0xff, 0xd8, 0xff, 0xe1};
    
    
    char buffer[512];           // Allocate 512 bytes for the buffer
    char bufferSignature[4];    // Allocate 4 bytes for the buffer's signature
    char title[8];              // Allocate 8 bytes for title(s) of JPGs
    int titleCounter = 0;       // Create counter for each JPG we find
    int jpgOpen = 0;            // Bool value to determine if we're appending data
    FILE *img;                  // Declare new JPG file
    

    

    
    // Loop through entire file in 512 byte chunks (since it's a FAT system)
    do {
        
        // Place the buffer's signature into it's respective array
        for (int i = 0; i < 4; i++) {
            bufferSignature[i] = buffer[i];
        }
        
        // Check to see if this is a JPG
        if (memcmp(jpgSignatureA, bufferSignature, 4) == 0 || memcmp(jpgSignatureB, bufferSignature, 4) == 0) {
            
            // If we already have a new JPG file in progress, close it
            if (jpgOpen == 1) {
                fclose(img);
            }
        
            // Generate the new JPG's title
            sprintf(title, "%03d.jpg", titleCounter);
            titleCounter++;
            printf("%s\n", title);
            
            // Write the bytes into a new JPG file
            img = fopen(title, "a");
            fwrite(buffer, 512, 1, img);
            jpgOpen = 1;
            
        } else {
            
            // Check to see if there's a JPG file in progess. If so, this is its additional data so keep appending.
            if (jpgOpen == 1) {
                fwrite(buffer, 512, 1, img);
            }
            
        }

    } while (fread(buffer, 512, 1, masterFile) == 1);
    
    
    // Close the raw file
    fclose(masterFile);
    
}
