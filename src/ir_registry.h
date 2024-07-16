/* Copyright 2024 Kinglet B.V.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef IR_REGISTRY_H
#define IR_REGISTRY_H

#ifdef __cplusplus
extern "C" {
#endif

enum ireg_architecture {
  /* Generic instructions */
  IRARC_IR,

  /* Instructions specific to shading language intermediate representation */
  IRARC_SLIR
};

/* Whether operands are used or defined by an instruction, combine with OR to
 * indicate both. */
#define IREG_USE 1
#define IREG_DEF 2

/* an operand in an instruction */
struct ireg_operand {
  /* Combination of IREG_ flags. */
  int flags_;
};

/* an instruction belonging to a particular category */
struct ireg_instr {
  /* The architecture of the instruction */
  enum ireg_architecture architecture_;

  /* The index of the instruction, this is assigned when adding the instruction
   * to the registry and may be used to identify it externally (e.g. in ir_instr::instr_index_ )
   */
  int instr_index_;

  /* The operands */
  int num_operands_;
  struct ireg_operand *operands_;

  /* The mnemonic representing the instruction in textual form */
  const char *mnemonic_;

  /* The format string for the instruction
   * The format here uses %0, %1, %2, and so on, for specifying the location of the 
   * argument string in the overal instruction, so, for instance:
   * "[%0 * 4 + %1], %2"
   * could be the format string for a hypothetical "MOV [EAX * 4 + EBX], ECX" instruction.
   * Note that, because there may be an inordinate number of arguments, a multi-digit
   * number may be used, e.g. %10, %11, %12, etc., to a maximum of two digits. So, if you
   * wish to have argument 0 be followed by the digit 0, you would use "%000" where "%00"
   * is the number 0 using the maximum number of digits ("%00"), followed by the literal
   * digit 0. It is currently not possible to go beyond 100 arguments.
   */
  const char *format_;
};

/* a register belonging to a particular category */
struct ireg_register {
  /* The architecture of the register */
  enum ireg_architecture architecture_;

  /* Index of the register */
  int register_index_;

  /* The name of the register */
  const char *name_;

  /* The size of the register in bits */
  int bitsize_;

  /* The containing register index (if any)
   * For example: AL and AH are contained in AX, which is contained in EAX, which is contained in RAX */
  int containing_register_index_;
};

/* the registry of instructions and registers */
struct ireg_registry {
  /* Note that the registers and instructions are not owned by the registry, and must 
   * be freed separately. The idea behind this is that, for fixed sets of registers and
   * instructions, you may want to have a statically allocated set of instructions and
   * registers, and may wish to mix them with a dynamic set that is generated at runtime,
   * depending on the mix of architectures being added to the registry, and how the instructions
   * or registers belonging to an architecture are being determined.
   */

  /* Table of pointers to all instructions. */
  size_t num_instructions_;
  size_t num_instructions_allocated_;
  struct ireg_instr **instructions_;

  /* Table of pointers to all registers. */
  size_t num_registers_;
  size_t num_registers_allocated_;
  struct ireg_register **registers_;
};

void ireg_init(struct ireg_registry *ireg);
void ireg_cleanup(struct ireg_registry *ireg);

int ireg_add_instruction(struct ireg_registry *ireg, struct ireg_instr *instr);
int ireg_set_instruction(struct ireg_registry *ireg, struct ireg_instr *instr);

int ireg_add_register(struct ireg_registry *ireg, struct ireg_register *reg);
int ireg_set_register(struct ireg_registry *ireg, struct ireg_register *reg);


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* IR_REGISTRY_H */
