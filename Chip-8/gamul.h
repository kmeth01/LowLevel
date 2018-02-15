#include <vector>
#ifndef GAMUL_H
#define GAMUL_H
#define SCREEN_WIDTH	64
#define SCREEN_HEIGHT	32
#define FONTSET_SIZE	80
#define STACK_SIZE      20
#define NUM_REGISTERS   16
#define PC_START        0x200
#define MAX_SIZE      4096




class gamul8{
public:

	gamul8(); // Constructor 
	~gamul8(); // Destructor

/*
// Init function will initialize the memory area (512 bytes)
// Initializes the Program Counter to 0x200
// Loads the Instructions to memory (512 to 4096)
// If the Program Loaded is bigger than 4096 program returns with error Messsage
// Loads the Drawing and Graphic Sprites Data in MEMORY (0 to 512 bytes)
*/
	bool clrscr;
	bool init(char *filename);
	
	// To Fetch Decode and Execute One Instruction 
	void ExecuteOneCycle();
	void display_func();
	unsigned char display[SCREEN_WIDTH][SCREEN_HEIGHT];
    unsigned char  key[16];	
	

private:
	//typedef void(Dog::*BarkFunction)(void);
	typedef void (gamul8:: *Funpointer)();
	// Processor as a vector of Function Pointers
	std::vector<Funpointer> Processor;

	// OpCode Implementations
	// Function_X for opcode XNNN

	void Function_0();
	void Function_1();
	void Function_2();
	void Function_3();
	void Function_4();
	void Function_5();
	void Function_6();
	void Function_7();
	void Function_8();
	void Function_9();
	void Function_A();
	void Function_B();
	void Function_C();
	void Function_D();
	void Function_E();
	void Function_F();


   
	unsigned char V_Reg[NUM_REGISTERS]; // NUM_REGISTERS Registers
	
	unsigned short PC; // Program Counter
	unsigned short SP;// Stack Pointer
	unsigned short I; // Memory Index Register
	unsigned short Stack[STACK_SIZE];// Stack
	unsigned char Memory[MAX_SIZE]; // Memory Area

	unsigned short delay_timer;
	unsigned short sound_timer;


    

};

#endif