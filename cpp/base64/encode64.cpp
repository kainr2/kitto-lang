#include <stdio.h>
#include <math.h>
#include <iostream>


//------------------------------------------------------------------------------
/// Convert given 3 bytes of characters to 4 chars in base64
///
/// @args[out] output  An array for storing the output
/// @args[in] input  An array of chars (3 bytes)
/// @args[in] nBits  How many bytes of valid input
///
/// @return  Number of bytes written
///
int encode64(char output[], const char* const input, int nBytes)
{
    // Define binary map for each of the three chars
    // 0000-0011 1111-2222 2233-3333
    // * http://en.wikipedia.org/wiki/Base64
    static const int BYTE0  = 252;
    static const int BYTE1A = 3;
    static const int BYTE1B = 240;
    static const int BYTE2A = 15;
    static const int BYTE2B = 192;
    static const int BYTE3  = 63;

    static const char TERM_CHAR = '=';
    static const int  MAX_OUTPUT = 4;


    // Error check: Check for how many bytes are going to be read
    if (nBytes < 1 || nBytes > 3) {
        printf("Error: incorrect amount of bytes (%d)\n", nBytes);
        return 0;
    }

    // Error check: size of output
    unsigned int unOutputSizeGiven = sizeof(output)/sizeof(char);
    if (unOutputSizeGiven < static_cast<unsigned int>(MAX_OUTPUT)) {
        printf("Insufficient amount of output buffer (%d)\n", unOutputSizeGiven);
        return 0;
    }


    // Read each char input
    int nBytesOut = nBytes+1;
    if (nBytes>=1)
    {
        output[0] = (input[0] & BYTE0) >> 2;
        output[1] = ((input[0] & BYTE1A) << 4) + ((input[1] & BYTE1B) >> 4);
        output[2] = output[3] = TERM_CHAR;
    }

    if (nBytes>=2) {
        output[2] = ((input[1] & BYTE2A) << 2) + ((input[2] & BYTE2B) >> 6);
    }

    if (nBytes==3) {
        output[3] = (input[2] & BYTE3);
    }


    // Convert to base64 -- five sets
    // (1) 0-25 [A-Z]=[65-90]
    // (2) 26-51 [a-z]=[97-122]
    // (3) 52-61 [0-9]=[48-57]
    // (4) 62 [+]=[43]
    // (5) 63 [/]=[47]
    // * http://en.wikipedia.org/wiki/Base64
    // * http://www.asciitable.com/
    for (int i=0;i<nBytesOut;i++)
    {
        if (output[i]<=25) { 
            output[i] += 65;
        } else if (output[i]<=51) {
            output[i] += 71;  // 97-26
        } else if (output[i]<=61) {
            output[i] -= 4;   // 48-52
        } else if (output[i]==62) {
            output[i] = 43;
        } else if (output[i]==63) {
            output[i] = 47;
        }
    }

    return nBytesOut;
}


//------------------------------------------------------------------------------
/// Convert the content of the given file to base64, and store in output
///
/// @args[out] output  Output buffer
/// @args[in] filename  File path and name to read
///
/// @return  On success return 1. If failed, return negative value
/// 
int fileToBase64(char* output, const char* const filename)
{
    static char SIZE_OF_CHAR = sizeof(char);
    static char READ_SIZE = 3;

    printf("file path = %s\n", filename);

    FILE* fp;
    fp = fopen(filename, "rb");

    // Error check: if the file doesn't exist
    if (fp==NULL) {
        printf("File doesn't exist: %s\n", filename);
        return -1;
    }

    // Check the file size to store buffer
    // * http://stackoverflow.com/questions/238603/how-can-i-get-a-files-size-in-c
    fseek(fp, 0, SEEK_END);
    long nInputSize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    // Calculate the buffer size. For each 3 bytes => 4 bytes
    long nMaxBufferSize = static_cast<long>(ceil(static_cast<double>(nInputSize)/3)) * 4;

    // Error check: valid size
    if (nMaxBufferSize < 1) {
        printf("Invalid file size: %ld\n", nInputSize);
        return -2;
    }


    // Read 3 chars per call until the end
    nMaxBufferSize += 1;  // add EOL char
    char* buffer = new char[nMaxBufferSize];
    long  nCurrPos = 0;   // Keep track of position in buffer

    size_t nRead = 0, nWritten = 0;
    char   temp[3];
    bool   bContinue = true;
    while (bContinue)
    {
        // Read n bytes, and skip EOF char
        nRead = fread(temp, SIZE_OF_CHAR, READ_SIZE, fp); 
        if (nRead>0 && feof(fp)) {
            --nRead;
        }

        // If less than 3 bytes, fill zeros
        switch (nRead) 
        {
        case 1:   // read 1 char, fill 1 & 2 (fall through case)
            temp[1] = 0;
        case 2:   // read 2 chars, fill 2
            temp[2] = 0;
        break;

        case 3:   // A valid case
        break;

        default:  // EOF
            bContinue = false;
        break;
        }

        // Exit loop
        if (!bContinue) {
            break;
        }

        // Convert the given 3 bytes to 4 bytes output(nRead)
        nWritten = encode64(&buffer[nCurrPos], temp, nRead);

        // If failed, stop reading
        if (nWritten==0) {
            bContinue = false;
        }
        // Else, update current position
        else
        {
            // Add EOS char (for printout)
            nWritten = 4;
            buffer[nCurrPos+nWritten] = '\0';
            printf ("input [%d][%.3s] = %s\n", 
                static_cast<unsigned int>(nRead), temp, &(buffer[nCurrPos]));

            // Move the pointer forward
            nCurrPos += nWritten;
        }

    }


    // Print out the content
    buffer[nCurrPos] = '\0';  // EOS char
    printf("Base64 output: %s\n\n", buffer);


    // Clean up the buffer
    delete [] buffer;


    fclose(fp);
    return 1;
} 




//------------------------------------------------------------------------------
int main(int argc, char* argv[])
{
    printf("hello world\n");

    //char input[] = { 'M','a','n' }; // => TWFu
    char output[4] = {0,0,0,0};

/*
    int status = encode64(output, input, 3);
    printf("encode64[%d] = {%d, %d, %d, %d}\n", status,
        output[0], output[1], output[2], output[3]);
    printf("==> {%c, %c, %c, %c}\n",
        output[0], output[1], output[2], output[3]);
*/


    // Testing input from the user
    if (argc==2)
    {
        int status = fileToBase64(output, argv[1]);
        printf ("Base64 status = %d\n", status);
    }



    return 0;
}


