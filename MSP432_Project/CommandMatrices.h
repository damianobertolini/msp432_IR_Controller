/*
 * command_matrices.h
 *
 *  Created on: 28 nov 2022
 *      Author: damiano
 */

#ifndef COMMANDMATRICES_H_
#define COMMANDMATRICES_H_


#define MAX_PULSE 34

extern unsigned int up_matrix[5][MAX_PULSE];
extern unsigned int up_matrix_p[5][MAX_PULSE];
extern unsigned int fw_matrix[5][MAX_PULSE];
extern unsigned int fw_matrix_p[5][MAX_PULSE];
extern unsigned int bk_matrix[5][MAX_PULSE];
extern unsigned int bk_matrix_p[5][MAX_PULSE];
extern unsigned int left_matrix[5][MAX_PULSE];
extern unsigned int left_matrix_p[5][MAX_PULSE];
extern unsigned int right_matrix[5][MAX_PULSE];
extern unsigned int right_matrix_p[5][MAX_PULSE];


#endif /* COMMANDMATRICES_H_ */
