
#include "ints.h"

int install_asm_irq_handler(int irq, void (*irq_func)(void), _go32_dpmi_seginfo *old_irq) {
	// endereço do handler original
	_go32_dpmi_get_protected_mode_interrupt_vector(IRQ_VECTOR(irq), old_irq);

	// instalação do handler na IDT
	_go32_dpmi_seginfo new_irq;
	new_irq.pm_selector = _go32_my_cs( ); // selector do código
	new_irq.pm_offset = (unsigned long) irq_func; // endereço do novo handler
	_go32_dpmi_set_protected_mode_interrupt_vector(IRQ_VECTOR(irq), &new_irq);

	return 0;
}

int install_c_irq_handler(int irq, void (*irq_func)(void), _go32_dpmi_seginfo *old_irq) {
	// endereço do handler original
	_go32_dpmi_get_protected_mode_interrupt_vector(IRQ_VECTOR(irq), old_irq);

	// instalação do handler na IDT
	_go32_dpmi_seginfo new_irq;
	new_irq.pm_selector = _go32_my_cs( ); // selector do código
	new_irq.pm_offset = (unsigned long) irq_func; // endereço do novo handler
	// prepara funcao assembly para chamar funcao C
	_go32_dpmi_allocate_iret_wrapper(&new_irq);
	_go32_dpmi_set_protected_mode_interrupt_vector(IRQ_VECTOR(irq), &new_irq);

	return 0;
}

void reinstall_asm_irq_handler(int irq, _go32_dpmi_seginfo *old_irq) {
	// importante: reposição do handler original
	_go32_dpmi_set_protected_mode_interrupt_vector(IRQ_VECTOR(irq), old_irq);
}

void reinstall_c_irq_handler(int irq, _go32_dpmi_seginfo *old_irq) {
	_go32_dpmi_seginfo new_irq;
	_go32_dpmi_get_protected_mode_interrupt_vector(IRQ_VECTOR(irq), &new_irq);

	// importante: reposição do handler original
	_go32_dpmi_set_protected_mode_interrupt_vector(IRQ_VECTOR(irq), old_irq);

	// liberta funcao assembly
	_go32_dpmi_free_iret_wrapper(&new_irq);
}

int disable_irq(int irq) {
	int pic = PIC1_MASK;
	if (irq > 7)
		pic = PIC2_MASK;

	int oldStatus = inportb(pic);
	outportb(pic, oldStatus | IRQ_MASK_BIT(irq));
	
	return (((oldStatus & IRQ_MASK_BIT(irq)) == 0) ? 0 : 1);
}

void enable_irq(int irq) {
	int pic = PIC1_MASK;
	if (irq > 7)
		pic = PIC2_MASK;

	outportb(pic, inportb(pic) & ~IRQ_MASK_BIT(irq));
}

void mask_pic(int irq) {
  int tmp;
  if(irq<8) {
    tmp = inportb(PIC1_MASK);
    tmp = tmp | IRQ_MASK_BIT(irq);
    outportb(PIC1_MASK, tmp);
  }
  else {
    tmp = inportb(PIC2_MASK);
    tmp = tmp | IRQ_MASK_BIT(irq);
    outportb(PIC2_MASK, tmp);	
  }
}

void unmask_pic(int irq) {
  int tmp;
  if(irq<8) {
    tmp = inportb(PIC1_MASK);
    tmp = tmp & ~(IRQ_MASK_BIT(irq));
    outportb(PIC1_MASK, tmp);
  }
  else {
    tmp = inportb(PIC2_MASK);
    tmp = tmp & ~(IRQ_MASK_BIT(irq));
    outportb(PIC2_MASK, tmp);	
  }
}
