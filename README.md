# Speech Recognition Data and K&R C Code

This repo contains an implementation of the speech recognition algorithms and data from my undergraduate final year project completed in 1987. The project comprised an 8-channel audio frequency analyser linked to a BBC Micro Model B over the 1MHz bus. The time-critical audio sampling routines and computationally demanding functions were implemented in 6502 Assembly along with some BASIC glue code. The code in this repo is a re-implementation of the speech recognition routines in K&R C.

The project hardware was built around the AD7581, an 8-channel 8-bit analogue-to-digital converter (ADC) from Analog Devices in Co. Limerick. Each ADC channel of the AD7581 was connected to one channel of an op-amp-based 8-channel audio frequency analyser. The 8-channel audio frequency analyser had 8 bandpass channels centred on frequencies 250Hz, 420Hz, 630Hz, 900Hz, 1230Hz, 1620Hz, 2260Hz and 2830Hz with -3db passbands chosen to coincide with the -3db passband of the adjacent channels. These frequencies and passbands were chosen after careful research to maximise the intelligibility of the sampled speech audio signal.

On the microprocessor side, the AD7581 was connected to address, control and data lines on the 6502 using appropriate address decode logic so that it appeared in the microprocessor's address space as 8 consecutive bytes of ROM from &FCC0 to &FCC7. In operation, the microcomputer would sample and record the 8-channels of the ADC every 10ms. In this way, the time evolution of an utterance's frequency profile would be sampled and stored to the microcomputer's RAM.

A single vocabulary consists of the 32 county names of Ireland and Northern Ireland arranged as follows: Cork, Down, Mayo, Cavan, Clare, Derry, Kerry, Laois, Louth, Meath, Sligo, Antrim, Armagh, Carlow, Dublin, Galway, Offaly, Tyrone, Donegal, Kildare, Leitrim, Wexford, Wicklow, Kilkenny, Limerick, Longford, Monaghan, Fermanagh, Roscommon, Tipperary, Waterford, Westmeath.

Speech recognition consists of measuring the *distance* in some metric space between an *unknown* utterance's frequency profile and each of the 32 *reference* utterances' frequency profiles stored in RAM. The reference utterance closest to the unknown utterance according to the distance measure is deemed to be the most likely match.

The data in the file `speech.d` consists of 19 complete vocabularies each with the 32 utterances listed above. The format is 8-bit binary - the first byte in an utterance's data represents the `length` of the utterance (in 10ms frames) and the following `length*8` bytes represents the utterance data itself.

It's a nice illustration of Moore's Law to note that a single speech recognition run comparing all 576 unknowns to the 32 references took about four hours on the 2MHz 6502-based microcomputer, about three hours on a PDP-11/84 minicomputer running 2.11BSD UNIX and about 30 seconds on a Raspberry Pi Model B running Raspbian 7 (wheezy).
