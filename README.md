# Projet PROG5 UGA (12/2020)

Tested arm operation:
B/BL
AND, EOR, SUB, RSB, ADD, ADC, SBC, RSC, TST,TEQ, CMP, CMN, ORR, MOV
LDR, LDRB, LDRH, STR, STRB, STRH,LDM(1), STM(1)

Compilation :

'git clone https://github.com/FDaCostaB/PROG5.git '

'cd PROG5/'

'./configure CFLAGS='-Wall -Werror -g' 

'make'

Execution - More info(french) on 'sujet.pdf' part 2.2 Utilisation

In a first shell:

'./arm_simulator' displaying some TCP port

In a second shell:

'arm-none-eabi-gdb'

'file <path of a arm script>'
  
'target remote localhost:<port given by the simulator>'
  
'load'

Executing on gdb as usual with breakpoints to observe the correct functionment
