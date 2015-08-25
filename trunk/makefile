############# change if condition to false to use liblcom.a ######################


## make without liblcom
ifeq (1,0)

zoids.exe: zoids.o sprite.o videog.o ints.o rtc.o rtc_asm.o pixmap.h queue.o list.o kbc.o kbd_isr.o music.o timer.o assembly.o anima.o codepage.o GQueue.o mouse.o kbc_init.o 
	gcc -Wall sprite.o videog.o zoids.o ints.o rtc.o rtc_asm.o queue.o list.o kbc.o kbd_isr.o music.o timer.o assembly.o anima.o codepage.o GQueue.o mouse.o kbc_init.o -o zoids.exe
	rename zoids.000 zoids.exe

zoids.o: zoids.c
	gcc -Wall -c zoids.c
	
sprite.o: sprite.c sprite.h videog.h
	gcc -Wall -c sprite.c

video-graphics.o: videog.c videog.h
	gcc -Wall -c videog.c

ints.o: ints.c ints.h utypes.h
	gcc -Wall -c ints.c
	
rtc.o: rtc.c rtc.h
	gcc -Wall -c rtc.c

queue.o: queue.h queue.c
	gcc -Wall -c queue.c
	
list.o: list.h list.c
	gcc -Wall -c list.c
	
rtc_asm.o: rtc_asm.asm
	nasm -t -f coff rtc_asm.asm -o rtc_asm.o
	
kbc.o: kbc.h kbc.c ints.h ints.c timer.h timer.c
	gcc -Wall -c kbc.c
	
kbd_isr.o: kbd_isr.asm
	nasm -t -f coff kbd_isr.asm -o kbd_isr.o

music.o: music.c music.h timer.h timer.c
	gcc -Wall -c music.c
	
timer.o: timer.c timer.h
	gcc -Wall -c timer.c
	
assembly.o: assembly.asm
	nasm -t -f coff assembly.asm -o assembly.o
	
anima.o: anima.c anima.h defines.h
	gcc -Wall -c anima.c
	
codepage.o: codepage.c codepage.h
	gcc -Wall -c codepage.c
	
mouse.o: mouse.asm
	nasm -t -f coff mouse.asm -o mouse.o
	
GQueue.o: GQueue.h GQueue.c
	gcc -Wall -c GQueue.c

clean:
	rename kbc_init.o kbc_init.0
	del *.o
	del *.exe
	del *.a
	rename kbc_init.0 kbc_init.o

## make with liblcom.a
else

zoids.exe: zoids.o sprite.o videog.o ints.o rtc.o rtc_asm.o pixmap.h queue.o list.o kbc.o kbd_isr.o music.o timer.o assembly.o zoid.o ovule.o sperm.o codepage.o GQueue.o mouse.o kbc_init.o 
	gcc -Wall zoid.o zoids.o sperm.o liblcom.a ovule.o kbc_init.o -o zoids.exe
#rename zoids.000 zoids.exe

videog.o: videog.c videog.h
	gcc -Wall -c videog.c
	ar r liblcom.a videog.o
	
zoids.o: zoids.c
	gcc -Wall -c zoids.c	

sprite.o: sprite.c sprite.h videog.h
	gcc -Wall -c sprite.c
	ar r liblcom.a sprite.o


ints.o: ints.c ints.h utypes.h
	gcc -Wall -c ints.c
	ar r liblcom.a ints.o
	
rtc.o: rtc.c rtc.h
	gcc -Wall -c rtc.c
	ar r liblcom.a rtc.o

queue.o: queue.h queue.c
	gcc -Wall -c queue.c
	ar r liblcom.a queue.o
	
list.o: list.h list.c
	gcc -Wall -c list.c
	ar r liblcom.a list.o
	
rtc_asm.o: rtc_asm.asm
	nasm -t -f coff rtc_asm.asm -o rtc_asm.o
	ar r liblcom.a rtc_asm.o
	
kbc.o: kbc.h kbc.c ints.h ints.c timer.h timer.c
	gcc -Wall -c kbc.c
	ar r liblcom.a kbc.o
	
kbd_isr.o: kbd_isr.asm
	nasm -t -f coff kbd_isr.asm -o kbd_isr.o
	ar r liblcom.a kbd_isr.o

music.o: music.c music.h timer.h timer.c
	gcc -Wall -c music.c
	ar r liblcom.a music.o
	
timer.o: timer.c timer.h
	gcc -Wall -c timer.c
	ar r liblcom.a timer.o
	
assembly.o: assembly.asm
	nasm -t -f coff assembly.asm -o assembly.o
	ar r liblcom.a assembly.o
	
zoid.o: zoid.c zoid.h defines.h
	gcc -Wall -c zoid.c
	
ovule.o: ovule.c ovule.h defines.h
	gcc -Wall -c ovule.c
	
sperm.o: sperm.c sperm.h defines.h
	gcc -Wall -c sperm.c
	
codepage.o: codepage.c codepage.h
	gcc -Wall -c codepage.c
	ar r liblcom.a codepage.o
	
mouse.o: mouse.asm
	nasm -t -f coff mouse.asm -o mouse.o
	ar r liblcom.a mouse.o
	
GQueue.o: GQueue.h GQueue.c
	gcc -Wall -c GQueue.c
	ar r liblcom.a GQueue.o
	
clean:
	rename kbc_init.o kbc_init.0
	del *.o
	del *.exe
	del *.a
	rename kbc_init.0 kbc_init.o
	
endif