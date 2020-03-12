# Speech Recognition Data and K&R C Code

This repo contains the speech data from my final year project completed in 1987. The code for the project was originally implemented in 6502 Assembler for the computationally intensive routines with some BASIC glue code. The code in this repo consists of a re-implementation of the same speech recognition algorithms in K&R C.

The hardware associated with the project included an AD7581, an 8-channel 8-bit ADC from Analog Devices in Co. Limerick. Each channel of the ADC was connected to one channel of an op-amp-based 8-channel audio frequency analyser. The AD7581 was connected to the appropriate clock, address, data and control lines on the 6502 so that it appeared in the microprocessor's address space as 8 consecutive bytes of ROM. In operation, the microcomputer would sample and record the 8-channels of the ADC every 10ms. In this way, the time evolution of an utterance's frequency profile would be sampled and stored to the microcomputer's RAM.

A single vocabulary consists of the 32 county names of Ireland and Northern Ireland arranged as follows: Cork, Down, Mayo, Cavan, Clare, Derry, Kerry, Laois, Louth, Meath, Sligo, Antrim, Armagh, Carlow, Dublin, Galway, Offaly, Tyrone, Donegal, Kildare, Leitrim, Wexford, Wicklow, Kilkenny, Limerick, Longford, Monaghan, Fermanagh, Roscommon, Tipperary, Waterford, Westmeath.

Speech recognition consists of comparing an *unknown* utterance's frequency profile to the 32 known (or *reference*) utterances' frequency profiles stored in RAM to determine the most probable match.

The data in the file speech.d consists of 19 complete vocabularies each with the 32 utterances listed above. The format is 8-bit binary - the first byte in an utterance's data represents the length (in 10ms frames) of the utterance. Therefore, the following length * 8 bytes represents the utterance data itself.

It's a nice illustration of Moore's Law to note that a full run of the speech recognition routines took several days on the 6502-based microcomputer, about 3 hours on a PDP-11/70 running 2.11BSD, about 30 minutes on a VAX-11/780 running VAX/VMS and about 3 seconds on a Raspberry Pi 3 Model B.
