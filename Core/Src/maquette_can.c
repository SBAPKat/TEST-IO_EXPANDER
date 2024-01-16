///*
// * maquette_can.c
// *
// *  Created on: 15 mai 2020
// *      Author: scamka
// */
//#include "maquette_can.h"
//
//void init_circular_buffer(void)
//{
//	CAN_buf_RD = 0;
//	CAN_buf_WR = 0;
//	val_can =0;
//}
//
///*
//void init_can(void)
//{
//	  //Cfg Filters for RX0
//
//	//Filtre for slave 0x50 (Comodos)
//	  Myfilter.FilterFIFOAssignment=CAN_FILTER_FIFO0;
//	  Myfilter.FilterIdHigh=0x0501;
//	  Myfilter.FilterIdLow=0x0000<<3 | 0x1 <<2;
//	  Myfilter.FilterMaskIdHigh=0;//0x1050;
//	  Myfilter.FilterMaskIdLow=0;//0x1<<2;
//	  Myfilter.FilterScale=CAN_FILTERSCALE_32BIT;
//	  Myfilter.FilterMode=CAN_FILTERMODE_IDMASK;
//	  Myfilter.FilterActivation=ENABLE;
//	  Myfilter.FilterBank=0;
//	  HAL_CAN_ConfigFilter(&hcan1, &Myfilter);
//
//	//Filtre for slave 0x51 (Feux avants)
//	Myfilter.FilterFIFOAssignment=CAN_FILTER_FIFO0;
//	Myfilter.FilterIdHigh=0x502;
//	Myfilter.FilterIdLow=0x0000<<3 | 0x1 <<2;
//	Myfilter.FilterMaskIdHigh=0x502;
//	Myfilter.FilterMaskIdLow=0x1<<2;
//	Myfilter.FilterScale=CAN_FILTERSCALE_32BIT;
//	Myfilter.FilterMode=CAN_FILTERMODE_IDMASK;
//	Myfilter.FilterActivation=ENABLE;
//	Myfilter.FilterBank=1;
//	HAL_CAN_ConfigFilter(&hcan1, &Myfilter);
//
//	//Filtre for slave 0x52 (Feux arri�res)
//	Myfilter.FilterFIFOAssignment=CAN_FILTER_FIFO0;
//	Myfilter.FilterIdHigh=0x503;
//	Myfilter.FilterIdLow=0x0000<<3 | 0x1 <<2;
//	Myfilter.FilterMaskIdHigh=0x503;
//	Myfilter.FilterMaskIdLow=0x1<<2;
//	Myfilter.FilterScale=CAN_FILTERSCALE_32BIT;
//	Myfilter.FilterMode=CAN_FILTERMODE_IDMASK;
//	Myfilter.FilterActivation=ENABLE;
//	Myfilter.FilterBank=2;
//	HAL_CAN_ConfigFilter(&hcan1, &Myfilter);
//
//	//Filtre for slave 0x53 (Satellite)
//	Myfilter.FilterFIFOAssignment=CAN_FILTER_FIFO0;
//	Myfilter.FilterIdHigh=0x500;
//	Myfilter.FilterIdLow=0x0000<<3 | 0x1 <<2;
//	Myfilter.FilterMaskIdHigh=0x500;
//	Myfilter.FilterMaskIdLow=0x1<<2;
//	Myfilter.FilterScale=CAN_FILTERSCALE_32BIT;
//	Myfilter.FilterMode=CAN_FILTERMODE_IDMASK;
//	Myfilter.FilterActivation=ENABLE;
//	Myfilter.FilterBank=3;
//	HAL_CAN_ConfigFilter(&hcan1, &Myfilter);
//
//	//Filtre for slave 0x54 (Porti�re)
//	Myfilter.FilterFIFOAssignment=CAN_FILTER_FIFO0;
//	Myfilter.FilterIdHigh=0x504;
//	Myfilter.FilterIdLow=0x0000<<3 | 0x1 <<2;
//	Myfilter.FilterMaskIdHigh=0x504;
//	Myfilter.FilterMaskIdLow=0x1<<2;
//	Myfilter.FilterScale=CAN_FILTERSCALE_32BIT;
//	Myfilter.FilterMode=CAN_FILTERMODE_IDMASK;
//	Myfilter.FilterActivation=ENABLE;
//	Myfilter.FilterBank=4;
//	HAL_CAN_ConfigFilter(&hcan1, &Myfilter);
//
//	  HAL_CAN_ActivateNotification(&hcan1,CAN_IT_RX_FIFO0_MSG_PENDING);
//	  HAL_CAN_Start(&hcan1);
//}
//*/
//void Receive_frame(void)
//{
//	switch(CAN_circ_buf[CAN_buf_RD].CAN_message.StdId & 0x0F0)
//	{
//	case (IO_EXPANDERS_U5):
//		Receive_frame_for_U5();
//	break;
//	case (IO_EXPANDERS_U8):
//		Receive_frame_for_U8();
//	break;
//	case (IO_EXPANDERS_U11):
//		Receive_frame_for_U11();
//	break;
//	case (IO_EXPANDERS_U6):
//		Receive_frame_for_U6();
//	break;
//	case (IO_EXPANDERS_U10):
//		Receive_frame_for_U10();
//	break;
//	}
//}
//
//void Receive_frame_for_U5(void)
//{
//	CAN_frame_Tx Frame_To_Send;
//	uint8_t traitement;
//	uint8_t donnees;
//	extern MCP23008_InitTypeDef GPIO_0;
//
//	switch(CAN_circ_buf[CAN_buf_RD].CAN_message.StdId & 0x0F3)
//	{
//		case (IO_EXPANDERS_U5):
//		// Check to know if it's a data or a remote frame
//				if(CAN_circ_buf[CAN_buf_RD].CAN_message.RTR > 0) //Remote frame RTR=1
//				{
//					//Send data frame
//					Frame_To_Send.CAN_message.StdId = CAN_circ_buf[CAN_buf_RD].CAN_message.StdId;
//					Frame_To_Send.CAN_message.RTR = CAN_RTR_DATA;
//					Frame_To_Send.CAN_message.IDE = CAN_ID_STD;
//					Frame_To_Send.CAN_message.DLC = 1;
//					Frame_To_Send.rbuffer_data[0] = GPIO_0->currentStatus;
//
//					HAL_CAN_AddTxMessage(&hcan1, &Frame_To_Send.CAN_message, Frame_To_Send.rbuffer_data,&pTxMailbox);
//					HAL_Delay(1);
//				}
//				else //Data frame RTR=0
//				{
//					//check if DLC is correct
//					if(CAN_circ_buf[CAN_buf_RD].CAN_message.DLC == 1)
//					{
//						traitement = CAN_circ_buf[CAN_buf_RD].rbuffer_data[0] | 0xCC; //Filtre la donnée reçue pour laisser les bits 7, 6, 3 et 2 à 1 au cas où l'étudiant les auraient mis à 0
//						if(traitement&0x30!=0x30)//Regarde si les bits 4 et 5 sont différents de 11
//						{
//							donnees = GPIO_0->currentStatus;
//							donnees &= 0xCF; // Met à 0 les bits 4 et 5
//							donnees |= (traitement & 0xF0) // Isole les bits 4, 5, 6 et 7 de la donnée reçue et les met dans la variable qui modifie les IO Expanders
//							MCP23008_WritePort(&GPIO_0, donnees, 100);
//						}
//						if (traitement&0x03!=0x03)
//						{
//							//Regarde si les bits 0 et 1 sont différents de 11
//							donnees = GPIO_0->currentStatus;
//							donnees &= 0xFC; // Met à 0 les bits 0 et 1
//							donnees |= (traitement & 0x0F) // Isole les bits 0, 1, 2 et 3 de la donnée reçue et les met dans la variable qui modifie les IO Expanders
//							MCP23008_WritePort(&GPIO_0, donnees, 100);
//						}
//					}
//					else
//					{
//						//Bad DLC, sending error
//
//						//Send data frame
//						Frame_To_Send.CAN_message.StdId = 0x5FF;
//						Frame_To_Send.CAN_message.RTR = CAN_RTR_DATA;
//						Frame_To_Send.CAN_message.IDE = CAN_ID_STD;
//						Frame_To_Send.CAN_message.DLC = 0;
//						Frame_To_Send.rbuffer_data[0] = 0;
//						HAL_CAN_AddTxMessage(&hcan1, &Frame_To_Send.CAN_message, Frame_To_Send.rbuffer_data,&pTxMailbox);
//						HAL_Delay(1);
//					}
//				}
//				CAN_buf_RD++;
//				if(CAN_buf_RD == BUF_CIRC_SIZE) CAN_buf_RD=0;
//				val_can--;;
//		break;
//		case (IO_EXPANDERS_U5_ETAT_MOT_1):
//				// Check to know if it's a data
//				if(CAN_circ_buf[CAN_buf_RD].CAN_message.RTR > 0) //Remote frame RTR=1
//				{
//					//Send data frame
//					Frame_To_Send.CAN_message.StdId = CAN_circ_buf[CAN_buf_RD].CAN_message.StdId;
//					Frame_To_Send.CAN_message.RTR = CAN_RTR_DATA;
//					Frame_To_Send.CAN_message.IDE = CAN_ID_STD;
//					Frame_To_Send.CAN_message.DLC = 2;
//					Frame_To_Send.rbuffer_data[0] = /*sens du moteur 1*/;
//					Frame_To_Send.rbuffer_data[1] = /*valeur du courant du moteur 1*/;
//
//					HAL_CAN_AddTxMessage(&hcan1, &Frame_To_Send.CAN_message, Frame_To_Send.rbuffer_data,&pTxMailbox);
//					HAL_Delay(1);
//				}
//
//				CAN_buf_RD++;
//				if(CAN_buf_RD == BUF_CIRC_SIZE) CAN_buf_RD=0;
//				val_can--;
//		break;
//		case (IO_EXPANDERS_U5_ETAT_MOT_2):
//		// Check to know if it's a data
//				if(CAN_circ_buf[CAN_buf_RD].CAN_message.RTR > 0) //Remote frame RTR=1
//				{
//					//Send data frame
//					Frame_To_Send.CAN_message.StdId = CAN_circ_buf[CAN_buf_RD].CAN_message.StdId;
//					Frame_To_Send.CAN_message.RTR = CAN_RTR_DATA;
//					Frame_To_Send.CAN_message.IDE = CAN_ID_STD;
//					Frame_To_Send.CAN_message.DLC = 2;
//					Frame_To_Send.rbuffer_data[0] = /*sens du moteur 2*/;
//					Frame_To_Send.rbuffer_data[1] = /*valeur du courantdu moteur 2*/;
//
//					HAL_CAN_AddTxMessage(&hcan1, &Frame_To_Send.CAN_message, Frame_To_Send.rbuffer_data,&pTxMailbox);
//					HAL_Delay(1);
//				}
//
//				CAN_buf_RD++;
//				if(CAN_buf_RD == BUF_CIRC_SIZE) CAN_buf_RD=0;
//				val_can--;
//		break;
//		}
//}
//
//void Receive_frame_for_U8(void)
//{
//	CAN_frame_Tx Frame_To_Send;
//	uint8_t traitement;
//	uint8_t donnees;
//	extern MCP23008_InitTypeDef GPIO_1;
//	switch(CAN_circ_buf[CAN_buf_RD].CAN_message.StdId & 0x0F3)
//	{
//		case (IO_EXPANDERS_U8):
//				// Check to know if it's a data or a remote frame
//				if(CAN_circ_buf[CAN_buf_RD].CAN_message.RTR > 0) //Remote frame RTR=1
//				{
//					//Send data frame
//					Frame_To_Send.CAN_message.StdId = CAN_circ_buf[CAN_buf_RD].CAN_message.StdId;
//					Frame_To_Send.CAN_message.RTR = CAN_RTR_DATA;
//					Frame_To_Send.CAN_message.IDE = CAN_ID_STD;
//					Frame_To_Send.CAN_message.DLC = 1;
//					Frame_To_Send.rbuffer_data[0] = GPIO_1->currentStatus;
//
//					HAL_CAN_AddTxMessage(&hcan1, &Frame_To_Send.CAN_message, Frame_To_Send.rbuffer_data,&pTxMailbox);
//					HAL_Delay(1);
//				}
//				else //Data frame RTR=0
//				{
//					//check if DLC is correct
//					if(CAN_circ_buf[CAN_buf_RD].CAN_message.DLC == 1)
//					{
//						traitement = CAN_circ_buf[CAN_buf_RD].rbuffer_data[0] | 0xCC; //Filtre la donnée reçue pour laisser les bits 7, 6, 3 et 2 à 1 au cas où l'étudiant les auraient mis à 0
//						if(traitement&0x30!=0x30)//Regarde si les bits 4 et 5 sont différents de 11
//						{
//							donnees = GPIO_1->currentStatus;
//							donnees &= 0xCF; // Met à 0 les bits 4 et 5
//							donnees |= (traitement & 0xF0) // Isole les bits 4, 5, 6 et 7 de la donnée reçue et les met dans la variable qui modifie les IO Expanders
//							MCP23008_WritePort(&GPIO_1, donnees, 100);
//						}
//						if(traitement&0x03!=0x03)//Regarde si les bits 0 et 1 sont différents de 11
//						{
//							donnees = GPIO_1->currentStatus;
//							donnees &= 0xFC; // Met à 0 les bits 0 et 1
//							donnees |= (traitement & 0x0F) // Isole les bits 0, 1, 2 et 3 de la donnée reçue et les met dans la variable qui modifie les IO Expanders
//							MCP23008_WritePort(&GPIO_1, donnees, 100);
//						}
//					}
//					else
//					{
//						//Bad DLC, sending error
//
//						//Send data frame
//						Frame_To_Send.CAN_message.StdId = 0x5FF;
//						Frame_To_Send.CAN_message.RTR = CAN_RTR_DATA;
//						Frame_To_Send.CAN_message.IDE = CAN_ID_STD;
//						Frame_To_Send.CAN_message.DLC = 0;
//						Frame_To_Send.rbuffer_data[0] = 0;
//						HAL_CAN_AddTxMessage(&hcan1, &Frame_To_Send.CAN_message, Frame_To_Send.rbuffer_data,&pTxMailbox);
//						HAL_Delay(1);
//					}
//				}
//				CAN_buf_RD++;
//				if(CAN_buf_RD == BUF_CIRC_SIZE) CAN_buf_RD=0;
//				val_can--;
//		break;
//		case (IO_EXPANDERS_U8_ETAT_MOT_1):
//				// Check to know if it's a data
//				if(CAN_circ_buf[CAN_buf_RD].CAN_message.RTR > 0) //Remote frame RTR=1
//				{
//					//Send data frame
//					Frame_To_Send.CAN_message.StdId = CAN_circ_buf[CAN_buf_RD].CAN_message.StdId;
//					Frame_To_Send.CAN_message.RTR = CAN_RTR_DATA;
//					Frame_To_Send.CAN_message.IDE = CAN_ID_STD;
//					Frame_To_Send.CAN_message.DLC = 2;
//					Frame_To_Send.rbuffer_data[0] = /*sens du moteur 1*/;
//					Frame_To_Send.rbuffer_data[1] = /*valeur du courant du moteur 1*/;
//
//					HAL_CAN_AddTxMessage(&hcan1, &Frame_To_Send.CAN_message, Frame_To_Send.rbuffer_data,&pTxMailbox);
//					HAL_Delay(1);
//				}
//
//				CAN_buf_RD++;
//				if(CAN_buf_RD == BUF_CIRC_SIZE) CAN_buf_RD=0;
//				val_can--;
//		break;
//		case (IO_EXPANDERS_U8_ETAT_MOT_2):
//				// Check to know if it's a data
//				if(CAN_circ_buf[CAN_buf_RD].CAN_message.RTR > 0) //Remote frame RTR=1
//				{
//					//Send data frame
//					Frame_To_Send.CAN_message.StdId = CAN_circ_buf[CAN_buf_RD].CAN_message.StdId;
//					Frame_To_Send.CAN_message.RTR = CAN_RTR_DATA;
//					Frame_To_Send.CAN_message.IDE = CAN_ID_STD;
//					Frame_To_Send.CAN_message.DLC = 2;
//					Frame_To_Send.rbuffer_data[0] = /*sens du moteur 2*/;
//					Frame_To_Send.rbuffer_data[1] = /*valeur du courant du moteur 2*/;
//
//					HAL_CAN_AddTxMessage(&hcan1, &Frame_To_Send.CAN_message, Frame_To_Send.rbuffer_data,&pTxMailbox);
//					HAL_Delay(1);
//				}
//
//				CAN_buf_RD++;
//				if(CAN_buf_RD == BUF_CIRC_SIZE) CAN_buf_RD=0;
//				val_can--;
//		break;
//		}
//}
//
//void Receive_frame_for_U11(void)
//{
//	CAN_frame_Tx Frame_To_Send;
//	uint8_t traitement;
//	uint8_t donnees;
//	extern MCP23008_InitTypeDef GPIO_2;
//	switch(CAN_circ_buf[CAN_buf_RD].CAN_message.StdId & 0x0F3)
//	{
//		case (IO_EXPANDERS_U11):
//				// Check to know if it's a data or a remote frame
//				if(CAN_circ_buf[CAN_buf_RD].CAN_message.RTR > 0) //Remote frame RTR=1
//				{
//					//Send data frame
//					Frame_To_Send.CAN_message.StdId = CAN_circ_buf[CAN_buf_RD].CAN_message.StdId;
//					Frame_To_Send.CAN_message.RTR = CAN_RTR_DATA;
//					Frame_To_Send.CAN_message.IDE = CAN_ID_STD;
//					Frame_To_Send.CAN_message.DLC = 1;
//					Frame_To_Send.rbuffer_data[0] = GPIO_2->currentStatus;
//
//					HAL_CAN_AddTxMessage(&hcan1, &Frame_To_Send.CAN_message, Frame_To_Send.rbuffer_data,&pTxMailbox);
//					HAL_Delay(1);
//				}
//				else //Data frame RTR=0
//				{
//					//check if DLC is correct
//					if(CAN_circ_buf[CAN_buf_RD].CAN_message.DLC == 1)
//					{
//						traitement = CAN_circ_buf[CAN_buf_RD].rbuffer_data[0] | 0x0C; //Filtre la donnée reçue pour laisser les bits 3 et 2 à 1 au cas où l'étudiant les auraient mis à 0
//						if (traitement&0x03!=0x03)//Regarde si les bits 0 et 1 sont différents de 11
//						{	//Moteur
//							donnees = GPIO_2->currentStatus;
//							donnees &= 0xFC; // Met à 0 les bits 0 et 1
//							donnees |= (traitement & 0x0F); // Isole les bits 0, 1, 2 et 3 de la donnée reçue et les met dans la variable qui modifie les IO Expanders
//							//Feux
//							donnees ^= (traitement & 0x30); // Isole les bits 4 et 5 de la donnée reçue et change l'état de ceux de la donnée récupérée à l'IO Expander
//							MCP23008_WritePort(&GPIO_2, donnees, 100);
//						}
//						else
//						{	//Feux
//							donnees ^= (traitement & 0x30); // Isole les bits 4 et 5 de la donnée reçue et change l'état de ceux de la donnée récupérée à l'IO Expander
//						}
//					}
//					else
//					{
//						//Bad DLC, sending error
//
//						//Send data frame
//						Frame_To_Send.CAN_message.StdId = 0x5FF;
//						Frame_To_Send.CAN_message.RTR = CAN_RTR_DATA;
//						Frame_To_Send.CAN_message.IDE = CAN_ID_STD;
//						Frame_To_Send.CAN_message.DLC = 0;
//						Frame_To_Send.rbuffer_data[0] = 0;
//						HAL_CAN_AddTxMessage(&hcan1, &Frame_To_Send.CAN_message, Frame_To_Send.rbuffer_data,&pTxMailbox);
//						HAL_Delay(1);
//					}
//				}
//				CAN_buf_RD++;
//				if(CAN_buf_RD == BUF_CIRC_SIZE) CAN_buf_RD=0;
//				val_can--;
//		break;
//		case (IO_EXPANDERS_U11_ETAT_MOT):
//				// Check to know if it's a data
//				if(CAN_circ_buf[CAN_buf_RD].CAN_message.RTR > 0) //Remote frame RTR=1
//				{
//					//Send data frame
//					Frame_To_Send.CAN_message.StdId = CAN_circ_buf[CAN_buf_RD].CAN_message.StdId;
//					Frame_To_Send.CAN_message.RTR = CAN_RTR_DATA;
//					Frame_To_Send.CAN_message.IDE = CAN_ID_STD;
//					Frame_To_Send.CAN_message.DLC = 2;
//					Frame_To_Send.rbuffer_data[0] = /*sens du moteur*/;
//					Frame_To_Send.rbuffer_data[1] = /*valeur du courant du moteur*/;
//
//					HAL_CAN_AddTxMessage(&hcan1, &Frame_To_Send.CAN_message, Frame_To_Send.rbuffer_data,&pTxMailbox);
//					HAL_Delay(1);
//				}
//
//				CAN_buf_RD++;
//				if(CAN_buf_RD == BUF_CIRC_SIZE) CAN_buf_RD=0;
//				val_can--;
//		break;
//	}
//}
//
//void Receive_frame_for_U6(void)
//{
//	CAN_frame_Tx Frame_To_Send;
//	uint8_t traitement;
//	extern MCP23008_InitTypeDef GPIO_3;
//
//
//				// Check to know if it's a data or a remote frame
//				if(CAN_circ_buf[CAN_buf_RD].CAN_message.RTR > 0) //Remote frame RTR=1
//				{
//					//Send data frame
//					Frame_To_Send.CAN_message.StdId = CAN_circ_buf[CAN_buf_RD].CAN_message.StdId;
//					Frame_To_Send.CAN_message.RTR = CAN_RTR_DATA;
//					Frame_To_Send.CAN_message.IDE = CAN_ID_STD;
//					Frame_To_Send.CAN_message.DLC = 1;
//					Frame_To_Send.rbuffer_data[0] = GPIO_3->currentStatus;
//
//					HAL_CAN_AddTxMessage(&hcan1, &Frame_To_Send.CAN_message, Frame_To_Send.rbuffer_data,&pTxMailbox);
//					HAL_Delay(1);
//				}
//				else //Data frame RTR=0
//				{
//					//check if DLC is correct
//					if(CAN_circ_buf[CAN_buf_RD].CAN_message.DLC == 1)
//					{
//						traitement = CAN_circ_buf[CAN_buf_RD].rbuffer_data[0];
//						donnees = GPIO_3->currentStatus;
//						donnees ^= traitement;
//						MCP23008_WritePort(GPIO_3, donnees, 100);
//					}
//					else
//					{
//						//Bad DLC, sending error
//
//						//Send data frame
//						Frame_To_Send.CAN_message.StdId = 0x5FF;
//						Frame_To_Send.CAN_message.RTR = CAN_RTR_DATA;
//						Frame_To_Send.CAN_message.IDE = CAN_ID_STD;
//						Frame_To_Send.CAN_message.DLC = 0;
//						Frame_To_Send.rbuffer_data[0] = 0;
//						HAL_CAN_AddTxMessage(&hcan1, &Frame_To_Send.CAN_message, Frame_To_Send.rbuffer_data,&pTxMailbox);
//						HAL_Delay(1);
//					}
//				}
//				CAN_buf_RD++;
//				if(CAN_buf_RD == BUF_CIRC_SIZE) CAN_buf_RD=0;
//				val_can--;
//}
//
//void Receive_frame_for_U10(void)
//{
//	CAN_frame_Tx Frame_To_Send;
//	uint8_t traitement;
//	extern MCP23008_InitTypeDef GPIO_4;
//
//
//				// Check to know if it's a data or a remote frame
//				if(CAN_circ_buf[CAN_buf_RD].CAN_message.RTR > 0) //Remote frame RTR=1
//				{
//					//Send data frame
//					Frame_To_Send.CAN_message.StdId = CAN_circ_buf[CAN_buf_RD].CAN_message.StdId;
//					Frame_To_Send.CAN_message.RTR = CAN_RTR_DATA;
//					Frame_To_Send.CAN_message.IDE = CAN_ID_STD;
//					Frame_To_Send.CAN_message.DLC = 1;
//					Frame_To_Send.rbuffer_data[0] = GPIO_4->currentStatus;
//
//					HAL_CAN_AddTxMessage(&hcan1, &Frame_To_Send.CAN_message, Frame_To_Send.rbuffer_data,&pTxMailbox);
//					HAL_Delay(1);
//				}
//				else //Data frame RTR=0
//				{
//					//check if DLC is correct
//					if(CAN_circ_buf[CAN_buf_RD].CAN_message.DLC == 1)
//					{
//						traitement = CAN_circ_buf[CAN_buf_RD].rbuffer_data[0];
//						donnees = GPIO_4->currentStatus;
//						donnees ^= traitement;
//						MCP23008_WritePort(&GPIO_4, donnees, 100);
//					}
//					else
//					{
//						//Bad DLC, sending error
//
//						//Send data frame
//						Frame_To_Send.CAN_message.StdId = 0x5FF;
//						Frame_To_Send.CAN_message.RTR = CAN_RTR_DATA;
//						Frame_To_Send.CAN_message.IDE = CAN_ID_STD;
//						Frame_To_Send.CAN_message.DLC = 0;
//						Frame_To_Send.rbuffer_data[0] = 0;
//						HAL_CAN_AddTxMessage(&hcan1, &Frame_To_Send.CAN_message, Frame_To_Send.rbuffer_data,&pTxMailbox);
//						HAL_Delay(1);
//					}
//				}
//				CAN_buf_RD++;
//				if(CAN_buf_RD == BUF_CIRC_SIZE) CAN_buf_RD=0;
//				val_can--;
//}
//
//// Anciennes fonctions
///*
//void Receive_frame_feux_av(void)
//{
//	CAN_frame_Tx Frame_To_Send;
//
//
//	//Check if received frame is Feux_av
//
//
//				if(CAN_circ_buf[CAN_buf_RD].CAN_message.RTR > 0)
//				{
//					//Send data frame
//					Frame_To_Send.CAN_message.StdId = CAN_circ_buf[CAN_buf_RD].CAN_message.StdId;
//					Frame_To_Send.CAN_message.RTR = CAN_RTR_DATA;
//					Frame_To_Send.CAN_message.IDE = CAN_ID_STD;
//					Frame_To_Send.CAN_message.DLC = 1;
//					Frame_To_Send.rbuffer_data[0] = Feux_av_PORTA;
//					Frame_To_Send.rbuffer_data[1] = Feux_av_PORTB;
//					Frame_To_Send.rbuffer_data[2] = Feux_av_PORTC;
//
//					HAL_CAN_AddTxMessage(&hcan1, &Frame_To_Send.CAN_message, Frame_To_Send.rbuffer_data,&pTxMailbox);
//					HAL_Delay(1);
//				}
//				else
//				{
//					//check if DLC is correct tous les ports
//					if(CAN_circ_buf[CAN_buf_RD].CAN_message.DLC == 2)
//					{
//
//						switch (CAN_circ_buf[CAN_buf_RD].rbuffer_data[7])
//						{
//						case FEUX_AV_ID_PORTA :
//							// Data Frame for feux_av, update Feux_av_PORTA variables with received value
//
//						Feux_av_PORTA = CAN_circ_buf[CAN_buf_RD].rbuffer_data[0] & FEUX_AV_MSK_OUTPUT_PORTA;
//
//						//Update control input: code droit
//						Feux_av_PORTA = (Feux_av_PORTA & 0xFD) | ((Feux_av_PORTA & 0x01) << 1);
//						//Update control input: code gauche
//						Feux_av_PORTA = (Feux_av_PORTA & 0xEF) | ((Feux_av_PORTA & 0x08) << 1);
//						break;
//
//						case FEUX_AV_ID_PORTB :
//							// Data Frame for feux_av, update Feux_av_PORTB variables with received value
//
//						Feux_av_PORTB = CAN_circ_buf[CAN_buf_RD].rbuffer_data[0] & FEUX_AV_MSK_OUTPUT_PORTB;
//
//						//Update control input: clignotant droit
//						Feux_av_PORTB = (Feux_av_PORTB & 0xFD) | ((Feux_av_PORTB & 0x01) << 1);
//						//Update control input: phare/veilleuse gauche
//						Feux_av_PORTB = (Feux_av_PORTB & 0xBF) | ((Feux_av_PORTB & 0x20) << 1) |  ((Feux_av_PORTB & 0x10) << 2);
//						break;
//
//						case FEUX_AV_ID_PORTC:
//							// Data Frame for feux_av, update Feux_av_PORTC variables with received value
//
//						Feux_av_PORTC = CAN_circ_buf[CAN_buf_RD].rbuffer_data[0] & FEUX_AV_MSK_OUTPUT_PORTC;
//
//						//Update control input: phare/veilleuse gauche
//						Feux_av_PORTC = (Feux_av_PORTC & 0xFD) | ((Feux_av_PORTC & 0x01) << 1) |  ((Feux_av_PORTC & 0x08) >> 2);
//						//Update control input: clignotant gauche
//						Feux_av_PORTC = (Feux_av_PORTC & 0xEF) | ((Feux_av_PORTC & 0x20) >> 1);
//						break;
//
//						}
//
//
//
//					}
//					else
//					{
//						//Bad DLC, sending error
//
//						//Send data frame
//						Frame_To_Send.CAN_message.StdId = 0x5FF;
//						Frame_To_Send.CAN_message.RTR = CAN_RTR_DATA;
//						Frame_To_Send.CAN_message.IDE = CAN_ID_STD;
//						Frame_To_Send.CAN_message.DLC = 0;
//						Frame_To_Send.rbuffer_data[0] = 0;
//						HAL_CAN_AddTxMessage(&hcan1, &Frame_To_Send.CAN_message, Frame_To_Send.rbuffer_data,&pTxMailbox);
//						HAL_Delay(1);
//					}
//				}
//				CAN_buf_RD++;
//				if(CAN_buf_RD == BUF_CIRC_SIZE) CAN_buf_RD=0;
//				val_can--;
//
//}
//
//void Receive_frame_feux_ar(void)
//{
//	CAN_frame_Tx Frame_To_Send;
//
//	//Check if received frame is Feux_ar/PORTA
//
//				if(CAN_circ_buf[CAN_buf_RD].CAN_message.RTR > 0)
//									{
//										//Send data frame
//										Frame_To_Send.CAN_message.StdId = CAN_circ_buf[CAN_buf_RD].CAN_message.StdId;
//										Frame_To_Send.CAN_message.RTR = CAN_RTR_DATA;
//										Frame_To_Send.CAN_message.IDE = CAN_ID_STD;
//										Frame_To_Send.CAN_message.DLC = 1;
//										Frame_To_Send.rbuffer_data[0] = Feux_ar_PORTA;
//										Frame_To_Send.rbuffer_data[1] = Feux_ar_PORTB;
//										Frame_To_Send.rbuffer_data[2] = Feux_ar_PORTC;
//
//										HAL_CAN_AddTxMessage(&hcan1, &Frame_To_Send.CAN_message, Frame_To_Send.rbuffer_data,&pTxMailbox);
//										HAL_Delay(1);
//									}
//
//				else
//				{
//					//check if DLC is correct
//					if(CAN_circ_buf[CAN_buf_RD].CAN_message.DLC == 1)
//					{
//
//						switch (CAN_circ_buf[CAN_buf_RD].rbuffer_data[7])
//						{
//							case FEUX_AR_ID_PORTA :
//						// Data Frame for feux_ar, update Feux_ar_PORTA variables with received value
//
//						Feux_ar_PORTA = CAN_circ_buf[CAN_buf_RD].rbuffer_data[0] & FEUX_AR_MSK_OUTPUT_PORTA;
//
//						//Update control input: clignotant
//						Feux_ar_PORTA = (Feux_ar_PORTA & 0xFD) | ((Feux_ar_PORTA & 0x01) << 1);
//						//Update control input: pos/stop
//						Feux_ar_PORTA = (Feux_ar_PORTA & 0xEF) | ((Feux_ar_PORTA & 0x04) << 2) | ((Feux_ar_PORTA & 0x08) << 1);
//						//Update control input: stop gauche
//						Feux_ar_PORTC = (Feux_ar_PORTC & 0xFD) | ((Feux_ar_PORTA & 0x20) >>4);
//						break;
//
//							case FEUX_AR_ID_PORTB :
//						// Data Frame for feux_ar, update Feux_ar_PORTB variables with received value
//
//						Feux_ar_PORTB = CAN_circ_buf[CAN_buf_RD].rbuffer_data[0] & FEUX_AR_MSK_OUTPUT_PORTB;
//						break;
//
//							case FEUX_AR_ID_PORTC :
//							// Data Frame for feux_ar, update Feux_ar_PORTC variables with received value
//
//							Feux_ar_PORTC = CAN_circ_buf[CAN_buf_RD].rbuffer_data[0] & FEUX_AR_MSK_OUTPUT_PORTC;
//
//							//Update control input: pos
//							Feux_ar_PORTC = (Feux_ar_PORTC & 0xFD) | ((Feux_ar_PORTC & 0x01) << 1);
//							//Update control input: clignotant, controle on C3
//							Feux_ar_PORTC = (Feux_ar_PORTC & 0xF7) | ((Feux_ar_PORTC & 0x04) << 1);
//						}
//
//					}
//					else
//					{
//						//Bad DLC, sending error
//
//						//Send data frame
//						Frame_To_Send.CAN_message.StdId = 0x5FF;
//						Frame_To_Send.CAN_message.RTR = CAN_RTR_DATA;
//						Frame_To_Send.CAN_message.IDE = CAN_ID_STD;
//						Frame_To_Send.CAN_message.DLC = 0;
//						Frame_To_Send.rbuffer_data[0] = 0;
//						HAL_CAN_AddTxMessage(&hcan1, &Frame_To_Send.CAN_message, Frame_To_Send.rbuffer_data,&pTxMailbox);
//						HAL_Delay(1);
//					}
//
//				}
//				CAN_buf_RD++;
//				if(CAN_buf_RD == BUF_CIRC_SIZE) CAN_buf_RD=0;
//				val_can--;
//			}
//
//
//
//
//void Receive_frame_portiere(void)
//{
//	CAN_frame_Tx Frame_To_Send;
//
//
//				if(CAN_circ_buf[CAN_buf_RD].CAN_message.RTR > 0)
//				{
//					//Send data frame
//					Frame_To_Send.CAN_message.StdId = CAN_circ_buf[CAN_buf_RD].CAN_message.StdId;
//					Frame_To_Send.CAN_message.RTR = CAN_RTR_DATA;
//					Frame_To_Send.CAN_message.IDE = CAN_ID_EXT;
//					Frame_To_Send.CAN_message.DLC = 8;
//					Frame_To_Send.rbuffer_data[0] = Portiere_PORTA;
//					Frame_To_Send.rbuffer_data[1] = Portiere_PORTB;
//					Frame_To_Send.rbuffer_data[2] = Portiere_PORTC;
//					Frame_To_Send.rbuffer_data[3] = Portiere_PORTD;
//					Frame_To_Send.rbuffer_data[4] = Portiere_AN0;
//					Frame_To_Send.rbuffer_data[5] = Portiere_AN1;
//					Frame_To_Send.rbuffer_data[6] = Portiere_AN2;
//					Frame_To_Send.rbuffer_data[7] = Portiere_AN3;
////manque 4-5-6
//					HAL_CAN_AddTxMessage(&hcan1, &Frame_To_Send.CAN_message, Frame_To_Send.rbuffer_data,&pTxMailbox);
//					HAL_Delay(1);
//				}
//				else
//				{
//					//check if DLC is correct
//					if(CAN_circ_buf[CAN_buf_RD].CAN_message.DLC == 2)
//					{
//					// Data Frame for portiere, update portiere_PORTA variables with received value
//
//						switch (CAN_circ_buf[CAN_buf_RD].rbuffer_data[7])
//						{
//							case PORTIERE_ID_PORTA :
//							Portiere_PORTA = CAN_circ_buf[CAN_buf_RD].rbuffer_data[0] & PORTIERE_MSK_OUTPUT_PORTA;
//
//							case PORTIERE_ID_PORTB :
//							Portiere_PORTB = CAN_circ_buf[CAN_buf_RD].rbuffer_data[0] & PORTIERE_MSK_OUTPUT_PORTB;
//							break;
//
//							case PORTIERE_ID_PORTC:
//							Portiere_PORTC = CAN_circ_buf[CAN_buf_RD].rbuffer_data[0] & PORTIERE_MSK_OUTPUT_PORTC;
//							break;
//							case PORTIERE_ID_PORTD:
//							Portiere_PORTD = CAN_circ_buf[CAN_buf_RD].rbuffer_data[0] & PORTIERE_MSK_OUTPUT_PORTD;
//							break;
//							default :
//								// Data Frame for portiere is not allowed, send error frame
//
//							//Send data frame
//							Frame_To_Send.CAN_message.StdId = 0x5FF;
//							Frame_To_Send.CAN_message.RTR = CAN_RTR_DATA;
//							Frame_To_Send.CAN_message.IDE = CAN_ID_STD;
//							Frame_To_Send.CAN_message.DLC = 0;
//							Frame_To_Send.rbuffer_data[0] = 0;
//							HAL_CAN_AddTxMessage(&hcan1, &Frame_To_Send.CAN_message, Frame_To_Send.rbuffer_data,&pTxMailbox);
//							HAL_Delay(1);
//
//						}
//
//
//					}
//					else
//					{
//						//Bad DLC, sending error
//
//						//Send data frame
//						Frame_To_Send.CAN_message.StdId = 0x5FF;
//						Frame_To_Send.CAN_message.RTR = CAN_RTR_DATA;
//						Frame_To_Send.CAN_message.IDE = CAN_ID_STD;
//						Frame_To_Send.CAN_message.DLC = 0;
//						Frame_To_Send.rbuffer_data[0] = 0;
//						HAL_CAN_AddTxMessage(&hcan1, &Frame_To_Send.CAN_message, Frame_To_Send.rbuffer_data,&pTxMailbox);
//						HAL_Delay(1);
//					}
//
//
//				}
//				CAN_buf_RD++;
//				if(CAN_buf_RD == BUF_CIRC_SIZE) CAN_buf_RD=0;
//				val_can--;
//
//}
//
//void Receive_frame_satellite(void)
//{
//	CAN_frame_Tx Frame_To_Send;
//
//				if(CAN_circ_buf[CAN_buf_RD].CAN_message.RTR > 0)
//				{
//					//Send data frame
//					Frame_To_Send.CAN_message.StdId = CAN_circ_buf[CAN_buf_RD].CAN_message.StdId;
//					Frame_To_Send.CAN_message.RTR = CAN_RTR_DATA;
//					Frame_To_Send.CAN_message.IDE = CAN_ID_STD;
//					Frame_To_Send.CAN_message.DLC = 8;
//					Frame_To_Send.rbuffer_data[0] = Satellite_PORTA;
//					Frame_To_Send.rbuffer_data[1] = Satellite_PORTB;
//					Frame_To_Send.rbuffer_data[2] = Satellite_PORTC;
//					Frame_To_Send.rbuffer_data[3] = Satellite_AN0;
//					Frame_To_Send.rbuffer_data[4] = Satellite_AN1;
//					Frame_To_Send.rbuffer_data[5] = Satellite_AN2;
//					Frame_To_Send.rbuffer_data[6] = Satellite_AN3;
//					Frame_To_Send.rbuffer_data[7] = Satellite_AN5;
//					HAL_CAN_AddTxMessage(&hcan1, &Frame_To_Send.CAN_message, Frame_To_Send.rbuffer_data,&pTxMailbox);
//					HAL_Delay(1);
//				}
//
//				else if (CAN_circ_buf[CAN_buf_RD].CAN_message.DLC == 2)
//					if (CAN_circ_buf[CAN_buf_RD].data[1]==SATELLITE_ID_PORTC)
//					{
//					// Data Frame for Satellite, update Satellite_PORTC variables with received value
//
//						Satellite_PORTC = CAN_circ_buf[CAN_buf_RD].rbuffer_data[0] & SATELLITE_MSK_OUTPUT_PORTC;
//					}
//
//				else
//				{
//					// Data Frame for satellite PORTA is not allowed, send error frame
//
//					//Send data frame
//					Frame_To_Send.CAN_message.StdId = 0x5FF;
//					Frame_To_Send.CAN_message.RTR = CAN_RTR_DATA;
//					Frame_To_Send.CAN_message.IDE = CAN_ID_STD;
//					Frame_To_Send.CAN_message.DLC = 0;
//					Frame_To_Send.rbuffer_data[0] = 0;
//					HAL_CAN_AddTxMessage(&hcan1, &Frame_To_Send.CAN_message, Frame_To_Send.rbuffer_data,&pTxMailbox);
//					HAL_Delay(1);
//
//				}
//				CAN_buf_RD++;
//				if(CAN_buf_RD == BUF_CIRC_SIZE) CAN_buf_RD=0;
//				val_can--;
//			}
//
//
//void Receive_frame_comodos(void)
//{
//	CAN_frame_Tx Frame_To_Send;
//
//
//	//Check if received frame is commodo
//
//				if(CAN_circ_buf[CAN_buf_RD].CAN_message.RTR > 0)
//				{
//
//
//					//Send data frame tous les ports a envoyer
//					Frame_To_Send.CAN_message.StdId = CAN_circ_buf[CAN_buf_RD].CAN_message.StdId;
//					Frame_To_Send.CAN_message.RTR = CAN_RTR_DATA;
//					Frame_To_Send.CAN_message.IDE = CAN_ID_STD;
//					Frame_To_Send.CAN_message.DLC = 1;
//
//
//					Frame_To_Send.rbuffer_data[0] = Comodos_PORTA;
//					Frame_To_Send.rbuffer_data[1] = Comodos_PORTB;
//					Frame_To_Send.rbuffer_data[2] = Comodos_PORTC;
//					Frame_To_Send.rbuffer_data[3] = Comodos_AN0;
//					Frame_To_Send.rbuffer_data[4] = Comodos_AN1;
//
//
//
//
//					HAL_CAN_AddTxMessage(&hcan1, &Frame_To_Send.CAN_message, Frame_To_Send.rbuffer_data,&pTxMailbox);
//					HAL_Delay(1);
//				}
//				else
//				{
//					// Data Frame for commodos is not allowed, send error frame
//
//					//Send data frame erreur 5FF
//					Frame_To_Send.CAN_message.StdId = 0x5FF;
//					Frame_To_Send.CAN_message.RTR = CAN_RTR_DATA;
//					Frame_To_Send.CAN_message.IDE = CAN_ID_STD;
//					Frame_To_Send.CAN_message.DLC = 0;
//					Frame_To_Send.rbuffer_data[0] = 0;
//					HAL_CAN_AddTxMessage(&hcan1, &Frame_To_Send.CAN_message, Frame_To_Send.rbuffer_data,&pTxMailbox);
//					HAL_Delay(1);
//
//				}
//				CAN_buf_RD++;
//				if(CAN_buf_RD == BUF_CIRC_SIZE) CAN_buf_RD=0;
//				val_can--;
//			}
//*/
//
///*
//void Init_send_frame(void)
//{
//	CAN_frame_Tx Frame_To_Send;
//*/
//	/**************
//	 * COMODOS
//	 **************/
//
//	//Comodos / PORTA
///*
//	Frame_To_Send.CAN_message.StdId = COMMODOS_IDMSK;
//	Frame_To_Send.CAN_message.RTR = CAN_RTR_DATA;
//	Frame_To_Send.CAN_message.IDE = CAN_ID_STD;
//	Frame_To_Send.CAN_message.DLC = 3;
//	Frame_To_Send.rbuffer_data[0] = 0;
//	Frame_To_Send.rbuffer_data[1] = COMMODOS_MSK_OUTPUT_PORTA;
//	Frame_To_Send.rbuffer_data[7] = COMMODOS_ID_PORTA;
//	HAL_CAN_AddTxMessage(&hcan1, &Frame_To_Send.CAN_message, Frame_To_Send.rbuffer_data,&pTxMailbox);
//	HAL_Delay(1);
//
//	//Comodos / PORTB
//
//	Frame_To_Send.CAN_message.StdId = COMMODOS_IDMSK;
//	Frame_To_Send.CAN_message.RTR = CAN_RTR_DATA;
//	Frame_To_Send.CAN_message.IDE = CAN_ID_STD;
//	Frame_To_Send.CAN_message.DLC = 3;
//	Frame_To_Send.rbuffer_data[0] = 0;
//	Frame_To_Send.rbuffer_data[1] = COMMODOS_MSK_OUTPUT_PORTB;
//	Frame_To_Send.rbuffer_data[7] = COMMODOS_ID_PORTB;
//	HAL_CAN_AddTxMessage(&hcan1, &Frame_To_Send.CAN_message, Frame_To_Send.rbuffer_data,&pTxMailbox);
//	HAL_Delay(1);
//
//	//Comodos / PORTC
//
//	Frame_To_Send.CAN_message.StdId = COMMODOS_IDMSK;
//	Frame_To_Send.CAN_message.RTR = CAN_RTR_DATA;
//	Frame_To_Send.CAN_message.IDE = CAN_ID_STD;
//	Frame_To_Send.CAN_message.DLC = 3;
//	Frame_To_Send.rbuffer_data[0] = 0;
//	Frame_To_Send.rbuffer_data[1] = COMMODOS_MSK_OUTPUT_PORTC;
//	Frame_To_Send.rbuffer_data[7] = COMMODOS_ID_PORTC;
//	HAL_CAN_AddTxMessage(&hcan1, &Frame_To_Send.CAN_message, Frame_To_Send.rbuffer_data,&pTxMailbox);
//	HAL_Delay(1);
//
//	//Comodos / AN0
//
//	Frame_To_Send.CAN_message.StdId = COMMODOS_IDMSK;
//	Frame_To_Send.CAN_message.RTR = CAN_RTR_DATA;
//	Frame_To_Send.CAN_message.IDE = CAN_ID_STD;
//	Frame_To_Send.CAN_message.DLC = 2;
//	Frame_To_Send.rbuffer_data[0] = 0;
//	Frame_To_Send.rbuffer_data[7] = COMMODOS_ID_AN0;
//	HAL_CAN_AddTxMessage(&hcan1, &Frame_To_Send.CAN_message, Frame_To_Send.rbuffer_data,&pTxMailbox);
//	HAL_Delay(1);
//
//	//Comodos / AN1
//
//	Frame_To_Send.CAN_message.StdId = COMMODOS_IDMSK;
//	Frame_To_Send.CAN_message.RTR = CAN_RTR_DATA;
//	Frame_To_Send.CAN_message.IDE = CAN_ID_STD;
//	Frame_To_Send.CAN_message.DLC = 2;
//	Frame_To_Send.rbuffer_data[0] = 0;
//	Frame_To_Send.rbuffer_data[7] = COMMODOS_ID_AN1;
//	HAL_CAN_AddTxMessage(&hcan1, &Frame_To_Send.CAN_message, Frame_To_Send.rbuffer_data,&pTxMailbox);
//	HAL_Delay(1);
//
//
//	/**************
//	 * SATELLITE
//	 **************/
//
//	//SATELLITE / PORTA
///*
//	Frame_To_Send.CAN_message.StdId = SATELLITE_IDMSK;
//	Frame_To_Send.CAN_message.RTR = CAN_RTR_DATA;
//	Frame_To_Send.CAN_message.IDE = CAN_ID_STD;
//	Frame_To_Send.CAN_message.DLC = 3;
//	Frame_To_Send.rbuffer_data[0] = 0;
//	Frame_To_Send.rbuffer_data[1] = SATELLITE_MSK_OUTPUT_PORTA;
//	Frame_To_Send.rbuffer_data[7] = SATELLITE_ID_PORTA;
//
//	HAL_CAN_AddTxMessage(&hcan1, &Frame_To_Send.CAN_message, Frame_To_Send.rbuffer_data,&pTxMailbox);
//	HAL_Delay(1);
//	//SATELLITE / PORTB
//
//	Frame_To_Send.CAN_message.StdId = SATELLITE_IDMSK;
//	Frame_To_Send.CAN_message.RTR = CAN_RTR_DATA;
//	Frame_To_Send.CAN_message.IDE = CAN_ID_STD;
//	Frame_To_Send.CAN_message.DLC = 3;
//	Frame_To_Send.rbuffer_data[0] = 0;
//	Frame_To_Send.rbuffer_data[1] = SATELLITE_MSK_OUTPUT_PORTB;
//	Frame_To_Send.rbuffer_data[7] = SATELLITE_ID_PORTB;
//	HAL_CAN_AddTxMessage(&hcan1, &Frame_To_Send.CAN_message, Frame_To_Send.rbuffer_data,&pTxMailbox);
//	HAL_Delay(1);
//	//SATELLITE / PORTC
//
//	Frame_To_Send.CAN_message.StdId = SATELLITE_IDMSK;
//	Frame_To_Send.CAN_message.RTR = CAN_RTR_DATA;
//	Frame_To_Send.CAN_message.IDE = CAN_ID_STD;
//	Frame_To_Send.CAN_message.DLC = 3;
//	Frame_To_Send.rbuffer_data[0] = 0;
//	Frame_To_Send.rbuffer_data[1] = SATELLITE_MSK_OUTPUT_PORTC;
//	Frame_To_Send.rbuffer_data[7] = SATELLITE_ID_PORTC;
//	HAL_CAN_AddTxMessage(&hcan1, &Frame_To_Send.CAN_message, Frame_To_Send.rbuffer_data,&pTxMailbox);
//	HAL_Delay(1);
//	//SATELLITE / AN0
//
//	Frame_To_Send.CAN_message.StdId = SATELLITE_IDMSK;
//	Frame_To_Send.CAN_message.RTR = CAN_RTR_DATA;
//	Frame_To_Send.CAN_message.IDE = CAN_ID_STD;
//	Frame_To_Send.CAN_message.DLC = 2;
//	Frame_To_Send.rbuffer_data[0] = 0;
//	Frame_To_Send.rbuffer_data[7] = SATELLITE_ID_AN0;
//	HAL_CAN_AddTxMessage(&hcan1, &Frame_To_Send.CAN_message, Frame_To_Send.rbuffer_data,&pTxMailbox);
//	HAL_Delay(1);
//	//SATELLITE / AN1
//
//	Frame_To_Send.CAN_message.StdId = SATELLITE_IDMSK;
//	Frame_To_Send.CAN_message.RTR = CAN_RTR_DATA;
//	Frame_To_Send.CAN_message.IDE = CAN_ID_STD;
//	Frame_To_Send.CAN_message.DLC = 2;
//	Frame_To_Send.rbuffer_data[0] = 0;
//	Frame_To_Send.rbuffer_data[7] = SATELLITE_ID_AN1;
//	HAL_CAN_AddTxMessage(&hcan1, &Frame_To_Send.CAN_message, Frame_To_Send.rbuffer_data,&pTxMailbox);
//	HAL_Delay(1);
//
//	//SATELLITE / AN2
//
//	Frame_To_Send.CAN_message.StdId = SATELLITE_IDMSK;
//	Frame_To_Send.CAN_message.RTR = CAN_RTR_DATA;
//	Frame_To_Send.CAN_message.IDE = CAN_ID_STD;
//	Frame_To_Send.CAN_message.DLC = 2;
//	Frame_To_Send.rbuffer_data[0] = 0;
//	Frame_To_Send.rbuffer_data[7] = SATELLITE_ID_AN2;
//	HAL_CAN_AddTxMessage(&hcan1, &Frame_To_Send.CAN_message, Frame_To_Send.rbuffer_data,&pTxMailbox);
//	HAL_Delay(1);
//
//	//SATELLITE / AN3
//
//	Frame_To_Send.CAN_message.StdId = SATELLITE_IDMSK;
//	Frame_To_Send.CAN_message.RTR = CAN_RTR_DATA;
//	Frame_To_Send.CAN_message.IDE = CAN_ID_STD;
//	Frame_To_Send.CAN_message.DLC = 2;
//	Frame_To_Send.rbuffer_data[0] = 0;
//	Frame_To_Send.rbuffer_data[7] = SATELLITE_ID_AN3;
//	HAL_CAN_AddTxMessage(&hcan1, &Frame_To_Send.CAN_message, Frame_To_Send.rbuffer_data,&pTxMailbox);
//	HAL_Delay(1);
//
//	//SATELLITE / AN5
//
//	Frame_To_Send.CAN_message.StdId = SATELLITE_IDMSK;
//	Frame_To_Send.CAN_message.RTR = CAN_RTR_DATA;
//	Frame_To_Send.CAN_message.IDE = CAN_ID_STD;
//	Frame_To_Send.CAN_message.DLC = 2;
//	Frame_To_Send.rbuffer_data[0] = 0;
//	Frame_To_Send.rbuffer_data[7] = SATELLITE_ID_AN4;
//	HAL_CAN_AddTxMessage(&hcan1, &Frame_To_Send.CAN_message, Frame_To_Send.rbuffer_data,&pTxMailbox);
//	HAL_Delay(1);
//
//	/**************
//	 * FEUX AV
//	 **************/
//
//	//SATELLITE / PORTA
///*
//	Frame_To_Send.CAN_message.StdId = FEUX_AV_IDMSK;
//	Frame_To_Send.CAN_message.RTR = CAN_RTR_DATA;
//	Frame_To_Send.CAN_message.IDE = CAN_ID_STD;
//	Frame_To_Send.CAN_message.DLC = 3;
//	Frame_To_Send.rbuffer_data[0] = FEUX_AV_MSK_INPUT_PORTA;
//	Frame_To_Send.rbuffer_data[1] = FEUX_AV_MSK_OUTPUT_PORTA;
//	Frame_To_Send.rbuffer_data[7] = FEUX_AV_ID_PORTA;
//	HAL_CAN_AddTxMessage(&hcan1, &Frame_To_Send.CAN_message, Frame_To_Send.rbuffer_data,&pTxMailbox);
//	HAL_Delay(1);
//
//	//SATELLITE / PORTB
//
//	Frame_To_Send.CAN_message.StdId = FEUX_AV_IDMSK;
//	Frame_To_Send.CAN_message.RTR = CAN_RTR_DATA;
//	Frame_To_Send.CAN_message.IDE = CAN_ID_STD;
//	Frame_To_Send.CAN_message.DLC = 3;
//	Frame_To_Send.rbuffer_data[0] = FEUX_AV_MSK_INPUT_PORTB;
//	Frame_To_Send.rbuffer_data[1] = FEUX_AV_MSK_OUTPUT_PORTB;
//	Frame_To_Send.rbuffer_data[7] = FEUX_AV_ID_PORTB;
//	HAL_CAN_AddTxMessage(&hcan1, &Frame_To_Send.CAN_message, Frame_To_Send.rbuffer_data,&pTxMailbox);
//	HAL_Delay(1);
//
//	//SATELLITE / PORTC
//
//	Frame_To_Send.CAN_message.StdId = FEUX_AV_IDMSK;
//	Frame_To_Send.CAN_message.RTR = CAN_RTR_DATA;
//	Frame_To_Send.CAN_message.IDE = CAN_ID_STD;
//	Frame_To_Send.CAN_message.DLC = 3;
//	Frame_To_Send.rbuffer_data[0] = FEUX_AV_MSK_INPUT_PORTB;
//	Frame_To_Send.rbuffer_data[1] = FEUX_AV_MSK_OUTPUT_PORTB;
//	Frame_To_Send.rbuffer_data[7] = FEUX_AV_ID_PORTB;
//	HAL_CAN_AddTxMessage(&hcan1, &Frame_To_Send.CAN_message, Frame_To_Send.rbuffer_data,&pTxMailbox);
//	HAL_Delay(1);
//
//	/**************
//	 * FEUX AR
//	 **************/
///*
//	//FEUX AR / PORTA
//
//	Frame_To_Send.CAN_message.StdId = FEUX_AR_IDMSK;
//	Frame_To_Send.CAN_message.RTR = CAN_RTR_DATA;
//	Frame_To_Send.CAN_message.IDE = CAN_ID_STD;
//	Frame_To_Send.CAN_message.DLC = 3;
//	Frame_To_Send.rbuffer_data[0] = FEUX_AR_MSK_INPUT_PORTA;
//	Frame_To_Send.rbuffer_data[1] = FEUX_AR_MSK_OUTPUT_PORTA;
//	Frame_To_Send.rbuffer_data[7] = FEUX_AR_ID_PORTA;
//	HAL_CAN_AddTxMessage(&hcan1, &Frame_To_Send.CAN_message, Frame_To_Send.rbuffer_data,&pTxMailbox);
//	HAL_Delay(1);
//
//	//FEUX AR / PORTB
//
//	Frame_To_Send.CAN_message.StdId = FEUX_AR_IDMSK;
//	Frame_To_Send.CAN_message.RTR = CAN_RTR_DATA;
//	Frame_To_Send.CAN_message.IDE = CAN_ID_STD;
//	Frame_To_Send.CAN_message.DLC = 3;
//	Frame_To_Send.rbuffer_data[0] = FEUX_AR_MSK_INPUT_PORTB;
//	Frame_To_Send.rbuffer_data[1] = FEUX_AR_MSK_OUTPUT_PORTB;
//	Frame_To_Send.rbuffer_data[7] = FEUX_AR_ID_PORTB;
//	HAL_CAN_AddTxMessage(&hcan1, &Frame_To_Send.CAN_message, Frame_To_Send.rbuffer_data,&pTxMailbox);
//	HAL_Delay(1);
//
//	//FEUX AR / PORTC
//
//	Frame_To_Send.CAN_message.StdId = FEUX_AR_IDMSK;
//	Frame_To_Send.CAN_message.RTR = CAN_RTR_DATA;
//	Frame_To_Send.CAN_message.IDE = CAN_ID_STD;
//	Frame_To_Send.CAN_message.DLC = 3;
//	Frame_To_Send.rbuffer_data[0] = FEUX_AR_MSK_INPUT_PORTC;
//	Frame_To_Send.rbuffer_data[1] = FEUX_AR_MSK_OUTPUT_PORTC;
//	Frame_To_Send.rbuffer_data[7] = FEUX_AR_ID_PORTC;
//	HAL_CAN_AddTxMessage(&hcan1, &Frame_To_Send.CAN_message, Frame_To_Send.rbuffer_data,&pTxMailbox);
//	HAL_Delay(1);
//
//	/**************
//	 * PORTIERE
//	 **************/
//
//	//PORTIERE / PORTA
///*
//	Frame_To_Send.CAN_message.StdId = PORTIERE_ID_MSK;
//	Frame_To_Send.CAN_message.RTR = CAN_RTR_DATA;
//	Frame_To_Send.CAN_message.IDE = CAN_ID_STD;
//	Frame_To_Send.CAN_message.DLC = 3;
//	Frame_To_Send.rbuffer_data[0] = PORTIERE_MSK_INPUT_PORTA;
//	Frame_To_Send.rbuffer_data[1] = PORTIERE_MSK_OUTPUT_PORTA;
//	Frame_To_Send.rbuffer_data[7] = PORTIERE_ID_PORTA;
//	HAL_CAN_AddTxMessage(&hcan1, &Frame_To_Send.CAN_message, Frame_To_Send.rbuffer_data,&pTxMailbox);
//	HAL_Delay(1);
//
//	//PORTIERE / PORTB
//
//	Frame_To_Send.CAN_message.StdId = PORTIERE_ID_MSK;
//	Frame_To_Send.CAN_message.RTR = CAN_RTR_DATA;
//	Frame_To_Send.CAN_message.IDE = CAN_ID_STD;
//	Frame_To_Send.CAN_message.DLC = 3;
//	Frame_To_Send.rbuffer_data[0] = PORTIERE_MSK_INPUT_PORTB;
//	Frame_To_Send.rbuffer_data[1] = PORTIERE_MSK_OUTPUT_PORTB;
//	Frame_To_Send.rbuffer_data[7] = PORTIERE_ID_PORTB;
//	HAL_CAN_AddTxMessage(&hcan1, &Frame_To_Send.CAN_message, Frame_To_Send.rbuffer_data,&pTxMailbox);
//	HAL_Delay(1);
//
//	//PORTIERE / PORTC
//
//	Frame_To_Send.CAN_message.StdId = PORTIERE_ID_MSK;
//	Frame_To_Send.CAN_message.RTR = CAN_RTR_DATA;
//	Frame_To_Send.CAN_message.IDE = CAN_ID_STD;
//	Frame_To_Send.CAN_message.DLC = 3;
//	Frame_To_Send.rbuffer_data[0] = PORTIERE_MSK_INPUT_PORTC;
//	Frame_To_Send.rbuffer_data[1] = PORTIERE_MSK_OUTPUT_PORTC;
//	Frame_To_Send.rbuffer_data[7] = PORTIERE_ID_PORTC;
//	HAL_CAN_AddTxMessage(&hcan1, &Frame_To_Send.CAN_message, Frame_To_Send.rbuffer_data,&pTxMailbox);
//	HAL_Delay(1);
//
//	//PORTIERE / AN0
//
//	Frame_To_Send.CAN_message.StdId = PORTIERE_ID_MSK;
//	Frame_To_Send.CAN_message.RTR = CAN_RTR_DATA;
//	Frame_To_Send.CAN_message.IDE = CAN_ID_STD;
//	Frame_To_Send.CAN_message.DLC = 2;
//	Frame_To_Send.rbuffer_data[0] = 0;
//	Frame_To_Send.rbuffer_data[1] = PORTIERE_ID_AN0;
//	HAL_CAN_AddTxMessage(&hcan1, &Frame_To_Send.CAN_message, Frame_To_Send.rbuffer_data,&pTxMailbox);
//	HAL_Delay(1);
//
//	//PORTIERE / AN1
//
//	Frame_To_Send.CAN_message.StdId = PORTIERE_ID_MSK;
//	Frame_To_Send.CAN_message.RTR = CAN_RTR_DATA;
//	Frame_To_Send.CAN_message.IDE = CAN_ID_STD;
//	Frame_To_Send.CAN_message.DLC = 2;
//	Frame_To_Send.rbuffer_data[0] = 0;
//	Frame_To_Send.rbuffer_data[1] = PORTIERE_ID_AN1;
//	HAL_CAN_AddTxMessage(&hcan1, &Frame_To_Send.CAN_message, Frame_To_Send.rbuffer_data,&pTxMailbox);
//	HAL_Delay(1);
//
//	//PORTIERE / AN2
//
//	Frame_To_Send.CAN_message.StdId = PORTIERE_ID_MSK;
//	Frame_To_Send.CAN_message.RTR = CAN_RTR_DATA;
//	Frame_To_Send.CAN_message.IDE = CAN_ID_STD;
//	Frame_To_Send.CAN_message.DLC = 2;
//	Frame_To_Send.rbuffer_data[0] = 0;
//	Frame_To_Send.rbuffer_data[1] = PORTIERE_ID_AN2;
//	HAL_CAN_AddTxMessage(&hcan1, &Frame_To_Send.CAN_message, Frame_To_Send.rbuffer_data,&pTxMailbox);
//	HAL_Delay(1);
//
//	//PORTIERE / AN3
//
//	Frame_To_Send.CAN_message.StdId = PORTIERE_ID_MSK;
//	Frame_To_Send.CAN_message.RTR = CAN_RTR_DATA;
//	Frame_To_Send.CAN_message.IDE = CAN_ID_STD;
//	Frame_To_Send.CAN_message.DLC = 2;
//	Frame_To_Send.rbuffer_data[0] = 0;
//	Frame_To_Send.rbuffer_data[1] = PORTIERE_ID_AN3;
//	HAL_CAN_AddTxMessage(&hcan1, &Frame_To_Send.CAN_message, Frame_To_Send.rbuffer_data,&pTxMailbox);
//	HAL_Delay(1);
//
//	//PORTIERE / AN4
//
//	Frame_To_Send.CAN_message.StdId = PORTIERE_ID_MSK;
//	Frame_To_Send.CAN_message.RTR = CAN_RTR_DATA;
//	Frame_To_Send.CAN_message.IDE = CAN_ID_STD;
//	Frame_To_Send.CAN_message.DLC = 2;
//	Frame_To_Send.rbuffer_data[0] = 0;
//	Frame_To_Send.rbuffer_data[1] = PORTIERE_ID_AN4;
//	HAL_CAN_AddTxMessage(&hcan1, &Frame_To_Send.CAN_message, Frame_To_Send.rbuffer_data,&pTxMailbox);
//	HAL_Delay(1);
//
//	//PORTIERE / AN5
//
//	Frame_To_Send.CAN_message.StdId = PORTIERE_ID_MSK;
//	Frame_To_Send.CAN_message.RTR = CAN_RTR_DATA;
//	Frame_To_Send.CAN_message.IDE = CAN_ID_STD;
//	Frame_To_Send.CAN_message.DLC = 2;
//	Frame_To_Send.rbuffer_data[0] = 0;
//	Frame_To_Send.rbuffer_data[1] = PORTIERE_ID_AN5;
//	HAL_CAN_AddTxMessage(&hcan1, &Frame_To_Send.CAN_message, Frame_To_Send.rbuffer_data,&pTxMailbox);
//	HAL_Delay(1);
//}
//*/
