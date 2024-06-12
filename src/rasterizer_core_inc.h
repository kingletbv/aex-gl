

for (py = top; py < bottom; py += 2) {
  // Copy row values to be ready for column increments
  Dp01_TL = Dp01_row_TL;
  Dp12_TL = Dp12_row_TL;
  Dp20_TL = Dp20_row_TL;
  Dp01_TR = Dp01_row_TR;
  Dp12_TR = Dp12_row_TR;
  Dp20_TR = Dp20_row_TR;
  Dp01_BL = Dp01_row_BL;
  Dp12_BL = Dp12_row_BL;
  Dp20_BL = Dp20_row_BL;
  Dp01_BR = Dp01_row_BR;
  Dp12_BR = Dp12_row_BR;
  Dp20_BR = Dp20_row_BR;
  Dp01_row_TL += Dp01_dy_2;
  Dp12_row_TL += Dp12_dy_2;
  Dp20_row_TL += Dp20_dy_2;
  Dp01_row_TR += Dp01_dy_2;
  Dp12_row_TR += Dp12_dy_2;
  Dp20_row_TR += Dp20_dy_2;
  Dp01_row_BL += Dp01_dy_2;
  Dp12_row_BL += Dp12_dy_2;
  Dp20_row_BL += Dp20_dy_2;
  Dp01_row_BR += Dp01_dy_2;
  Dp12_row_BR += Dp12_dy_2;
  Dp20_row_BR += Dp20_dy_2;

  if (direction_xy_flips) {
    /* Flip numerator to column (x) direction */
    z_sx_TL = D012 - z_s_TL - 1;
    z_sx_TR = D012 - z_s_TR - 1;
    z_sx_BL = D012 - z_s_BL - 1;
    z_sx_BR = D012 - z_s_BR - 1;
  }
  else {
    /* x and y are both ascending, or both descending, keep as-is */
    z_sx_TL = z_s_TL;
    z_sx_TR = z_s_TR;
    z_sx_BL = z_s_BL;
    z_sx_BR = z_s_BR;
  }

  z_x_TL = z_TL;
  z_x_TR = z_TR;
  z_x_BL = z_BL;
  z_x_BR = z_BR;

  /* Step z_s to next row */
  z_TL += z_yq;
  z_s_TL -= z_yp;
  step_mask = z_s_TL >> 63;
  z_s_TL += D012 & step_mask;
  z_TL += z_yi & step_mask;

  z_TR += z_yq;
  z_s_TR -= z_yp;
  step_mask = z_s_TR >> 63;
  z_s_TR += D012 & step_mask;
  z_TR += z_yi & step_mask;

  z_BL += z_yq;
  z_s_BL -= z_yp;
  step_mask = z_s_BL >> 63;
  z_s_BL += D012 & step_mask;
  z_BL += z_yi & step_mask;

  z_BR += z_yq;
  z_s_BR -= z_yp;
  step_mask = z_s_BR >> 63;
  z_s_BR += D012 & step_mask;
  z_BR += z_yi & step_mask;

  // (py - scissor_top)
  // negative when top row is above top of scissor window.
  scissor_top_row_mask = ~((py - scissor_top) >> 63);

  // ((py + 1) - scissor_bottom)
  // positive when bottom row of fragment quadruples are below 
  // bottom of scissor window, negative otherwise
  scissor_bottom_row_mask = ((py + 1) - scissor_bottom) >> 63;

  // (left - scissor_left)
  // negative when leftmost column is left of scissor window
  scissor_left_column_mask = ~((left - scissor_left) >> 63);

  for (px = left; px < right; px += 2) {
    // Compute the masks for each determinant at each of the four pixels
    // The idea is that if the determinant is positive, then the pixel is
    // inside the triangle because all barycentric sub-triangles will be clockwise;
    // consequently, if the MSB sign bit is zero, then the mask should be all
    // 1's.
    // Because the test is >= 0, we can OR together the sign bits and create
    // a unified mask (if any sign bit is set, then the mask is all zeroes,
    // otherwise it is all ones.)
    // Now also consider the counter-clockwise case, then, for the point to
    // be inside the triangle, all the barycentric sub-triangles have to be
    // counterclockwise (negative).
    // So there are two cases: either all positive, or all negative. In the
    // case of a clockwise triangle, it is impossible for all barycentric
    // sub-triangles to be negative (because there is no area outside all
    // three edges.) Similarly, for the counterclockwise triangle, the only
    // way we can be on the same side of all three edges (in this case the
    // negative side) is if we're on the triangle.
    // So, in summary, we only need to check if we are on the same side of
    // all three edges. Irrespective of the actual side that the edges agree
    // on, we'll be inside the triangle.
    TL_Mask = ((Dp01_TL & Dp12_TL & Dp20_TL) | ~(Dp01_TL | Dp12_TL | Dp20_TL)) >> 63;
    TR_Mask = ((Dp01_TR & Dp12_TR & Dp20_TR) | ~(Dp01_TR | Dp12_TR | Dp20_TR)) >> 63;
    BL_Mask = ((Dp01_BL & Dp12_BL & Dp20_BL) | ~(Dp01_BL | Dp12_BL | Dp20_BL)) >> 63;
    BR_Mask = ((Dp01_BR & Dp12_BR & Dp20_BR) | ~(Dp01_BR | Dp12_BR | Dp20_BR)) >> 63;

  //  counterclockwise_edge_area_mask_inverter

    // ((px + 1) - scissor_right)
    // positive when the right side column of fragment quadruples are to 
    // the right of the scissor window, negative otherwise.
    scissor_right_column_mask = ((px + 1) - scissor_right) >> 63;

    TL_Mask = TL_Mask & scissor_top_row_mask    & scissor_left_column_mask  & RASTERIZER_EARLY_Z_CHECK(zbuf_TL, z_x_TL);
    TR_Mask = TR_Mask & scissor_top_row_mask    & scissor_right_column_mask & RASTERIZER_EARLY_Z_CHECK(zbuf_TR, z_x_TR);
    BL_Mask = BL_Mask & scissor_bottom_row_mask & scissor_left_column_mask  & RASTERIZER_EARLY_Z_CHECK(zbuf_BL, z_x_BL);
    BR_Mask = BR_Mask & scissor_bottom_row_mask & scissor_right_column_mask & RASTERIZER_EARLY_Z_CHECK(zbuf_BR, z_x_BR);
     
    scissor_left_column_mask = ~(uint64_t)0;  /* only relevant for first column */

    int64_t Any_Fragment_Valid = TL_Mask | TR_Mask | BL_Mask | BR_Mask;

    if (Any_Fragment_Valid) {
      while ((fragbf->num_rows_ + 4) > FRAGMENT_BUFFER_MAX_ROWS) {
        /* Store context so we can resume from yield */
        rasterizer->direction_xy_flips_ = direction_xy_flips;
        rasterizer->D012_ = D012;
        rasterizer->orientation_ = orientation;
        rasterizer->z_offset_ = z_offset;
        rasterizer->x0_ = x0;
        rasterizer->y0_ = y0;
        rasterizer->z0_ = z0;
        rasterizer->x1_ = x1;
        rasterizer->y1_ = y1;
        rasterizer->z1_ = z1;
        rasterizer->x2_ = x2;
        rasterizer->y2_ = y2;
        rasterizer->z2_ = z2;
        rasterizer->pixel_TL_ = pixel_TL;
        rasterizer->pixel_TR_ = pixel_TR;
        rasterizer->pixel_BL_ = pixel_BL;
        rasterizer->pixel_BR_ = pixel_BR;
        rasterizer->zbuf_TL_ = zbuf_TL;
        rasterizer->zbuf_TR_ = zbuf_TR;
        rasterizer->zbuf_BL_ = zbuf_BL;
        rasterizer->zbuf_BR_ = zbuf_BR;
        rasterizer->stencil_TL_ = stencil_TL;
        rasterizer->stencil_TR_ = stencil_TR;
        rasterizer->stencil_BL_ = stencil_BL;
        rasterizer->stencil_BR_ = stencil_BR;
        rasterizer->pixel_mod_ = pixel_mod;
        rasterizer->zbuf_mod_ = zbuf_mod;
        rasterizer->stencil_mod_ = stencil_mod;
        rasterizer->px_ = px;
        rasterizer->py_ = py;
        rasterizer->Dp01_TL_ = Dp01_TL;
        rasterizer->Dp12_TL_ = Dp12_TL;
        rasterizer->Dp20_TL_ = Dp20_TL;
        rasterizer->Dp01_TR_ = Dp01_TR;
        rasterizer->Dp12_TR_ = Dp12_TR;
        rasterizer->Dp20_TR_ = Dp20_TR;
        rasterizer->Dp01_BL_ = Dp01_BL;
        rasterizer->Dp12_BL_ = Dp12_BL;
        rasterizer->Dp20_BL_ = Dp20_BL;
        rasterizer->Dp01_BR_ = Dp01_BR;
        rasterizer->Dp12_BR_ = Dp12_BR;
        rasterizer->Dp20_BR_ = Dp20_BR;
        rasterizer->Dp01_row_TL_ = Dp01_row_TL;
        rasterizer->Dp12_row_TL_ = Dp12_row_TL;
        rasterizer->Dp20_row_TL_ = Dp20_row_TL;
        rasterizer->Dp01_row_TR_ = Dp01_row_TR;
        rasterizer->Dp12_row_TR_ = Dp12_row_TR;
        rasterizer->Dp20_row_TR_ = Dp20_row_TR;
        rasterizer->Dp01_row_BL_ = Dp01_row_BL;
        rasterizer->Dp12_row_BL_ = Dp12_row_BL;
        rasterizer->Dp20_row_BL_ = Dp20_row_BL;
        rasterizer->Dp01_row_BR_ = Dp01_row_BR;
        rasterizer->Dp12_row_BR_ = Dp12_row_BR;
        rasterizer->Dp20_row_BR_ = Dp20_row_BR;
        rasterizer->z_TL_ = z_TL;
        rasterizer->z_TR_ = z_TR;
        rasterizer->z_BL_ = z_BL;
        rasterizer->z_BR_ = z_BR;
        rasterizer->z_s_TL_ = z_s_TL;
        rasterizer->z_s_TR_ = z_s_TR;
        rasterizer->z_s_BL_ = z_s_BL;
        rasterizer->z_s_BR_ = z_s_BR;
        rasterizer->z_sx_TL_ = z_sx_TL;
        rasterizer->z_sx_TR_ = z_sx_TR;
        rasterizer->z_sx_BL_ = z_sx_BL;
        rasterizer->z_sx_BR_ = z_sx_BR;
        rasterizer->z_x_TL_ = z_x_TL;
        rasterizer->z_x_TR_ = z_x_TR;
        rasterizer->z_x_BL_ = z_x_BL;
        rasterizer->z_x_BR_ = z_x_BR;
        rasterizer->scissor_top_row_mask_ = scissor_top_row_mask;
        rasterizer->scissor_bottom_row_mask_ = scissor_bottom_row_mask;
        rasterizer->scissor_left_column_mask_ = scissor_left_column_mask;
        rasterizer->scissor_right_column_mask_ = scissor_right_column_mask;
        rasterizer->left_ = left;
        rasterizer->top_ = top;
        rasterizer->right_ = right;
        rasterizer->bottom_ = bottom;
        rasterizer->TL_Mask_ = TL_Mask;
        rasterizer->TR_Mask_ = TR_Mask;
        rasterizer->BL_Mask_ = BL_Mask;
        rasterizer->BR_Mask_ = BR_Mask;
        rasterizer->Dp01_dx_2_ = Dp01_dx_2;
        rasterizer->Dp12_dx_2_ = Dp12_dx_2;
        rasterizer->Dp20_dx_2_ = Dp20_dx_2;
        rasterizer->Dp01_dy_2_ = Dp01_dy_2;
        rasterizer->Dp12_dy_2_ = Dp12_dy_2;
        rasterizer->Dp20_dy_2_ = Dp20_dy_2;
        rasterizer->z_xp_ = z_xp;
        rasterizer->z_xq_ = z_xq;
        rasterizer->z_xi_ = z_xi;
        rasterizer->z_yp_ = z_yp;
        rasterizer->z_yq_ = z_yq;
        rasterizer->z_yi_ = z_yi;

        rasterizer->resume_at_ = RASTERIZER_RESUME_CONDITION;
        return orientation;
            RASTERIZER_RESUME_LABEL:;
      }
      // Emit 4 fragments.
      ((uint8_t *)fragbf->column_data_[FB_IDX_EXECUTION_CHAIN])[fragbf->num_rows_ + 0] = 1;
      ((uint8_t *)fragbf->column_data_[FB_IDX_EXECUTION_CHAIN])[fragbf->num_rows_ + 1] = 1;
      ((uint8_t *)fragbf->column_data_[FB_IDX_EXECUTION_CHAIN])[fragbf->num_rows_ + 2] = 1;
      ((uint8_t *)fragbf->column_data_[FB_IDX_EXECUTION_CHAIN])[fragbf->num_rows_ + 3] = 1; /* don't actually know this one yet (!!) careful later! */
      ((uint8_t *)fragbf->column_data_[FB_IDX_MASK])[fragbf->num_rows_ + 0] = (uint8_t)TL_Mask;
      ((uint8_t *)fragbf->column_data_[FB_IDX_MASK])[fragbf->num_rows_ + 1] = (uint8_t)TR_Mask;
      ((uint8_t *)fragbf->column_data_[FB_IDX_MASK])[fragbf->num_rows_ + 2] = (uint8_t)BL_Mask;
      ((uint8_t *)fragbf->column_data_[FB_IDX_MASK])[fragbf->num_rows_ + 3] = (uint8_t)BR_Mask;
      ((void **)fragbf->column_data_[FB_IDX_PIXEL_PTR])[fragbf->num_rows_ + 0] = pixel_TL;
      ((void **)fragbf->column_data_[FB_IDX_PIXEL_PTR])[fragbf->num_rows_ + 1] = pixel_TR;
      ((void **)fragbf->column_data_[FB_IDX_PIXEL_PTR])[fragbf->num_rows_ + 2] = pixel_BL;
      ((void **)fragbf->column_data_[FB_IDX_PIXEL_PTR])[fragbf->num_rows_ + 3] = pixel_BR;
      ((void **)fragbf->column_data_[FB_IDX_ZBUF_PTR])[fragbf->num_rows_ + 0] = zbuf_TL;
      ((void **)fragbf->column_data_[FB_IDX_ZBUF_PTR])[fragbf->num_rows_ + 1] = zbuf_TR;
      ((void **)fragbf->column_data_[FB_IDX_ZBUF_PTR])[fragbf->num_rows_ + 2] = zbuf_BL;
      ((void **)fragbf->column_data_[FB_IDX_ZBUF_PTR])[fragbf->num_rows_ + 3] = zbuf_BR;
      ((void **)fragbf->column_data_[FB_IDX_STENCIL_PTR])[fragbf->num_rows_ + 0] = stencil_TL;
      ((void **)fragbf->column_data_[FB_IDX_STENCIL_PTR])[fragbf->num_rows_ + 1] = stencil_TR;
      ((void **)fragbf->column_data_[FB_IDX_STENCIL_PTR])[fragbf->num_rows_ + 2] = stencil_BL;
      ((void **)fragbf->column_data_[FB_IDX_STENCIL_PTR])[fragbf->num_rows_ + 3] = stencil_BR;
      ((int32_t *)fragbf->column_data_[FB_IDX_X_COORD])[fragbf->num_rows_ + 0] = (int32_t)px;
      ((int32_t *)fragbf->column_data_[FB_IDX_X_COORD])[fragbf->num_rows_ + 1] = (int32_t)px + 1;
      ((int32_t *)fragbf->column_data_[FB_IDX_X_COORD])[fragbf->num_rows_ + 2] = (int32_t)px;
      ((int32_t *)fragbf->column_data_[FB_IDX_X_COORD])[fragbf->num_rows_ + 3] = (int32_t)px + 1;
      ((int32_t *)fragbf->column_data_[FB_IDX_Y_COORD])[fragbf->num_rows_ + 0] = (int32_t)py;
      ((int32_t *)fragbf->column_data_[FB_IDX_Y_COORD])[fragbf->num_rows_ + 1] = (int32_t)py;
      ((int32_t *)fragbf->column_data_[FB_IDX_Y_COORD])[fragbf->num_rows_ + 2] = (int32_t)py + 1;
      ((int32_t *)fragbf->column_data_[FB_IDX_Y_COORD])[fragbf->num_rows_ + 3] = (int32_t)py + 1;
      ((int64_t *)fragbf->column_data_[FB_IDX_DP01])[fragbf->num_rows_ + 0] = Dp01_TL;
      ((int64_t *)fragbf->column_data_[FB_IDX_DP01])[fragbf->num_rows_ + 1] = Dp01_TR;
      ((int64_t *)fragbf->column_data_[FB_IDX_DP01])[fragbf->num_rows_ + 2] = Dp01_BL;
      ((int64_t *)fragbf->column_data_[FB_IDX_DP01])[fragbf->num_rows_ + 3] = Dp01_BR;
      ((int64_t *)fragbf->column_data_[FB_IDX_DP12])[fragbf->num_rows_ + 0] = Dp12_TL;
      ((int64_t *)fragbf->column_data_[FB_IDX_DP12])[fragbf->num_rows_ + 1] = Dp12_TR;
      ((int64_t *)fragbf->column_data_[FB_IDX_DP12])[fragbf->num_rows_ + 2] = Dp12_BL;
      ((int64_t *)fragbf->column_data_[FB_IDX_DP12])[fragbf->num_rows_ + 3] = Dp12_BR;
      ((int64_t *)fragbf->column_data_[FB_IDX_DP20])[fragbf->num_rows_ + 0] = Dp20_TL;
      ((int64_t *)fragbf->column_data_[FB_IDX_DP20])[fragbf->num_rows_ + 1] = Dp20_TR;
      ((int64_t *)fragbf->column_data_[FB_IDX_DP20])[fragbf->num_rows_ + 2] = Dp20_BL;
      ((int64_t *)fragbf->column_data_[FB_IDX_DP20])[fragbf->num_rows_ + 3] = Dp20_BR;
      ((uint32_t *)fragbf->column_data_[FB_IDX_ZBUF_VALUE])[fragbf->num_rows_ + 0] = (uint32_t)z_x_TL + z_offset; /* XXX: Should be clamping to z-buffer range */
      ((uint32_t *)fragbf->column_data_[FB_IDX_ZBUF_VALUE])[fragbf->num_rows_ + 1] = (uint32_t)z_x_TR + z_offset;
      ((uint32_t *)fragbf->column_data_[FB_IDX_ZBUF_VALUE])[fragbf->num_rows_ + 2] = (uint32_t)z_x_BL + z_offset;
      ((uint32_t *)fragbf->column_data_[FB_IDX_ZBUF_VALUE])[fragbf->num_rows_ + 3] = (uint32_t)z_x_BR + z_offset;
      fragbf->num_rows_ += 4;
    }

    Dp01_TL += Dp01_dx_2;
    Dp12_TL += Dp12_dx_2;
    Dp20_TL += Dp20_dx_2;
    Dp01_TR += Dp01_dx_2;
    Dp12_TR += Dp12_dx_2;
    Dp20_TR += Dp20_dx_2;
    Dp01_BL += Dp01_dx_2;
    Dp12_BL += Dp12_dx_2;
    Dp20_BL += Dp20_dx_2;
    Dp01_BR += Dp01_dx_2;
    Dp12_BR += Dp12_dx_2;
    Dp20_BR += Dp20_dx_2;

    /* Step z_x to next column */
    z_x_TL += z_xq;
    z_sx_TL -= z_xp;
    step_mask = z_sx_TL >> 63;
    z_sx_TL += D012 & step_mask;
    z_x_TL += z_xi & step_mask;

    z_x_TR += z_xq;
    z_sx_TR -= z_xp;
    step_mask = z_sx_TR >> 63;
    z_sx_TR += D012 & step_mask;
    z_x_TR += z_xi & step_mask;

    z_x_BL += z_xq;
    z_sx_BL -= z_xp;
    step_mask = z_sx_BL >> 63;
    z_sx_BL += D012 & step_mask;
    z_x_BL += z_xi & step_mask;

    z_x_BR += z_xq;
    z_sx_BR -= z_xp;
    step_mask = z_sx_BR >> 63;
    z_sx_BR += D012 & step_mask;
    z_x_BR += z_xi & step_mask;

    /* Step pixels to the next (stepping over 2 pixels at a time) */
    pixel_TL += 4 * 2;
    pixel_TR += 4 * 2;
    pixel_BL += 4 * 2;
    pixel_BR += 4 * 2;
    zbuf_TL += zstep * 2;
    zbuf_TR += zstep * 2;
    zbuf_BL += zstep * 2;
    zbuf_BR += zstep * 2;
    stencil_TL += stencil_step * 2;
    stencil_TR += stencil_step * 2;
    stencil_BL += stencil_step * 2;
    stencil_BR += stencil_step * 2;
  }

  pixel_TL += pixel_mod;
  pixel_TR += pixel_mod;
  pixel_BL += pixel_mod;
  pixel_BR += pixel_mod;

  zbuf_TL += zbuf_mod;
  zbuf_TR += zbuf_mod;
  zbuf_BL += zbuf_mod;
  zbuf_BR += zbuf_mod;

  stencil_TL += stencil_mod;
  stencil_TR += stencil_mod;
  stencil_BL += stencil_mod;
  stencil_BR += stencil_mod;
}
