

void sl_reg_move(struct sl_execution *exec,
                 uint8_t row,
                 struct sl_reg_alloc *from_ra, struct sl_reg_alloc *from_ra_offset,
                 struct sl_reg_alloc *to_ra, struct sl_reg_alloc *to_ra_offset,
                 int from_offset_step_size, int to_offset_step_size,
                 int array_quantity);
