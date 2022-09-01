// Copyright Martin Furuberg. All Rights Reserved.


#include "UI/ThrowWidget.h"


void UThrowWidget::Show(uint8 Number)
{
	checkf(Number > 0 && Number < 7, TEXT("Unexpected number!"));
	OnShow(Number);
}
