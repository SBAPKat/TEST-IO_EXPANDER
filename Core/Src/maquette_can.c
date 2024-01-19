/*
 * maquette_can.c
 *
 *  Created on: 15 mai 2020
 *      Author: scamka
 */
#include "maquette_can.h"
#include "MCP3008.h"
#include "MCP23008.h"

extern MCP3008_InitTypeDef ADC_1;
extern MCP3008_InitTypeDef ADC_2;
extern MCP3008_InitTypeDef ADC_3;

void init_circular_buffer(void)
{
	CAN_buf_RD = 0;
	CAN_buf_WR = 0;
	val_can =0;
}

void Receive_frame(void)
{
	switch(CAN_circ_buf[CAN_buf_RD].CAN_message.StdId & 0x0F0)
	{
	case (IO_EXPANDERS_U5):
		Receive_frame_for_U5();
	break;
	case (IO_EXPANDERS_U8):
		Receive_frame_for_U8();
	break;
	case (IO_EXPANDERS_U11):
		Receive_frame_for_U11();
	break;
	case (IO_EXPANDERS_U6):
		Receive_frame_for_U6();
	break;
	case (IO_EXPANDERS_U10):
		Receive_frame_for_U10();
	break;
	}
}

void Receive_frame_for_U5(void)
{
	CAN_frame_Tx Frame_To_Send;
	uint8_t traitement;
	uint8_t donnees;
	extern MCP23008_InitTypeDef GPIO_0;

	switch(CAN_circ_buf[CAN_buf_RD].CAN_message.StdId & 0x0F3)
	{
		case (IO_EXPANDERS_U5): // SIEGE
		// Check to know if it's a data or a remote frame
				if(CAN_circ_buf[CAN_buf_RD].CAN_message.RTR > 0) //Remote frame RTR=1
				{
					//Send data frame
					Frame_To_Send.CAN_message.StdId = CAN_circ_buf[CAN_buf_RD].CAN_message.StdId;
					Frame_To_Send.CAN_message.RTR = CAN_RTR_DATA;
					Frame_To_Send.CAN_message.IDE = CAN_ID_STD;
					Frame_To_Send.CAN_message.DLC = 1;
					Frame_To_Send.rbuffer_data[0] = GPIO_0.currentStatus;

					HAL_CAN_AddTxMessage(&hcan1, &Frame_To_Send.CAN_message, Frame_To_Send.rbuffer_data,&pTxMailbox);
					HAL_Delay(1);
				}
				else //Data frame RTR=0
				{
					//check if DLC is correct
					if(CAN_circ_buf[CAN_buf_RD].CAN_message.DLC == 1)
					{
						traitement = CAN_circ_buf[CAN_buf_RD].rbuffer_data[0] | 0xCC; //Filtre la donnée reçue pour laisser les bits 7, 6, 3 et 2 à 1 au cas où l'étudiant les auraient mis à 0
						if((traitement&0x30) != 0x30)//Regarde si les bits 4 et 5 sont différents de 11 (moteur 2)
						{
							if((traitement&0x30) == 0x10 || (traitement&0x30) == 0x20)//Regarde si le moteur va être mis en marche
							{
								ADC_3.update_request |= 0b00001000;//Mise en marche de la surveillance
							}
							else //Sinon le moteur va s'éteindre
							{
								ADC_3.update_request &= 0b11110111;//Arrêt de la surveillance
							}
							donnees = GPIO_0.currentStatus;//Va chercher l'état du GPIO0
							donnees &= 0xCF; // Met à 0 les bits 4 et 5
							donnees |= (traitement & 0xF0); // Isole les bits 4, 5, 6 et 7 de la donnée reçue et les met dans la variable qui modifie les IO Expanders
							MCP23008_WritePort(&GPIO_0, donnees, 100);
						}
						if ((traitement&0x03) != 0x03)//Regarde si les bits 0 et 1 sont différents de 11 (moteur 1)
						{
							if((traitement&0x03) == 0x01 || (traitement&0x03) == 0x02)//Regarde si le moteur va être mis en marche
							{
								ADC_3.update_request |= 0b00000100;//Mise en marche de la surveillance
							}
							else //Sinon le moteur va s'éteindre
							{
								ADC_3.update_request &= 0b11111011;//Arrêt de la surveillance
							}
							donnees = GPIO_0.currentStatus;//Va chercher l'état du GPIO0
							donnees &= 0xFC; // Met à 0 les bits 0 et 1
							donnees |= (traitement & 0x0F); // Isole les bits 0, 1, 2 et 3 de la donnée reçue et les met dans la variable qui modifie les IO Expanders
							MCP23008_WritePort(&GPIO_0, donnees, 100);
						}
					}
					else
					{
						//Bad DLC, sending error

						//Send data frame
						Frame_To_Send.CAN_message.StdId = 0x5FF;
						Frame_To_Send.CAN_message.RTR = CAN_RTR_DATA;
						Frame_To_Send.CAN_message.IDE = CAN_ID_STD;
						Frame_To_Send.CAN_message.DLC = 0;
						Frame_To_Send.rbuffer_data[0] = 0;
						HAL_CAN_AddTxMessage(&hcan1, &Frame_To_Send.CAN_message, Frame_To_Send.rbuffer_data,&pTxMailbox);
						HAL_Delay(1);
					}
				}
				CAN_buf_RD++;
				if(CAN_buf_RD == BUF_CIRC_SIZE) CAN_buf_RD=0;
				val_can--;;
		break;
		case (IO_EXPANDERS_U5_ETAT_MOT_1):
				// Check to know if it's a data
				if(CAN_circ_buf[CAN_buf_RD].CAN_message.RTR > 0) //Remote frame RTR=1
				{
					//Send data frame
					Frame_To_Send.CAN_message.StdId = CAN_circ_buf[CAN_buf_RD].CAN_message.StdId;
					Frame_To_Send.CAN_message.RTR = CAN_RTR_DATA;
					Frame_To_Send.CAN_message.IDE = CAN_ID_STD;
					Frame_To_Send.CAN_message.DLC = 2;
					Frame_To_Send.rbuffer_data[0] = 0xFF;/*sens du moteur 1*/
					Frame_To_Send.rbuffer_data[1] = (uint8_t)ADC_3.result[ADC3_S_M1];

					HAL_CAN_AddTxMessage(&hcan1, &Frame_To_Send.CAN_message, Frame_To_Send.rbuffer_data,&pTxMailbox);
					HAL_Delay(1);
				}

				CAN_buf_RD++;
				if(CAN_buf_RD == BUF_CIRC_SIZE) CAN_buf_RD=0;
				val_can--;
		break;
		case (IO_EXPANDERS_U5_ETAT_MOT_2):
		// Check to know if it's a data
				if(CAN_circ_buf[CAN_buf_RD].CAN_message.RTR > 0) //Remote frame RTR=1
				{
					//Send data frame
					Frame_To_Send.CAN_message.StdId = CAN_circ_buf[CAN_buf_RD].CAN_message.StdId;
					Frame_To_Send.CAN_message.RTR = CAN_RTR_DATA;
					Frame_To_Send.CAN_message.IDE = CAN_ID_STD;
					Frame_To_Send.CAN_message.DLC = 2;
					Frame_To_Send.rbuffer_data[0] = 0xFE;/*sens du moteur 2*/
					Frame_To_Send.rbuffer_data[1] = ADC_3.result[ADC3_S_M2];

					HAL_CAN_AddTxMessage(&hcan1, &Frame_To_Send.CAN_message, Frame_To_Send.rbuffer_data,&pTxMailbox);
					HAL_Delay(1);
				}

				CAN_buf_RD++;
				if(CAN_buf_RD == BUF_CIRC_SIZE) CAN_buf_RD=0;
				val_can--;
		break;
		}
}

void Receive_frame_for_U8(void)
{
	CAN_frame_Tx Frame_To_Send;
	uint8_t traitement;
	uint8_t donnees;
	extern MCP23008_InitTypeDef GPIO_1;
	switch(CAN_circ_buf[CAN_buf_RD].CAN_message.StdId & 0x0F3)
	{
		case (IO_EXPANDERS_U8): //RETRO
				// Check to know if it's a data or a remote frame
				if(CAN_circ_buf[CAN_buf_RD].CAN_message.RTR > 0) //Remote frame RTR=1
				{
					//Send data frame
					Frame_To_Send.CAN_message.StdId = CAN_circ_buf[CAN_buf_RD].CAN_message.StdId;
					Frame_To_Send.CAN_message.RTR = CAN_RTR_DATA;
					Frame_To_Send.CAN_message.IDE = CAN_ID_STD;
					Frame_To_Send.CAN_message.DLC = 1;
					Frame_To_Send.rbuffer_data[0] = GPIO_1.currentStatus;

					HAL_CAN_AddTxMessage(&hcan1, &Frame_To_Send.CAN_message, Frame_To_Send.rbuffer_data,&pTxMailbox);
					HAL_Delay(1);
				}
				else //Data frame RTR=0
				{
					//check if DLC is correct
					if(CAN_circ_buf[CAN_buf_RD].CAN_message.DLC == 1)
					{
						traitement = CAN_circ_buf[CAN_buf_RD].rbuffer_data[0] | 0xCC; //Filtre la donnée reçue pour laisser les bits 7, 6, 3 et 2 à 1 au cas où l'étudiant les auraient mis à 0
						if((traitement&0x30) != 0x30)//Regarde si les bits 4 et 5 sont différents de 11 (moteur 2)
						{
							if((traitement&0x30) == 0x10 || (traitement&0x30) == 0x20)//Regarde si le moteur va être mis en marche
								{
									ADC_3.update_request |= 0b00000010;//Mise en marche de la surveillance
								}
								else //Sinon le moteur va s'éteindre
								{
									ADC_3.update_request &= 0b11111101;//Arrêt de la surveillance
								}
							donnees = GPIO_1.currentStatus;
							donnees &= 0xCF; // Met à 0 les bits 4 et 5
							donnees |= (traitement & 0xF0); // Isole les bits 4, 5, 6 et 7 de la donnée reçue et les met dans la variable qui modifie les IO Expanders
							MCP23008_WritePort(&GPIO_1, donnees, 100);
						}
						if((traitement&0x03) != 0x03)//Regarde si les bits 0 et 1 sont différents de 11 (moteur 1)
						{
							if((traitement&0x03) == 0x01 || (traitement&0x03) == 0x02)//Regarde si le moteur va être mis en marche
							{
								ADC_3.update_request |= 0b00000001;//Mise en marche de la surveillance
							}
							else //Sinon le moteur va s'éteindre
							{
								ADC_3.update_request &= 0b11111110;//Arrêt de la surveillance
							}
							donnees = GPIO_1.currentStatus;
							donnees &= 0xFC; // Met à 0 les bits 0 et 1
							donnees |= (traitement & 0x0F); // Isole les bits 0, 1, 2 et 3 de la donnée reçue et les met dans la variable qui modifie les IO Expanders
							MCP23008_WritePort(&GPIO_1, donnees, 100);
						}
					}
					else
					{
						//Bad DLC, sending error

						//Send data frame
						Frame_To_Send.CAN_message.StdId = 0x5FF;
						Frame_To_Send.CAN_message.RTR = CAN_RTR_DATA;
						Frame_To_Send.CAN_message.IDE = CAN_ID_STD;
						Frame_To_Send.CAN_message.DLC = 0;
						Frame_To_Send.rbuffer_data[0] = 0;
						HAL_CAN_AddTxMessage(&hcan1, &Frame_To_Send.CAN_message, Frame_To_Send.rbuffer_data,&pTxMailbox);
						HAL_Delay(1);
					}
				}
				CAN_buf_RD++;
				if(CAN_buf_RD == BUF_CIRC_SIZE) CAN_buf_RD=0;
				val_can--;
		break;
		case (IO_EXPANDERS_U8_ETAT_MOT_1):
				// Check to know if it's a data
				if(CAN_circ_buf[CAN_buf_RD].CAN_message.RTR > 0) //Remote frame RTR=1
				{
					//Send data frame
					Frame_To_Send.CAN_message.StdId = CAN_circ_buf[CAN_buf_RD].CAN_message.StdId;
					Frame_To_Send.CAN_message.RTR = CAN_RTR_DATA;
					Frame_To_Send.CAN_message.IDE = CAN_ID_STD;
					Frame_To_Send.CAN_message.DLC = 2;
					Frame_To_Send.rbuffer_data[0] = 0xFD;/*sens du moteur 1*/
					Frame_To_Send.rbuffer_data[1] = ADC_3.result[ADC3_R_M1];

					HAL_CAN_AddTxMessage(&hcan1, &Frame_To_Send.CAN_message, Frame_To_Send.rbuffer_data,&pTxMailbox);
					HAL_Delay(1);
				}

				CAN_buf_RD++;
				if(CAN_buf_RD == BUF_CIRC_SIZE) CAN_buf_RD=0;
				val_can--;
		break;
		case (IO_EXPANDERS_U8_ETAT_MOT_2):
				// Check to know if it's a data
				if(CAN_circ_buf[CAN_buf_RD].CAN_message.RTR > 0) //Remote frame RTR=1
				{
					//Send data frame
					Frame_To_Send.CAN_message.StdId = CAN_circ_buf[CAN_buf_RD].CAN_message.StdId;
					Frame_To_Send.CAN_message.RTR = CAN_RTR_DATA;
					Frame_To_Send.CAN_message.IDE = CAN_ID_STD;
					Frame_To_Send.CAN_message.DLC = 2;
					Frame_To_Send.rbuffer_data[0] = 0xFC;/*sens du moteur 2*/
					Frame_To_Send.rbuffer_data[1] = ADC_3.result[ADC3_R_M2];

					HAL_CAN_AddTxMessage(&hcan1, &Frame_To_Send.CAN_message, Frame_To_Send.rbuffer_data,&pTxMailbox);
					HAL_Delay(1);
				}

				CAN_buf_RD++;
				if(CAN_buf_RD == BUF_CIRC_SIZE) CAN_buf_RD=0;
				val_can--;
		break;
		}
}

void Receive_frame_for_U11(void)
{
	CAN_frame_Tx Frame_To_Send;
	uint8_t traitement;
	uint8_t donnees;
	extern MCP23008_InitTypeDef GPIO_4;
	switch(CAN_circ_buf[CAN_buf_RD].CAN_message.StdId & 0x0F3)
	{
		case (IO_EXPANDERS_U11):
				// Check to know if it's a data or a remote frame
				if(CAN_circ_buf[CAN_buf_RD].CAN_message.RTR > 0) //Remote frame RTR=1
				{
					//Send data frame
					Frame_To_Send.CAN_message.StdId = CAN_circ_buf[CAN_buf_RD].CAN_message.StdId;
					Frame_To_Send.CAN_message.RTR = CAN_RTR_DATA;
					Frame_To_Send.CAN_message.IDE = CAN_ID_STD;
					Frame_To_Send.CAN_message.DLC = 1;
					Frame_To_Send.rbuffer_data[0] = GPIO_4.currentStatus;

					HAL_CAN_AddTxMessage(&hcan1, &Frame_To_Send.CAN_message, Frame_To_Send.rbuffer_data,&pTxMailbox);
					HAL_Delay(1);
				}
				else //Data frame RTR=0
				{
					//check if DLC is correct
					if(CAN_circ_buf[CAN_buf_RD].CAN_message.DLC == 1)
					{
						traitement = CAN_circ_buf[CAN_buf_RD].rbuffer_data[0] | 0x0C; //Filtre la donnée reçue pour laisser les bits 3 et 2 à 1 au cas où l'étudiant les auraient mis à 0
						if ((traitement&0x03) != 0x03)//Regarde si les bits 0 et 1 sont différents de 11 (moteur 1)
						{
							//Moteur
							if((traitement&0x03) == 0x01 || (traitement&0x03) == 0x02)//Regarde si le moteur va être mis en marche
							{
								ADC_3.update_request |= 0b00010000;//Mise en marche de la surveillance
							}
							else //Sinon le moteur va s'éteindre
							{
								ADC_3.update_request &= 0b11101111;//Arrêt de la surveillance
							}
							donnees = GPIO_4.currentStatus;
							donnees &= 0xFC; // Met à 0 les bits 0 et 1
							donnees |= (traitement & 0x0F); // Isole les bits 0, 1, 2 et 3 de la donnée reçue et les met dans la variable qui modifie les IO Expanders
							//Feux
							ADC_3.update_request ^= (traitement & 0x30)<<1;//Vérifie le courant des IO mis en marche
							donnees ^= (traitement & 0x30); // Isole les bits 4 et 5 de la donnée reçue et change l'état de ceux de la donnée récupérée à l'IO Expander
							MCP23008_WritePort(&GPIO_4, donnees, 100);
						}
						else
						{	//Feux
							donnees = GPIO_4.currentStatus;
							donnees ^= (traitement & 0x30); // Isole les bits 4 et 5 de la donnée reçue et change l'état de ceux de la donnée récupérée à l'IO Expander
							ADC_3.update_request ^= (traitement & 0x30)<<1;//Vérifie le courant des IO mis en marche
							MCP23008_WritePort(&GPIO_4, donnees, 100);
						}
					}
					else
					{
						//Bad DLC, sending error

						//Send data frame
						Frame_To_Send.CAN_message.StdId = 0x5FF;
						Frame_To_Send.CAN_message.RTR = CAN_RTR_DATA;
						Frame_To_Send.CAN_message.IDE = CAN_ID_STD;
						Frame_To_Send.CAN_message.DLC = 0;
						Frame_To_Send.rbuffer_data[0] = 0;
						HAL_CAN_AddTxMessage(&hcan1, &Frame_To_Send.CAN_message, Frame_To_Send.rbuffer_data,&pTxMailbox);
						HAL_Delay(1);
					}
				}
				CAN_buf_RD++;
				if(CAN_buf_RD == BUF_CIRC_SIZE) CAN_buf_RD=0;
				val_can--;
		break;
		case (IO_EXPANDERS_U11_ETAT_MOT):
				// Check to know if it's a data
				if(CAN_circ_buf[CAN_buf_RD].CAN_message.RTR > 0) //Remote frame RTR=1
				{
					//Send data frame
					Frame_To_Send.CAN_message.StdId = CAN_circ_buf[CAN_buf_RD].CAN_message.StdId;
					Frame_To_Send.CAN_message.RTR = CAN_RTR_DATA;
					Frame_To_Send.CAN_message.IDE = CAN_ID_STD;
					Frame_To_Send.CAN_message.DLC = 2;
					Frame_To_Send.rbuffer_data[0] = 0xFB;/*sens du moteur*/
					Frame_To_Send.rbuffer_data[1] = ADC_3.result[ADC3_L_M1];

					HAL_CAN_AddTxMessage(&hcan1, &Frame_To_Send.CAN_message, Frame_To_Send.rbuffer_data,&pTxMailbox);
					HAL_Delay(1);
				}

				CAN_buf_RD++;
				if(CAN_buf_RD == BUF_CIRC_SIZE) CAN_buf_RD=0;
				val_can--;
		break;
		case (IO_EXPANDERS_U11_ETAT_FEUX):
		// Check to know if it's a data
				if(CAN_circ_buf[CAN_buf_RD].CAN_message.RTR > 0) //Remote frame RTR=1
				{
					//Send data frame
					Frame_To_Send.CAN_message.StdId = CAN_circ_buf[CAN_buf_RD].CAN_message.StdId;
					Frame_To_Send.CAN_message.RTR = CAN_RTR_DATA;
					Frame_To_Send.CAN_message.IDE = CAN_ID_STD;
					Frame_To_Send.CAN_message.DLC = 2;
					Frame_To_Send.rbuffer_data[0] = ADC_3.result[ADC3_FARG_BROUILL];
					Frame_To_Send.rbuffer_data[1] = ADC_3.result[ADC3_FARD_BROUILL];

					HAL_CAN_AddTxMessage(&hcan1, &Frame_To_Send.CAN_message, Frame_To_Send.rbuffer_data,&pTxMailbox);
					HAL_Delay(1);
				}

				CAN_buf_RD++;
				if(CAN_buf_RD == BUF_CIRC_SIZE) CAN_buf_RD=0;
				val_can--;
		break;
	}
}

void Receive_frame_for_U6(void)
{
	CAN_frame_Tx Frame_To_Send;
	uint8_t traitement;
	uint8_t donnees;
	extern MCP23008_InitTypeDef GPIO_3;
	switch(CAN_circ_buf[CAN_buf_RD].CAN_message.StdId & 0x0F3)
	{
		case (IO_EXPANDERS_U6):
				// Check to know if it's a data or a remote frame
				if(CAN_circ_buf[CAN_buf_RD].CAN_message.RTR > 0) //Remote frame RTR=1
				{
					//Send data frame
					Frame_To_Send.CAN_message.StdId = CAN_circ_buf[CAN_buf_RD].CAN_message.StdId;
					Frame_To_Send.CAN_message.RTR = CAN_RTR_DATA;
					Frame_To_Send.CAN_message.IDE = CAN_ID_STD;
					Frame_To_Send.CAN_message.DLC = 1;
					Frame_To_Send.rbuffer_data[0] = GPIO_3.currentStatus;

					HAL_CAN_AddTxMessage(&hcan1, &Frame_To_Send.CAN_message, Frame_To_Send.rbuffer_data,&pTxMailbox);
					HAL_Delay(1);
				}
				else //Data frame RTR=0
				{
					//check if DLC is correct
					if(CAN_circ_buf[CAN_buf_RD].CAN_message.DLC == 1)
					{
						traitement = CAN_circ_buf[CAN_buf_RD].rbuffer_data[0];//Stockage de la data
						donnees = GPIO_3.currentStatus;//Stockage de l'état des IO
						donnees ^= traitement;//Inversion de l'état
						ADC_1.update_request ^= traitement;//Vérifie le courant des IO mis en marche
						MCP23008_WritePort(&GPIO_3, donnees, 100);
					}
					else
					{
						//Bad DLC, sending error

						//Send data frame
						Frame_To_Send.CAN_message.StdId = 0x5FF;
						Frame_To_Send.CAN_message.RTR = CAN_RTR_DATA;
						Frame_To_Send.CAN_message.IDE = CAN_ID_STD;
						Frame_To_Send.CAN_message.DLC = 0;
						Frame_To_Send.rbuffer_data[0] = 0;
						HAL_CAN_AddTxMessage(&hcan1, &Frame_To_Send.CAN_message, Frame_To_Send.rbuffer_data,&pTxMailbox);
						HAL_Delay(1);
					}
				}
				CAN_buf_RD++;
				if(CAN_buf_RD == BUF_CIRC_SIZE) CAN_buf_RD=0;
				val_can--;
				break;
		case(IO_EXPANDERS_U6_ETAT_FEUX):
		// Check to know if it's a data
				if(CAN_circ_buf[CAN_buf_RD].CAN_message.RTR > 0) //Remote frame RTR=1
				{

					//Send data frame
					Frame_To_Send.CAN_message.StdId = CAN_circ_buf[CAN_buf_RD].CAN_message.StdId;
					Frame_To_Send.CAN_message.RTR = CAN_RTR_DATA;
					Frame_To_Send.CAN_message.IDE = CAN_ID_STD;
					Frame_To_Send.CAN_message.DLC = 8;
					Frame_To_Send.rbuffer_data[0] = ADC_1.result[ADC1_FAVG_ROUTE];
					Frame_To_Send.rbuffer_data[1] = ADC_1.result[ADC1_FAVG_CROISEMENT];
					Frame_To_Send.rbuffer_data[2] = ADC_1.result[ADC1_FAVG_CLIGNO];
					Frame_To_Send.rbuffer_data[3] = ADC_1.result[ADC1_FAVG_VEILLE];
					Frame_To_Send.rbuffer_data[4] = ADC_1.result[ADC1_FAVD_ROUTE];
					Frame_To_Send.rbuffer_data[5] = ADC_1.result[ADC1_FAVD_CROISEMENT];
					Frame_To_Send.rbuffer_data[6] = ADC_1.result[ADC1_FAVD_CLIGNO];
					Frame_To_Send.rbuffer_data[7] = ADC_1.result[ADC1_FAVD_VEILLE];

					HAL_CAN_AddTxMessage(&hcan1, &Frame_To_Send.CAN_message, Frame_To_Send.rbuffer_data,&pTxMailbox);
					HAL_Delay(1);
				}

				CAN_buf_RD++;
				if(CAN_buf_RD == BUF_CIRC_SIZE) CAN_buf_RD=0;
				val_can--;
		break;
	}
}


void Receive_frame_for_U10(void)
{
	CAN_frame_Tx Frame_To_Send;
	uint8_t traitement;
	uint8_t donnees;
	extern MCP23008_InitTypeDef GPIO_2;
	switch(CAN_circ_buf[CAN_buf_RD].CAN_message.StdId & 0x0F3)
	{
		case (IO_EXPANDERS_U10):
				// Check to know if it's a data or a remote frame
				if(CAN_circ_buf[CAN_buf_RD].CAN_message.RTR > 0) //Remote frame RTR=1
				{
					//Send data frame
					Frame_To_Send.CAN_message.StdId = CAN_circ_buf[CAN_buf_RD].CAN_message.StdId;
					Frame_To_Send.CAN_message.RTR = CAN_RTR_DATA;
					Frame_To_Send.CAN_message.IDE = CAN_ID_STD;
					Frame_To_Send.CAN_message.DLC = 1;
					Frame_To_Send.rbuffer_data[0] = GPIO_2.currentStatus;

					HAL_CAN_AddTxMessage(&hcan1, &Frame_To_Send.CAN_message, Frame_To_Send.rbuffer_data,&pTxMailbox);
					HAL_Delay(1);
				}
				else //Data frame RTR=0
				{
					//check if DLC is correct
					if(CAN_circ_buf[CAN_buf_RD].CAN_message.DLC == 1)
					{
						traitement = CAN_circ_buf[CAN_buf_RD].rbuffer_data[0];//Stockage de la data
						donnees = GPIO_2.currentStatus;//Stockage de l'état des IO
						donnees ^= traitement;//Inversion de l'état
						ADC_2.update_request ^= traitement;//Vérifie le courant des IO mis en marche
						MCP23008_WritePort(&GPIO_2, donnees, 100);
					}
					else
					{
						//Bad DLC, sending error

						//Send data frame
						Frame_To_Send.CAN_message.StdId = 0x5FF;
						Frame_To_Send.CAN_message.RTR = CAN_RTR_DATA;
						Frame_To_Send.CAN_message.IDE = CAN_ID_STD;
						Frame_To_Send.CAN_message.DLC = 0;
						Frame_To_Send.rbuffer_data[0] = 0;
						HAL_CAN_AddTxMessage(&hcan1, &Frame_To_Send.CAN_message, Frame_To_Send.rbuffer_data,&pTxMailbox);
						HAL_Delay(1);
					}
				}
				CAN_buf_RD++;
				if(CAN_buf_RD == BUF_CIRC_SIZE) CAN_buf_RD=0;
				val_can--;
		break;
		case(IO_EXPANDERS_U10_ETAT_FEUX):
			// Check to know if it's a data
			if(CAN_circ_buf[CAN_buf_RD].CAN_message.RTR > 0) //Remote frame RTR=1
			{

				//Send data frame
				Frame_To_Send.CAN_message.StdId = CAN_circ_buf[CAN_buf_RD].CAN_message.StdId;
				Frame_To_Send.CAN_message.RTR = CAN_RTR_DATA;
				Frame_To_Send.CAN_message.IDE = CAN_ID_STD;
				Frame_To_Send.CAN_message.DLC = 8;
				Frame_To_Send.rbuffer_data[0] = ADC_2.result[ADC2_FARG_VEILLE];
				Frame_To_Send.rbuffer_data[1] = ADC_2.result[ADC2_FARG_STOP];
				Frame_To_Send.rbuffer_data[2] = ADC_2.result[ADC2_FARG_CLIGNO];
				Frame_To_Send.rbuffer_data[3] = ADC_2.result[ADC2_FARG_RECUL];
				Frame_To_Send.rbuffer_data[4] = ADC_2.result[ADC2_FARD_VEILLE];
				Frame_To_Send.rbuffer_data[5] = ADC_2.result[ADC2_FARD_STOP];
				Frame_To_Send.rbuffer_data[6] = ADC_2.result[ADC2_FARD_CLIGNO];
				Frame_To_Send.rbuffer_data[7] = ADC_2.result[ADC2_FARD_RECUL];

				HAL_CAN_AddTxMessage(&hcan1, &Frame_To_Send.CAN_message, Frame_To_Send.rbuffer_data,&pTxMailbox);
				HAL_Delay(1);
			}

			CAN_buf_RD++;
			if(CAN_buf_RD == BUF_CIRC_SIZE) CAN_buf_RD=0;
			val_can--;
		break;
	}
}


