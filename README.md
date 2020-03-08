# speech
Speech recognition data and codes

This repo contains the speech data from my final year project undertaken in 1987. The code for the project was originally implemented in a combination of BASIC and 6502 Assembler. The code in this repo consists of a re-implementation of the same speech recognition algorithms in K&R C.

The hardware associated with the project consisted of an AD7581, an 8-bit 8-channel ADC by Analog Devices in Co. Limerick. Each channel of the ADC was connected to one channel of an 8-channel bandpass audio frequency analyser. In operation, a 6502-based microcomputer would sample and record the 8-channels of the ADC every 10ms. In this way, the evolution of an utterance's frequency profile over time would be sampled and stored to the microcomputer's RAM.

Speech recognition consists of comparing an *unknown* utterance's frequency profile against the 32 known (or *reference*) utterances' frequency profiles stored in RAM to determine the most probable match.

A single vocabulary consists of the 32 county names of Ireland and Northern Ireland arranged as follows: Cork, Down, Mayo, Cavan, Clare, Derry, Kerry, Laois, Louth, Meath, Sligo, Antrim, Armagh, Carlow, Dublin, Galway, Offaly, Tyrone, Donegal, Kildare, Leitrim, Wexford, Wicklow, Kilkenny, Limerick, Longford, Monaghan, Fermanagh, Roscommon, Tipperary, Waterford, Westmeath.

The data in speech.d consists of 18 complete vocabularies each with the 32 utterances listed above. The format is 8-bit binary - the first byte in an utterance's data represents the length (in 10ms frames) of the utterance. Therefore, the following length * 8 bytes represents the utterance speech data itself.
