MEMORY AREAS:

0000 = E012		; screen position of PACMAN
0002 = 30 		; PACMAN's color(?) 30 should be YELLOW
0003 = 01 		; ?? toggles between 01 and FF
0004 = 01 		; this is set to 0A when PACMAN eats a POWER PILL
0005 = 00 		; PACMANs last movement (FE: left, 02: right, 10: down, F0: up)
0007 = 00 		; input sprite color for sprite drawing subroutine at 0228

0008 = 6010		; screen position of PURPLE GHOST 
000A = 6014		; screen position of GREEN GHOST
000C = 8010		; screen position of WHITE GHOST
000E = 8014		; screen position of BLUE GHOST

0010 = 03C0		; start of sprite pixel data of PURPLE GHOST (changes when PACMAN is in POWER PILL mode)
0012 = 03C0 	; start of sprite pixel data of GREEN GHOST
0014 = 03C0 	; start of sprite pixel data of WHITE GHOST
0016 = 03C0		; start of sprite pixel data of BLUE GHOST

0018 = 00 		; must be color "under" PURPLE GHOST
0019 = 00 		; must be color "under" GREEN GHOST
001A = 00 		; must be color "under" WHITE GHOST
001B = 00 		; must be color "under" BLUE GHOST

001C  			; copy of current GHOST position 
001E  			; copy of current GHOST sprite data

0020 			; backup of color under GHOST?

0021 = 03 		; current lifes
0022 = 0000 	; current score
0024 			; 0025, 0026, 0027, 0028 is copied here (direction of current AI ghost?)
0025 = F0 		; could be direction of PURPLE GHOST? (up)
0026 = 02  		; could be direction of GREEN GHOST? (right)
0027 = FE 		; could be direction of WHITE GHOST (left)
0028 = 10 		; could be direction of BLUE GHOST? (down)
0029 = 00 		; counts from 1 to 4, and is then reset to 0

002F 			; ?? ghost color is stored here
0030  			; indirection pointer (0008, 000A, 000C, 000E): address of screen pos of current AI ghost
0032 			; indirection pointer (0010, 0012, 0014, 0016): address of sprite data for current AI ghost
0034 			; indirection pointer (0018, 0019, 001A, 001B): address of color under current AI ghost
0036 			; indirection pointer (0025, 0026, 0027, 0028): address of direction for current AI ghost

0037			; ?? compared against 02 in 0CB8, doesn't make sense since 0036 is a 16 bit address...
0038			; GHOST AI: new current ghost position
003A 			; GHOST AI: stored random direction for GHOST
0040 			; barrier-counter for current ghost (incremented by 1 for each direction with a barrier)
0041 = 06 		; game speed (6 * 6ms) set in 0B72

0300 .. 031F:   ; an empty sprite (for some reason, there's 1 pixel set?!)
300: ........ 310: ........
301: ........ 311: ........
302: ........ 312: ........
303: ........ 313: ........
304: ........ 314: ........
305: ........ 315: ........
306: ........ 316: ........
307: ........ 317: ........
308: .......X 318: ........
309: ........ 319: ........
30A: ........ 31A: ........
30B: ........ 31B: ........
30C: ........ 31C: ........
30D: ........ 31D: ........
30E: ........ 31E: ........
30F: ........ 31F: ........

0320 .. 033F:	PACMAN closed mouth

320: ......XX 328: XX......
321: ....XXXX 329: XXXX....
322: ..XXXXXX 32A: XXXXXX..
323: ..XXXXXX 32B: XXXXXX..
324: .XXXXXXX 32C: XXXXXXX.
325: .XXXXXXX 32D: XXXXXXX.
326: XXXXXXXX 32E: XXXXXXXX
327: XXXXXXXX 32F: XXXXXXXX

330: XXXXXXXX 338: XXXXXXXX
331: XXXXXXXX 339: XXXXXXXX
332: .XXXXXXX 33A: XXXXXXX.
333: .XXXXXXX 33B: XXXXXXX.
334: ..XXXXXX 33C: XXXXXX..
335: ..XXXXXX 33D: XXXXXX..
336: ....XXXX 33E: XXXX....
337: ......XX 33F: XX......

0340 .. 035F: 	PACMAN sprite pixel data (open mouth, looking right)

340: ......XX 348: XX......
341: ....XXXX 349: XXXX....
342: ..XXXXXX 34A: XXXXXX..
343: ..XXXXXX 34B: XXXX....
344: .XXXXXXX 34C: XXX.....
345: .XXXXXXX 34D: XX......
346: XXXXXXXX 34E: X.......
347: XXXXXXXX 34F: ........

350: XXXXXXXX 358: ........
351: XXXXXXXX 359: X.......
352: .XXXXXXX 35A: XX......
353: .XXXXXXX 35B: XXX.....
354: ..XXXXXX 35C: XXXX....
355: ..XXXXXX 35D: XXXXXX..
356: ....XXXX 35E: XXXX....
357: ......XX 35F: XX

0360 .. 037F: 	PACMAN sprite pixel (open mouth, looking left):

360: ......XX 368: XX...... 
361: ....XXXX 369: XXXX....
362: ..XXXXXX 36A: XXXXXX..
363: ....XXXX 36B: XXXXXX..
364: .....XXX 36C: XXXXXXX.
365: ......XX 36D: XXXXXXX.
366: .......X 36E: XXXXXXXX
367: ........ 36F: XXXXXXXX

370: ........ 378: XXXXXXXX
371: .......X 379: XXXXXXXX
372: ......XX 37A: XXXXXXX.
373: .....XXX 37B: XXXXXXX.
374: ....XXXX 37C: XXXXXX..
375: ..XXXXXX 37D: XXXXXX..
376: ....XXXX 37E: XXXX....
377: ......XX 27F: XX......

0380 .. 039F: 	PACMAN sprite pixel data (open mouth, lookup up)

380: ........ 388: ........
381: ........ 389: ........
382: ..X..... 38A: .....X..
383: ..X..... 38B: .....X..
384: .XXX.... 38C: ....XXX.
385: .XXXX... 38D: ...XXXX.
386: XXXXXX.. 38E: ..XXXXXX
387: XXXXXXX. 38F: .XXXXXXX

390: XXXXXXXX 398: XXXXXXXX
391: XXXXXXXX 399: XXXXXXXX
392: .XXXXXXX 39A: XXXXXXX.
393: .XXXXXXX 39B: XXXXXXX.
394: ..XXXXXX 39C: XXXXXX..
395: ..XXXXXX 39D: XXXXXX..
396: ....XXXX 39E: XXXX....
397: ......XX 39F: XX......

03A0 .. 03BF: 	PACMAN sprite pixel data (open mouth, looking down)

3A0: ......XX 3A8: XX......
3A1: ....XXXX 3A9: XXXX....
3A2: ..XXXXXX 3AA: XXXXXX..
3A3: ..XXXXXX 3AB: XXXXXX..
3A4: .XXXXXXX 3AC: XXXXXXX.
3A5: .XXXXXXX 3AD: XXXXXXX.
3A6: XXXXXXXX 3AE: XXXXXXXX
3A7: XXXXXXXX 3AF: XXXXXXXX

3B0: XXXXXXX. 3B8: .XXXXXXX
3B1: XXXXXX.. 3B9: ..XXXXXX
3B2: .XXXX... 3BA: ...XXXX.
3B3: .XXX.... 3BB: .....XX.
3B4: ..X..... 3BC: .....X..
3B5: ..X..... 3BD: .....X..
3B6: ........ 3BE: ........
3B7: ........ 3BF: ........

03C0 .. 03DF: NORMAL GHOST SPRITE

3C0: ......XX 3C8: XX......
3C1: ....XXXX 3C9: XXXX....
3C2: ...XXXXX 3CA: XXXXX...
3C3: ..XXX.XX 3CB: XX.XXX..
3C4: ..XX...X 3CC: X...XX..
3C5: .XXX.X.X 3CD: X.X.XXX.
3C6: .XXX...X 3CE: X...XXX.
3C7: ..XXX.XX 3CF: XX.XXX..

3D0: ...XXXXX 3D8: XXXXX...
3D1: ...XXXXX 3D9: XXXXX...
3D2: ...X..XX 3DA: XX..X...
3D3: ...XX... 3DB: ...XX...
3D4: ..XXXX.. 3DC: ..XXXX..
3D5: ..XXXXXX 3DD: XXXXXX..
3D6: .XX.XXX. 3DE: XXX.XXX.
3D7: XX...X.. 3DF: .X...XXX

03E0 .. 03FF: SCARED GHOST SPRITE

3E0: ........ 3E8: ........
3E1: ........ 3E9: XX......
3E2: ..XX.... 3EA: XX......
3E3: ..XX.... 3EB: X.......
3E4: ....X... 3EC: ....XX..
3E5: ........ 3ED: ...XXX..
3E6: ........ 3EE: ........
3E7: ........ 3EF: ........

3F0: ....XXXX 3F8: XXXX....
3F1: ...XXX.X 3F9: X.XXX...
3F2: ..XXX..X 3FA: X..XXX..
3F3: .XXX...X 3FB: X...XXX.
3F4: XXX..X.X 3FC: X.X..XXX
3F5: XXX....X 3FD: X....XXX
3F6: XXXXXXXX 3FE: XXXXXXXX
3F7: ..XXXXXX 3FF: XXXXXX..

0400 .. 041F: HOLLOW GHOST SPRITE

400: .....X.. 408: ..X.....
401: ....XXX. 409: .XXX....
402: ...XX.XX 40A: XX.XX...
403: ..XX...X 40B: X...XX..
404: ..XX.X.. 40C: ..X.XX..
405: ..XX...X 40D: X...XX..
406: ...XX.XX 40E: XX.XX...
407: ....XXX. 40F: .XXX....

410: .....X.. 418: ..X.....
411: ........ 419: ........
412: ........ 41A: ........
413: ....XXX. 41B: ..XXX...
414: ...XX.XX 41C: .XX.XX..
415: ..XX...X 41D: XX..XX..
416: ........ 41E: ........
417: ........ 41F: ........

0420 .. 043F: POWER PILL sprite data

420: ......XX 428: XX......
421: ....XXXX 429: XXXX....
422: ..XXX... 42A: ...XXX..
423: ..XXX... 42B: ...XXX..
424: .XXXX..X 42C: XXXXXXX.
425: .XXXX..X 42D: XXXXXXX.
426: XXXXX..X 42E: XXXXXXXX
427: XXXXX... 42F: .XXXXXXX

430: XXXXX... 438: .XXXXXXX
431: XXXXX..X 439: XXXXXXXX
432: .XXXX..X 43A: XXXXXXX.
433: .XXXX..X 43B: XXXXXXX.
434: ..XXX... 43C: ...XXX..
435: ..XXX... 43D: ...XXX..
436: ....XXXX 43E: XXXX....
437: ......XX 43F: XX......

0440 .. 045F: orange POINT sprite data

440: ........ 448: ........
441: ........ 449: ........
442: ........ 44A: ........
443: ........ 44B: ........
444: ........ 44C: ........
445: .......X 44D: X.......
446: ......XX 44E: XX......
447: .....XXX 44F: XXX.....

450: .....XXX 458: XXX.....
451: ......XX 459: XX......
452: .......X 45A: X.......
453: ........ 45B: ........
454: ........ 45C: ........
455: ........ 45D: ........
456: ........ 45E: ........
457: ........ 45F: ........


1000 .. 19FF 	; Playfield Color Bytes
1A00 .. 1EFF 	; Playfield ASCII
2000 .. 29FF	; Intro Screen Color Bytes
2A00 .. 2EFF	; Intro Screen ASCII
2F00 .. 2FFF 	; random number table with directions (02, FE, 10, F0), 2FFF is 0!!!

1F00 - ??:	custom font

SUBROUTINES:

--- SUBROUTINE 0200: draw-sprite helper, write 2 color bytes for an 8x8 pixel block
    in: DE = color address
        0007: color byte(?)

0200: F5            push af
0201: C5            push bc
0202: D5            push de
0203: E5            push hl
0204: 3A 07 00      ld   a,($0007) 		; load color byte
0207: 12            ld   (de),a  		; write first color byte
0208: EB            ex   de,hl 			; add 20 to get to next line in color mem
0209: 11 20 00      ld   de,$0020
020C: 19            add  hl,de
020D: EB            ex   de,hl
020E: 12            ld   (de),a 		; write second color byte
020F: E1            pop  hl
0210: D1            pop  de
0211: C1            pop  bc
0212: F1            pop  af
0213: C9            ret

--- SUBROUTINE 02B0: draw sprite helper: copy 8 pixel bytes for an 8x8 pixel block
 	in: HL: 	source address with continues sprite pixel data bytes
 	    DE: 	target address in pixel video RAM

LOCAL VARIABLES
02AA: 	store DE
02A8:   store HL

potential bug: C is "unitialized" at start of function, but BC is used??
			   C must be 00 ??

02B0: F5            push af
02B1: C5            push bc
02B2: D5            push de
02B3: E5            push hl
02B4: ED 53 AA 02   ld   ($02AA),de 		; store target address
02B8: 06 04         ld   b,$04

// 4x loop to copy a 8x4 pixel block
02BA: ED A0         ldi 					; (HL++) -> (DE++), BC--
02BC: 22 A8 02      ld   ($02A8),hl 		; store sprite data source address
02BF: EB            ex   de,hl 				; set DE to next pixel video ram address
02C0: 11 7F 00      ld   de,$007F
02C3: 19            add  hl,de
02C4: EB            ex   de,hl
02C5: 2A A8 02      ld   hl,($02A8) 		; restore sprite data source address
02C8: 03            inc  bc 				
02C9: 10 EF         djnz $02BA				; decrease B and jump if not zero


02CB: 2A AA 02      ld   hl,($02AA) 		; original pixel video ram start address
02CE: 11 20 00      ld   de,$0020 			; skip 4 lines
02D1: 19            add  hl,de
02D2: EB            ex   de,hl
02D3: 2A A8 02      ld   hl,($02A8) 		; current sprite data source address
02D6: 06 04         ld   b,$04

// and another 4x loop to copy 4x8 pixel block
02D8: ED A0         ldi
02DA: 22 A8 02      ld   ($02A8),hl
02DD: EB            ex   de,hl
02DE: 11 7F 00      ld   de,$007F
02E1: 19            add  hl,de
02E2: EB            ex   de,hl
02E3: 2A A8 02      ld   hl,($02A8)
02E6: 03            inc  bc
02E7: 10 EF         djnz $02D8

02E9: E1            pop  hl
02EA: D1            pop  de
02EB: C1            pop  bc
02EC: F1            pop  af
02ED: C9            ret


--- SUBROUTINE 0228: draw sprite a 16x16 sprite
in: HL:   position
	DE:   pixel data source address
	0007: color

LOCAL VARIABLES:
021C: 	store HL (position: L=x, H=y)
021E:	store DE (source pixel data address)


0228: F5            push af
0229: C5            push bc
022A: D5            push de
022B: E5            push hl
022C: 22 1C 02      ld   ($021C),hl 	; store HL (position)
022F: ED 53 1E 02   ld   ($021E),de 	; store DE (?)

; draw top-left 8x8 sprite fragment
0233: CD 03 F0      call $F003 
0236: 34            system call 34: compute pixel and color address from coords
									in: L: x, H: y, 
									out: HL: pixel address, DE: color address

0237: CD 00 02      call $0200 			; write 2 color bytes for top-left 8x8 block

023A: ED 5B 1E 02   ld   de,($021E)     ; restore DE
023E: EB            ex   de,hl  		; HL: source pixel data, DE: pixel RAM address
023F: CD B0 02      call $02B0 			; copy top-left 8x8 pixel block (8 bytes)

; compute source and target addresses for top-right sprite fragment
0242: 2A 1E 02      ld   hl,($021E) 	; next pixel source address (+8 bytes) in 021E
0245: 11 08 00      ld   de,$0008
0248: 19            add  hl,de
0249: 22 1E 02      ld   ($021E),hl     
024C: 2A 1C 02      ld   hl,($021C) 	; compute video ram addresses of top-right block
024F: 2C            inc  l
0250: CD 03 F0      call $F003
0253: 34            ; system call 34: compute pixel/video RAM address from coords

; draw top-right 8x8 sprite fragment
0254: CD 00 02      call $0200 			; write 2 color bytes
0257: 00            nop
0258: ED 5B 1E 02   ld   de,($021E)
025C: EB            ex   de,hl
025D: CD B0 02      call $02B0 			; copy top-right 8x8 pixel block

; compute source and target addresses for bottom-left 8x8 block
0260: 2A 1E 02      ld   hl,($021E)
0263: 11 08 00      ld   de,$0008
0266: 19            add  hl,de 			; sprite source data + 8

0267: 22 1E 02      ld   ($021E),hl 	; coords Y + 8
026A: 2A 1C 02      ld   hl,($021C)
026D: 7C            ld   a,h
026E: C6 08         add  a,$08 			
0270: 67            ld   h,a

0271: CD 03 F0      call $F003
0274: 34            ; system call 34: compute pixel/video RAM addr from coords

; draw bottom-left 8x8 sprite fragment
0275: CD 00 02      call $0200 			; write 2 color bytes
0278: ED 5B 1E 02   ld   de,($021E)
027C: EB            ex   de,hl
027D: CD B0 02      call $02B0 			; copy bottom-left 8x8 pixel block

; compute source and target address for bottom-right 8x8 block
0280: 2A 1E 02      ld   hl,($021E)
0283: 11 08 00      ld   de,$0008
0286: 19            add  hl,de
0287: 22 1E 02      ld   ($021E),hl
028A: 2A 1C 02      ld   hl,($021C)
028D: 7C            ld   a,h
028E: C6 08         add  a,$08
0290: 67            ld   h,a
0291: 2C            inc  l
0292: CD 03 F0      call $F003
0295: 34            inc  (hl)

; draw bottom-right 8x8 sprite fragment
0296: CD 00 02      call $0200 			; write 2 color bytes
0299: ED 5B 1E 02   ld   de,($021E)
029D: EB            ex   de,hl
029E: CD B0 02      call $02B0 			; copy bottom-right 8x8 pixel block
02A1: E1            pop  hl
02A2: D1            pop  de
02A3: C1            pop  bc
02A4: F1            pop  af
02A5: C9            ret

--- SUBROUTINE 0486: called when PACMAN collides with a "point"

6 bytes at 0480: 01 01 01 01 00 01

0486: F5            push af

--- play a sound
0487: 21 80 04      ld   hl,$0480 		; copy "01 01 01 01 00 01: to B782 (ARG1 ARG2 ARG3)
048A: 11 82 B7      ld   de,$B782
048D: 01 06 00      ld   bc,$0006
0490: ED B0         ldir
0492: CD 03 F0      call $F003
0495: 35            ; system call 35: play a sound defined by ARG1 ARG2 ARG3

--- increment score by 2, this is a decimal corrected addition
0496: 2A 22 00      ld   hl,($0022)
0499: 3E 02         ld   a,$02 			; add 2 to low byte
049B: 85            add  a,l
049C: 27            daa 				; decimal correction
049D: 6F            ld   l,a
049E: D2 A6 04      jp   nc,$04A6
04A1: 3E 01         ld   a,$01 			; carry over: need to increment high-byte
04A3: 84            add  a,h
04A4: 27            daa 				; deciman correction for high-byte
04A5: 67            ld   h,a
04A6: 22 22 00      ld   ($0022),hl
04A9: F1            pop  af
04AA: C9            ret

--- SUBROUTINE 04B6: PACMAN collides with (eats) a POWER PILL

04B6: F5            push af
04B7: CD 8A 07      call $078A
--- subroutine call to 078A: play a complex "POWER PILL" sound effect
078A: 21 80 07      ld   hl,$0780 		; copy sound args "10 01 10 01 00 00"
078D: 11 82 B7      ld   de,$B782
0790: 01 06 00      ld   bc,$0006
0793: ED B0         ldir
0795: CD 03 F0      call $F003
0798: 35            ; system call 35: play sound

0799: 3A 82 B7      ld   a,($B782) 		; decrement sound ARG1 
079C: 3D            dec  a
079D: FE F0         cp   $F0
079F: CA B5 07      jp   z,$07B5

07A2: 32 82 B7      ld   ($B782),a 		; update sound ARG1 and ARG2
07A5: 32 84 B7      ld   ($B784),a
07A8: 06 FF         ld   b,$FF
07AA: 10 FE         djnz $07AA 			; wait a little bit
07AC: C3 95 07      jp   $0795 			; loop to play sound

; stop sound, also called from other places as subroutine
07AF: 00 00 00 00 00 00 ; 6x 00 byte (sound arg)

07B5: 21 AF 07      ld   hl,$07AF 		; play a sound with all 0 args (no sound?)
07B8: 11 82 B7      ld   de,$B782
07BB: 01 06 00      ld   bc,$0006
07BE: ED B0         ldir
07C0: CD 03 F0      call $F003
07C3: 35            ; system call 35: play sound
07C4: C9            ret 				; return to 4BA

--- continue after sound effect: PACMAN eats a POWER PILL
04BA: 3E 10         ld   a,$10 			; set PACMAN's color to 10 (RED)
04BC: 32 02 00      ld   ($0002),a
04BF: 3E 0A         ld   a,$0A
04C1: 32 04 00      ld   ($0004),a
04C4: CD 70 08      call $0870

--- tail of "PACMAN eats POWER PILL subroutine"
04C7: F1            pop  af
04C8: C9            ret

--- SUBROUTINE 0460: PACMAN collides with playfield border, restore previous position

0460: F5            push af
0461: 3A 05 00      ld   a,($0005) 		; load PACMAN's last movement
0464: FE 02         cp   $02 			; right: horizontal movement
0466: CA 74 04      jp   z,$0474
0469: FE FE         cp   $FE 			; left: horizontal movement
046B: CA 74 04      jp   z,$0474		; fallthrough: was up or down (F0 or 10)
046E: 21 01 00      ld   hl,$0001 		; addr 0001 is vertical position byte
0471: C3 77 04      jp   $0477
0474: 21 00 00      ld   hl,$0000 		; addr 0000 is horizontal position byte
0477: ED 44         neg 				; a now has neg of previous movement
0479: 86            add  a,(hl) 		; restore previous position
047A: 77            ld   (hl),a 		; store the restored position
047B: F1            pop  af
047C: C9            ret


--- SUBROUTINE 07C8: PACMAN collides with GHOST

in A: ghost color

07C8: F5            push af
07C9: FE 18         cp   $18 			; PURPLE GHOST
07CB: CA 10 08      jp   z,$0810
07CE: FE 20         cp   $20 			; GREEN GHOST
07D0: CA 26 08      jp   z,$0826
07D3: FE 38         cp   $38 			; WHITE GHOST
07D5: CA 3C 08      jp   z,$083C
07D8: FE 68         cp   $68 			; BLUE GHOST
07DA: CA 52 08      jp   z,$0852

; POTENTIAL BUG: fallthrough to data if subroutine 07C8 is called with neither 18,20,38,68 in A

07DD: 01 01 01 01 00 00 	; 6x bytes for play sound config

--- continued from 04EA (PACMAN has eaten a GHOST handling)
--- plays a complex sound effect (PACMAN eats GHOST sound...?)

07E3: 21 DD 07      ld   hl,$07DD
07E6: 11 82 B7      ld   de,$B782
07E9: 01 06 00      ld   bc,$0006
07EC: ED B0         ldir
07EE: CD 03 F0      call $F003
07F1: 35            ; system call 35: play sound

07F2: 3A 82 B7      ld   a,($B782)
07F5: 3C            inc  a
07F6: 32 82 B7      ld   ($B782),a 		; increment audio ARGS
07F9: 32 84 B7      ld   ($B784),a
07FC: 06 FF         ld   b,$FF 			; 2x wait loop
07FE: 10 FE         djnz $07FE
0800: 06 FF         ld   b,$FF
0802: 10 FE         djnz $0802
0804: FE 10         cp   $10
0806: C2 EE 07      jp   nz,$07EE 		; loop back to play sound system call

0809: CD B5 07      call $07B5 			; reused "stop current sound" call(?)

080C: F1            pop  af
080D: C9            ret 				; return to... PACMAN collission detection at 0716
080E: 00            nop
080F: 00            nop

--- PACMAN collides with PURPLE GHOST
0810: 3A 10 00      ld   a,($0010) 		; check PURPLE GHOST mode via its sprite address
0813: FE C0         cp   $C0
0815: CA 68 08      jp   z,$0868 		; PURPLE GHOST is in normal mode (PACMAN dies)
0818: FE 00         cp   $00
081A: CA 0C 08      jp   z,$080C 		; PURPLE GHOST is already in hollow mode (nothing happens)
081D: 21 00 04      ld   hl,$0400 		; GHOST is in SCARED mode -> set GHOST to HOLLOW mode
0820: 22 10 00      ld   ($0010),hl  	
0823: C3 A0 08      jp   $08A0 			; continue at 08A0

--- PACMAN collides with GREEN GHOST
0826: 3A 12 00      ld   a,($0012) 		; check GREEN GHOST mode via its sprite address
0829: FE C0         cp   $C0
082B: CA 68 08      jp   z,$0868 		; GREEN GHOST is in normal mode (PACMAN dies)
082E: FE 00         cp   $00 			
0830: CA 0C 08      jp   z,$080C 		; GREEN GHOST is already hollow (nothing happens)
0833: 21 00 04      ld   hl,$0400 		; GHOST is in SCARED mode -> set GHOST to HOLLOW mode
0836: 22 12 00      ld   ($0012),hl 	
0839: C3 B0 08      jp   $08B0 		 	; continue at 08B0

--- PACMAN collides with WHITE GHOST
083C: 3A 14 00      ld   a,($0014) 		; check WHITE GHOST mode via its sprite address
083F: FE C0         cp   $C0 		
0841: CA 68 08      jp   z,$0868 		; WHITE GHOST is in normal mode (PACMAN dies)
0844: FE 00         cp   $00
0846: CA 0C 08      jp   z,$080C 		; WHITE GHOST is already hollow (nothing happens)
0849: 21 00 04      ld   hl,$0400 		; GHOST is in SCARED mode -> set GHOST to HOLLOW mode
084C: 22 14 00      ld   ($0014),hl 	
084F: C3 C0 08      jp   $08C0 			; continue at 08C0

--- PACMAN collides with BLUE GHOST
0852: 3A 16 00      ld   a,($0016) 		; check BLUE GHOST mode via its sprite address
0855: FE C0         cp   $C0
0857: CA 68 08      jp   z,$0868 		; BLUE GHOST is in normal mode (PACMAN dies)
085A: FE 00         cp   $00
085C: CA 0C 08      jp   z,$080C 		; BLUE GHOST is already hollow (nothing happens)
085F: 21 00 04      ld   hl,$0400 		; GHOST is in SCARED mode -> set GHOST to HOLLOW mode
0862: 22 16 00      ld   ($0016),hl 
0865: C3 D0 08      jp   $08D0 			; continue at 08D0

0868: F1            pop  af 			; jump here if collided with NORMAL GHOST (PACMAN dies?)
0869: C3 40 0D      jp   $0D40 			; continue at 0D40
086C: 00            nop
086D: 00            nop
086E: 00            nop
086F: 00            nop

--- subroutine 0870: patches ghost sprite data adresses to "SCARED GHOST"
0870: 21 10 00      ld   hl,$0010 		; sprite pixel address of ghosts
0873: 7E            ld   a,(hl) 		; test lower byte of PURPLE GHOST
0874: FE C0         cp   $C0 			; PURPLE GHOST in normal mode?
0876: CC 92 08      call z,$0892
0879: 23            inc  hl
087A: 23            inc  hl
087B: 7E            ld   a,(hl) 		; GREEN GHOST in normal mode?
087C: FE C0         cp   $C0
087E: CC 92 08      call z,$0892
0881: 23            inc  hl
0882: 23            inc  hl
0883: 7E            ld   a,(hl) 		
0884: FE C0         cp   $C0 			; WHITE GHOST in normal mode?
0886: CC 92 08      call z,$0892
0889: 23            inc  hl
088A: 23            inc  hl
088B: 7E            ld   a,(hl)
088C: FE C0         cp   $C0 			; BLUE GHOST in normal mode?
088E: CC 92 08      call z,$0892
0891: C9            ret
0892: 3E E0         ld   a,$E0 			; patch ghost sprite data to "SCARED GHOST" (03E0)
0894: 77            ld   (hl),a
0895: C9            ret 				; return to 04C7

--- PACMAN has collided with PURPLE SCARED GHOST (called from 0823), check what was under PURPLE GHOST
08A0: 3A 18 00      ld   a,($0018) 		; hmm what is in 0018 (last color under GHOST?)
08A3: FE 50         cp   $50 			; 50 is the orange "point" color
08A5: CC 86 04      call z,$0486 		; this is the "PACMAN has eaten a point" subroutine
08A8: FE 70         cp   $70 			; 70 is the POWER PILL color
08AA: CC B6 04      call z,$04B6 		; this is the "PACMAN has eaten a POWER PILL" subroutine
08AD: C3 DF 04      jp   $04DF

--- PACMAN has collided with GREEN SCARED GHOST (called from 0839), check what was under GREEN GHOST
08B0: 3A 19 00      ld   a,($0019)
08B3: FE 50         cp   $50
08B5: CC 86 04      call z,$0486 		; PACMAN eats the point under GREEN GHOST
08B8: FE 70         cp   $70
08BA: CC B6 04      call z,$04B6 		; PACMAN eats the POWER PILL under GREEN GHOST
08BD: C3 DF 04      jp   $04DF

--- PACMAN has collided with WHITE SCARED GHOST (called from 084F), check what was under WHITE GHOST
08C0: 3A 1A 00      ld   a,($001A)
08C3: FE 50         cp   $50
08C5: CC 86 04      call z,$0486 		; PACMAN eats the point under WHITE GHOST
08C8: FE 70         cp   $70
08CA: CC B6 04      call z,$04B6 		; PACMAN eats the POWER PILL under WHITE GHOST
08CD: C3 DF 04      jp   $04DF

--- PACMAN has collided with BLUE SCARED GHOST (called from 0865), check what was under BLUE GHOST
08D0: 3A 1B 00      ld   a,($001B)
08D3: FE 50         cp   $50
08D5: CC 86 04      call z,$0486 		; PACMAN eats the point under BLUE GHOST
08D8: FE 70         cp   $70
08DA: CC B6 04      call z,$04B6 		; PACMAN eats the POWER PILL under BLUE GHOST
08DD: C3 DF 04      jp   $04DF

--- called when PACMAN has "eaten" a SCARED GHOST (called from 08AD, 08BD, 08CD, 08DD)
04DF: 2A 22 00      ld   hl,($0022) 	; add decimal 100 points to current score
04E2: 3E 01         ld   a,$01
04E4: 84            add  a,h
04E5: 27            daa
04E6: 67            ld   h,a
04E7: 22 22 00      ld   ($0022),hl
04EA: C3 E3 07      jp   $07E3 			; continue at 07E3










=================== PROGSTART: ================================================================


=================== PROGSTART: ================================================================


=================== PROGSTART: ================================================================

--- the header
0500: 7F 7F 50 41 43 4D 41 4E 01
--- execution starts here
0509: 00 			nop
050A: 00      	    nop
050B: 00            nop
--- copy 3 bytes to start of program (silly copy protection?)
050C: 21 1A 05      ld   hl,$051A
050F: 11 09 05      ld   de,$0509
0512: 01 03 00      ld   bc,$0003
0515: ED B0         ldir
0517: C3 1D 05      jp   $051D
--- this has been modified propably
051A: 00            nop
051B: 00            nop
051C: 00            nop
--- actual start of program
051D: 21 00 1F      ld   hl,$1F00		
0520: 22 AA B7      ld   ($B7AA),hl		; B7AA: contains font addr for 0xA0 to 0xD0
0523: 3E 01         ld   a,$01 			; setup window 1 at (x0=0 y0=0, w=40 h=32) (fullscreen)
0525: 21 00 00      ld   hl,$0000
0528: 11 28 20      ld   de,$2028
052B: CD 03 F0      call $F003
052E: 3C            ; system call 3C: setup new window
052F: 3E 02         ld   a,$02
0531: 21 08 1F      ld   hl,$1F08
0534: 11 1B 01      ld   de,$011B
0537: CD 60 0B      call $0B60			; this is strange? should be F003?

--- insertion at 0B60
0B60: CD 03 F0      call $F003			; setup window 2 (LIFES/POINTSHUD) (x0=8 y0=31, w=27,h=1)
0B63: 3C            ; system call 3C: setup new window
0B64: 3E 04         ld   a,$04 			; setup window 4 (POINTS HUD) (x0=29 y0=31, w=5, h=1)
0B66: 21 1D 1F      ld   hl,$1F1D
0B69: 11 05 01      ld   de,$0105
0B6C: CD 03 F0      call $F003
0B6F: 3C            ; system call 3D: setup a new window

--- set initial game speed
0B70: 3E 06         ld   a,$06
0B72: 32 41 00      ld   ($0041),a 		; set initial game speed to 6 * 6ms
0B75: C9            ret					; return to 053A
---
053A: 3C            					; bug: this is a system call id, but executes as inc a
053B: 00            nop
053C: 00            nop
053D: 00            nop
053E: 21 12 E0      ld   hl,$E012		; setup variables
0541: 22 00 00      ld   ($0000),hl 	; PACMAN's start position
0544: 3E 30         ld   a,$30 			
0546: 32 02 00      ld   ($0002),a 		; PACMAN's normal color (30 == YELLOW)
0549: 3E 01         ld   a,$01
054B: 32 03 00      ld   ($0003),a 		; PACMAN mouth open/closed state (toggles between 01 and FF)
054E: 32 04 00      ld   ($0004),a 		; 
0551: 3E 00         ld   a,$00
0553: 32 05 00      ld   ($0005),a
0556: 3E 00         ld   a,$00
0558: 32 07 00      ld   ($0007),a
055B: 21 10 60      ld   hl,$6010
055E: 22 08 00      ld   ($0008),hl
0561: 21 14 60      ld   hl,$6014
0564: 22 0A 00      ld   ($000A),hl
0567: 21 10 80      ld   hl,$8010
056A: 22 0C 00      ld   ($000C),hl
056D: 21 14 80      ld   hl,$8014
0570: 22 0E 00      ld   ($000E),hl
0573: 21 C0 03      ld   hl,$03C0
0576: 22 10 00      ld   ($0010),hl
0579: 22 12 00      ld   ($0012),hl
057C: 22 14 00      ld   ($0014),hl
057F: 22 16 00      ld   ($0016),hl
0582: 3E 00         ld   a,$00
0584: 32 18 00      ld   ($0018),a
0587: 32 19 00      ld   ($0019),a
058A: 32 1A 00      ld   ($001A),a
058D: 32 1B 00      ld   ($001B),a
0590: 3E 03         ld   a,$03
0592: 32 21 00      ld   ($0021),a
0595: 21 00 00      ld   hl,$0000
0598: 22 22 00      ld   ($0022),hl
059B: 3E F0         ld   a,$F0
059D: 32 25 00      ld   ($0025),a
05A0: 3E 02         ld   a,$02
05A2: 32 26 00      ld   ($0026),a
05A5: 3E FE         ld   a,$FE
05A7: 32 27 00      ld   ($0027),a
05AA: 3E 10         ld   a,$10
05AC: 32 28 00      ld   ($0028),a
05AF: 3E 00         ld   a,$00
05B1: 32 29 00      ld   ($0029),a
---
05B4: 3E 01         ld   a,$01
05B6: CD 30 0B      call $0B30

--- insertion at 0B30: draw intro screen
0B30: CD 03 F0      call $F003		; select window 1 (fullscreen)
0B33: 3D            ; system call 3D: select window
0B34: CD 03 F0      call $F003
0B37: 23            ; system call 23: draw string 
0B38: 10 11         ; 10 = curso to top-left, 11 = page-mode..?
0B3A: 00            ; end of string

0B3B: 3E 00         ld   a,$00
0B3D: 32 A3 B7      ld   ($B7A3),a 	; set color byte (0 = black)
0B40: 21 00 2A      ld   hl,$2A00 	; this writes the intro screen as ASCII (black on black)
0B43: CD 03 F0      call $F003
0B46: 45            ; system call: draw string at (HL)

0B47: 21 00 20      ld   hl,$2000   ; this copies over the color bytes, intro screen visible
0B4A: 11 00 A8      ld   de,$A800
0B4D: 01 FF 09      ld   bc,$09FF
0B50: ED B0         ldir

0B52: 3A FD 01      ld   a,($01FD)  ; wait for any key (keycode is at 01FD)
0B55: FE 00         cp   $00
0B57: CA 52 0B      jp   z,$0B52
0B5A: C9            ret 			; continue at 05B9

--- continue after Intro Screen, draw the play-field
05B9: 00            nop
05BA: CD 03 F0      call $F003 		; position cursor at top-left
05BD: 23            ; system call 23: draw string
05BE: 10 11         
05C0: 00            
05C1: 3E 00         ld   a,$00 		; set text color to black
05C3: 32 A3 B7      ld   ($B7A3),a
05C6: 21 00 1A      ld   hl,$1A00 	; copy playfield ASCIIs
05C9: CD 03 F0      call $F003
05CC: 45            ; system call 45, draw string at 1A00 

05CD: 21 00 10      ld   hl,$1000   ; copy playfield color bytes
05D0: 11 00 A8      ld   de,$A800
05D3: 01 FF 09      ld   bc,$09FF
05D6: ED B0         ldir

--- draw PACMAN
05D8: 2A 00 00      ld   hl,($0000) 	; position of Pacman
05DB: 11 60 03      ld   de,$0360 		; PACMAN sprite source data
05DE: 3A 02 00      ld   a,($0002) 		; PACMAN's current color (may change)
05E1: 32 07 00      ld   ($0007),a
05E4: CD 28 02      call $0228

--- draw PURPLE GHOST
05E7: 2A 08 00      ld   hl,($0008)
05EA: ED 5B 10 00   ld   de,($0010)
05EE: 3E 18         ld   a,$18 			; 18 = PURPLE
05F0: 32 07 00      ld   ($0007),a
05F3: CD 28 02      call $0228

--- draw GREEN GHOST
05F6: 2A 0A 00      ld   hl,($000A)
05F9: ED 5B 12 00   ld   de,($0012)
05FD: 3E 20         ld   a,$20 			; 20 = GREEN
05FF: 32 07 00      ld   ($0007),a
0602: CD 28 02      call $0228

--- draw WHITE GHOST
0605: 2A 0C 00      ld   hl,($000C)
0608: ED 5B 14 00   ld   de,($0014)
060C: 3E 38         ld   a,$38 			; 38 = WHITE
060E: 32 07 00      ld   ($0007),a
0611: CD 28 02      call $0228

--- draw BLUE GHOST
0614: 2A 0E 00      ld   hl,($000E)
0617: ED 5B 16 00   ld   de,($0016)
061B: 3E 68         ld   a,$68 			; 68 = BLUE
061D: 32 07 00      ld   ($0007),a
0620: C3 B0 0A      jp   $0AB0

--- INSERTION 0AB0: draw initial HUD (lifes and score)

0AB0: CD 28 02      call $0228 			; actual sprite drawing call for blue ghost

0AB3: 3E 02         ld   a,$02 			; select window 2 
0AB5: CD 03 F0      call $F003
0AB8: 3D            ; system call 3D: select window

; draw HUD "LIVES: " (sic)
0AB9: 3E 30         ld   a,$30
0ABB: 32 A3 B7      ld   ($B7A3),a 		; set current text color to 30 (YELLOW)
0ABE: CD 03 F0      call $F003
0AC1: 23            ; system call 23: draw text
0AC2: 11 10 4C 49 56 45 53 3A 20  ; cursor home + "LIVES: "
0ACB: 00 					      ; 00 end-of-string

0ACC: 3A 21 00      ld   a,($0021) 		; draw current number of lifes text
0ACF: CD 03 F0      call $F003
0AD2: 1C            ; system call 1C: draw A as hex number

0AD3: CD 03 F0      call $F003
0AD6: 23            ; system call 23: draw text
0AD7: 09 09 09 09 50 4F 49 4E 54 53 3A 20  ; 4x cursor right + "POINTS: "
0AE3: 00 			; 00 end-of-string

0AE4: 2A 22 00      ld   hl,($0022)
0AE7: CD 03 F0      call $F003
0AEA: 1A            ; system call 1A: draw HL as hex number

0AEB: 3A FD 01      ld   a,($01FD) 		; wait for key
0AEE: FE 00         cp   $00
0AF0: CA EB 0A      jp   z,$0AEB

0AF3: C3 23 06      jp   $0623 			; continue at 0623

--- 0623: continue from drawing initial lifes and score HUD, and waiting for start key

--- draw current score points text
0623: 3E 04         ld   a,$04 			; select window 4 (score points window)
0625: CD 03 F0      call $F003
0628: 3D            ; system call 3D: select window
0629: 3E 30         ld   a,$30
062B: 32 A3 B7      ld   ($B7A3),a 		; set text color to YELLOW
062E: CD 03 F0      call $F003
0631: 23            ; system call 23: draw text
0632: 10 11         ; cursor home
0634: 00            ; end-of-string
0635: 2A 22 00      ld   hl,($0022) 	; load current score to HL
0638: CD 03 F0      call $F003
063B: 1A            ; system call 1A: draw HL as text (hex??)

--- handle game speed (waits for 36 ms)
063C: 3A 41 00      ld   a,($0041) 		; game speed waiting loop (36 ms)
063F: CD 03 F0      call $F003
0642: 14            ; system call 14: waiting loop A * 6ms

0643: CD 80 0B      call $0B80

--- subroutine at 0B80, something about game speed...
--- looks like game speed is set to max (6 ms) if score is > hex 1000 ??
0B80: 3A 23 00      ld   a,($0023) 		; 0023 is current score hi-byte
0B83: FE 10         cp   $10
0B85: C0            ret  nz
0B86: 3E 01         ld   a,$01
0B88: 32 41 00      ld   ($0041),a
0B8B: C9            ret 				; return to 0646
---
0646: 00            nop
.. more nops
065A: 00            nop

--- draw a black sprite at PACMAN's position
065B: 3E 00         ld   a,$00 			
065D: 32 07 00      ld   ($0007),a 		; set sprite color to 0
0660: 2A 00 00      ld   hl,($0000) 	; PACMAN's current coordinates
0663: 11 00 03      ld   de,$0300		; sprite source data all 0 except for 1 pixel (?)
0666: CD 28 02      call $0228

0669: 3A 03 00      ld   a,($0003) 		; hmmmmm, this toggles between 01 and FF, PACMAN mouth on/off?
066C: ED 44         neg
066E: 32 03 00      ld   ($0003),a

0671: 3A 02 00      ld   a,($0002) 		; hmm, this compares PACMAN's current color to 10 (RED)
0674: FE 10         cp   $10 			; PACMAN RED means it is in POWER PILL mode
0676: CC 70 07      call z,$0770 		; 0770 handle PACMAN in power pill mode...

--- FIXME: 0770

0679: 00            nop
.. more nops
0681: 00            nop

--- handle keyboard input
0682: 3A FD 01      ld   a,($01FD) 		; current key code is at 01FD
0685: FE 08         cp   $08 			; ASCII 08: cursor left
0687: CA 9C 06      jp   z,$069C
068A: FE 09         cp   $09 			; ASCII 09: cursor right
068C: CA AF 06      jp   z,$06AF
068F: FE 0A         cp   $0A 			; ASCII 0A: cursor down
0691: CA C2 06      jp   z,$06C2
0694: FE 0B         cp   $0B 			; ASCII 0B: cursor up
0696: CA D5 06      jp   z,$06D5
0699: C3 F8 04      jp   $04F8 			; no key pressed, continue to 04F8

--- handle input: cursor left pressed
069C: 3A 00 00      ld   a,($0000) 		; PACMAN current X position
069F: C6 FE         add  a,$FE 			; X - 02 (16 pixels to left)
06A1: 32 00 00      ld   ($0000),a 		; write back to position variable
06A4: 3E FE         ld   a,$FE 			; hmm, store last movement in 0005?
06A6: 32 05 00      ld   ($0005),a
06A9: 11 60 03      ld   de,$0360 		; PACMAN sprite, looking left, open mouth 
06AC: C3 E5 06      jp   $06E5

--- handle input: cursor right pressed
06AF: 3A 00 00      ld   a,($0000) 		; PACMAN X = X + 02 (16 pixels to right)
06B2: C6 02         add  a,$02
06B4: 32 00 00      ld   ($0000),a
06B7: 3E 02         ld   a,$02 			; store last movement in 0005?
06B9: 32 05 00      ld   ($0005),a 		
06BC: 11 40 03      ld   de,$0340 		; PACMAN sprite, looking right, open mouth
06BF: C3 E5 06      jp   $06E5

--- handle input: cursor down pressed
06C2: 3A 01 00      ld   a,($0001) 		; PACMAN Y = Y + 10 (16 pixels down)
06C5: C6 10         add  a,$10
06C7: 32 01 00      ld   ($0001),a
06CA: 3E 10         ld   a,$10 			; last movement in 0005
06CC: 32 05 00      ld   ($0005),a
06CF: 11 A0 03      ld   de,$03A0 		; PACMAN sprite data, looking down, open mouth
06D2: C3 E5 06      jp   $06E5

--- handle input: cursor up pressed
06D5: 3A 01 00      ld   a,($0001) 		; PACMAN Y = Y - 10 (16 pixels up)
06D8: C6 F0         add  a,$F0 			
06DA: 32 01 00      ld   ($0001),a
06DD: 3E F0         ld   a,$F0 			; last movement in 0005
06DF: 32 05 00      ld   ($0005),a
06E2: 11 80 03      ld   de,$0380 		; PACMAN sprite data, lookup up, open mouth

--- continue after cursor input, or no key pressed

--- collision detection
06E5: D9            exx 				; exchange register set with shadow register set
06E6: 2A 00 00      ld   hl,($0000) 	; load new PACMAN coords
06E9: CD 03 F0      call $F003
06EC: 34            ; system call 34: compute pixel and color video RAM address
06ED: 1A            ld   a,(de) 		; load color byte at PACMAN's new video RAM address
06EE: FE 50         cp   $50 			; 50: orange, a "point"
06F0: CC 86 04      call z,$0486 		; play a sound, and increment score by 2
06F3: FE 70         cp   $70 			; 70: bright green: power pill
06F5: CC B6 04      call z,$04B6
06F8: FE 58         cp   $58 			; 58: bright purple: playfield border
06FA: CC 60 04      call z,$0460
06FD: FE 18         cp   $18 			; 18: PURPLE GHOST
06FF: CC C8 07      call z,$07C8 		; VERY COMPLEX SUBROUTINE CALL 
0702: FE 20         cp   $20 			; 20: GREEN GHOST
0704: CC C8 07      call z,$07C8 		; VERY COMPLEX SUBROUTINE CALL 
0707: FE 38         cp   $38 			; 38: WHITE GHOST
0709: CC C8 07      call z,$07C8 		; VERY COMPLEX SUBROUTINE CALL 
070C: FE 68         cp   $68 			; 68: BLUE GHOST
070E: CC C8 07      call z,$07C8 		; VERY COMPLEX SUBROUTINE CALL 
0711: FE 32         cp   $32 			; 32: yellow on red: regeneration area in the center
0713: CC 60 04      call z,$0460

--- no collision
0716: D9            exx					; restore original register set
0717: 3A 03 00      ld   a,($0003) 		; 01/FF PACMAN open/closed mouth
071A: FE 01         cp   $01 			; DE has PACMAN open-mouth sprite address from input handling
071C: CA 22 07      jp   z,$0722

--- draw PACMAN with open/closed mouth
071F: 11 20 03      ld   de,$0320 		; PACMAN closed mouth sprite data
0722: 3A 02 00      ld   a,($0002) 		; PACMAN's color
0725: 32 07 00      ld   ($0007),a
0728: 2A 00 00      ld   hl,($0000)  	; PACMAN's position
072B: CD 28 02      call $0228
072E: 00            nop
072F: 00            nop
0730: 00            nop
0731: 00            nop
0732: 00            nop
0733: 00            nop
0734: C3 00 0C      jp   $0C00 			; continue at 0C00

=== BEGIN OF GHOST AI, EXECUTED 4X, one for each ghost
--- continued from 0734, also jumped here from 0D3A (end of ghost AI)
0C00: 3A 29 00      ld a,($0029) 		; 0029 counts from 1 to 4 and is then reset
0C03: 3C            inc  a
0C04: 32 29 00      ld   ($0029),a
0C07: FE 01         cp   $01
0C09: CA 00 09      jp   z,$0900 		; execute GHOST AI for PURPLE GHOST
0C0C: FE 02         cp   $02
0C0E: CA 40 09      jp   z,$0940 		; execute GHOST AI for GREEN GHOST
0C11: FE 03         cp   $03
0C13: CA 80 09      jp   z,$0980 		; execute GHOST AI for WHITE GHOST
0C16: FE 04         cp   $04
0C18: CA C0 09      jp   z,$09C0 		; execute GHOST AI for BLUE GHOST
0C1B: 3E 00         ld   a,$00 			; reset counter for next frame
0C1D: 32 29 00      ld   ($0029),a
0C20: C3 90 0A      jp   $0A90 			; all 4 GHOST AIs handled, continue 

--- continued after GHOST AI, FIXME FIXME: THIS HAS BEEN TAMPERED WITH!!!
--- BUG: TAMPERING !!!
--- THIS CODE RENDERS 2 POWER PILLS, DEFINITELY TAMPERED
0A90: 3E 70         ld   a,$70
0A92: 32 07 00      ld   ($0007),a
0A95: 21 06 10      ld   hl,$1006
0A98: 11 20 04      ld   de,$0420
0A9B: CD 28 02      call $0228
0A9E: 21 24 30      ld   hl,$3024
0AA1: 11 20 04      ld   de,$0420
0AA4: CD 28 02      call $0228
0AA7: C3 23 06      jp   $0623 			; and jump to start of game loop

--- called when 0029 is 01: prepare indirect pointers for PURPLE GHOST and call 0C23
0900: 2A 08 00      ld   hl,($0008) 	; copy current PURPLE GHOST screen position to 001C
0903: 22 1C 00      ld   ($001C),hl
0906: 2A 10 00      ld   hl,($0010) 	; copy current GREEN GHOST screen pos to 001E
0909: 22 1E 00      ld   ($001E),hl
090C: 3A 18 00      ld   a,($0018) 		; should be current color under PURPLE GHOST
090F: 32 20 00      ld   ($0020),a 		
0912: 3A 25 00      ld   a,($0025) 		; ?? this is a directionn
0915: 32 24 00      ld   ($0024),a
0918: 3E 18         ld   a,$18 			; 18 is the PURPLE GHOST color
091A: 32 2F 00      ld   ($002F),a
091D: 21 08 00      ld   hl,$0008 		; 0008 stores PURPLE GHOST position
0920: 22 30 00      ld   ($0030),hl
0923: 21 10 00      ld   hl,$0010 		; 0010 stores PURPLE GHOST sprite data start
0926: 22 32 00      ld   ($0032),hl
0929: 21 18 00      ld   hl,$0018 		; 0018 is where "color under" PURPLE GHOST is stored
092C: 22 34 00      ld   ($0034),hl
092F: 21 25 00      ld   hl,$0025 		; 0025 is where direction of PURPLE GHOST is stored
0932: 22 36 00      ld   ($0036),hl
0935: C3 23 0C      jp   $0C23
0938: 00            nop
... more nops
093F: 00            nop

--- called when 0029 is 02: prepare indirect pointers for GREEN GHOST and call 0C23
0940: 2A 0A 00      ld   hl,($000A)
0943: 22 1C 00      ld   ($001C),hl
0946: 2A 12 00      ld   hl,($0012)
0949: 22 1E 00      ld   ($001E),hl
094C: 3A 19 00      ld   a,($0019)
094F: 32 20 00      ld   ($0020),a
0952: 3A 26 00      ld   a,($0026)
0955: 32 24 00      ld   ($0024),a
0958: 3E 20         ld   a,$20
095A: 32 2F 00      ld   ($002F),a
095D: 21 0A 00      ld   hl,$000A
0960: 22 30 00      ld   ($0030),hl
0963: 21 12 00      ld   hl,$0012
0966: 22 32 00      ld   ($0032),hl
0969: 21 19 00      ld   hl,$0019
096C: 22 34 00      ld   ($0034),hl
096F: 21 26 00      ld   hl,$0026
0972: 22 36 00      ld   ($0036),hl
0975: C3 23 0C      jp   $0C23
0978: 00            nop
.. more nops
097F: 00            nop

--- called when 0029 is 03: preapre indirect pointers for WHITE GHOST and call 0C23
0980: 2A 0C 00      ld   hl,($000C)
0983: 22 1C 00      ld   ($001C),hl
0986: 2A 14 00      ld   hl,($0014)
0989: 22 1E 00      ld   ($001E),hl
098C: 3A 1A 00      ld   a,($001A)
098F: 32 20 00      ld   ($0020),a
0992: 3A 27 00      ld   a,($0027)
0995: 32 24 00      ld   ($0024),a
0998: 3E 38         ld   a,$38
099A: 32 2F 00      ld   ($002F),a
099D: 21 0C 00      ld   hl,$000C
09A0: 22 30 00      ld   ($0030),hl
09A3: 21 14 00      ld   hl,$0014
09A6: 22 32 00      ld   ($0032),hl
09A9: 21 1A 00      ld   hl,$001A
09AC: 22 34 00      ld   ($0034),hl
09AF: 21 27 00      ld   hl,$0027
09B2: 22 36 00      ld   ($0036),hl
09B5: C3 23 0C      jp   $0C23
09B8: 00            nop
... more nops
09BF: 00            nop

--- called when 0029 is 04: prepare indirect pointers for BLUE GHOST and call 0C23
09C0: 2A 0E 00      ld   hl,($000E)
09C3: 22 1C 00      ld   ($001C),hl
09C6: 2A 16 00      ld   hl,($0016)
09C9: 22 1E 00      ld   ($001E),hl
09CC: 3A 1B 00      ld   a,($001B)
09CF: 32 20 00      ld   ($0020),a
09D2: 3A 28 00      ld   a,($0028)
09D5: 32 24 00      ld   ($0024),a
09D8: 3E 68         ld   a,$68
09DA: 32 2F 00      ld   ($002F),a
09DD: 21 0E 00      ld   hl,$000E
09E0: 22 30 00      ld   ($0030),hl
09E3: 21 16 00      ld   hl,$0016
09E6: 22 32 00      ld   ($0032),hl
09E9: 21 1B 00      ld   hl,$001B
09EC: 22 34 00      ld   ($0034),hl
09EF: 21 28 00      ld   hl,$0028
09F2: 22 36 00      ld   ($0036),hl
09F5: C3 23 0C      jp   $0C23
09F8: 00            nop
... more nops
09FF: 00            nop

--- this is called when left, right, up OR down of current ghost is a playfield barrier
0A00: F5            push af
0A01: 3A 40 00      ld   a,($0040) 		; increment collide barrier counter
0A04: 3C            inc  a
0A05: 32 40 00      ld   ($0040),a
0A08: F1            pop  af
0A09: C9            ret

--- this is called when left, right, up OR down of current ghost is a regeneration field
0A0A: 3A 40 00      ld   a,($0040) 		; increment collide barrier counter
0A0D: 3C            inc  a
0A0E: 32 40 00      ld   ($0040),a
0A11: 3A 1E 00      ld   a,($001E) 		; test if current ghost is HOLLOW GHOST (sprite addr 0400)
0A14: FE 00         cp   $00
0A16: CA 1A 0A      jp   z,$0A1A 
0A19: C9            ret

0A1A: 21 C0 03      ld   hl,$03C0 		; convert HOLLOW GHOST back to NORMAL GHOST
0A1D: 22 1E 00      ld   ($001E),hl 	; set stored sprite address of current ghost to 03C0
0A20: 2A 32 00      ld   hl,($0032)  	; set original sprite address to 03C0 through 
0A23: 3E C0         ld   a,$C0 			; current ghost indirection pointer
0A25: 77            ld   (hl),a
0A26: 3E 03         ld   a,$03
0A28: 23            inc  hl
0A29: 77            ld   (hl),a
0A2A: C9            ret

--- called from ghost AI, update current GHOST position 
in: A current ghost direction(?)
    HL address of current ghost position
0A30: F5            push af
0A31: FE F0         cp   $F0 			; up?
0A33: CA 3F 0A      jp   z,$0A3F
0A36: FE 10         cp   $10 			; down?
0A38: CA 3F 0A      jp   z,$0A3F
0A3B: 85            add  a,l 			; horizontal movement (A is FE or 02)
0A3C: 6F            ld   l,a
0A3D: F1            pop  af
0A3E: C9            ret
0A3F: 84            add  a,h 			; vertical movement (A is 10 or F0)
0A40: 67            ld   h,a
0A41: F1            pop  af
0A42: C9            ret


--- continued from 0029==1, 0029==2, 0029==3, 0029==4, PROBABLY GHOST "AI" ??
--- restore background under current GHOST
--- FIXME: BUG WHEN NEITHER A POINT NOR POWER PILL IS UNDER GHOST: WILL ALWAYS RENDER A POINT
0C23: 3A 20 00      ld   a,($0020)
0C26: 32 07 00      ld   ($0007),a 		; color under current ghost
0C29: FE 50         cp   $50 			; a POINT (orange)
0C2B: CA 41 0C      jp   z,$0C41
0C2E: FE 70         cp   $70 			; a POWER PILL
0C30: CA 47 0C      jp   z,$0C47
0C33: C3 3B 0C      jp   $0C3B 			; anything else - BUG?

0C36: 00            nop 				; BUG BUG BUG - THIS LOOKS LIKE A CHEAT HACK??
0C37: 00            nop 				; I think setup code for rendering "black" should be here...
0C38: C3 4A 0C      jp   $0C4A 			; original call?

--- continue when neither a POWER PILL nor a POINT is under current GHOST
0C3B: 3E 50         ld   a,$50 			; shouldn't this be 0?
0C3D: 32 07 00      ld   ($0007),a
0C40: 00            nop 				; BUG BUG BUG??? this will always render a POINT!!!

--- called when color under current ghost is 50 (point)
0C41: 11 40 04      ld   de,$0440 		; pointer to POINT sprite data
0C44: C3 4A 0C      jp   $0C4A

--- called when color under current ghost is 70 (POWER PILL)
0C47: 11 20 04      ld   de,$0420 		; pointer to POWER PILL data

---  draws the current GHOST background sprite 
0C4A: 2A 1C 00      ld   hl,($001C) 	; current GHOST position
0C4D: CD 28 02      call $0228 			; draw POINT or POWER PILL sprite 

0C50: 3E 00         ld   a,$00 			; 0040 incremented for each direction 
0C52: 32 40 00      ld   ($0040),a 		; with a barrier for current ghost

--- is right of the current ghost a barrier ??
0C55: 2A 1C 00      ld   hl,($001C) 	; current ghost position
0C58: 3E 02         ld   a,$02 			; right of current ghost position
0C5A: 85            add  a,l
0C5B: 6F            ld   l,a
0C5C: CD 03 F0      call $F003
0C5F: 34            ; system call 34: compute pixel and color video RAM address
0C60: 1A            ld   a,(de)
0C61: FE 58         cp   $58 			; 58 is playfield border color
0C63: CC 00 0A      call z,$0A00 		; increment barrier counter (0040)
0C66: FE 32         cp   $32 			; 32 is regeneration cross in the middle
0C68: CC 0A 0A      call z,$0A0A 		; increment barrier counter (0040) and HOLLOW -> NORMAL

--- is left of the current ghost a barrior ??
0C6B: 2A 1C 00      ld   hl,($001C) 	; current GHOST position
0C6E: 3E FE         ld   a,$FE 			
0C70: 85            add  a,l 			; left of current ghost position
0C71: 6F            ld   l,a
0C72: CD 03 F0      call $F003
0C75: 34            ; system call 34: compute pixel and color video RAM address
0C76: 1A            ld   a,(de)
0C77: FE 58         cp   $58
0C79: CC 00 0A      call z,$0A00 		; increment barrier counter (0040)
0C7C: FE 32         cp   $32
0C7E: CC 0A 0A      call z,$0A0A 		; increment barrier counter (0040) and HOLLOW -> NORMAL

--- is below the current ghost a barrier?
0C81: 2A 1C 00      ld   hl,($001C)
0C84: 3E 10         ld   a,$10
0C86: 84            add  a,h
0C87: 67            ld   h,a
0C88: CD 03 F0      call $F003
0C8B: 34            ; system call 34: compute pixel and color video RAM address
0C8C: 1A            ld   a,(de)
0C8D: FE 58         cp   $58
0C8F: CC 00 0A      call z,$0A00 		; increment barrier counter (0040)
0C92: FE 32         cp   $32
0C94: CC 0A 0A      call z,$0A0A 		; increment barrier counter (0040) and HOLLOW -> NORMAL

--- is above current ghost a barrier?
0C97: 2A 1C 00      ld   hl,($001C)
0C9A: 3E F0         ld   a,$F0
0C9C: 84            add  a,h
0C9D: 67            ld   h,a
0C9E: CD 03 F0      call $F003
0CA1: 34            ; system call 34: compute pixel and color video RAM address
0CA2: 1A            ld   a,(de)
0CA3: FE 58         cp   $58
0CA5: CC 00 0A      call z,$0A00 		; increment barrier counter (0040)
0CA8: FE 32         cp   $32
0CAA: CC 0A 0A      call z,$0A0A 		; increment barrier counter (0040) and HOLLOW -> NORMAL

--- 0040 now has contains the number of barriers (0..4)
0CAD: 00            nop
... more nops
0CB4: 00            nop

--- FIXME FIXME: not clear what's happening here
0CB5: 3A 37 00      ld   a,($0037) 		; hmmmmm nobody seems to write 0037...?
0CB8: FE 02         cp   $02
0CBA: C2 C9 0C      jp   nz,$0CC9 		; this is ALWAYS taken??

--- this is never called
0CBD: 3A 24 00      ld   a,($0024) 		; direction of current AI ghost(?)
0CC0: 2A 1C 00      ld   hl,($001C) 	; current ghost position is stored in 001C
0CC3: CD 30 0A      call $0A30 			; this updates the current GHOST position (A is direction)
0CC6: C3 D5 0C      jp   $0CD5

--- continue from 0CBA if $(0037) != 02
--- this basically moves the current ghost into a random direction
0CC9: ED 5F         ld   a,r 			; R is DRAM refresh counter (random numer?)
0CCB: 6F            ld   l,a
0CCC: 26 2F         ld   h,$2F 			; 2F00 .. 2FFF is a table of random directions
0CCE: 7E            ld   a,(hl) 		; FIXME: 2FFF contains 0! potential bug???
0CCF: 2A 1C 00      ld   hl,($001C)
0CD2: CD 30 0A      call $0A30 			; move ghost into random direction

0CD5: 32 3A 00      ld   ($003A),a 		; store random position in 003A
0CD8: 22 38 00      ld   ($0038),hl  	; store new GHOST position in 0038
0CDB: CD 03 F0      call $F003
0CDE: 34            ; system call 34: compute pixel and color video RAM address
0CDF: 1A            ld   a,(de) 		; load color byte in new GHOST position (collision control?)
0CE0: 00            nop 				; why are here 2 nops??
0CE1: 00            nop
0CE2: C3 58 0A      jp   $0A58

--- continue at 0A58: collision check, and retry with new random direction, potential infinite loop?
0A58: FE 58         cp   $58 			; 58: collision with playfield barrier
0A5A: CA C9 0C      jp   z,$0CC9 		; -> loop to random direction, basically "retry"
0A5D: FE 32         cp   $32 			; 32: collision with regeneration field
0A5F: CA C9 0C      jp   z,$0CC9 		; -> loop to random direction, basically "retry"
0A62: C3 E5 0C      jp   $0CE5 			; no collision

--- continue if GHOST movement was valid (no barrier collision)

--- write new current AI ghost position back to actual GHOST position variable
0CE5: 2A 30 00      ld   hl,($0030)  	; HL: address of current GHOST screen pos
0CE8: 3A 38 00      ld   a,($0038) 		; A: new current ghost position (X)
0CEB: 77            ld   (hl),a 		; update GHOST position X
0CEC: 23            inc  hl
0CED: 3A 39 00      ld   a,($0039) 		
0CF0: 77            ld   (hl),a 		; update GHOST position Y
0CF1: 2A 36 00      ld   hl,($0036)
0CF4: 3A 3A 00      ld   a,($003A)
0CF7: 77            ld   (hl),a

0CF8: 2A 38 00      ld   hl,($0038) 	; current new GHOST position
0CFB: CD 03 F0      call $F003
0CFE: 34            ; system call 34: compute pixel / color video RAM address 
0CFF: 1A            ld   a,(de) 		; A: color byte under new GHOST position
0D00: 00            nop
... more nops
0D13: 00            nop
0D14: FE 10         cp   $10 			; collision with RED PACMAN?
0D16: CC 48 0A      call z,$0A48
0D19: FE 30         cp   $30 			; collision with YELLOW PACMAN?
0D1B: CC 48 0A      call z,$0A48

--- SUBROUTINE 0A48: collision of current ghost with YELLOW or RED PACMAN
0A48: 00            nop
0A49: 3A 2F 00      ld   a,($002F) 		; current AI ghost color
0A4C: CD C8 07      call $07C8 			; 07C8: central PACMAN/GHOST collision subroutine
0A4F: 3E 00         ld   a,$00
0A51: C9            ret

--- continue: after collision check with PACMAN, A is now color of new ghost position
0D1E: 2A 34 00      ld   hl,($0034)
0D21: 77            ld   (hl),a 		; store color of new ghost position
0D22: 2A 38 00      ld   hl,($0038) 	; 0038 is new current ghost position
0D25: ED 5B 1E 00   ld   de,($001E) 	; 001E is current GHOST sprite data
0D29: 3A 2F 00      ld   a,($002F) 		; 002F is current GHOST color
0D2C: 32 07 00      ld   ($0007),a
0D2F: CD 28 02      call $0228 			; draw current GHOST
0D32: 00            nop
... more nops
0D39: 00            nop
0D3A: C3 00 0C      jp   $0C00






