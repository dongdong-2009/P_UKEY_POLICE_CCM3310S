
	.import USBDev_ISR
    .import main
    .import g_counter

.text
	
    .export  USB_int_Service
    .align 4
USB_int_Service:
	subi	r0,32					## Decrement SP to save registers
	subi	r0,28					## Decrement SP to save registers
	stm		r1-r15,(r0) 			## Save all registers to the stack

	subi	r0,8					## Decrement SP to save PC and PSR

	mfcr	r1,EPC					## Save the PC for the current task
	stw 	r1,(r0,0)

	mfcr	r1,EPSR					## Save the PSR for the current task
	stw 	r1,(r0,4)
	
	jsri	USBDev_ISR
	
	ldw 	r1,(r0,4)				## Get the PSR for the task
	mtcr	r1,EPSR

	ldw 	r1,(r0,0)				## Get the PC for the task
	mtcr	r1,EPC
	addi	r0,8					## Increment SP past the PC and PSR
	
	ldm		r1-r15,(r0) 			## Load all registers from the stack
	addi	r0,28					## Increment SP past the registers
	addi	r0,32					## Increment SP past the registers
	
	rte	