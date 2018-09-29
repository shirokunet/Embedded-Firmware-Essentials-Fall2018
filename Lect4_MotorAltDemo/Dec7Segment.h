/*
 * Dec7Segment.h
 *
 *  Created on: Sep 28, 2018
 *      Author: wacheda
 */

#ifndef DEC7SEGMENT_H_
#define DEC7SEGMENT_H_


const unsigned int total_display_time_ms = 150;
const unsigned int persist_time_ms = 4;

BusOut number(p13,p14,p15,p16,p17,p18,p19);
BusOut digit(p9,p10,p11,p12);

void Dec7Segment(int);

#endif /* DEC7SEGMENT_H_ */
