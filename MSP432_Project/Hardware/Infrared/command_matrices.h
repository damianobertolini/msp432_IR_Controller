/*
 * command_matrices.h
 *
 *  Created on: 28 nov 2022
 *      Author: damiano
 */

#ifndef COMMANDMATRICES_H_
#define COMMANDMATRICES_H_

#include <stdint.h>

// Length of the commands
#define MAX_PULSE 34

extern const uint16_t up_matrix[5][MAX_PULSE];
extern const uint16_t up_matrix_p[5][MAX_PULSE];
extern const uint16_t fw_matrix[5][MAX_PULSE];
extern const uint16_t fw_matrix_p[5][MAX_PULSE];
extern const uint16_t bk_matrix[5][MAX_PULSE];
extern const uint16_t bk_matrix_p[5][MAX_PULSE];
extern const uint16_t left_matrix[5][MAX_PULSE];
extern const uint16_t left_matrix_p[5][MAX_PULSE];
extern const uint16_t right_matrix[5][MAX_PULSE];
extern const uint16_t right_matrix_p[5][MAX_PULSE];


#endif /* COMMANDMATRICES_H_ */
