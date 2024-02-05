# NES-Emulator
A Nintendo Entertainment System game console emulator for learning about low level concepts.

Many of the most important concepts to understand for building this program can be found here... https://www.nesdev.org/wiki/Nesdev_Wiki

## Basic Concepts
### Hexadecimal
Similar as how in binary we use a combination of 2 digits to represent a number, and 10 for decimal, in hexadecimal we use 16. These 16 digits being 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, A, B, C, D, E, and F.

Let's look at an example:
- Decimal: 65
- Binary: 0b01000001
- Hexadecimal: 0x41, $41, #41, x41

Here we can also notice that when not showing a decimal representation that we have a prefix.

### Bitwise Operations
Their are 6 basic biwise operations we will be using in this project which are...

- `&` - AND
- `|` - OR
- `~` - NOT
- `^` - XOR
- `<<` - SHIFT LEFT
- `>>` - SHIFT RIGHT

## NES Architecture
### Basics
![[Pasted image 20240108195853.png]]

The CPU is the model 2AO3, a variant of the 6502, which is an 8 bit device. In theory we say, this CPU is connected to a BUS with:
- MEMORY (2KB)
	- Address $0000 -> $07FF
- APU, Audio Processing Unit.
	- Address $4000 -> $4017
- Game Cartridge.
	- Address $4020 -> $FFFF
	- Some Cartridges have additional circuitry which uses MAPPER technology to allow for more interesting games and technical capability.
- PPU, Picture Processing Unit (16 KB)
	- Address $2000 -> $2007
	- The PPU also has it's own BUS which has some memory allocated to...
		- GRAPHICS (8 KB), this memory is actually stored on the cartridge though.
			- Address $0800 -> $1FFF
		- VRAM (2KB)
			- Address $2000 -> $27FF
		- PALETTES
			- Address $3F00 -> $3FFF
	- Another Memory not on the PPUs BUS is...
		- OAM (Object Attribute Memory)
			- Where the CPU puts in the memory for the PPU to then take and draw on the screen
			- This is available via using the the DMA (Direct Memory Access) Module, which facilitates the transfer of data between the CPU and the OAM, and therefore the PPU.

Some of these components may actually be inbuilt to the CPU but for the purpose of education it is easier to imagine they are all separate.

The PPU and CPU both have clocks to determine the length of their cycles. Although these clocks are not always running at the same rate.

### CPU
The 6502 CPU is very famous for having been prolifically used in many machines during its time. The 6502 was created by MOS Technologies.

For a CPU to be useful it has to be connected to outside factors. A CPU can return an 16 bit Address, give/take Data, and output wether it is Reading/Writing. A CPU also requires a clock for it to be able to change states.

The CPU can access addresses on the BUS which holds addresses from 0x0000 - 0x7777. For our purposes we will treat the BUS as only being connected to the CPU and RAM (64KB). In other architectures you may have many more modules which connect to the BUS on top of the ones we have mentioned.

Here is a data sheet for a 6502 processor ---> [[rockwell_r650x_r651x.pdf]]

Here is a picture of a diagram showing the pins on the CPU, we can notice our 16 bit address on pins A0 to A15.
![[Pasted image 20240202005033.png]]

There are 3 primary registers on the CPU, each holding 8 bits:
- A: Accumulator
- X: Register
- Y: Register

Additionally the CPU also has:
- stkp: Stack Pointer, 8 bit
- pc: Program Counter, 16 bit
- status: Status Register, 8 bit
	- This allows us to interrogate the state of the CPU for things such as...
		- Z?, was the last result equal to 0
		- C?, has there been a carry operation
		- I, to enable or disable interrupts

Because of the way that the CPU works one can use illegal operation codes to trigger operations on the CPU which weren't designed by the manufacturer.

### RAM
One of the interesting things about the NES is that it sports an advertised 2KB RAM size whilst actually being connected to 8KB RAM module. The reason for this is to have 3 copies of the original 2KB on the stick along with the master copy. This technique is known as Mirroring. With this we also have 4 different memory addresses to access the same piece of data on the RAM.

### Picture Processing Unit (PPU)
The PPU, the module for handling the graphics processing, is another device connected to the CPU's BUS. On top of that, the PPU also has it's own BUS. This BUS is attached to...
1. 8KB Patter (0x0000 -> 0x1FFF), for sprites etc.
2. 2KB Name Table (0x2000 -> 0x2FFF), 2D Arrays to store IDs of which patterns to show in background
3. PALETTES (0x3F00 -> 0x3FFF), describe which colours should be displayed on the screen when combining the sprites and the background.

## Instructions, Opcodes, & Addressing Modes
The two most complicated instructions to emulate are the addition and subtraction. More specifically Addition with Carry In (ADC) and Subtraction with Carry In (SBC).

#### Addition with Carry In (ADC)
"The 6502 has an 8-bit addition operation ADC (add with carry) which adds two numbers and a carry-in bit, yielding an 8-bit result and a carry out bit. The following diagram shows an example addition in binary, decimal, and hexadecimal.

![[Pasted image 20240205110113.png]]

The carry flag is used as the carry-in for the operation, and the resulting carry-out value is stored in the carry flag. The carry flag can be used to chain together multiple ADC operations to perform multi-byte addition." - From Ken Shirriff's Blog (Accessed: 5/2/2024, Site: [The 6502 overflow flag explained mathematically](https://www.righto.com/2012/12/the-6502-overflow-flag-explained.html#:~:text=Addition%20on%20the%206502,binary%2C%20decimal%2C%20and%20hexadecimal.))

#### Subtraction with Carry In (SBC)
TODO

## The Stack
The stack is many things, but fundamentally it is a data structure. To make a real life comparison many people think of the stack in memory as a stack of plates.

"You only get to pick **out** the **last** plate that you put in **first**. Stack is a data structure that’s exactly like that, _Lastin, first out_." - From Chubak's Machine Learning Imporium (Accessed: 5/2/2024, Site: [Explaining the Stack Through 6502 Assembly | Chubak’s Machine Learning Imporium](https://chubakbidpaa.com/retro/2020/12/15/6502-stack-copy.html))

Many languages already have stack implementations (e.g. `queue` in Python). Here is a basic implementation in C++, using arrays...

```cpp
int stack[100], n=100, top=-1; //define the array and variables
void push(int val) {
   if(top>=n-1)
   cout<<"Stack Overflow"<<endl; //if the value of the stack pointer is higher than 100 already, we overflow the stack
   else {
      top++; //increase the value of stack pointer
      stack[top]=val; //add value to stack at the stack pointer
   }
}
void pop() {
   if(top<=-1)
   cout<<"Stack Underflow"<<endl; //if the value of the stack pointer is lower than -1, we underflow the stack
   else {
      cout<<"The popped element is "<< stack[top] <<endl;
      top--; //decrease the value of stack pointer
   }
}
```

"Stacks are used for memory allocation. Stacks are used for _static memory allocation_ whilst heaps are used for _dynamic memory allocation_. When a function is called, the language uses stacks because after execution, the memory is freed, as in, function is called, pushed to the memory, does its job, and is pulled.

This is just one use of the stack in memory allocation though. Another use of stack in memory allocation is keeping array indices.

I explained that heaps are like pyramids of binary trees. In C, you need to manually allocate a space on the heap.

I’m not gonna delve into this much deeper. If you wanna know more about memory stacks, read [this](https://b-ok.cc/book/2619769/0a64e4)." - From Chubak's Machine Learning Imporium (Accessed: 5/2/2024, Site: [Explaining the Stack Through 6502 Assembly | Chubak’s Machine Learning Imporium](https://chubakbidpaa.com/retro/2020/12/15/6502-stack-copy.html))

# References
- [NES Emulator From Scratch](https://www.youtube.com/playlist?list=PLrOv9FMX8xJHqMvSGB_9G9nZZ_4IgteYf)
- [Explaining the Stack Through 6502 Assembly | Chubak’s Machine Learning Imporium](https://chubakbidpaa.com/retro/2020/12/15/6502-stack-copy.html)
- [An introduction to 6502 math: addition, subtraction and more | Retro64](https://retro64.altervista.org/blog/an-introduction-to-6502-math-addiction-subtraction-and-more/)
- 