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

#ifndef STRING_H_INCLUDED
#define STRING_H_INCLUDED
#include <string.h>
#endif

#ifndef INTTYPES_H_INCLUDED
#define INTTYPES_H_INCLUDED
#include <inttypes.h>
#endif

#ifndef IR_H_INCLUDED
#define IR_H_INCLUDED
#include "ir.h"
#endif

#ifndef SOURCE_GEN_H_INCLUDED
#define SOURCE_GEN_H_INCLUDED
#include "source_gen.h"
#endif

#ifndef IR_REGISTRY_H_INCLUDED
#define IR_REGISTRY_H_INCLUDED
#include "ir_registry.h"
#endif

static void ir_temp_init(struct ir_temp *temp);
static void ir_temp_cleanup(struct ir_temp *temp);
static struct ir_temp *ir_temp_alloc();
static void ir_temp_free(struct ir_temp *temp);
static void ir_temp_attach_to_body(struct ir_temp *temp, struct ir_body *body);
static void ir_temp_detach_from_body(struct ir_temp *temp);
static void ir_temp_attach_entry_point_block(struct ir_temp *temp, struct ir_block *block);
static void ir_temp_detach_entry_point_block(struct ir_temp *temp);

static void ir_arg_init(struct ir_arg *arg);
static void ir_arg_cleanup(struct ir_arg *arg);
static struct ir_arg *ir_arg_alloc();
static void ir_arg_free(struct ir_arg *arg);
static void ir_arg_attach_to_temp(struct ir_arg *arg, struct ir_temp *temp, int as_use, int as_def);
static void ir_arg_detach_from_temp(struct ir_arg *arg);
static int ir_arg_append_to_instruction(struct ir_arg *arg, struct ir_instr *instr);
static void ir_arg_detach_from_instruction(struct ir_arg *arg);

static void ir_instr_init(struct ir_instr *instr);
static void ir_instr_cleanup(struct ir_instr *instr);
static struct ir_instr *ir_instr_alloc();
static void ir_instr_free(struct ir_instr *instr);
static void ir_instr_append_to_block(struct ir_instr *instr, struct ir_block *block);
static void ir_instr_detach_from_block(struct ir_instr *instr);

static void ir_control_flow_edge_init(struct ir_control_flow_edge *edge);
static void ir_control_flow_edge_cleanup(struct ir_control_flow_edge *edge);
static struct ir_control_flow_edge *ir_control_flow_edge_alloc();
static void ir_control_flow_edge_free(struct ir_control_flow_edge *edge);
static void ir_control_flow_edge_attach_to_block(struct ir_control_flow_edge *edge, struct ir_block *block);
static void ir_control_flow_edge_detach_to_block(struct ir_control_flow_edge *edge);
static void ir_control_flow_edge_attach_from_block(struct ir_control_flow_edge *edge, struct ir_block *block);
static void ir_control_flow_edge_detach_from_block(struct ir_control_flow_edge *edge);

static void ir_block_detach_from_body(struct ir_block *blk) {
  struct ir_body *body = blk->body_;
  if (!body) return;

  blk->next_in_body_->prev_in_body_ = blk->prev_in_body_;
  blk->prev_in_body_->next_in_body_ = blk->next_in_body_;
  if (body->blocks_ == blk) {
    if (blk->next_in_body_ == blk) {
      body->blocks_ = NULL;
    }
    else {
      body->blocks_ = blk->next_in_body_;
    }
  }

  blk->body_ = NULL;
  blk->next_in_body_ = blk->prev_in_body_ = NULL;
  blk->serial_num_ = 0;
}

static void ir_block_attach_to_body(struct ir_block *blk, struct ir_body *body) {
  if (blk->body_) {
    ir_block_detach_from_body(blk);
  }
  blk->body_ = body;
  if (body->blocks_) {
    blk->next_in_body_ = body->blocks_;
    blk->prev_in_body_ = body->blocks_->prev_in_body_;
    blk->next_in_body_->prev_in_body_ = blk;
    blk->prev_in_body_->next_in_body_ = blk;
    blk->serial_num_ = blk->prev_in_body_->serial_num_ + 1;
  }
  else {
    blk->next_in_body_ = blk->prev_in_body_ = blk;
    body->blocks_ = blk;
    blk->serial_num_ = 1;
  }
}

static void ir_temp_init(struct ir_temp *temp) {
  temp->kind_ = IR_VIRTUAL;
  temp->temp_value_ = 0;
  temp->args_ = NULL;
  temp->body_ = NULL;
  temp->next_in_body_ = temp->prev_in_body_ = NULL;
  temp->literal_value_ = 0;
  temp->sym_name_ = NULL;
  temp->sym_ = NULL;
  temp->entry_point_block_ = NULL;
  temp->next_entry_point_temp_ = temp->prev_entry_point_temp_ = NULL;
  temp->bitsize_ = 0;
  temp->external_id_ = 0;
}

static void ir_temp_cleanup(struct ir_temp *temp) {
  ir_temp_detach_from_body(temp);
  ir_temp_detach_entry_point_block(temp);
  while (temp->args_) {
    ir_arg_free(temp->args_);
  }
  if (temp->sym_name_) {
    free(temp->sym_name_);
  }
}

static struct ir_temp *ir_temp_alloc() {
  struct ir_temp *temp = (struct ir_temp *)malloc(sizeof(struct ir_temp));
  if (temp) {
    ir_temp_init(temp);
  }
  return temp;
}

static void ir_temp_free(struct ir_temp *temp) {
  if (!temp) return;
  ir_temp_cleanup(temp);
  free(temp);
}

static void ir_temp_attach_to_body(struct ir_temp *temp, struct ir_body *body) {
  ir_temp_detach_from_body(temp);
  temp->body_ = body;
  if (body->temporaries_) {
    temp->next_in_body_ = body->temporaries_;
    temp->prev_in_body_ = body->temporaries_->prev_in_body_;
    temp->next_in_body_->prev_in_body_ = temp;
    temp->prev_in_body_->next_in_body_ = temp;
  }
  else {
    temp->next_in_body_ = temp->prev_in_body_ = temp;
    body->temporaries_ = temp;
  }
}

static void ir_temp_detach_from_body(struct ir_temp *temp) {
  struct ir_body *body = temp->body_;
  if (!body) return;

  temp->next_in_body_->prev_in_body_ = temp->prev_in_body_;
  temp->prev_in_body_->next_in_body_ = temp->next_in_body_;
  if (body->temporaries_ == temp) {
    if (temp->next_in_body_ == temp) {
      body->temporaries_ = NULL;
    }
    else {
      body->temporaries_ = temp->next_in_body_;
    }
  }

  temp->body_ = NULL;
  temp->next_in_body_ = temp->prev_in_body_ = NULL;
}

static void ir_temp_attach_entry_point_block(struct ir_temp *temp, struct ir_block *block) {
  ir_temp_detach_entry_point_block(temp);

  temp->entry_point_block_ = block;
  if (block->entry_point_temps_) {
    temp->next_entry_point_temp_ = block->entry_point_temps_;
    temp->prev_entry_point_temp_ = block->entry_point_temps_->prev_entry_point_temp_;
    temp->next_entry_point_temp_->prev_entry_point_temp_ = temp;
    temp->prev_entry_point_temp_->next_entry_point_temp_ = temp;
  }
  else {
    temp->next_entry_point_temp_ = temp->prev_entry_point_temp_ = temp;
    block->entry_point_temps_ = temp;
  }
}

static void ir_temp_detach_entry_point_block(struct ir_temp *temp) {
  struct ir_block *block = temp->entry_point_block_;
  if (!block) return;

  temp->next_entry_point_temp_->prev_entry_point_temp_ = temp->prev_entry_point_temp_;
  temp->prev_entry_point_temp_->next_entry_point_temp_ = temp->next_entry_point_temp_;
  if (block->entry_point_temps_ == temp) {
    if (temp->next_entry_point_temp_ == temp) {
      block->entry_point_temps_ = NULL;
    }
    else {
      block->entry_point_temps_ = temp->next_entry_point_temp_;
    }
  }

  temp->entry_point_block_ = NULL;
  temp->next_entry_point_temp_ = temp->prev_entry_point_temp_ = NULL;
}

static void ir_arg_init(struct ir_arg *arg) {
  arg->usage_ = IR_USE;
  arg->ins_ = NULL;
  arg->temp_ = NULL;
  arg->next_in_temp_ = arg->prev_in_temp_ = NULL;

}

static void ir_arg_cleanup(struct ir_arg *arg) {
  ir_arg_detach_from_instruction(arg);
  ir_arg_detach_from_temp(arg);
}

static struct ir_arg *ir_arg_alloc() {
  struct ir_arg *arg = (struct ir_arg *)malloc(sizeof(struct ir_arg));
  if (arg) {
    ir_arg_init(arg);
  }
  return arg;
}

static void ir_arg_free(struct ir_arg *arg) {
  if (!arg) return;
  ir_arg_cleanup(arg);
  free(arg);

}

static void ir_arg_attach_to_temp(struct ir_arg *arg, struct ir_temp *temp, int as_use, int as_def) {
  ir_arg_detach_from_temp(arg);
  arg->temp_ = temp;
  if (!temp) return;
  if (temp->args_) {
    arg->next_in_temp_ = temp->args_;
    arg->prev_in_temp_ = temp->args_->prev_in_temp_;
    arg->next_in_temp_->prev_in_temp_ = arg;
    arg->prev_in_temp_->next_in_temp_ = arg;
  }
  else {
    arg->next_in_temp_ = arg->prev_in_temp_ = arg;
    temp->args_ = arg;
  }
  if (as_use && !as_def) {
    arg->usage_ = IR_USE;
  }
  else if (as_def && !as_use) {
    arg->usage_ = IR_DEF;
  }
  else /* (as_use && as_def) */ {
    arg->usage_ = IR_USEDEF;
  }
}

static void ir_arg_detach_from_temp(struct ir_arg *arg) {
  struct ir_temp *temp = arg->temp_;
  if (!temp) return;

  arg->next_in_temp_->prev_in_temp_ = arg->prev_in_temp_;
  arg->prev_in_temp_->next_in_temp_ = arg->next_in_temp_;
  if (temp->args_ == arg) {
    if (arg->next_in_temp_ == arg) {
      temp->args_ = NULL;
    }
    else {
      temp->args_ = arg->next_in_temp_;
    }
  }

  arg->temp_ = NULL;
  arg->next_in_temp_ = arg->prev_in_temp_ = NULL;
}

static int ir_arg_append_to_instruction(struct ir_arg *arg, struct ir_instr *instr) {
  ir_arg_detach_from_instruction(arg);
  void *new_array = realloc(instr->args_, (instr->num_args_ + 1) * sizeof(struct ir_arg *));
  if (!new_array) {
    /* XXX: error */
    return -1;
  }
  instr->args_ = (struct ir_arg **)new_array;
  instr->args_[instr->num_args_++] = arg;
  arg->ins_ = instr;
  return 0;
}

static void ir_arg_detach_from_instruction(struct ir_arg *arg) {
  struct ir_instr *instr = arg->ins_;
  if (!instr) return;

  /* We don't change the number of arguments (as the interpretation
   * of an argument likely depends on its position in the argument list),
   * but we leave a hole, setting it to NULL. */
  int n;
  for (n = 0; n < instr->num_args_; ++n) {
    if (instr->args_[n] == arg) {
      arg->ins_->args_[n] = NULL;
      break;
    }
  }

  arg->ins_ = NULL;
}

static void ir_instr_init(struct ir_instr *instr) {
  instr->instr_index_ = 0;
  instr->block_ = NULL;
  instr->next_in_block_ = instr->prev_in_block_ = NULL;
  instr->instruction_code_ = 0;
  instr->num_args_ = 0;
  instr->args_ = NULL;
}

static void ir_instr_cleanup(struct ir_instr *instr) {
  ir_instr_detach_from_block(instr);
  size_t n;
  for (n = 0; n < instr->num_args_; ++n) {
    ir_arg_free(instr->args_[n]);
  }
  if (instr->args_) {
    free(instr->args_);
  }
}

static struct ir_instr *ir_instr_alloc() {
  struct ir_instr *instr = (struct ir_instr *)malloc(sizeof(struct ir_instr));
  if (instr) {
    ir_instr_init(instr);
  }
  return instr;
}

static void ir_instr_free(struct ir_instr *instr) {
  if (!instr) return;
  ir_instr_cleanup(instr);
  free(instr);
}

static void ir_instr_append_to_block(struct ir_instr *instr, struct ir_block *block) {
  ir_instr_detach_from_block(instr);
  instr->block_ = block;
  if (block->instructions_) {
    instr->next_in_block_ = block->instructions_;
    instr->prev_in_block_ = block->instructions_->prev_in_block_;
    instr->next_in_block_->prev_in_block_ = instr;
    instr->prev_in_block_->next_in_block_ = instr;
  }
  else {
    instr->next_in_block_ = instr->prev_in_block_ = instr;
    block->instructions_ = instr;
  }
}

static void ir_instr_detach_from_block(struct ir_instr *instr) {
  struct ir_block *block = instr->block_;
  if (!block) return;

  instr->next_in_block_->prev_in_block_ = instr->prev_in_block_;
  instr->prev_in_block_->next_in_block_ = instr->next_in_block_;
  if (block->instructions_ == instr) {
    if (instr->next_in_block_ == instr) {
      block->instructions_ = NULL;
    }
    else {
      block->instructions_ = instr->next_in_block_;
    }
  }

  instr->block_ = NULL;
  instr->next_in_block_ = instr->prev_in_block_ = NULL;
}

static void ir_control_flow_edge_init(struct ir_control_flow_edge *edge) {
  edge->from_ = edge->to_ = NULL;
  edge->next_in_from_ = edge->prev_in_from_ = NULL;
  edge->next_in_to_ = edge->prev_in_to_ = NULL;

}

static void ir_control_flow_edge_cleanup(struct ir_control_flow_edge *edge) {
  ir_control_flow_edge_detach_to_block(edge);
  ir_control_flow_edge_detach_from_block(edge);
}

static struct ir_control_flow_edge *ir_control_flow_edge_alloc() {
  struct ir_control_flow_edge *edge = (struct ir_control_flow_edge *)malloc(sizeof(struct ir_control_flow_edge));
  if (edge) {
    ir_control_flow_edge_init(edge);
  }
  return edge;
}

static void ir_control_flow_edge_free(struct ir_control_flow_edge *edge) {
  if (!edge) return;
  ir_control_flow_edge_cleanup(edge);
  free(edge);
}

static void ir_control_flow_edge_attach_to_block(struct ir_control_flow_edge *edge, struct ir_block *block) {
  ir_control_flow_edge_detach_to_block(edge);
  edge->to_ = block;
  if (block->predecessors_) {
    edge->next_in_to_ = block->predecessors_;
    edge->prev_in_to_ = block->predecessors_->prev_in_to_;
    edge->next_in_to_->prev_in_to_ = edge;
    edge->prev_in_to_->next_in_to_ = edge;
  }
  else {
    edge->next_in_to_ = edge->prev_in_to_ = edge;
    block->predecessors_ = edge;
  }
}

static void ir_control_flow_edge_detach_to_block(struct ir_control_flow_edge *edge) {
  struct ir_block *block = edge->to_;
  if (!block) return;

  edge->next_in_to_->prev_in_to_ = edge->prev_in_to_;
  edge->prev_in_to_->next_in_to_ = edge->next_in_to_;
  if (block->predecessors_ == edge) {
    if (edge->next_in_to_ == edge) {
      block->predecessors_ = NULL;
    }
    else {
      block->predecessors_ = edge->next_in_to_;
    }
  }

  edge->to_ = NULL;
  edge->next_in_to_ = edge->prev_in_to_ = NULL;
}

static void ir_control_flow_edge_attach_from_block(struct ir_control_flow_edge *edge, struct ir_block *block) {
  ir_control_flow_edge_detach_from_block(edge);
  edge->from_ = block;
  if (block->successors_) {
    edge->next_in_from_ = block->successors_;
    edge->prev_in_from_ = block->successors_->prev_in_from_;
    edge->next_in_from_->prev_in_from_ = edge;
    edge->prev_in_from_->next_in_from_ = edge;
  }
  else {
    edge->next_in_from_ = edge->prev_in_from_ = edge;
    block->successors_ = edge;
  }
}

static void ir_control_flow_edge_detach_from_block(struct ir_control_flow_edge *edge) {
  struct ir_block *block = edge->from_;
  if (!block) return;

  edge->next_in_from_->prev_in_from_ = edge->prev_in_from_;
  edge->prev_in_from_->next_in_from_ = edge->next_in_from_;
  if (block->successors_ == edge) {
    if (edge->next_in_from_ == edge) {
      block->successors_ = NULL;
    }
    else {
      block->successors_ = edge->next_in_from_;
    }
  }

  edge->from_ = NULL;
  edge->next_in_from_ = edge->prev_in_from_ = NULL;
}


void ir_body_init(struct ir_body *body) {
  body->temporaries_ = NULL;
  body->blocks_ = NULL;
  body->alloc_error_ = 0;
}

void ir_body_cleanup(struct ir_body *body) {
  while (body->temporaries_) {
    ir_temp_free(body->temporaries_);
  }
  while (body->blocks_) {
    ir_block_free(body->blocks_);
  }
}

void ir_block_init(struct ir_block *blk) {
  blk->body_ = NULL;
  blk->next_in_body_ = blk->prev_in_body_ = NULL;
  blk->entry_point_temps_ = NULL;
  blk->instructions_ = NULL;
  blk->predecessors_ = NULL;
  blk->successors_ = NULL;
  blk->sym_ = NULL;
  blk->serial_num_ = 0;
}

void ir_block_cleanup(struct ir_block *blk) {
  ir_block_detach_from_body(blk);

  while (blk->entry_point_temps_) {
    ir_temp_free(blk->entry_point_temps_);
  }
  while (blk->instructions_) {
    ir_instr_free(blk->instructions_);
  }
  while (blk->predecessors_) {
    ir_control_flow_edge_free(blk->predecessors_);
  }
  while (blk->successors_) {
    ir_control_flow_edge_free(blk->successors_);
  }
}

static struct ir_block *ir_block_alloc() {
  struct ir_block *blk = (struct ir_block *)malloc(sizeof(struct ir_block));
  if (blk) {
    ir_block_init(blk);
  }
  return blk;
}

void ir_block_free(struct ir_block *blk) {
  if (!blk) return;
  ir_block_cleanup(blk);
  free(blk);
}


struct ir_block *ir_body_alloc_block(struct ir_body *body) {
  struct ir_block *blk = ir_block_alloc();
  if (blk) {
    ir_block_attach_to_body(blk, body);
  }
  else {
    body->alloc_error_ = 1;
  }
  return blk;
}

struct ir_temp *ir_body_alloc_temp(struct ir_body *body) {
  struct ir_temp *t = ir_temp_alloc();
  if (t) {
    ir_temp_attach_to_body(t, body);
  }
  else {
    body->alloc_error_ = 1;
  }
  return t;
}

struct ir_temp *ir_body_alloc_temp_register(struct ir_body *body, int reg) {
  struct ir_temp *t = ir_body_alloc_temp(body);
  if (!t) return NULL;

  t->kind_ = IR_REGISTER;
  t->temp_value_ = reg;

  return t;
}

struct ir_temp *ir_body_alloc_temp_virtual(struct ir_body *body, int temp_index) {
  struct ir_temp *t = ir_body_alloc_temp(body);
  if (!t) return NULL;

  t->kind_ = IR_VIRTUAL;
  t->external_id_ = temp_index;

  return t;
}

struct ir_temp *ir_body_alloc_temp_lit(struct ir_body *body, uint64_t val) {
  struct ir_temp *t = ir_body_alloc_temp(body);
  if (!t) return NULL;

  t->kind_ = IR_LITERAL;
  t->literal_value_ = val;

  return t;
}

struct ir_temp *ir_body_alloc_temp_block(struct ir_body *body, struct ir_block *blk) {
  struct ir_temp *t = ir_body_alloc_temp(body);
  if (!t) return NULL;

  t->kind_ = IR_BLOCK_ENTRY;
  ir_temp_attach_entry_point_block(t, blk);

  return t;
}

struct ir_temp *ir_body_alloc_temp_symref(struct ir_body *body, char *sym_name) {
  char *s = strdup(sym_name);
  if (!s) {
    body->alloc_error_ = 1;
    return NULL;
  }
  struct ir_temp *t = ir_body_alloc_temp(body);
  if (!t) {
    free(s);
    return NULL;
  }

  t->kind_ = IR_SYMBOL_REF;
  t->sym_name_ = s;

  return t;
}

void ir_print_temp(struct source_gen *sg, struct ireg_registry *ireg, struct ir_temp *temp) {
  switch (temp->kind_) {
    case IR_REGISTER_SET:
      sg_printf(sg, "rr%d", temp->external_id_);
      break;
    case IR_REGISTER: {
      struct ireg_register *reg = NULL;
      if (temp->temp_value_ < ireg->num_registers_) {
        reg = ireg->registers_[temp->temp_value_];
      }
      if (!reg) {
        sg_printf(sg, "<illegal register>");
      }
      else {
        sg_printf(sg, "%s", reg->name_);
      }
      break;
    }
    case IR_VIRTUAL:
      sg_printf(sg, "t%d", temp->external_id_);
      break;
    case IR_LITERAL:
      sg_printf(sg, "0x%"PRIX64"", temp->literal_value_);
      break;
    case IR_BLOCK_ENTRY:
      sg_printf(sg, "L%d", temp->entry_point_block_ ? temp->entry_point_block_->serial_num_ : -1);
      break;
    case IR_SYMBOL_REF:
      sg_printf(sg, "%s", temp->sym_name_);
      break;
    default:
      sg_printf(sg, "<illegal temp>");
      break;
  }
}

void ir_print_instr(struct source_gen *sg, struct ireg_registry *ireg, int mnemonic_space, struct ir_instr *instr) {
  struct ireg_instr *ireg_instr = NULL;
  if (instr->instruction_code_ < ireg->num_instructions_) {
    ireg_instr = ireg->instructions_[instr->instruction_code_];
  }
  const char *mnemonic = ireg_instr ? ireg_instr->mnemonic_ : "???";

  if (mnemonic_space) {
    sg_printf(sg, "%-*s", (int)mnemonic_space + 1, mnemonic);
  }
  else {
    sg_printf(sg, "%s ", mnemonic);
  }

  /* Scan the format string for arguments and print as we go along */
  const char *p;
  p = ireg_instr ? ireg_instr->format_ : "";
  while (*p) {
    if ((p[0] == '%') && (p[1] >= '0') && (p[1] <= '9')){
      int arg_num = p[1] - '0';
      if ((p[2] >= '0') && (p[2] <= '9')) {
        /* Double-digit argument placeholder */
        arg_num = arg_num * 10 + (p[2] - '0');
        p += 3;
      }
      else {
        p += 2;
      }
      struct ireg_operand *opd = NULL;
      struct ir_arg *arg = NULL;
      if (arg_num < ireg_instr->num_operands_) {
        opd = ireg_instr->operands_ + arg_num;
      }
      if (arg_num < instr->num_args_) {
        arg = instr->args_[arg_num];
      }

      if (!arg) {
        sg_printf(sg, "<null arg>");
      }
      else if (!arg->temp_) {
        sg_printf(sg, "<null temp>");
      }
      else {
        ir_print_temp(sg, ireg, arg->temp_);
      }
    }
  }
}

void ir_print_block(struct source_gen *sg, struct ireg_registry *ireg, struct ir_block *blk) {
  struct ir_instr *instr = blk->instructions_;
  size_t max_mnemonic_len = 0;
  if (instr) {
    do {
      struct ireg_instr *ireg_instr = NULL;
      if (instr->instruction_code_ < ireg->num_instructions_) {
        ireg_instr = ireg->instructions_[instr->instruction_code_];
      }

      size_t len = strlen(ireg_instr ? ireg_instr->mnemonic_ : "???");
      if (len > max_mnemonic_len) {
        max_mnemonic_len = len;
      }

      instr = instr->next_in_block_;
    } while (instr != blk->instructions_);
  }

  sg_printf(sg, "L%s:\n", blk->serial_num_);
  sg->indent_ += 2;

  if (instr) {
    do {
      ir_print_instr(sg, ireg, (int)max_mnemonic_len, instr);

      instr = instr->next_in_block_;
    } while (instr != blk->instructions_);
  }
  sg->indent_ -= 2;
}

void ir_print_body(struct source_gen *sg, struct ireg_registry *ireg, struct ir_body *body) {
  struct ir_block *blk = body->blocks_;
  if (blk) {
    do {
      ir_print_block(sg, ireg, blk);

      blk = blk->next_in_body_;
    } while (blk != body->blocks_);
  }
}

struct ir_instr *ir_block_append_instr(struct ir_block *blk, int instruction_code) {
  struct ir_instr *ins = ir_instr_alloc();
  if (!ins) return NULL;
  ins->instruction_code_ = instruction_code;
  ir_instr_append_to_block(ins, blk);
  return ins;
}

struct ir_arg *ir_instr_append_use(struct ir_instr *ins, struct ir_temp *temp) {
  struct ir_arg *a = ir_arg_alloc();
  if (!a) {
    if (ins->block_ && ins->block_->body_) {
      ins->block_->body_->alloc_error_ = 1;
    }
    return NULL;
  }
  ir_arg_attach_to_temp(a, temp, 1, 0);
  return a;
}

struct ir_arg *ir_instr_append_def(struct ir_instr *ins, struct ir_temp *temp) {
  struct ir_arg *a = ir_arg_alloc();
  if (!a) {
    if (ins->block_ && ins->block_->body_) {
      ins->block_->body_->alloc_error_ = 1;
    }
    return NULL;
  }
  ir_arg_attach_to_temp(a, temp, 0, 1);
  return a;
}

struct ir_arg *ir_instr_append_usedef(struct ir_instr *ins, struct ir_temp *temp) {
  if (!ins) return NULL;
  struct ir_arg *a = ir_arg_alloc();
  if (!a) {
    if (ins->block_ && ins->block_->body_) {
      ins->block_->body_->alloc_error_ = 1;
    }
    return NULL;
  }
  ir_arg_attach_to_temp(a, temp, 1, 1);
  return a;
}


