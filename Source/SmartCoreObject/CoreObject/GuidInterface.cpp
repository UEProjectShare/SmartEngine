#include "GuidInterface.h"

IGuidInterface::IGuidInterface()
{
	create_guid(&Guid);
}

string IGuidInterface::GetGuidString() const
{
	char Buff[128] = { 0 };
	guid_to_string(Buff, &Guid);

	return Buff;
}