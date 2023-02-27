#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//Signatures
int arr4_toHexValue(int *arr);
int arr2_toHexValue(int *arr);

void main(){
    FILE* f = fopen("dump.zip","rb");

    if(!f) {
        puts("FILE NOT FOUND!");
        exit(0);
    }

    fseek(f,0L,SEEK_END);
    int file_size = ftell(f);
    fseek(f,0L,SEEK_SET);

    unsigned char* buffer = calloc(file_size, 2);
    fread(buffer, sizeof(char), file_size,f);
    //file read to buffer

    //Print the dictionary
    printf("1.Dictionary?: ");
    int i;
    int counter= 0;
    for(i=0;i<file_size;i++){
    if(buffer[i-3] == 'f' && buffer[i-2] == 'l' && buffer[i-1] == 'a' 
    && buffer[i] == 'g' && buffer[i+3] != 'P' && counter < 36) { // Scan for flagXX<char>
        counter++; 
        putchar(buffer[i+3]);
    }
    }
    
    puts("\n2.List Of Offsets to central directories:");
    // We know that there are 21 EOCDS

    unsigned int offsets[21] ;

    int j;
    //Count EOCD records
    int counter1 = 0;
    unsigned int arr[4];
    for(j=0;j<file_size;j++){
        if(buffer[j] == 0x50 && buffer[j+1] == 0x4B && buffer[j+2] == 0x05 && buffer[j+3] == 0x06){
            arr[0] = buffer[j+16];
            arr[1] = buffer[j+17];
            arr[2] = buffer[j+18];
            arr[3] = buffer[j+19];
            printf("%d ", arr4_toHexValue(arr));
            offsets[counter1] = arr4_toHexValue(arr);
            counter1++;
        }
    }
    printf("\nEOCD count: %d", counter1);

    // Let's check those central directories:
    // offset 42 of CDFH gives 4 bytes of relative offset of local file header

    puts("\n3.Check for relative offset of LFH:");
    for(int k = 0; k< 21; k++){
        arr[0] = buffer[offsets[k] + 42];
        arr[1] = buffer[offsets[k] + 43];
        arr[2] = buffer[offsets[k] + 44];
        arr[3] = buffer[offsets[k] + 45];

        printf("%d ", arr4_toHexValue(arr));
        offsets[k] = arr4_toHexValue(arr);
    }

    // Now we got the 21 LFH we can get content from

    puts("\n4.Find Content Of Files:");

    int extra_size, name_size;

    for(int k=0;k<21;k++){
        arr[0] = buffer[offsets[k] + 8];
        arr[1] = buffer[offsets[k] + 9];

        if(arr2_toHexValue(arr) != 0) printf("FOUND COMPRESSION AT LINE: %x", offsets[k]); // Find compression

        // Find uncompressed size:

        arr[0] = buffer[offsets[k] + 22];
        arr[1] = buffer[offsets[k] + 23];
        arr[2] = buffer[offsets[k] + 24];
        arr[3] = buffer[offsets[k] + 25];

        extra_size = arr2_toHexValue(arr); 

        if(extra_size > 1) continue; // each file should contain one char from dictionary...

        //printf("\nExtra size: %d", extra_size);

        // Find name size to add to offset:

        arr[0] = buffer[offsets[k] + 26];
        arr[1] = buffer[offsets[k] + 27];

        name_size = arr2_toHexValue(arr);

        // Find Extra field (SIZE OF CHAR)

        char content = buffer[offsets[k] + 30 + name_size];
        putchar(content); // That's it !
    }

    

    fclose(f);
    free(buffer);

    puts("\n");
    system("pause");

}

//Convert 4 bytes of char array to int value
int arr4_toHexValue(int *arr){
  return (arr[3] << 24) | (arr[2] << 16) | (arr[1] << 8) | (arr[0]) ;
}

int arr2_toHexValue(int *arr){
    return (arr[1] << 8) | arr[0];
}