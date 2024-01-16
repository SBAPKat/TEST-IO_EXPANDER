/*
 * maquette_can.h
 *
 *  Created on: 15 mai 2020
 *      Author: scamka
 */

#ifndef MAQUETTE_CAN_H_
#define MAQUETTE_CAN_H_

//includes
#include "stm32f4xx_hal.h"
#include "MCP23008.h"

//defines
#define BUF_CIRC_SIZE 20

//IO-EXPANDERS U5 : Moteur 1 et 2 du siège

#define IO_EXPANDERS_U5 0x010
#define IO_EXPANDERS_U5_ETAT_MOT_1 0x011
#define IO_EXPANDERS_U5_ETAT_MOT_2 0x012

//IO-EXPANDERS U8 : Moteur 1 et 2 rétroviseur

#define IO_EXPANDERS_U8 0x020
#define IO_EXPANDERS_U8_ETAT_MOT_1 0x021
#define IO_EXPANDERS_U8_ETAT_MOT_2 0x022

//IO-EXPANDERS U11 : Moteur lève-vitre et antibrouillard feu arrière droit et gauche

#define IO_EXPANDERS_U11 0x030
#define IO_EXPANDERS_U11_ETAT_MOT 0x031
#define IO_EXPANDERS_U11_ETAT_FEUX 0x032

//IO-EXPANDERS U6 : Feux avants droit et gauche

#define IO_EXPANDERS_U6 0x040
#define IO_EXPANDERS_U6_ETAT_FEUX 0X041

//IO-EXPANDERS U10 : Feux arrières droit et gauche sauf feux de brouillard (voir IO_EXPANDERS_U8 adresse 0x020)

#define IO_EXPANDERS_U10 0x050
#define IO_EXPANDERS_U10_ETAT_FEUX 0x051

//IO-EXPANDERS U7 : Futures GPIO

#define IO_EXPANDERS_U7 0x060

//IO-EXPANDERS U9 : Futures GPIO

#define IO_EXPANDERS_U9 0x070

//IO-EXPANDERS U12 : Futures GPIO Input

#define IO_EXPANDERS_U12 0x080

//variables
typedef struct{
	CAN_RxHeaderTypeDef CAN_message;
	uint8_t rbuffer_data[8];			// Request ou Data frame

}CAN_frame_rcv;

//variables
typedef struct{
	CAN_TxHeaderTypeDef CAN_message;
	uint8_t rbuffer_data[8];			// Request ou Data frame

}CAN_frame_Tx;


//Circular Buffer Variables
extern uint8_t CAN_buf_RD;
extern uint8_t CAN_buf_WR;
extern CAN_frame_rcv CAN_circ_buf[BUF_CIRC_SIZE];
extern uint8_t val_can;

//Can frame variables
extern uint32_t pTxMailbox;
CAN_FilterTypeDef Myfilter;
extern CAN_HandleTypeDef hcan1;
/*
//Components variables
extern uint8_t Comodos_PORTA;
extern uint8_t Comodos_PORTB;
extern uint8_t Comodos_PORTC;
extern uint8_t Comodos_AN0;
extern uint8_t Comodos_AN1;

extern uint8_t Satellite_PORTA;
extern uint8_t Satellite_PORTB;
extern uint8_t Satellite_PORTC;
extern uint8_t Satellite_AN0;
extern uint8_t Satellite_AN1;
extern uint8_t Satellite_AN2;
extern uint8_t Satellite_AN3;
extern uint8_t Satellite_AN5;

extern uint8_t Portiere_PORTA;
extern uint8_t Portiere_PORTB;
extern uint8_t Portiere_PORTC;
extern uint8_t Portiere_PORTD;
extern uint8_t Portiere_AN0;
extern uint8_t Portiere_AN1;
extern uint8_t Portiere_AN2;
extern uint8_t Portiere_AN3;
extern uint8_t Portiere_AN4;
extern uint8_t Portiere_AN5;
extern uint8_t Portiere_AN6;

extern uint8_t Feux_av_PORTA;
extern uint8_t Feux_av_PORTB;
extern uint8_t Feux_av_PORTC;

extern uint8_t Feux_ar_PORTA;
extern uint8_t Feux_ar_PORTB;
extern uint8_t Feux_ar_PORTC;
*/
/*
//Prototypes
void init_circular_buffer(void);
void Receive_frame(void);
void Receive_frame_comodos(void);
void Receive_frame_satellite(void);
void Receive_frame_feux_ar(void);
void Receive_frame_feux_av(void);
void Receive_frame_portiere(void);
void init_can(void);
void Init_send_frame(void);
*/
//Nouvelles fonctions
void Receive_frame_for_U5(void);
void Receive_frame_for_U8(void);
void Receive_frame_for_U11(void);
void Receive_frame_for_U6(void);
void Receive_frame_for_U10(void);

#endif /* MAQUETTE_CAN_H_ */
