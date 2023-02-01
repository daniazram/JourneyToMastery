#include "ComboItem.h"

bool FComboItem::IsValid()
{
	return Montage != nullptr;
}

bool FComboItem::operator==(const FComboItem OtherItem)
{
	return this->Montage == OtherItem.Montage && this->InputAction == OtherItem.InputAction;
}
