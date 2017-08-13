#include "../picoc.h"
#include "../interpreter.h"

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/cm3/scb.h>

#ifdef DEBUGGER
static int gEnableDebugger = true;
#else
/* static int gEnableDebugger = false; */
#endif

void BasicIOInit(Picoc *pc)
{
	/* Enable clocks for GPIO port B (for GPIO_USART3_TX) and USART3. */

	/* Setup GPIO pin GPIO_USART3_TX/GPIO10 on GPIO port B for transmit. */
	/* Setup UART parameters. */

	/* Finally enable the USART. */
}

void PlatformInit(Picoc *pc) {
	rcc_clock_setup_in_hse_8mhz_out_72mhz();
	rcc_periph_clock_enable(RCC_USART3);

	/* Enable GPIOC clock. */
	rcc_periph_clock_enable(RCC_GPIOB);	 /* Try removeing this at some point */
	rcc_periph_clock_enable(RCC_GPIOC);

	/* Set GPIO13 (in GPIO port C) to 'output push-pull'. */
	gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_2_MHZ,
		      GPIO_CNF_OUTPUT_PUSHPULL, GPIO13);

	gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ,
		      GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO_USART3_TX);

	usart_set_baudrate(USART3, 38400);
	usart_set_databits(USART3, 8);
	usart_set_stopbits(USART3, USART_STOPBITS_1);
	usart_set_mode(USART3, USART_MODE_TX | USART_MODE_RX);
	usart_set_parity(USART3, USART_PARITY_NONE);
	usart_set_flow_control(USART3, USART_FLOWCONTROL_NONE);
	usart_enable(USART3);

}

void PlatformCleanup(Picoc *pc) { }

/* get a line of interactive input */
char *PlatformGetLine(char *Buf, int MaxLen, const char *Prompt)
{
    if (Prompt != NULL)
         PrintStr(Prompt, NULL);

    char *c;
    for (c = Buf; c < Buf + MaxLen - 1; c += 1) {
         *c = PlatformGetCharacter();
         if (*c == '\n' || *c == '\r') break;
    }
    c[1] = '\0';
    return Buf;
}

/* get a character of interactive input */
int PlatformGetCharacter()
{
     gpio_toggle(GPIOC, GPIO13);	/* Toggle the LED, just because */
     return usart_recv_blocking(USART3);
}

/* write a character to the console */
void PlatformPutc(unsigned char OutCh, union OutputStreamInfo *Stream)
{
    usart_send_blocking(USART3, OutCh);
}

/* read a file into memory */
char *PlatformReadFile(Picoc *pc, const char *FileName)
{
     ProgramFailNoParser(pc, "can't read file %s\n", FileName);
     return NULL ;
}

/* read and scan a file for definitions */
void PicocPlatformScanFile(Picoc *pc, const char *FileName)
{
     ProgramFailNoParser(pc, "can't read file %s\n", FileName);
}

/* exit the program */
void PlatformExit(Picoc *pc, int RetVal) {
     scb_reset_system();
}

/* Some IO primitives because we don't have stdio. */
void PrintCh(char OutCh, IOFILE *ignored) {
     usart_send_blocking(USART3, OutCh);
}
     
void PrintStr(const char *str, IOFILE *ignored) {
     for (const char *c = str; *c; c += 1)
          usart_send_blocking(USART3, *c);
}

void PrintSimpleInt(long Num, IOFILE *ignored) {
     char buffer[30];	/* Ought to be a way to work this size out... */
     snprintf(buffer, 30, "%ld", Num);
     PrintStr(buffer, ignored);
}

int main() {
     extern unsigned _stack, _ebss;
     Picoc pc;

     // Give 80% of our free ram to the interpreter heap.
     PicocInitialize(&pc, 12 * 1024);
     PicocIncludeAllSystemHeaders(&pc);
     PicocParseInteractive(&pc);
     PicocCleanup(&pc);
     return pc.PicocExitValue;
}
