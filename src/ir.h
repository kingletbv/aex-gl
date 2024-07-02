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
#ifndef IR_H
#define IR_H

#ifdef __cplusplus
extern "C" {
#endif

struct ir_arg;
struct ir_body;
struct ir_block;
struct ir_instr;
struct ir_control_flow;

struct lob_sym;
struct source_gen;

enum ir_temp_type {
  /* Can be any register from a machine dependant register set */
  IR_REGISTER_SET,

  /* Specific single machine-dependant register */
  IR_REGISTER,

  /* Virtual Temporary, these are unlimited and what
   * front-ends typically use to pass data from instruction
   * to instruction. */
  IR_VIRTUAL,

  /* Register banks; these correspond to a single column of float,
   * int, bool, sampler2D or samplerCube  registers across multiple 
   * rows in a shader execution
   * The ordinal of the column assigned is stored in temp_value_.  */
  IR_FLOAT_BANK,
  IR_INT_BANK,
  IR_BOOL_BANK,
  IR_SAMPLER2D_BANK,
  IR_SAMPLERCUBE_BANK,

  /* Literal data values */
  IR_LITERAL,

  /* Entry point address of a block */
  IR_BLOCK_ENTRY,

  /* Reference to a symbol */
  IR_SYMBOL_REF
};

enum ir_temp_usage {
  /* Temporary value is used as input to the instruction but
   * not changed by it. */
  IR_USE,

  /* Temporary value is defined by the instruction (used as
   * output,) the value the temp held before this instruction
   * is discarded. */
  IR_DEF,

  /* Temporary value is both used and defined by this instruction,
   * the value the temp held before this instruction is used as
   * an input, and the temp is updated with a new value by this
   * instruction (the temporary is therefore clobbered.) */
  IR_USEDEF
};

/* An ir_temp is a temporary register, which then become subject 
 * for register allocation and assigned to an actual register. */
struct ir_temp {
  /* Kind of temporary register */
  enum ir_temp_type kind_;

  /* The ordinal of the temporary register assigned (if kind_ == IR_REGISTER, or IR_XXX_BANK) */
  int temp_value_;

  /* Pointer to list of arguments where this temporary is used */
  struct ir_arg *args_;

  /* Each body has a list of temps used by it, this is that list. */
  struct ir_body *body_;
  struct ir_temp *next_in_body_, *prev_in_body_;

  /* If kind_ == IR_LITERAL, this is that literal value */
  uint64_t literal_value_;

  /* Name of the symbol (if kind_ == IR_SYMBOL_REF). */
  char *sym_name_;

  /* Definition of sym_name_ as a symbol */
  struct lob_sym *sym_;

  /* If kind_ == IR_BLOCK_ENTRY, this is the block that the temp points to,
   * and additionally, the chain of other temps pointing to the same block */
  struct ir_block *entry_point_block_;
  struct ir_temp *next_entry_point_temp_, *prev_entry_point_temp_;

  /* Bitsize of the value represented by this temp
   * (for kind_ == IR_REGISTER, IR_VIRTUAL or IR_LITERAL */
  int bitsize_;

  /* The external ID of this temp; external in the sense of an "external key"
   * in database parlance, in that you may use this to externally identify a
   * temporary value. Valid for kind_ == IR_VIRTUAL */
  int external_id_;
};

/* An ir_arg is a reference to a temporary as an operand of an instruction */
struct ir_arg {
  /* The kind of usage (use, def or usedef) that this argument implies for the temp */
  enum ir_temp_usage usage_;

  /* The instruction this argument appears in; there is no chain, you'd have to search
   * the instruction linearly. */
  struct ir_instr *ins_;

  /* The temporary being referenced, and the chain of other arguments referencing
   * the same temp. */
  struct ir_temp *temp_;
  struct ir_arg *next_in_temp_, *prev_in_temp_;
};

/* An instance of an instruction inside a basic block */
struct ir_instr {
  /* Runtime field indicating index of this instruction inside
   * a block. Used to quickly determine relative order of instructions;
   * may become out of date. */
  int instr_index_;

  /* Pointer to the block containing this instruction */
  struct ir_block *block_;

  /* Next/prev chain for IR_Block::instructions */
  struct ir_instr *next_in_block_, *prev_in_block_;

  /* instruction code defining what instruction this represents, used
   * with the instruction registry to look up the actual instruction. */
  int instruction_code_;

  /* Number of arguments (and allocated size) of args field. */
  size_t num_args_;

  /* Arguments to this instruction. */
  struct ir_arg **args_;
};

/* Association object between basic blocks to model the flow graph */
struct ir_control_flow_edge {
  /* The basic blocks that are connected by this control flow edge */
  struct ir_block *from_, *to_;

  /* Chain of sibling control flow egdes that share the from block as a source */
  struct ir_control_flow_edge *next_in_from_, *prev_in_from_;
  
  /* Chain of sibling control flow edges that point to the same to block as target */
  struct ir_control_flow_edge *next_in_to_, *prev_in_to_;
};

/* Basic block inside the flow graph of a body */
struct ir_block {
  /* The body that this block is part of */
  struct ir_body *body_;
  struct ir_block *next_in_body_, *prev_in_body_;

  /* The entry point of this block as a temporary value */
  struct ir_temp *entry_point_temps_;

  /* The chain of instructions that make up this basic block */
  struct ir_instr *instructions_;

  /* Control flow edges going into this basic block */
  struct ir_control_flow_edge *predecessors_; /* chained via next_in_to_, prev_in_to_ */

  /* Control flow edges coming out of this basic block */
  struct ir_control_flow_edge *successors_; /* chained via next_in_from_, prev_in_from_ */

  /* Definition of the entrypoint to this basic block as a symbol */
  struct lob_symbol *sym_;

  /* Serial number to distinguish this block externally (e.g. generate label names etc.)
   * Valid numbers start from 1, 0 signifies "not initialized" */
  int serial_num_;
};

/* A set of basic blocks making up a flow graph, and temporaries used inside those
 * basic blocks. */
struct ir_body {
  /* Chain of all temporaries used by the instructions in the blocks of this body */
  struct ir_temp *temporaries_;

  /* Chain of all blocks that together make the flow graph. The body entry is the
   * first block in this chain. */
  struct ir_block *blocks_;

  /* If non-zero, construction failed at one point or another due to a failure
   * to allocate. This flag allows you to continue to construct and check for
   * failure once at the end. */
  int alloc_error_ : 1;
};

void ir_body_init(struct ir_body *body);
void ir_body_cleanup(struct ir_body *body);

struct ir_block *ir_body_alloc_block(struct ir_body *body);
struct ir_temp *ir_body_alloc_temp(struct ir_body *body);
struct ir_temp *ir_body_alloc_temp_register(struct ir_body *body, int reg);
struct ir_temp *ir_body_alloc_temp_virtual(struct ir_body *body, int temp_index);
struct ir_temp *ir_body_alloc_temp_banked_float(struct ir_body *body, int reg_index);
struct ir_temp *ir_body_alloc_temp_banked_int(struct ir_body *body, int reg_index);
struct ir_temp *ir_body_alloc_temp_banked_bool(struct ir_body *body, int reg_index);
struct ir_temp *ir_body_alloc_temp_banked_sampler2D(struct ir_body *body, int reg_index);
struct ir_temp *ir_body_alloc_temp_banked_samplerCube(struct ir_body *body, int reg_index);
struct ir_temp *ir_body_alloc_temp_lit(struct ir_body *body, uint64_t val);
struct ir_temp *ir_body_alloc_temp_block(struct ir_body *body, struct ir_block *blk);
struct ir_temp *ir_body_alloc_temp_symref(struct ir_body *body, char *sym_name);

void ir_block_init(struct ir_block *blk);
void ir_block_cleanup(struct ir_block *blk);
void ir_block_free(struct ir_block *blk);

struct ir_instr *ir_block_append_instr(struct ir_block *blk, int instruction_code);
struct ir_arg *ir_instr_append_use(struct ir_instr *ins, struct ir_temp *temp);
struct ir_arg *ir_instr_append_def(struct ir_instr *ins, struct ir_temp *temp);
struct ir_arg *ir_instr_append_usedef(struct ir_instr *ins, struct ir_temp *temp);

void ir_print_temp(struct source_gen *sg, struct ireg_registry *ireg, struct ir_temp *temp);
void ir_print_instr(struct source_gen *sg, struct ireg_registry *ireg, int mnemonic_space, struct ir_instr *instr);
void ir_print_block(struct source_gen *sg, struct ireg_registry *ireg, struct ir_block *blk);
void ir_print_body(struct source_gen *sg, struct ireg_registry *ireg, struct ir_body *body);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* IR_H */
