10  rem ***********************************************************************
15  rem *                                                                     *
20  rem * Menu program for Basic                                              *
30  rem *                                                                     *
40  rem * Allows the user to select a Basic program from a menu of them.      *
50  rem * Enough program lines are output to fill the screen at 24 lines,     *
60  rem * then the user is given a prompt, and can enter the following        *
70  rem * commands:                                                           *
80  rem *                                                                     *
90  rem * +        - Go to next screen.                                       *
100 rem * -        - Go back to last screen.                                  *
110 rem * <letter> - Go to first item starting with the letter.               *
120 rem * <number> - Select the program by number.                            *
130 rem *                                                                     *
140 rem * This program was created by Scott A. Moore for IP Basic. It uses    *
150 rem * "minimal" Basic, but also uses the statement:                       *
160 rem *                                                                     *
170 rem * EXEC "name"                                                         *
180 rem *                                                                     *
190 rem * Which means to execute the indicated Basic program by filename,     *
200 rem * then return to this program. You can find this statement in line    *
210 rem * 670 and replace it with your own Basic's equivalent.                *
215 rem *                                                                     *
220 rem ***********************************************************************
222 rem
225 dim n$((25-2)/3, 40): rem holds the program name strings to execute.
230 l = 25: rem This is the number of lines that will be output on a screen.
235 k = int((l-2)/3): rem This is the number of data entries that fit on a screen.
237 u = (l-2) mod 3: rem This is the number of extra lines to fill the screen.
240 print
250 print "Welcome to the Basic games menu. Here you can select a game to run"
260 print "by number. You will be shown each page of games and their"
270 print "descriptions. You can type:"
280 print
290 print "1. The number of the program you want to run."
300 print
310 print "2. The first letter of the programs you want to look at."
320 print
330 print "3. A ""+"" sign to move to the next menu page."
340 print
350 print "4. A ""-"" sign to move to the last menu page."
360 print
370 input "*** Press return to continue ***", a$
380 restore: rem reset to start of data.
385 d = 1: rem set starting data number
386 e = 0: rem set not at end condition
390 rem
400 rem Print out the next page of data.
410 rem
420
425 o = d: rem save beginning data index
430 for i = 1 to k: rem the number of entries that will fit in a page
440
445    if e then print: print: print: goto 500: rem skip on end
450    read c$, d$, e$: rem read a game entry set
455    if c$="" then e = 1: print: print: print: goto 500
460    print i, d$: rem print number and name
470    print e$: rem print description
480    print: rem space off
485    n$(i) = c$: rem place program name string in holding
486    d = d+1: rem count total data entries read
490
500 next i
510 for i = 1 to u: print: next i: rem space off to fill screen
515 print: rem space off
520 input "Number of program (or +, -, or letter): ", a$
530 if a$="" then goto 862: rem default to next page of data
540 if asc(a$) < asc("0") or asc(a$) > asc("9") then goto 690
550 rem
550 rem Execute program by number
560 rem
570 a = val(a$): rem get number requested
580 if a >= 1 and a <= k then goto 630: rem valid number
590 input "*** Invalid program number, press return to try again ***", a$
600 s=o: gosub 2000: rem reset and find starting entry
620 goto 390: rem go back and print page again
630 rem
635 rem ********************************************************************
640 rem ***** CHANGE "EXEC" STATEMENT HERE TO APPROPRIATE STATEMENT ON *****
650 rem *****              YOUR BASIC IMPLEMENTATION                   *****
655 rem ********************************************************************
660 rem
665 rem print "Executing: ";n$(val(a$)): rem diagnostic, uncomment
666 for i = 1 to l: print: next: rem clear off screen, remove if printer
670 EXEC n$(val(a$)): rem execute program
672 input "*** Program complete, press return to continue ***", a$
675 goto 380: rem return to start of menu
680 rem
690 c$=a$: gosub 3000: a$=c$: rem convert answer to lower case
700 if asc(a$) < asc("a") or asc(a$) > asc("z") then goto 860 
710 rem
720 rem Select first letter of programs to list
730 rem
740 restore: rem reset to start search
750 d = 1
755 e = 0: rem set not at end condition
760 read c$, d$, e$: rem get a set of data
770 if c$ <> "" then goto 800: rem not end of data
780 input "*** First letter does not match any program, press return ***", a$
790 goto 380: return to first page print
800 q$=left$(c$, 1): rem get 1st letter of data
810 c$=q$: gosub 3000: q$=c$: rem convert to lower case
820 if q$<>a$ then d = d+1: goto 760: rem loop next data entry
830 s=d: gosub 2000: rem reset to before this data
840 goto 390: loop back to meny
850 rem
860 if a$<>"+" then 870: rem check forward
862 if e then s = o: gosub 2000: rem end, reposition
864 goto 390
870 if a$="-" then s = o-k: gosub 2000: goto 390: backward, return a page
880 input "*** Invalid response, press return to try again ***", a$
890 s=o: gosub 2000: rem reset and find starting entry
900 goto 390
2000 rem
2010 rem Routine to find data by number
2020 rem
2030 rem S holds the number of data entry to find, and returns just at that
2040 rem data entry (will read it next), and d contains data sequence number.
2050 rem
2060 restore: rem reset to start of data.
2080 d = 1: rem set starting data number
2085 e = 0: rem set not at end condition
2090 for i = 1 to s-1: read c$, d$, e$: d = d+1: next i
2100 return
3000 rem
3010 rem Routine to find lower case character
3020 rem
3030 rem C$ holds the character, and returns lower case
3040 rem
3050 if asc(c$) < asc("A") or asc(c$) > asc("Z") then goto 3070
3060 c$ = chr$(asc(c$)-asc("A")+asc("a")): rem convert to lower case
3070 return
10000 rem *********************************************************************
10005 rem
10010 rem Basic program data. There are three strings repeated over and over:
10020 rem
10030 rem 1. The name of the program in storage (the name of the file to open).
10040 rem
10050 rem 2. The normal name of the program for printing to the user.
10060 rem
10070 rem 3. A one line program description.
10080 rem
10085 rem The last line of the data consists of three null strings.
10086 rem
10088 rem *********************************************************************
10089 rem
10090 data "aceyducy", "aceyducy"                 
10100 data "Acey Ducey card game."
10110 data "amazing",  "Amazing"                  
10120 data "Generates mazes."
10130 data "animal",     "Animal"                 
10140 data "Guess the animal."
10150 data "awari",      "Awari"                  
10160 data "African stone board game."
10170 data "bagels",     "Bagels"                 
10180 data "Numbers guessing game."
10190 data "banner",     "Banner"                 
10200 data "Draw a banner."
10210 data "basketbl", "Basketball"           
10220 data "Basketball game simulation."
10230 data "batnum",     "Battle Of The Numbers"  
10240 data "Number strategy game."
10250 data "battle",     "Battle"                 
10260 data "Battleship board strategy game."
10270 data "blackjck", "Blackjack"            
10280 data "The card game 21, Vegas style."
10290 data "bombard",  "Bombardment"              
10300 data "Bomb the hidden player positions."
10310 data "bombsawy", "Bombs Away"           
10320 data "Bomb run simulator."
10330 data "bounce",     "Bounce"                 
10340 data "Plot the bouncing ball."
10350 data "bowling",  "Bowling"                  
10360 data "Bowling simulator."
10370 data "boxing",     "Boxing"                 
10380 data "Boxing simulator."
10390 data "bug",    "Bug"                    
10400 data "Draw the bug before the computer does."
10410 data "bullfght", "Bullfight"            
10420 data "Bullfight simulator."
10430 data "bullseye", "Bullseye"                 
10440 data "Dart board simulator."
10450 data "bunny",      "Bunny"                  
10460 data "Draw a playboy bunny, not PC."
10470 data "buzzword", "Buzzword"                 
10480 data "Generate buzzwords for your next meeting."
10490 data "calendar", "Calendar"                 
10500 data "Generate calendars."
10510 data "change",     "Change"                 
10520 data "Calculates correct change for item."
10530 data "checkers", "Checkers"                 
10540 data "Checkers, yes, the board game."
10550 data "chemist",  "Chemist"                  
10560 data "Play with a chemical formula."
10570 data "chief",      "Chief"                  
10580 data "Math game."
10590 data "chomp",      "Chomp"                  
10600 data "Find the cookie game."
10610 data "combat",     "Combat"                 
10620 data "Battle simulation."
10630 data "craps",      "Craps"                  
10640 data "Dice game simulation."
10650 data "cube",   "Cube"                   
10660 data "Board game on the face of a cube."
10670 data "depthchg", "Depth charge"             
10680 data "Bomb the submarine."
10690 data "diamond",  "Diamond"                  
10700 data "Prints diamond patterns."
10710 data "dice",   "Dice"                   
10720 data "Simulate rolling dice and show probabilities."
10730 data "digits",     "Digits"                 
10740 data "Guess the next digit."
10750 data "evenwin1", "Even Wins"            
10760 data "Even number of objects wins. Like NIM."
10770 data "evenwin2", "Even Wins #2"             
10780 data "Another version"
10790 data "flipflop", "Flip Flop"            
10800 data "Change X's to O's.."
10810 data "footbal1", "Football"                 
10820 data "Football simulation."
10830 data "footbal2", "Football #2"              
10840 data "Another version."
10850 data "furtradr", "Fur Trader"           
10860 data "Fur trading simulation."
10870 data "golf",   "Golf"                   
10880 data "Golf simulation."
10890 data "gomoko",     "Gomoko"                 
10900 data "Oriental board game (GO)."
10910 data "guess",      "Guess"                  
10920 data "Guess the number."
10930 data "gunner",     "Gunner"                 
10940 data "Hit the target."
10950 data "hamurabi", "Hammurabri"           
10960 data "Kingdom simulation."
10970 data "hangman",  "Hangman"                  
10980 data "Guess the word, or else..."
10990 data "hello",      "Hello"                  
11000 data "Conversational (AI) simulator."
11010 data "hexapawn", "Hexapawn"                 
11020 data "Board game simulation."
11030 data "hi-q",   "Hi-lo"                  
11040 data "Money guessing game."
11050 data "hilo",   "Hi I-Q"                 
11060 data "The famous, very annoying, board game."
11070 data "hockey",     "Hockey"                 
11080 data "Hockey simulation."
11090 data "horsrace", "Horserace"            
11100 data "Horse racing simulation."
11110 data "hurkle",     "Hurkle"                 
11120 data "Hunt the hurkle on a grid."
11130 data "kinema",     "Kinema"                 
11140 data "Answer a kinetics question."
11150 data "king",   "King"                   
11160 data "Another kingdom simulation."
11170 data "letter",     "Letter"                 
11180 data "Guess the letter."
11190 data "life2",      "Life For Two"           
11200 data "Life as two player game."
11210 data "litquiz",  "Literature Quiz"          
11220 data "Child's book quiz."
11230 data "love",   "Love"                   
11240 data "Print in ""love"" font."
11250 data "lem",    "Lunar LEM Rocket #2"    
11260 data "Simulate a lunar landing."
11270 data "rocket",     "Lunar LEN rocket #3"    
11280 data "Simulate a lunar landing. "
11290 data "mastrmnd", "Master Mind"              
11300 data "Crack a code."
11310 data "mathdice", "Math Dice"            
11320 data "Math game using dice."
11330 data "mugwump",  "Mugwump"                  
11340 data "Find the mugwump on a grid."
11350 data "name",   "Name"                   
11360 data "Does amusing things with your name."
11370 data "nicoma",     "Nicomachus"             
11380 data "Computer guesses what number you are thinking of."
11390 data "nim",    "Nim"                    
11400 data "Move the stones game."
11410 data "number",     "Number"                 
11420 data "Guess the number."
11430 data "onecheck", "One Check"            
11440 data "Solitaire checkers."
11450 data "orbit",      "Orbit"                  
11460 data "Shoot a spaceship in orbit."
11470 data "pizza",      "Pizza"                  
11480 data "Deliver pizzas to a small town."
11490 data "poetry",     "Poetry"                 
11500 data "Yep, generate random poetry."
11510 data "poker",      "Poker"                  
11520 data "The classic card game."
11530 data "queen",      "Queen"                  
11540 data "Game on chessboard with queens only."
11550 data "reverse",  "Reverse"                  
11560 data "Reverse a numbered list."
11570 data "rocksp",     "Rock, Scissors, Paper"  
11580 data "The classic game."
11590 data "roulette", "Roulette"                 
11600 data "Roulette wheel simulator."
11610 data "rusrou",     "Russian Roulette"       
11620 data "Games dead people play."
11630 data "salvo",      "Salvo"                  
11640 data "Battleship again."
11650 data "sinewave", "Sine Wave"            
11660 data "Draw sine waves."
11670 data "slalom",     "Slalom"                 
11680 data "Simulate skiing downhill."
11690 data "splat",      "Splat"                  
11700 data "Parachute on various worlds of the solar system."
11710 data "stars",      "Stars"                  
11720 data "Another guess the number game."
11730 data "stock",      "Stock Market"           
11740 data "Stock market simulation."
11750 data "synonym",  "Synonym"                  
11760 data "Synonym knowledge test."
11770 data "target",     "Target"                 
11780 data "Another shoot the target game."
11790 data "qubic",      "3-D Tic-Tac-Toe"        
11800 data "Tic Tac toe, but more confusing."
11810 data "tictac1",  "Tic Tac Toe"              
11820 data "Yep. Lots of tie games."
11830 data "tictac2",  "Tic Tac Toe #2"       
11840 data "Another version."
11850 data "towers",     "Tower"                  
11860 data "Towers of Hanoi, a stacking puzzle."
11870 data "train",      "Train"                  
11880 data "Generate time-speed-distance problems."
11890 data "trap",   "Trap"                   
11900 data "Another guess the number game !"
11910 data "23-match", "23 Matches"           
11920 data "Takeaway matches game."
11930 data "war",    "War"                    
11940 data "The card game."
11950 data "weekday",  "Weekday"                  
11960 data "Interesting facts about a date."
11970 data "word",   "Word"                   
11980 data "Guess the word !"
11990 data "", "", ""