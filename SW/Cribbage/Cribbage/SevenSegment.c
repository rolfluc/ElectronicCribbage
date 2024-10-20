#include "SevenSegment.h"

void DisplayOnSegment(Segment* sevenSegment, DisplayStates display, bool doShowDP)
{
	GPIO_PinState driveHigh = sevenSegment->IsCommonCathode ? GPIO_PIN_SET : GPIO_PIN_RESET;
	GPIO_PinState driveLow = sevenSegment->IsCommonCathode ? GPIO_PIN_RESET : GPIO_PIN_SET;
	//    A
	//	 ---
	//F |   | B
	//  | G |
	//   ---
	//E |   | C
	//  |   |
	//   ---
	//    D
	
	switch (display)
	{
	case Display_O:
	case Display_0:
		{
			HAL_GPIO_WritePin(sevenSegment->A->pinPort, sevenSegment->A->pinNumber, driveHigh);
			HAL_GPIO_WritePin(sevenSegment->B->pinPort, sevenSegment->B->pinNumber, driveHigh);
			HAL_GPIO_WritePin(sevenSegment->C->pinPort, sevenSegment->C->pinNumber, driveHigh);
			HAL_GPIO_WritePin(sevenSegment->D->pinPort, sevenSegment->D->pinNumber, driveHigh);
			HAL_GPIO_WritePin(sevenSegment->E->pinPort, sevenSegment->E->pinNumber, driveHigh);
			HAL_GPIO_WritePin(sevenSegment->F->pinPort, sevenSegment->F->pinNumber, driveHigh);
			HAL_GPIO_WritePin(sevenSegment->G->pinPort, sevenSegment->G->pinNumber, driveLow);
			//HAL_GPIO_WritePin(sevenSegment->DP->pinPort, sevenSegment->DP->pinNumber, doShowDP ? driveHigh : driveLow);
			break;
		}
	case Display_1:
		{
			HAL_GPIO_WritePin(sevenSegment->A->pinPort, sevenSegment->A->pinNumber, driveLow);
			HAL_GPIO_WritePin(sevenSegment->B->pinPort, sevenSegment->B->pinNumber, driveHigh);
			HAL_GPIO_WritePin(sevenSegment->C->pinPort, sevenSegment->C->pinNumber, driveHigh);
			HAL_GPIO_WritePin(sevenSegment->D->pinPort, sevenSegment->D->pinNumber, driveLow);
			HAL_GPIO_WritePin(sevenSegment->E->pinPort, sevenSegment->E->pinNumber, driveLow);
			HAL_GPIO_WritePin(sevenSegment->F->pinPort, sevenSegment->F->pinNumber, driveLow);
			HAL_GPIO_WritePin(sevenSegment->G->pinPort, sevenSegment->G->pinNumber, driveLow);
			//HAL_GPIO_WritePin(sevenSegment->DP->pinPort, sevenSegment->DP->pinNumber, doShowDP ? driveHigh : driveLow);
			break;
		}
	case Display_2:
		{
			HAL_GPIO_WritePin(sevenSegment->A->pinPort, sevenSegment->A->pinNumber, driveHigh);
			HAL_GPIO_WritePin(sevenSegment->B->pinPort, sevenSegment->B->pinNumber, driveHigh);
			HAL_GPIO_WritePin(sevenSegment->C->pinPort, sevenSegment->C->pinNumber, driveLow);
			HAL_GPIO_WritePin(sevenSegment->D->pinPort, sevenSegment->D->pinNumber, driveHigh);
			HAL_GPIO_WritePin(sevenSegment->E->pinPort, sevenSegment->E->pinNumber, driveHigh);
			HAL_GPIO_WritePin(sevenSegment->F->pinPort, sevenSegment->F->pinNumber, driveLow);
			HAL_GPIO_WritePin(sevenSegment->G->pinPort, sevenSegment->G->pinNumber, driveHigh);
			//HAL_GPIO_WritePin(sevenSegment->DP->pinPort, sevenSegment->DP->pinNumber, doShowDP ? driveHigh : driveLow);
			break;                                 
		}
	case Display_3:
		{
			HAL_GPIO_WritePin(sevenSegment->A->pinPort, sevenSegment->A->pinNumber, driveHigh);
			HAL_GPIO_WritePin(sevenSegment->B->pinPort, sevenSegment->B->pinNumber, driveHigh);
			HAL_GPIO_WritePin(sevenSegment->C->pinPort, sevenSegment->C->pinNumber, driveHigh);
			HAL_GPIO_WritePin(sevenSegment->D->pinPort, sevenSegment->D->pinNumber, driveHigh);
			HAL_GPIO_WritePin(sevenSegment->E->pinPort, sevenSegment->E->pinNumber, driveLow);
			HAL_GPIO_WritePin(sevenSegment->F->pinPort, sevenSegment->F->pinNumber, driveLow);
			HAL_GPIO_WritePin(sevenSegment->G->pinPort, sevenSegment->G->pinNumber, driveHigh);
			//HAL_GPIO_WritePin(sevenSegment->DP->pinPort, sevenSegment->DP->pinNumber, doShowDP ? driveHigh : driveLow);
			break;
		}
	case Display_4:
		{
			HAL_GPIO_WritePin(sevenSegment->A->pinPort, sevenSegment->A->pinNumber, driveLow);
			HAL_GPIO_WritePin(sevenSegment->B->pinPort, sevenSegment->B->pinNumber, driveHigh);
			HAL_GPIO_WritePin(sevenSegment->C->pinPort, sevenSegment->C->pinNumber, driveHigh);
			HAL_GPIO_WritePin(sevenSegment->D->pinPort, sevenSegment->D->pinNumber, driveLow);
			HAL_GPIO_WritePin(sevenSegment->E->pinPort, sevenSegment->E->pinNumber, driveLow);
			HAL_GPIO_WritePin(sevenSegment->F->pinPort, sevenSegment->F->pinNumber, driveHigh);
			HAL_GPIO_WritePin(sevenSegment->G->pinPort, sevenSegment->G->pinNumber, driveHigh);
			//HAL_GPIO_WritePin(sevenSegment->DP->pinPort, sevenSegment->DP->pinNumber, doShowDP ? driveHigh : driveLow);
			break;
		}
	case Display_S:
	case Display_5:
		{
			HAL_GPIO_WritePin(sevenSegment->A->pinPort, sevenSegment->A->pinNumber, driveHigh);
			HAL_GPIO_WritePin(sevenSegment->B->pinPort, sevenSegment->B->pinNumber, driveLow);
			HAL_GPIO_WritePin(sevenSegment->C->pinPort, sevenSegment->C->pinNumber, driveHigh);
			HAL_GPIO_WritePin(sevenSegment->D->pinPort, sevenSegment->D->pinNumber, driveHigh);
			HAL_GPIO_WritePin(sevenSegment->E->pinPort, sevenSegment->E->pinNumber, driveLow);
			HAL_GPIO_WritePin(sevenSegment->F->pinPort, sevenSegment->F->pinNumber, driveHigh);
			HAL_GPIO_WritePin(sevenSegment->G->pinPort, sevenSegment->G->pinNumber, driveHigh);
			//HAL_GPIO_WritePin(sevenSegment->DP->pinPort, sevenSegment->DP->pinNumber, doShowDP ? driveHigh : driveLow);
			break;
		}
	case Display_6:
		{
			HAL_GPIO_WritePin(sevenSegment->A->pinPort, sevenSegment->A->pinNumber, driveHigh);
			HAL_GPIO_WritePin(sevenSegment->B->pinPort, sevenSegment->B->pinNumber, driveLow);
			HAL_GPIO_WritePin(sevenSegment->C->pinPort, sevenSegment->C->pinNumber, driveHigh);
			HAL_GPIO_WritePin(sevenSegment->D->pinPort, sevenSegment->D->pinNumber, driveHigh);
			HAL_GPIO_WritePin(sevenSegment->E->pinPort, sevenSegment->E->pinNumber, driveHigh);
			HAL_GPIO_WritePin(sevenSegment->F->pinPort, sevenSegment->F->pinNumber, driveHigh);
			HAL_GPIO_WritePin(sevenSegment->G->pinPort, sevenSegment->G->pinNumber, driveHigh);
			//HAL_GPIO_WritePin(sevenSegment->DP->pinPort, sevenSegment->DP->pinNumber, doShowDP ? driveHigh : driveLow);
			break;
		}
	case Display_7:
		{
			HAL_GPIO_WritePin(sevenSegment->A->pinPort, sevenSegment->A->pinNumber, driveHigh);
			HAL_GPIO_WritePin(sevenSegment->B->pinPort, sevenSegment->B->pinNumber, driveHigh);
			HAL_GPIO_WritePin(sevenSegment->C->pinPort, sevenSegment->C->pinNumber, driveHigh);
			HAL_GPIO_WritePin(sevenSegment->D->pinPort, sevenSegment->D->pinNumber, driveLow);
			HAL_GPIO_WritePin(sevenSegment->E->pinPort, sevenSegment->E->pinNumber, driveLow);
			HAL_GPIO_WritePin(sevenSegment->F->pinPort, sevenSegment->F->pinNumber, driveLow);
			HAL_GPIO_WritePin(sevenSegment->G->pinPort, sevenSegment->G->pinNumber, driveLow);
			//HAL_GPIO_WritePin(sevenSegment->DP->pinPort, sevenSegment->DP->pinNumber, doShowDP ? driveHigh : driveLow);
			break;
		}
	case Display_8:
		{
			HAL_GPIO_WritePin(sevenSegment->A->pinPort, sevenSegment->A->pinNumber, driveHigh);
			HAL_GPIO_WritePin(sevenSegment->B->pinPort, sevenSegment->B->pinNumber, driveHigh);
			HAL_GPIO_WritePin(sevenSegment->C->pinPort, sevenSegment->C->pinNumber, driveHigh);
			HAL_GPIO_WritePin(sevenSegment->D->pinPort, sevenSegment->D->pinNumber, driveHigh);
			HAL_GPIO_WritePin(sevenSegment->E->pinPort, sevenSegment->E->pinNumber, driveHigh);
			HAL_GPIO_WritePin(sevenSegment->F->pinPort, sevenSegment->F->pinNumber, driveHigh);
			HAL_GPIO_WritePin(sevenSegment->G->pinPort, sevenSegment->G->pinNumber, driveHigh);
			//HAL_GPIO_WritePin(sevenSegment->DP->pinPort, sevenSegment->DP->pinNumber, doShowDP ? driveHigh : driveLow);
			break;
		}
	case Display_9:
		{
			HAL_GPIO_WritePin(sevenSegment->A->pinPort, sevenSegment->A->pinNumber, driveHigh);
			HAL_GPIO_WritePin(sevenSegment->B->pinPort, sevenSegment->B->pinNumber, driveHigh);
			HAL_GPIO_WritePin(sevenSegment->C->pinPort, sevenSegment->C->pinNumber, driveHigh);
			HAL_GPIO_WritePin(sevenSegment->D->pinPort, sevenSegment->D->pinNumber, driveLow);
			HAL_GPIO_WritePin(sevenSegment->E->pinPort, sevenSegment->E->pinNumber, driveLow);
			HAL_GPIO_WritePin(sevenSegment->F->pinPort, sevenSegment->F->pinNumber, driveHigh);
			HAL_GPIO_WritePin(sevenSegment->G->pinPort, sevenSegment->G->pinNumber, driveHigh);
			//HAL_GPIO_WritePin(sevenSegment->DP->pinPort, sevenSegment->DP->pinNumber, doShowDP ? driveHigh : driveLow);
			break;
		}
	case Display_A:
		{
			HAL_GPIO_WritePin(sevenSegment->A->pinPort, sevenSegment->A->pinNumber, driveHigh);
			HAL_GPIO_WritePin(sevenSegment->B->pinPort, sevenSegment->B->pinNumber, driveHigh);
			HAL_GPIO_WritePin(sevenSegment->C->pinPort, sevenSegment->C->pinNumber, driveHigh);
			HAL_GPIO_WritePin(sevenSegment->D->pinPort, sevenSegment->D->pinNumber, driveLow);
			HAL_GPIO_WritePin(sevenSegment->E->pinPort, sevenSegment->E->pinNumber, driveHigh);
			HAL_GPIO_WritePin(sevenSegment->F->pinPort, sevenSegment->F->pinNumber, driveHigh);
			HAL_GPIO_WritePin(sevenSegment->G->pinPort, sevenSegment->G->pinNumber, driveHigh);
			//HAL_GPIO_WritePin(sevenSegment->DP->pinPort, sevenSegment->DP->pinNumber, doShowDP ? driveHigh : driveLow);
			break;
		}
	case Display_b:
		{
			HAL_GPIO_WritePin(sevenSegment->A->pinPort, sevenSegment->A->pinNumber, driveLow);
			HAL_GPIO_WritePin(sevenSegment->B->pinPort, sevenSegment->B->pinNumber, driveLow);
			HAL_GPIO_WritePin(sevenSegment->C->pinPort, sevenSegment->C->pinNumber, driveHigh);
			HAL_GPIO_WritePin(sevenSegment->D->pinPort, sevenSegment->D->pinNumber, driveHigh);
			HAL_GPIO_WritePin(sevenSegment->E->pinPort, sevenSegment->E->pinNumber, driveHigh);
			HAL_GPIO_WritePin(sevenSegment->F->pinPort, sevenSegment->F->pinNumber, driveHigh);
			HAL_GPIO_WritePin(sevenSegment->G->pinPort, sevenSegment->G->pinNumber, driveHigh);
			//HAL_GPIO_WritePin(sevenSegment->DP->pinPort, sevenSegment->DP->pinNumber, doShowDP ? driveHigh : driveLow);
			break;
		}
	case Display_C:
		{
			HAL_GPIO_WritePin(sevenSegment->A->pinPort, sevenSegment->A->pinNumber, driveHigh);
			HAL_GPIO_WritePin(sevenSegment->B->pinPort, sevenSegment->B->pinNumber, driveLow);
			HAL_GPIO_WritePin(sevenSegment->C->pinPort, sevenSegment->C->pinNumber, driveLow);
			HAL_GPIO_WritePin(sevenSegment->D->pinPort, sevenSegment->D->pinNumber, driveHigh);
			HAL_GPIO_WritePin(sevenSegment->E->pinPort, sevenSegment->E->pinNumber, driveHigh);
			HAL_GPIO_WritePin(sevenSegment->F->pinPort, sevenSegment->F->pinNumber, driveHigh);
			HAL_GPIO_WritePin(sevenSegment->G->pinPort, sevenSegment->G->pinNumber, driveLow);
			//HAL_GPIO_WritePin(sevenSegment->DP->pinPort, sevenSegment->DP->pinNumber, doShowDP ? driveHigh : driveLow);
			break;
		}
	case Display_d:
		{
			HAL_GPIO_WritePin(sevenSegment->A->pinPort, sevenSegment->A->pinNumber, driveLow);
			HAL_GPIO_WritePin(sevenSegment->B->pinPort, sevenSegment->B->pinNumber, driveHigh);
			HAL_GPIO_WritePin(sevenSegment->C->pinPort, sevenSegment->C->pinNumber, driveHigh);
			HAL_GPIO_WritePin(sevenSegment->D->pinPort, sevenSegment->D->pinNumber, driveHigh);
			HAL_GPIO_WritePin(sevenSegment->E->pinPort, sevenSegment->E->pinNumber, driveHigh);
			HAL_GPIO_WritePin(sevenSegment->F->pinPort, sevenSegment->F->pinNumber, driveLow);
			HAL_GPIO_WritePin(sevenSegment->G->pinPort, sevenSegment->G->pinNumber, driveHigh);
			//HAL_GPIO_WritePin(sevenSegment->DP->pinPort, sevenSegment->DP->pinNumber, doShowDP ? driveHigh : driveLow);
			break;
		}
	case Display_E:
		{
			HAL_GPIO_WritePin(sevenSegment->A->pinPort, sevenSegment->A->pinNumber, driveHigh);
			HAL_GPIO_WritePin(sevenSegment->B->pinPort, sevenSegment->B->pinNumber, driveLow);
			HAL_GPIO_WritePin(sevenSegment->C->pinPort, sevenSegment->C->pinNumber, driveLow);
			HAL_GPIO_WritePin(sevenSegment->D->pinPort, sevenSegment->D->pinNumber, driveHigh);
			HAL_GPIO_WritePin(sevenSegment->E->pinPort, sevenSegment->E->pinNumber, driveHigh);
			HAL_GPIO_WritePin(sevenSegment->F->pinPort, sevenSegment->F->pinNumber, driveHigh);
			HAL_GPIO_WritePin(sevenSegment->G->pinPort, sevenSegment->G->pinNumber, driveHigh);
			//HAL_GPIO_WritePin(sevenSegment->DP->pinPort, sevenSegment->DP->pinNumber, doShowDP ? driveHigh : driveLow);
			break;
		}
	case Display_F:
		{
			HAL_GPIO_WritePin(sevenSegment->A->pinPort, sevenSegment->A->pinNumber, driveHigh);
			HAL_GPIO_WritePin(sevenSegment->B->pinPort, sevenSegment->B->pinNumber, driveLow);
			HAL_GPIO_WritePin(sevenSegment->C->pinPort, sevenSegment->C->pinNumber, driveLow);
			HAL_GPIO_WritePin(sevenSegment->D->pinPort, sevenSegment->D->pinNumber, driveLow);
			HAL_GPIO_WritePin(sevenSegment->E->pinPort, sevenSegment->E->pinNumber, driveHigh);
			HAL_GPIO_WritePin(sevenSegment->F->pinPort, sevenSegment->F->pinNumber, driveHigh);
			HAL_GPIO_WritePin(sevenSegment->G->pinPort, sevenSegment->G->pinNumber, driveHigh);
			//HAL_GPIO_WritePin(sevenSegment->DP->pinPort, sevenSegment->DP->pinNumber, doShowDP ? driveHigh : driveLow);
			break;
		}
	case Display_H:
		{
			HAL_GPIO_WritePin(sevenSegment->A->pinPort, sevenSegment->A->pinNumber, driveLow);
			HAL_GPIO_WritePin(sevenSegment->B->pinPort, sevenSegment->B->pinNumber, driveHigh);
			HAL_GPIO_WritePin(sevenSegment->C->pinPort, sevenSegment->C->pinNumber, driveHigh);
			HAL_GPIO_WritePin(sevenSegment->D->pinPort, sevenSegment->D->pinNumber, driveLow);
			HAL_GPIO_WritePin(sevenSegment->E->pinPort, sevenSegment->E->pinNumber, driveHigh);
			HAL_GPIO_WritePin(sevenSegment->F->pinPort, sevenSegment->F->pinNumber, driveHigh);
			HAL_GPIO_WritePin(sevenSegment->G->pinPort, sevenSegment->G->pinNumber, driveHigh);
			//HAL_GPIO_WritePin(sevenSegment->DP->pinPort, sevenSegment->DP->pinNumber, doShowDP ? driveHigh : driveLow);
			break;
		}
	case Display_L:
		{
			HAL_GPIO_WritePin(sevenSegment->A->pinPort, sevenSegment->A->pinNumber, driveLow);
			HAL_GPIO_WritePin(sevenSegment->B->pinPort, sevenSegment->B->pinNumber, driveLow);
			HAL_GPIO_WritePin(sevenSegment->C->pinPort, sevenSegment->C->pinNumber, driveLow);
			HAL_GPIO_WritePin(sevenSegment->D->pinPort, sevenSegment->D->pinNumber, driveHigh);
			HAL_GPIO_WritePin(sevenSegment->E->pinPort, sevenSegment->E->pinNumber, driveHigh);
			HAL_GPIO_WritePin(sevenSegment->F->pinPort, sevenSegment->F->pinNumber, driveHigh);
			HAL_GPIO_WritePin(sevenSegment->G->pinPort, sevenSegment->G->pinNumber, driveLow);
			//HAL_GPIO_WritePin(sevenSegment->DP->pinPort, sevenSegment->DP->pinNumber, doShowDP ? driveHigh : driveLow);
			break;
		}
	case Display_P:
		{
			HAL_GPIO_WritePin(sevenSegment->A->pinPort, sevenSegment->A->pinNumber, driveHigh);
			HAL_GPIO_WritePin(sevenSegment->B->pinPort, sevenSegment->B->pinNumber, driveHigh);
			HAL_GPIO_WritePin(sevenSegment->C->pinPort, sevenSegment->C->pinNumber, driveLow);
			HAL_GPIO_WritePin(sevenSegment->D->pinPort, sevenSegment->D->pinNumber, driveLow);
			HAL_GPIO_WritePin(sevenSegment->E->pinPort, sevenSegment->E->pinNumber, driveHigh);
			HAL_GPIO_WritePin(sevenSegment->F->pinPort, sevenSegment->F->pinNumber, driveHigh);
			HAL_GPIO_WritePin(sevenSegment->G->pinPort, sevenSegment->G->pinNumber, driveHigh);
			//HAL_GPIO_WritePin(sevenSegment->DP->pinPort, sevenSegment->DP->pinNumber, doShowDP ? driveHigh : driveLow);
			break;
		}
	case Display_Y:
		{
			HAL_GPIO_WritePin(sevenSegment->A->pinPort, sevenSegment->A->pinNumber, driveLow);
			HAL_GPIO_WritePin(sevenSegment->B->pinPort, sevenSegment->B->pinNumber, driveHigh);
			HAL_GPIO_WritePin(sevenSegment->C->pinPort, sevenSegment->C->pinNumber, driveHigh);
			HAL_GPIO_WritePin(sevenSegment->D->pinPort, sevenSegment->D->pinNumber, driveHigh);
			HAL_GPIO_WritePin(sevenSegment->E->pinPort, sevenSegment->E->pinNumber, driveLow);
			HAL_GPIO_WritePin(sevenSegment->F->pinPort, sevenSegment->F->pinNumber, driveHigh);
			HAL_GPIO_WritePin(sevenSegment->G->pinPort, sevenSegment->G->pinNumber, driveHigh);
			//HAL_GPIO_WritePin(sevenSegment->DP->pinPort, sevenSegment->DP->pinNumber, doShowDP ? driveHigh : driveLow);
			break;
		}
	default:
		break;
	}
	HAL_GPIO_WritePin(sevenSegment->Enable->pinPort, sevenSegment->Enable->pinNumber, GPIO_PIN_SET);
}

void TurnOffSegment(Segment* sevenSegment)
{
	GPIO_PinState driveLow = sevenSegment->IsCommonCathode ? GPIO_PIN_RESET : GPIO_PIN_SET;
	HAL_GPIO_WritePin(sevenSegment->Enable->pinPort, sevenSegment->Enable->pinNumber, GPIO_PIN_RESET);
}