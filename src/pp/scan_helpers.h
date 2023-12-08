#ifndef SCAN_HELPERS_H
#define SCAN_HELPERS_H

#ifndef STDINT_H_INCLUDED
#define STDINT_H_INCLUDED
#include <stdint.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

struct preprocessor;
struct situs;

/* WARNING: These are not general use functions, instead they factor out functionality that would
 *          otherwise be replicated in pattern action code. The input already has to meticulously
 *          match the pattern expected by the function, or results will be unpredictable / wrong. */

int sch_parse_decimal(struct preprocessor *pp, uint64_t *pval, const char *text, size_t len, struct situs *loc);
int sch_parse_octal(struct preprocessor *pp, uint64_t *pval, const char *text, size_t len, struct situs *loc);
int sch_parse_hexadecimal(struct preprocessor *pp, uint64_t *pval, const char *text, size_t len, struct situs *loc);
int sch_parse_binary(struct preprocessor *pp, uint64_t *pval, const char *text, size_t len, struct situs *loc);

void sch_process_pptk_dec_none(struct preprocessor *pp, struct pptk **pp_chain, const char *text, uint64_t val, struct pptk **pptk, struct situs *loc);
void sch_process_pptk_dec_U(struct preprocessor *pp, struct pptk **pp_chain, const char *text, uint64_t val, struct pptk **pptk, struct situs *loc);
void sch_process_pptk_dec_L(struct preprocessor *pp, struct pptk **pp_chain, const char *text, uint64_t val, struct pptk **pptk, struct situs *loc);
void sch_process_pptk_dec_UL(struct preprocessor *pp, struct pptk **pp_chain, const char *text, uint64_t val, struct pptk **pptk, struct situs *loc);
void sch_process_pptk_dec_LL(struct preprocessor *pp, struct pptk **pp_chain, const char *text, uint64_t val, struct pptk **pptk, struct situs *loc);
void sch_process_pptk_dec_ULL(struct preprocessor *pp, struct pptk **pp_chain, const char *text, uint64_t val, struct pptk **pptk, struct situs *loc);

void sch_process_pptk_ohb_none(struct preprocessor *pp, struct pptk **pp_chain, const char *text, uint64_t val, struct pptk **pptk, struct situs *loc);
void sch_process_pptk_ohb_U(struct preprocessor *pp, struct pptk **pp_chain, const char *text, uint64_t val, struct pptk **pptk, struct situs *loc);
void sch_process_pptk_ohb_L(struct preprocessor *pp, struct pptk **pp_chain, const char *text, uint64_t val, struct pptk **pptk, struct situs *loc);
void sch_process_pptk_ohb_UL(struct preprocessor *pp, struct pptk **pp_chain, const char *text, uint64_t val, struct pptk **pptk, struct situs *loc);
void sch_process_pptk_ohb_LL(struct preprocessor *pp, struct pptk **pp_chain, const char *text, uint64_t val, struct pptk **pptk, struct situs *loc);
void sch_process_pptk_ohb_ULL(struct preprocessor *pp, struct pptk **pp_chain, const char *text, uint64_t val, struct pptk **pptk, struct situs *loc);

int sch_read_oct_esc_value(const char *octal_num, int *val);
int sch_read_hex_esc_value(const char *hex_xx_num, int *val);
int sch_read_char_value(struct preprocessor *pp, const char *lit_pos, int *val, int max_char, struct situs *loc, const char *kind);


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* SCAN_HELPERS_H */
