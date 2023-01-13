/*
 * command_matrices.c
 *
 *  Created on: 28 nov 2022
 *      Author: damiano
 */

#include "command_matrices.h"

// This file includes all matrices which represent the signals sent through Infrared
// This values were decoded from the original remote controller with an IR Receiver

const uint16_t up_matrix[5][MAX_PULSE] =      {{1950,250,250,400,250,250,250,300,300,250,300,200,250,250,250,250,300,250,250,200,400,350,200,250,250,250,400,300,350,250,250,400,350,250},
                                             {1950,250,250,350,300,400,300,200,250,300,250,250,250,250,300,250,250,250,300,400,200,250,300,350,350,250,350,300,300,350,400,400,350,250},
                                             {1950,300,350,350,250,400,250,250,400,300,350,350,300,250,250,300,300,250,300,350,250,250,250,200,250,400,300,350,250,300,350,250,300,250},
                                             {1950,250,300,350,250,250,300,200,400,250,350,350,250,250,200,250,250,250,250,300,350,250,250,300,300,300,350,350,400,350,300,300,300,250},
                                             {1950,300,350,250,250,350,300,250,400,250,400,350,250,250,300,300,250,250,250,250,250,250,250,300,300,250,250,300,400,350,350,350,300,250}};

const uint16_t up_matrix_p[5][MAX_PULSE] =    {{1950,350,200,700,350,750,700,250,750,300,350,750,750,700,750,700,700,750,350,250,200,750,750,750,750,200,250,300,250,400,200,250,750,1000},
                                             {1950,400,200,700,200,700,700,400,700,300,350,750,750,650,700,750,750,700,200,700,400,700,200,250,750,200,300,300,250,200,200,200,750,1000},
                                             {1950,200,250,700,200,750,700,200,750,200,250,700,700,700,750,700,650,750,200,700,350,750,750,750,200,300,200,400,300,200,350,300,700,1000},
                                             {1950,350,200,750,350,700,700,250,750,200,250,750,750,700,750,750,750,700,750,200,750,700,300,700,700,200,250,200,200,350,250,300,750,1000},
                                             {1950,200,350,750,200,750,700,200,700,200,250,750,700,700,700,700,750,700,750,750,700,750,250,750,700,300,350,200,200,250,200,350,700,1000}};

const uint16_t fw_matrix[5][MAX_PULSE] =      {{1950,250,400,250,350,400,400,250,400,400,350,400,400,400,350,400,450,250,250,400,400,250,400,250,200,400,350,300,350,400,400,300,350,400},
                                             {1950,250,350,250,350,400,350,300,350,400,400,400,400,350,450,400,400,350,300,350,350,300,300,300,300,300,350,250,400,400,400,250,350,400},
                                             {1900,200,450,250,350,400,400,300,350,400,400,350,350,400,400,400,400,250,250,400,250,300,350,250,400,450,300,300,350,400,450,250,400,400},
                                             {1950,250,400,250,400,350,400,300,400,350,400,400,400,400,350,450,350,250,300,300,350,350,350,350,250,250,350,200,450,350,450,250,400,400},
                                             {1950,250,400,250,400,350,450,300,350,400,350,400,400,400,400,400,400,250,250,350,300,300,250,400,250,250,300,300,350,400,400,250,450,250}};

const uint16_t fw_matrix_p[5][MAX_PULSE] =    {{1950,200,750,200,200,200,750,200,200,200,200,200,200,200,200,200,700,700,200,200,750,200,700,750,250,200,750,200,200,200,700,200,200,1000},
                                             {2000,200,800,200,200,200,750,200,200,200,200,200,200,200,150,200,700,650,200,200,750,700,700,650,700,200,700,200,200,200,750,200,200,1000},
                                             {2000,200,750,200,200,200,750,150,200,200,200,250,200,200,200,200,750,700,200,750,700,200,700,200,200,200,750,200,200,200,650,250,200,1000},
                                             {1950,200,750,200,200,200,700,200,200,200,200,200,200,200,200,200,750,700,700,200,700,200,200,750,750,200,750,200,200,200,700,200,200,1000},
                                             {1950,200,750,200,200,200,700,200,200,200,200,200,200,200,200,200,700,750,700,650,650,700,200,750,750,250,750,200,200,200,700,150,750,1000}};

const uint16_t bk_matrix[5][MAX_PULSE] =      {{1950,250,400,400,300,250,250,250,350,300,350,300,250,250,300,350,300,300,300,350,400,250,400,450,350,250,350,300,350,350,450,300,350,300},
                                             {1950,250,400,400,300,300,250,300,300,250,400,200,300,300,250,400,300,300,250,400,300,350,400,400,350,250,400,250,350,400,400,300,400,350},
                                             {1950,250,400,400,250,350,250,300,350,300,350,250,350,300,300,350,250,300,250,350,250,250,400,350,400,400,350,250,350,400,350,400,300,300},
                                             {1900,250,400,400,250,250,300,250,300,300,350,350,250,300,300,350,250,350,250,250,350,250,350,450,350,350,450,250,400,350,450,250,400,400},
                                             {1900,250,400,400,250,300,300,300,300,300,350,300,250,350,250,350,250,300,250,250,250,300,200,300,300,200,450,300,400,350,400,250,400,400}};

const uint16_t bk_matrix_p[5][MAX_PULSE] =    {{1950,200,200,750,650,750,700,750,650,200,750,650,750,700,200,750,700,650,200,200,700,200,200,200,750,200,750,200,200,200,700,200,750,1000},
                                             {1950,200,200,750,700,650,750,700,650,200,750,800,650,700,200,700,700,700,200,700,200,200,200,200,750,200,750,200,200,200,700,200,200,1000},
                                             {1950,200,200,700,750,650,750,650,650,200,750,700,650,650,200,750,750,700,200,750,750,200,200,200,200,200,800,200,200,200,200,750,700,1000},
                                             {2000,200,200,750,700,750,650,750,700,200,700,650,700,700,200,750,700,700,700,200,800,200,200,200,200,200,700,200,200,200,700,200,200,1000},
                                             {2000,200,200,700,750,700,700,650,700,200,700,700,700,700,200,750,700,750,700,750,700,700,800,650,700,200,700,200,200,200,700,200,200,1000}};

const uint16_t left_matrix[5][MAX_PULSE] =    {{1950,250,400,250,250,300,350,350,250,350,400,400,300,250,250,250,250,250,300,350,400,350,250,250,250,250,400,250,350,400,400,250,400,300},
                                             {1950,250,450,200,250,350,350,400,300,350,350,400,300,300,300,300,250,300,250,400,400,250,250,400,400,250,400,250,400,350,400,250,400,250},
                                             {1950,250,400,250,300,300,350,350,300,350,350,400,250,250,250,250,250,250,250,400,250,250,400,400,350,300,350,250,400,350,450,250,400,250},
                                             {1950,250,400,300,250,300,300,400,250,400,350,400,250,250,250,250,250,250,250,250,400,250,400,400,400,350,350,250,400,350,350,300,350,250},
                                             {1950,250,400,300,300,350,300,400,250,400,400,400,250,250,300,250,250,300,250,300,300,250,350,350,300,350,350,250,350,400,400,250,400,250}};

const uint16_t left_matrix_p[5][MAX_PULSE] =  {{1950,200,700,750,750,200,200,800,200,200,200,700,700,750,700,750,700,750,200,200,200,800,700,750,700,200,750,200,200,200,750,200,700,1000},
                                             {1950,200,700,750,750,150,200,750,200,200,200,700,700,700,650,700,750,650,200,200,700,750,200,200,750,200,700,200,200,200,750,200,700,1000},
                                             {1950,200,750,700,700,200,200,750,200,200,250,700,750,750,700,750,750,700,200,750,700,200,200,200,750,200,750,200,200,200,650,200,800,1000},
                                             {1950,200,750,650,750,250,200,750,200,200,200,750,750,700,750,750,700,750,700,200,700,200,200,200,700,200,700,250,200,200,750,200,700,1000},
                                             {1950,200,750,700,650,200,200,700,200,200,200,750,750,700,700,750,700,650,750,700,700,700,150,750,650,200,700,250,200,200,700,200,750,1000}};

const uint16_t right_matrix[5][MAX_PULSE] =   {{1900,300,350,400,400,400,250,250,400,250,400,400,250,200,300,250,300,250,250,400,400,350,300,200,250,300,350,250,400,450,350,250,250,350},
                                             {1950,250,400,400,400,350,250,250,400,350,400,400,250,250,250,300,250,250,250,400,400,250,250,250,450,250,350,250,400,350,400,300,400,300},
                                             {1950,250,400,400,400,350,200,250,400,450,400,400,250,250,250,250,250,250,250,400,250,300,400,350,250,350,450,250,400,350,450,200,450,250},
                                             {1950,250,400,350,400,400,250,300,400,350,400,400,250,250,250,250,200,250,250,250,400,350,250,250,300,200,400,300,400,350,400,250,350,250},
                                             {1950,250,400,400,350,400,250,250,350,400,400,400,250,250,250,250,250,250,300,250,300,300,250,400,250,300,400,350,350,350,400,250,400,300}};

const uint16_t right_matrix_p[5][MAX_PULSE] = {{2000,200,200,200,200,700,700,200,750,200,200,700,750,800,650,750,700,700,200,200,200,750,700,800,700,200,700,200,200,200,750,750,200,1000},
                                             {2000,200,200,200,200,750,750,200,200,200,200,750,700,750,700,700,750,700,200,200,750,750,650,200,750,200,750,200,200,200,700,200,700,1000},
                                             {2000,200,200,200,200,750,750,200,200,150,200,750,700,750,700,750,750,700,200,750,700,200,200,750,200,200,700,200,200,200,700,200,700,1000},
                                             {2000,200,200,200,200,750,700,200,200,200,200,750,700,750,700,750,800,700,750,200,200,750,750,700,700,250,700,200,200,200,750,200,750,1000},
                                             {2000,200,200,200,200,750,750,200,200,200,200,750,700,750,700,750,750,700,700,750,650,700,200,700,750,150,700,200,200,200,700,200,750,1000}};



