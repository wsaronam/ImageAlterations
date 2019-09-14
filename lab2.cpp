/*
    ICS 51, Lab #2

    --------------------------------------------------------------------
 
    IMPORTATNT NOTES:
    
    - To test your code use the tests in lab2-tester.cpp
    
    - Write your assembly code only in the marked areas.
    
    - You are not allowed to change anything in this file except for adding your assembly code 
    between the lines marked with "YOUR CODE STARTS HERE" and "YOUR CODE ENDS HERE".

    - This lab has to be done individually.

    - Remember to fill in your name, student ID below.

    - Two test images are provided to you (lena.png and authoritah.png)
    
    - Submit ONLY your completed lab2.cpp for grading.
*/

char *yourName = "Willy Saronamihardja";
char *yourStudentID = "80408898";


/***********************************************************************************************

    PART 1: Image Thresholding

    - This function takes in a square sized grayscale image and applies thresholding on each pixel.
      i.e. it should change pixel values according to this formula:
                        0xFF       if x >= threshold
                        0x00       if x <  threshold
    - The width and height of the image are equal to dim. 
    - You are not allowed to define additional variables.

************************************************************************************************/
void imageThresholding(unsigned char* image, int dim, unsigned char threshold) {
    __asm 
    {
        // YOUR CODE STARTS HERE
		mov edi, image
		mov esi, dim
		mov dh, threshold

		mov ebx, 0  // row counter
		mov ecx, 0	// column counter

	StartOfLoop:
		mov eax, ebx  
		imul eax, esi
		add eax, ecx  // offset creation

		imul eax, 1
		add eax, edi  // current element-address

		cmp [eax], dh
		jae AboveThreshold
		mov byte ptr [eax], 0x00
		jmp ElementEnd

	AboveThreshold:
		mov byte ptr[eax], 0xFF
		jmp ElementEnd

	ElementEnd:
		dec esi  // decrement for zero-based indexing for comparison
		cmp ecx, esi
		je CheckRow  // if current column is at max row #, jump
		inc ecx
		inc esi  // increment for original dim #
		jmp StartOfLoop

	CheckRow:
		cmp ebx, esi
		je EndOfLoop  // if current column and current row are at max #, finish loop
		mov ecx, 0
		inc esi  // increment for original dim #
		inc ebx
		jmp StartOfLoop

	EndOfLoop:
        // YOUR CODE ENDS HERE
    }
}


/***********************************************************************************************

    PART 2: Image Rotation

    - This function rotates a square sized color image:
      90 degress clockwise if direction > 0
      90 degress counterclockwise if direction < 0
    - If direction == 0, the image is not rotated.
    - The width and height of the image are both equal to dim.
    - Four variables (a0, a90, a180, a270) are defined that you may use in your implementation for
    temporary data storage. You are not allowed to define additional variables.

************************************************************************************************/
void imageRotation(unsigned int* image, int dim, int direction) {
    unsigned int a0, a90, a180, a270;
    __asm 
    {
        // YOUR CODE STARTS HERE

		mov eax, image

		mov ebx, 0  // column counter
		mov ecx, 0	// row counter

        // Jump to the end (i.e, return) if direction == 0
		cmp direction, 0
		je EndOfRotation

        // Iterate over the red triangle (row by row starting from top left)
	StartOfRotation:

            // compute index of pixel p0, find the corresponding memory address and store it in a0
			mov edx, ecx	// row
			imul edx, dim
			add edx, ebx

			imul edx, 4
			add edx, eax
			mov a0, edx

            // compute index of pixel p90, find the corresponding memory address and store it in a90
			mov esi, dim
			dec esi
			sub esi, ecx	// p90's column
			mov edx, ebx	// p90's row

			imul edx, dim
			add edx, esi

			imul edx, 4
			add edx, eax
			mov a90, edx

            // compute index of pixel p180, find the corresponding memory address and store it in a180
			mov esi, dim
			dec esi
			sub esi, ebx	// p180's column
			mov edx, dim
			dec edx
			sub edx, ecx	// p180's row

			imul edx, dim
			add edx, esi

			imul edx, 4
			add edx, eax
			mov a180, edx

            // compute index of pixel p270, find the corresponding memory address and store it in a270
			mov esi, ecx	// p270's column
			mov edx, dim
			dec edx
			sub edx, ebx	// p270's row

			imul edx, dim
			add edx, esi

			imul edx, 4
			add edx, eax
			mov a270, edx

            // rotationally swap pixel values
			cmp direction, 0
			jl LessThanZero

            // direction > 0 : p0 -> p90 -> p180 -> p270 -> p0
			mov edx, a0					// load a0
			mov edx, dword ptr [edx]	// store value of a0
			mov esi, a90				// load a90
			mov edi, dword ptr [esi]	// store a90 value
			mov dword ptr [esi], edx	// change a90's value to a0's value

			mov esi, a180				// load a180
			mov edx, dword ptr[esi]		// store a180 value
			mov dword ptr [esi], edi	// change a180's value to a90's value

			mov esi, a270				// load a270
			mov edi, dword ptr [esi]	// store a270 value
			mov dword ptr [esi], edx	// change a270's value into a180's value

			mov esi, a0					// load a0
			mov dword ptr [esi], edi	// change a0's value into a270's value

			jmp CheckColumn

            // direction < 0 : p0 -> p270 -> p180 -> p90 -> p0
		LessThanZero:
			mov edx, a0					// load a0
			mov edx, dword ptr[edx]		// store value of a0
			mov esi, a270				// load a270
			mov edi, dword ptr[esi]		// store a270 value
			mov dword ptr[esi], edx		// change a270's value to a0's value

			mov esi, a180				// load a180
			mov edx, dword ptr[esi]		// store a180 value
			mov dword ptr[esi], edi		// change a180's value to a270's value

			mov esi, a90				// load a90
			mov edi, dword ptr[esi]		// store a90 value
			mov dword ptr[esi], edx		// change a90's value into a180's value

			mov esi, a0					// load a0
			mov dword ptr[esi], edi		// change a0's value into a90's value

		CheckColumn:
			mov edx, dim
			sub edx, 2
			sub edx, ecx
			cmp ebx, edx
			jge CheckRow
			inc ebx
			jmp StartOfRotation

		CheckRow:
			mov edx, dim
			dec edx
			shr edx, 1
			cmp ecx, edx
			jge EndOfRotation
			inc ecx
			mov ebx, 0
			add ebx, ecx
			jmp StartOfRotation

	EndOfRotation:

        // YOUR CODE ENDS HERE
    }
}