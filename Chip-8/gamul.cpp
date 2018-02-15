#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gamul.h"
#include <fstream>
#include <iostream>


#define MAX_ALLOWED_SIZE 3584 // 4096-512

using namespace std;

unsigned short opcode;



// font set for rendering
const unsigned char fontset[FONTSET_SIZE] = {
	0xF0, 0x90, 0x90, 0x90, 0xF0,		// 0
	0x20, 0x60, 0x20, 0x20, 0x70,		// 1
	0xF0, 0x10, 0xF0, 0x80, 0xF0,		// 2
	0xF0, 0x10, 0xF0, 0x10, 0xF0,		// 3
	0x90, 0x90, 0xF0, 0x10, 0x10,		// 4
	0xF0, 0x80, 0xF0, 0x10, 0xF0,		// 5
	0xF0, 0x80, 0xF0, 0x90, 0xF0,		// 6
	0xF0, 0x10, 0x20, 0x40, 0x40,		// 7
	0xF0, 0x90, 0xF0, 0x90, 0xF0,		// 8
	0xF0, 0x90, 0xF0, 0x10, 0xF0,		// 9
	0xF0, 0x90, 0xF0, 0x90, 0x90,		// A
	0xE0, 0x90, 0xE0, 0x90, 0xE0,		// B
	0xF0, 0x80, 0x80, 0x80, 0xF0,		// C
	0xE0, 0x90, 0x90, 0x90, 0xE0,		// D
	0xF0, 0x80, 0xF0, 0x80, 0xF0,		// E
	0xF0, 0x80, 0xF0, 0x80, 0x80		// F
};
gamul8:: gamul8(){
	cout<<"Object is Constructed"<<endl;
}
gamul8::~gamul8(){
	cout<<"object is deleted"<<endl;
}
/*
Function : OpCode 0x
*/

void gamul8::Function_0(){
	
	switch(opcode&0x000F){
		case 0x0000:
		// Clears the Screen Display 
		
		for(int i=0;i<SCREEN_WIDTH;i++){
			
			for(int j=0;j<SCREEN_HEIGHT;j++){
				display[i][j]=0;
				clrscr = true;
			}
		}
		PC=PC+2;
		break;
		case 0x000E:
		// Returns from Subroutine
		// Change the Stack Pointer 
		// Update PC from the value stored in Stack
		SP=SP-1;
		PC=Stack[SP];
		PC=PC+2;
		break;
		default:
		cout<<"OpCode Not Implemented"<<endl;
		
	}
	return;
}

void gamul8::Function_1(){
	// Update PC with value in Opcode
	PC=opcode&0x0FFF;
	return ;
}

void gamul8::Function_2(){
	// Function Call 
	// Store the value of current PC on Stack and update the Stack Pointer 
	// Update the Program Counter to value in Opcode
	Stack[SP]=PC;
	SP++;
	PC=opcode&0x0FFF;

	return;

}

//f(Vx==NN) 	Skips the next instruction if VX equals NN.
void gamul8::Function_3(){
	 // If Condition
	 int RegX=(opcode&0x0F00)>>8;
	 if(V_Reg[RegX]==(opcode&0x00FF)){
		 PC=PC+4;
	 }
	 else{
		 PC=PC+2;
	 }

	 return;

}

// if(Vx!=NN) 	Skips the next instruction if VX doesn't equal NN.
void gamul8::Function_4(){

	int RegX=(opcode&0x0F00)>>8;
	if(V_Reg[RegX]!=(opcode&0x00FF)){
		PC=PC+4;
	}
	else{
		PC=PC+2;
	}
	return;	
}


//if(Vx==Vy) 	Skips the next instruction if VX equals VY.
void gamul8::Function_5(){

	int RegX=(opcode&0x0F00)>>8;
	int RegY=(opcode&0x00F0)>>4;
	if(V_Reg[RegX]==V_Reg[RegY]){
		PC=PC+4;
	}
	else{
		PC=PC+2;
	}
	return;
}

//Sets VX to NN.
void gamul8::Function_6(){

	int RegX=(opcode&0x0F00)>>8;
	V_Reg[RegX]=(opcode&0x00FF);
	PC=PC+2;
	return;
}

//Adds NN to VX.
void gamul8::Function_7(){
	
	int RegX=(opcode&0x0F00)>>8;
	V_Reg[RegX]=V_Reg[RegX]+(opcode&0x00FF);
	PC=PC+2;
	return;
}

//if(Vx!=Vy) 	Skips the next instruction if VX doesn't equal VY.
void gamul8::Function_9(){
	
	int RegX=(opcode&0x0F00)>>8;
	int RegY=(opcode&0x00F0)>>4;
	if(V_Reg[RegX]!=V_Reg[RegY]){
		PC=PC+4;
	}
	else{
		PC=PC+2;
	}
	return;
}


void gamul8::Function_8(){

	int Reg_X=(opcode&0x0F00)>>8;
	int Reg_Y=(opcode&0x00F0)>>4;

	switch(opcode&0x000F){
		case 0x1:
        // OR
		V_Reg[Reg_X]=V_Reg[Reg_X]|V_Reg[Reg_Y];

		break;

		case 0x2:
		//AND
		V_Reg[Reg_X]=V_Reg[Reg_X]&V_Reg[Reg_Y];

		break;

		case 0x3:
		// XOR
		V_Reg[Reg_X]=V_Reg[Reg_X]^V_Reg[Reg_Y];


		break;

		case 0x4:
		//Addition
		if(V_Reg[Reg_X]>(0xFF-V_Reg[Reg_Y])){
			V_Reg[0x000F]=1;
		}
		else{
			V_Reg[0x000F]=0;
		}
		V_Reg[Reg_X]=V_Reg[Reg_X]+V_Reg[Reg_Y];

		break;

		case 0x5:
		//Subtraction
		// Carry in Subtraction when RegY is greater than RegX
		//Vx=Vx-Vy
		if(V_Reg[Reg_X]>(V_Reg[Reg_Y])){
			V_Reg[0x000F]=0;
		}
		else{
			V_Reg[0x000F]=1;
		}
		V_Reg[Reg_X]=V_Reg[Reg_X]-V_Reg[Reg_Y];


		break;

		case 0x6:

		//Right Shift VY by one and copies the value to VX
		V_Reg[0x000F]=V_Reg[Reg_Y]&(0x1);
		V_Reg[Reg_X]=V_Reg[Reg_Y]=V_Reg[Reg_Y]>>1;

		break;

		case 0x7:
		//Subtraction
		// Carry in Subtraction when RegX is greater than Regy
		// Vx=Vy-Vx
		if(V_Reg[Reg_X]>(V_Reg[Reg_Y])){
			V_Reg[0x000F]=1;
		}
		else{
			V_Reg[0x000F]=0;
		}
		V_Reg[Reg_X]=V_Reg[Reg_Y]-V_Reg[Reg_X];

		break;

		case 0xE:
		// V_F is most significant of Regx 
		// RegX and RegY take Reg_X left shift by 1
		V_Reg[0x000F]=V_Reg[Reg_X]>>7;
		V_Reg[Reg_X]=V_Reg[Reg_Y]=V_Reg[Reg_X]<<1;

		break;

	}
	PC=PC+2;
	return;

}

void gamul8::Function_A(){

	I=opcode&0X0FFF;
	PC=PC+2;

}

void gamul8::Function_B(){
	PC=V_Reg[0]+(opcode&0x0FFF);
	
}

void gamul8::Function_C(){
	int randomnumber=rand()%256;
	V_Reg[opcode&0x0F00>>8]=randomnumber&(opcode&0x00FF);
	PC=PC+2;
	
}

/*Draws a sprite at coordinate (VX, VY) that has a width of 8 pixels and a height of N pixels. Each row of
8 pixels is read as bit-coded starting from memory location I; I value doesn’t change after the execution
of this instruction. As described above, VF is set to 1 if any screen pixels are flipped from set to unset
when the sprite is drawn, and to 0 if that doesn’t happen.*/

void gamul8::Function_D(){
	int Reg_X = (opcode & 0x0F00)>>8;
	int Reg_Y = (opcode & 0x00F0)>>4;
	int i,j;
	unsigned short height = (opcode & 0x000F);
	unsigned short width = 8;
	unsigned short pixel;
	unsigned short X = V_Reg[Reg_X];
	unsigned short Y = V_Reg[Reg_Y];
	V_Reg[0xF] = 0;

	for (j=0; j < height; j++){
		pixel = Memory[I + j];
		for(i=0; i < width; i++){
			if( (pixel & (0x80 >> i)) != 0){
				if(display[X + i][Y + j] == 1){
					V_Reg[0xF] = 1;
				}
			display[X + i][Y + j] ^= 1;	

			}

		}
	}

    clrscr = true;
    PC = PC + 2;	
}


void gamul8::Function_E(){
	//Skips the next instruction if the key stored in VX is pressed.
	switch(opcode & 0x00FF)
	{
		case 0x009E: // EX9E: Skips the next instruction if the key stored in VX is pressed
			if(key[V_Reg[(opcode & 0x0F00) >> 8]] != 0)
				PC += 4;
			else
				PC += 2;
		break;
		
		case 0x00A1: // EXA1: Skips the next instruction if the key stored in VX isn't pressed
			if(key[V_Reg[(opcode & 0x0F00) >> 8]] == 0)
				PC += 4;
			else
				PC += 2;
		break;

		default:
			printf ("Unknown opcode [0xE000]: 0x%X\n", opcode);
	}

	
}

void gamul8::Function_F(){
	int Reg_X = (opcode & 0x0F00)>>8;
	bool key_is_pressed = false;
	
	//Sets VX to the value of the delay timer.
    if ((opcode & 0x00FF) == 0x0007){
    	V_Reg[Reg_X] = delay_timer;
    	PC = PC + 2;    	
    }

    //A key press is awaited, and then stored in VX

    else if ((opcode & 0x00FF) == 0x000A){

    	for(int i = 0; i < 16; i++){
			if(key[i] != 0)
			{
				V_Reg[Reg_X] = i;
				key_is_pressed = true;
			}
		}

    	if(key_is_pressed){
    		PC = PC + 2;
    	}
    	else{
    		return;
    	}
    }

    // Sets the delay timer to VX.
    else if ((opcode & 0x00FF) == 0x0015){
    	delay_timer = V_Reg[Reg_X];
    	PC = PC + 2;
    }

    // Sets the sound timer to VX.
    else if ((opcode & 0x00FF) == 0x0018){
    	sound_timer = V_Reg[Reg_X];
    	PC = PC + 2;
    }

    // Adds VX to I.
    else if ((opcode & 0x00FF) == 0x001E){
    	if ((I + V_Reg[Reg_X]) > 0xFFF){
    		V_Reg[0xF] = 1;
    	}
    	else{
    		V_Reg[0xF] = 0;
    	}
    	I = I + V_Reg[Reg_X];
    	PC = PC + 2;
    }

    // Sets I to the location of the sprite for the character in VX. Characters 0-F (in hexadecimal) are represented by a 4x5 font.
    else if ((opcode & 0x00FF) == 0x0029){
    	I = V_Reg[Reg_X] * 0x5;
    	PC = PC + 2;
    }

    // Stores the binary-coded decimal representation of VX, with the most significant of three digits at the
    // address in I, the middle digit at I plus 1, and the least significant digit at I plus 2.
    else if ((opcode & 0x00FF) == 0x0033){
    	Memory[I] = V_Reg[Reg_X] / 100;
    	Memory[I + 1] = (V_Reg[Reg_X] / 10) % 10;
    	Memory[I + 2] = (V_Reg[Reg_X] % 100) % 10;
    	PC = PC + 2;
    }

    // Stores V0 to VX (including VX) in memory starting at address I.
    else if ((opcode & 0x00FF) == 0x0055){
    	for (int i = 0; i <= (Reg_X); i++)
			Memory[I + i] = V_Reg[i];	
		
		I = I + Reg_X + 1;
		PC = PC + 2;
    }

    // Fills V0 to VX (including VX) with values from memory starting at address I.
    else if ((opcode & 0x00FF) == 0x0065){
    	for (int i = 0; i <= (Reg_X); i++)
			V_Reg[i] = Memory[I + i];	
		
		I = I + Reg_X + 1;
		PC = PC + 2;
    }

    else{
    	cout<<"OpCode Not Implemented"<<endl;
    }
}
/*	FUNCTION: display_func
 *  ----------------------
 *	Sample function that displays a pixel on the screen
 *	PARAMETERS: 
 *  gamer: architecture to be emulated, defined in gamul.h
 *	RETURNS: none
 */
void gamul8::display_func()
{
	display[20][40] = 1;
	display[10][30] = 0;
}

void gamul8::ExecuteOneCycle(){
	
	// Fetch Instruction 
	// Read Instruction present at Program Counter
	
	opcode=(Memory[PC]<<8)|(Memory[PC+1]); // Reading Two bytes 
	// Decode Instruction

	// Execute Instruction
	(this->*Processor[((opcode&0xF000)>>12)])();
    
		// Update timers
	if(delay_timer > 0)
		--delay_timer;

	if(sound_timer > 0)
	{
		system("paplay beep.aiff &>/dev/null &");
		if(sound_timer == 1)
			printf("BEEP!\n");
		--sound_timer;
	}

    
}

bool gamul8::init(char *filename){

	
	PC=PC_START;  // Initialize Program Counter
	SP=0;         // Initialize Stack Pointer
	I=0;          // Initialize Memory Index

	// Initialize Timers
	delay_timer =0; 
	sound_timer =0;
	clrscr=true;

	for(int i=0;i<16;i++){
		key[i]=0;
	}

	for(int i=0;i<SCREEN_WIDTH;i++){
		
		for(int j=0;j<SCREEN_HEIGHT;j++){
			display[i][j]=0;
		}
	}

	// Stack Initialization
	for(int i=0;i<STACK_SIZE;i++){
		Stack[i]=0;
	}

	for(int i=0;i<MAX_SIZE;i++){
		Memory[i]=0; // Initial Value of the Memory
	}

	for(int i=0;i<FONTSET_SIZE;i++){
		Memory[i]=fontset[i];
	}

	Processor.push_back(&gamul8::Function_0);
	Processor.push_back(&gamul8::Function_1);
	Processor.push_back(&gamul8::Function_2);
	Processor.push_back(&gamul8::Function_3);
	Processor.push_back(&gamul8::Function_4);
	Processor.push_back(&gamul8::Function_5);
	Processor.push_back(&gamul8::Function_6);
	Processor.push_back(&gamul8::Function_7);
	Processor.push_back(&gamul8::Function_8);
	Processor.push_back(&gamul8::Function_9);
	Processor.push_back(&gamul8::Function_A);
	Processor.push_back(&gamul8::Function_B);
	Processor.push_back(&gamul8::Function_C);
	Processor.push_back(&gamul8::Function_D);
	Processor.push_back(&gamul8::Function_E);
	Processor.push_back(&gamul8::Function_F);
    


	ifstream gamul8app(filename,ios_base::binary);
	

	if(gamul8app.is_open()){
        
		int begin;
		int end;
		int size;

		begin =0;
		end =0;

		begin=gamul8app.tellg();
		gamul8app.seekg(0,ios::end);
		end = gamul8app.tellg();
		size=end-begin;

		char * buffer;
		

		if(size>MAX_ALLOWED_SIZE){
			cout<<"ERROR: Application size is greater that 4096-512"<<endl;
			return false;
		}

		//Load FontSet to memory
        buffer=new char[size];
		// First Instruction is loaded at this address
		
		//inputFile.clear();
		gamul8app.seekg(0, ios::beg);

		gamul8app.read(buffer,size);
		
		// Read the application into Memory 
        for(int i=0;i<size;i++){
			Memory[i+512]=buffer[i];
		}
		free(buffer);
	}
	else{
		cout<<"ERROR:Cannot Open the Input File"<<endl;
		return false;
	}

    
	
	return true;
}