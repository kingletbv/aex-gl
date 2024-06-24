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

#ifndef STDLIB_H_INCLUDED
#define STDLIB_H_INCLUDED
#include <stdlib.h>
#endif

#ifndef STDINT_H_INCLUDED
#define STDINT_H_INCLUDED
#include <stdint.h>
#endif

#ifndef ASSERT_H_INCLUDED
#define ASSERT_H_INCLUDED
#include <assert.h>
#endif

#ifndef IR_REGISTRY_H_INCLUDED
#define IR_REGISTRY_H_INCLUDED
#include "ir_registry.h"
#endif

void ireg_init(struct ireg_registry *ireg) {
  ireg->num_instructions_ = ireg->num_instructions_allocated_ = 0;
  ireg->instructions_ = NULL;
  ireg->num_registers_ = ireg->num_registers_allocated_ = 0;
  ireg->registers_ = NULL;
}

void ireg_cleanup(struct ireg_registry *ireg) {
  /* we do not free the instructions or registers, as they are not owned by the registry */
  if (ireg->instructions_) free(ireg->instructions_);
  if (ireg->registers_) free(ireg->registers_);
}

/* Both instructions and registers are essentially arrays of pointers, so we can
 * use common functions to add or set them. */
static int ireg_add_element(void *element, 
                            size_t *num_elements, size_t *num_elements_allocated,
                            void ***elements) {
  if ((*num_elements) == INT_MAX) {
    return 0;
  }
  if ((*num_elements) == (*num_elements_allocated)) {
    size_t new_num_allocated = *num_elements_allocated;
    if (new_num_allocated) {
      new_num_allocated += new_num_allocated;
      if (new_num_allocated < (*num_elements_allocated)) {
        /* overflow */
        return 0;
      }
    }
    else {
      new_num_allocated = 16;
    }
    if (((~(size_t)0) / sizeof(void *)) < new_num_allocated) {
      /* Would overflow if multiplied to byte size */
      return 0;
    }
    void *new_buf = (void ***)realloc(*elements, sizeof(void *) * new_num_allocated);
    if (!new_buf) {
      return 0;
    }
    *elements = new_buf;
    *num_elements_allocated = new_num_allocated;
  }
  if (!*num_elements) {
    /* Skip element 0 -- minimum allocated is 16 here so it's safe to do this */
    (*elements)[0] = NULL;
    (*num_elements)++;
  }
  ((*elements)[*num_elements]) = element;
  int instr_index = (int)(*num_elements);
  (*num_elements)++;
  return instr_index;
}

static int ireg_set_element(int element_index, void *element, 
                            size_t *num_elements, size_t *num_elements_allocated,
                            void ***elements) {
  if (element_index < 1) {
    /* Zero or less not permitted */
    return 0;
  }
  size_t index = (size_t)element_index;
  if (index < (*num_elements)) {
    /* Overwrite existing element */
    (*elements)[index] = element;
    return element_index;
  }
  if (index < (*num_elements_allocated)) {
    size_t n;
    for (n = (*num_elements); n < index; ++n) {
      (*elements)[n] = NULL;
    }
    (*elements)[index] = element;
    (*num_elements) = index + 1;
    return element_index;
  }
  if (((~(size_t)0) / sizeof(void *)) < index) {
    /* Cannot support the memory for an index this large */
    return 0;
  }
  /* Does not fit in allocated range; find next power of two for the
   * allocated range that *will* fit. This is cumbersome but ensures
   * that a for loop that is 'setting' elements at incremental 
   * indices will grow/reallocate its buffers exponentially. */
  uint64_t cheat; /* cheat a little bit, size_t will always fit in uint64 */
  assert(sizeof(uint64_t) >= sizeof(size_t));
  int msb = 0;
  cheat = (uint64_t)index;
  if (cheat & 0xFFFFFFFF00000000ULL) {
    msb += 32;
    cheat >>= 32;
  }
  if (cheat & 0xFFFF0000ULL) {
    msb += 16;
    cheat >>= 16;
  }
  if (cheat & 0xFF00ULL) {
    msb += 8;
    cheat >>= 8;
  }
  if (cheat & 0xF0ULL) {
    msb += 4;
    cheat >>= 4;
  }
  if (cheat & 0xCULL) {
    msb += 2;
    cheat >>= 2;
  }
  if (cheat & 0x2ULL) {
    msb += 1;
    cheat >>= 1;
  }
  /* Allocate to the next greater power of 2 */
  size_t new_num_allocated;
  if (msb < 63) {
    new_num_allocated = ((size_t)1) << (msb + 1);
  }
  else {
    /* Unlikely to succeed but we'll let it fail below */
    new_num_allocated = SIZE_MAX;
  }

  if (((~(size_t)0) / sizeof(void *)) < new_num_allocated) {
    /* Would overflow if multiplied to byte size */
    return 0;
  }

  void *new_buf = realloc((*elements), sizeof(void *) * new_num_allocated);
  if (!new_buf) {
    return 0;
  }
  *elements = new_buf;
  *num_elements_allocated = new_num_allocated;
  size_t n;
  for (n = (*num_elements); n < index; ++n) {
    (*elements)[n] = NULL;
  }
  (*elements)[index] = element;
  (*num_elements) = index + 1;
  return element_index;
}


int ireg_add_instruction(struct ireg_registry *ireg, struct ireg_instr *instr) {
  int instr_index;
  instr_index = ireg_add_element(instr, 
                                 &ireg->num_instructions_, 
                                 &ireg->num_instructions_allocated_, 
                                 &ireg->instructions_);
  if (!instr_index) return 0;
  instr->instr_index_ = instr_index;
  return instr_index;
}

int ireg_set_instruction(struct ireg_registry *ireg, struct ireg_instr *instr) {
  int instr_index;
  instr_index = ireg_set_element(instr->instr_index_, instr,
                                 &ireg->num_instructions_,
                                 &ireg->num_instructions_allocated_,
                                 &ireg->instructions_);
  return instr_index;
}

int ireg_add_register(struct ireg_registry *ireg, struct ireg_register *reg) {
  int reg_index;
  reg_index = ireg_add_element(reg,
                               &ireg->num_registers_,
                               &ireg->num_registers_allocated_,
                               &ireg->registers_);
  if (!reg_index) return 0;
  reg->register_index_ = reg_index;
  return reg_index;
}

int ireg_set_register(struct ireg_registry *ireg, struct ireg_register *reg) {
  int reg_index;
  reg_index = ireg_set_element(reg->register_index_, reg,
                               &ireg->num_registers_,
                               &ireg->num_registers_allocated_,
                               &ireg->registers_);
  return reg_index;
}

