/*
 * serial_com.c
 *
 *  Created on: 15.06.2019
 *	Rework of the 2009 avr_usart.h driver converted into a platform independent driver
 *  Author: Bogdan Tarnauca		
 */

#include "serial_com.h"


void abtSerTimeoutTick(SerialCom_t* serialCom) {
	//uart_nValoareTimeout - Daca are valoare 0 inseamna ca timeout-ul la nivel de telegrama
	//este dezactivat. Aceasta functionalitate este necesara atunci cand sistemul este
	//un slave si asteapta comenzi de la un sistem master. Momentul la care apar comenzile
	//nu este cunoscut, asadar sistemul poate astepta la nesfarsit fara a semnala time-out

	if (serialCom->rxTransferTimerCounting && serialCom->rxTransferTimeoutTicks != 0) {
		serialCom->rxTrasnferTimeoutTickCounter++;

		if (serialCom->rxTrasnferTimeoutTickCounter >= serialCom->rxTransferTimeoutTicks) {
			serialCom->rxTransferTimeout = true;
			serialCom->rxOngoing = false;
			serialCom->unprocessedState = true;
			serialCom->rxTransferTimerCounting = false;
		}
	}

	if (serialCom->rxCharTimerCounting && serialCom->rxCharTimeoutTicks != 0) {
		serialCom->rxCharTimeoutTickCounter++;

		if (serialCom->rxCharTimeoutTickCounter >= serialCom->rxCharTimeoutTicks) {
			serialCom->rxCharTimeout = true;
			serialCom->rxOngoing = false;
			serialCom->unprocessedState = true;
			serialCom->rxCharTimerCounting = false;
		}
	}
}

/******************************************************************************/
void abtSerCommInit(SerialCom_t* serialCom, SerialComInitStr_t* serialComInitStr){

    serialCom->halUsartHandle = serialComInitStr->halUsartHandle;
	serialCom->extUsartHandle = serialComInitStr->extUsartHandle;
    
    //serialCom->usartIndex = serialComInitStr->usartIndex;
	serialCom->rxCounter = 0;
	serialCom->rxLength = 0;
	serialCom->rxBufferSize = 1;

	serialCom->rxTransferTimeoutTicks = serialComInitStr->rxTransferTimeoutTicks;
	serialCom->rxCharTimeoutTicks = serialComInitStr->rxCharTimeoutTicks;

	serialCom->txOngoing = false;
	serialCom->rxOngoing = false;
	serialCom->hwError = false;
	serialCom->rxBufferLimitError = false;
	serialCom->rxCharTimeout = false;
	serialCom->rxTransferTimeout = false;
	serialCom->rxCharTimerCounting = false;
	serialCom->rxTransferTimerCounting = false;
	serialCom->unprocessedState = false;

	//Daca pointerul catre portul care selecteaza directia de transmisie
	//pe RS485 este NULL se considera ca transmisia se face prin RS232 si ca
	//atare nu mai este necesara selectarea directiei driverului
    serialCom->switchDriverToReceiveMode = serialComInitStr->switchDriverToReceiveMode;
    serialCom->switchDriverToTransmitMode = serialComInitStr->switchDriverToTransmitMode;
	serialCom->usingDriverDirectionControl = serialComInitStr->usingDriverDirectionControl;
}

/******************************************************************************/

void abtSerTxDataRegisterEmtpyHandler(SerialCom_t* serialCom) {
	abtSerWatchdogReset();
	serialCom->txCounter++;

	if (serialCom->txCounter == serialCom->txLength) {
		abtSerClearTxDataRegisterEmptyFlag(serialCom);
	} else {
		abtSerSendChar(serialCom, *(serialCom->txBufferPtr + serialCom->txCounter));
	}

}

/******************************************************************************/
void abtSerTxDataSentHandler(SerialCom_t* serialCom) {
	abtSerWatchdogReset();

	//character has been transmitted
	if (serialCom->txCounter == serialCom->txLength) {
		serialCom->txComplete = true;
		serialCom->txOngoing = false; //Nu mai transmit

		//In momentul in care am transmis (prin RS-485) ultimul caracter din buffer
		//trec driverul in regim de ascultare schimband directia acestuia
		//resetez pinul de directie
		//
		// ATENTIE !!!
		//
		// Daca semnalul de selectie a directiei este inversat de catre optocuplor
		//comanda pentru stabilirea directiei este data in mod corespunzator in functie
		//de parametrul stuart_1.bRS485Inversat
		if (serialCom->usingDriverDirectionControl && serialCom->switchDriverToReceiveMode != NULL) {
			serialCom->switchDriverToReceiveMode(serialCom);
		}
	}
}

/******************************************************************************/
void abtSerRxDataReceivedHandler(SerialCom_t* serialCom, uint8_t receivedChar) {
	serialCom->rxCharTimerCounting = true;
	serialCom->rxCharTimeoutTickCounter = 0;
	//Am receptionat un caracter pornesc timerul ca sa verific daca
	//pauze intre caractere - conform protocolului STOM in cadrul
	//unei telegrame nu pot aparea pauze de transmisie intre doua
	//caractere succesive. O astfel de pauza indica faptul ca
	//o telegrama (indiferent de emitatorul ei) s-a incheiat.
	//Corelata cu numarul de caractere receptionate si cu pozitia
	//acestora se poate obtine un criteriu de rejectie a telegramelor
	//care sunt transmise pe seriala dar care nu sunt considerate
	//ca fiind valide.

	abtSerWatchdogReset();
	//uart has received a character in UDR
	if (serialCom->rxOngoing) {
		*(serialCom->rxBufferPtr + serialCom->rxCounter) = receivedChar;

		if (serialCom->rxCounter < serialCom->rxBufferSize) {
			serialCom->rxCounter++;
			//Incrementez numai daca sunt in domeniul de memorie alocat
			//previn astfel suprascrierile
		} else {
			serialCom->rxCounter = serialCom->rxBufferSize;//ca sa iau si ultimul caracter
			serialCom->rxBufferLimitError = true;
			serialCom->rxOngoing = false;
			serialCom->unprocessedState = true;
			serialCom->rxTransferTimerCounting = false;
		}
	}
}


void abtSerLockTransferOnHWError(SerialCom_t* serialCom){
	serialCom->rxOngoing = false;
	serialCom->txOngoing = false;
	serialCom->rxCharTimerCounting = false;
	serialCom->rxTrasnferTimeoutTickCounter = false;
	serialCom->hwError = true;
	serialCom->unprocessedState = true;
}


/******************************************************************************/
void abtSerTransmitData(SerialCom_t* serialCom, uint8_t *txDataPtr,
		uint16_t txDataLength, bool waitOngoingTransmission,
		bool blockUntilTransmissionFinished) {
	abtSerWatchdogReset();
	if (txDataLength > 0) {
		if ((waitOngoingTransmission == true)
				&& (serialCom->txOngoing == true)) {
			do {	//Asteapta transmisia anterrioare
				abtSerWatchdogReset();
			} while (serialCom->txOngoing);
		}
		serialCom->txOngoing = true;
		serialCom->txBufferPtr = txDataPtr;
		//Memorez adresa primei locatii din bufferul de transmisie
		serialCom->txCounter = 0;
		//Resetez contorul care numara caracterele transmise
		serialCom->txComplete = false;
		//resetez flagul care indica ca trasmiterea caracterelor din buffer a fost
		//realizata
		serialCom->txLength = txDataLength;

		serialCom->hwError = false;

		//Daca transmitz prin RS-232 trebuie sa setez directia de transmisie.
		//Directia este resetata (ascultam) in momentul in care am transmis ultimul
		//caracter din buffer
		// ATENTIE !!!
		//
		// Daca semnalul de selectie a directiei este inversat de catre optocuplor
		//comanda pentru stabilirea directiei este data in mod corespunzator in functie
		//de parametrul stserialCom->uart0_1.bRS485Inversat

		if (serialCom->usingDriverDirectionControl && serialCom->switchDriverToTransmitMode != NULL) {
			serialCom->switchDriverToTransmitMode(serialCom);
		}

		abtSerSendChar(serialCom, *txDataPtr);//Transmit primul caracter din buffer

		if ((blockUntilTransmissionFinished == true)
				&& (serialCom->txOngoing == true)) {
			do {		  //Blocheaza metoda pana ce se transmite mesajul curent
				abtSerWatchdogReset();
			} while (serialCom->txOngoing);
		}
	}
}

/*****************************************************************************/

/*****************************************************************************/

//Rutina care imi verifica daca s-au inteplinit criteriiile de sfarsit de receptie
//Returneaza true daca receptia s-a inceiat false daca inca se mai asteapta caractere pe seriala.
bool abtSerEvaluateReceptionStatus(SerialCom_t* serialCom) {

	uint8_t i;
	uint16_t nPozitie, nTamponContorRX;
	abtSerWatchdogReset();

	nTamponContorRX = serialCom->rxCounter;

	for (nPozitie = serialCom->lastTestedIndex;
			nPozitie < nTamponContorRX; nPozitie++) {
		abtSerWatchdogReset();

		//--------------------------------------
		if (serialCom->rxTestCritertiaCount != 0) {//Receptie cu mai multe criterii de stop ((caracter terminator SI lungime))
													//Corespunde apelului serialCom->uart0_ReceptieLungSiTermTX
			for (i = 0; i < serialCom->rxTestCritertiaCount; i++) {//iterez printere criteriile de test.
				abtSerWatchdogReset();

				if (*(serialCom->rxBufferPtr + nPozitie)
						== *(serialCom->multiTerminalCharPtr + i)) {//daca caracterul receptionat se gaseste in lista caracterelor
																  //terminatoare testez daca se gaseste pe pozitia
																  //corespunzatoare
					if (*(serialCom->multiLengthRxPtr + i) == 0) {	//Daca pozitia  asignata a fost 0 inseamna ca de fapt nu ma
																		//intereseaza pozitia in care apare caracterul. si ca atare
																		//receptia este OK

						serialCom->rxLength = nPozitie + 1; //bufer dublu practic
						serialCom->rxTransferTimerCounting = false;

						//Daca nu blochez receptia la finalul receptiei unei telegrame
						//(situatia intalnita in protocoalele in care se receptioneaza headerul
						//iar apoi se da comanda pentru receptia blocului de date) atunci voi
						//mentine activ mecanismul de timeout la nivel de caracter pana cand
						//o comanda (de obicei cea pentru receptia bloclui de date) care se
						//bloceze receptia la final este executata.
						//Astfel pot sa detectez timeout-ul care apare daca dupa header nu a
						//inceput receptia blocului de date.
						if (serialCom->blockReceptionOnFinish == true)
							serialCom->rxCharTimerCounting = false;
						//Nu merge asa (mai jos) pentru toate cazurile deoarece caompilatorul da
						//urmatoarea eroare: internal error -- out of registers: opsize 2 free 300f0003 local cf0003fc free&local 0 TN type 5 ticks 8880.
						//serialCom->uart0_bTimerCarCounting = !serialCom->uart0_bBlocheazaReceptiaLaFinal;

						i = serialCom->rxTestCritertiaCount; //ies din ciclul de testare
						nPozitie = nTamponContorRX;	//ies din ciclul de testare mare ??
						serialCom->rxComplete = true;
						serialCom->unprocessedState = true;
						serialCom->rxOngoing =
								!serialCom->blockReceptionOnFinish;
						serialCom->rxTrasnferTimeoutTickCounter = 0;
					} else {//Daca pozitia  asignata a fost diferita de 0 inseamna ca ma
							//intereseaza pozitia in care apare caracterul si ca atare mai fac
							//un test in acest sens

						//ATENTIE - asa a fost testat si era stabil
						if (nTamponContorRX
								== *(serialCom->multiLengthRxPtr + i))

								//Asa se pare ca da erori
								//if(nPozitie + 1 == *(serialCom->uart0_sMultiLungimeRX + i))

								{//caracterul terminatort citit este la pozitisa dorita
							serialCom->rxLength = nPozitie + 1; //bufer dublu practic
							serialCom->rxTransferTimerCounting = false;

							//Daca nu blochez receptia la finalul receptiei unei telegrame
							//(situatia intalnita in protocoalele in care se receptioneaza headerul
							//iar apoi se da comanda pentru receptia blocului de date) atunci voi
							//mentine activ mecanismul de timeout la nivel de caracter pana cand
							//o comanda (de obicei cea pentru receptia bloclui de date) care se
							//bloceze receptia la final este executata.
							//Astfel pot sa detectez timeout-ul care apare daca dupa header nu a
							//inceput receptia blocului de date.
							if (serialCom->blockReceptionOnFinish
									== true)
								serialCom->rxCharTimerCounting = false;

							i = serialCom->rxTestCritertiaCount; //ies din ciclul de testare mic
							nPozitie = nTamponContorRX;	//ies din ciclul de testare mare
							serialCom->rxComplete = true;
							serialCom->unprocessedState = true;
							serialCom->rxOngoing =
									!serialCom->blockReceptionOnFinish;
							serialCom->rxTrasnferTimeoutTickCounter = 0;
						}
					}
				} //end if
			} //end for
		} //end if
		else { //Receptie cu un singur criteriu de stop (caracter terminator SAU lungime)
			   //Corespunde apelului serialCom->uart0_ReceptieTermTX sau serialCom->uart0_ReceptieLunigme
			if (serialCom->terminalCharBasedReception) { //Receptie pana la intalnirea caracterului terminator
				if (*(serialCom->rxBufferPtr + nPozitie)
						== serialCom->rxTerminalChar) {
					serialCom->rxLength = nPozitie + 1;
					serialCom->rxTransferTimerCounting = false;

					//Daca nu blochez receptia la finalul receptiei unei telegrame
					//(situatia intalnita in protocoalele in care se receptioneaza headerul
					//iar apoi se da comanda pentru receptia blocului de date) atunci voi
					//mentine activ mecanismul de timeout la nivel de caracter pana cand
					//o comanda (de obicei cea pentru receptia bloclui de date) care se
					//bloceze receptia la final este executata.
					//Astfel pot sa detectez timeout-ul care apare daca dupa header nu a
					//inceput receptia blocului de date.
					if (serialCom->blockReceptionOnFinish == true)
						serialCom->rxCharTimerCounting = false;

					i = serialCom->rxTestCritertiaCount; //ies din ciclul de testare
					serialCom->rxComplete = true;
					serialCom->unprocessedState = true;
					serialCom->rxOngoing =
							!serialCom->blockReceptionOnFinish;
					serialCom->rxTrasnferTimeoutTickCounter = 0;
				}
			} else { //Receptia unui numar fix de caractere
				if (nTamponContorRX >= serialCom->rxLength) {
					//serialCom->uart0_nLungimeRX = nTampoContorRX; NU se aplica deaorece stiu dinainte lungimea
					serialCom->rxTransferTimerCounting = false;

					//Daca nu blochez receptia la finalul receptiei unei telegrame
					//(situatia intalnita in protocoalele in care se receptioneaza headerul
					//iar apoi se da comanda pentru receptia blocului de date) atunci voi
					//mentine activ mecanismul de timeout la nivel de caracter pana cand
					//o comanda (de obicei cea pentru receptia bloclui de date) care se
					//bloceze receptia la final este executata.
					//Astfel pot sa detectez timeout-ul care apare daca dupa header nu a
					//inceput receptia blocului de date.
					if (serialCom->blockReceptionOnFinish == true)
						serialCom->rxCharTimerCounting = false;

					i = serialCom->rxTestCritertiaCount; //ies din ciclul de testare
					serialCom->rxComplete = true;
					serialCom->unprocessedState = true;
					serialCom->rxOngoing =
							!serialCom->blockReceptionOnFinish;
					serialCom->rxTrasnferTimeoutTickCounter = 0;
				}
			}
		}
	}
	serialCom->lastTestedIndex = nTamponContorRX;
	return serialCom->rxComplete;
}

/******************************************************************************/
void fixedLengthReception(SerialCom_t *serialCom, uint8_t *rxDataPtr,
		uint16_t rxDataLength, uint16_t rxBufferSize,
		bool resetBuffer,
		bool blockReceptionOnFinish) {
	abtSerWatchdogReset();

	if (serialCom->usingDriverDirectionControl && serialCom->switchDriverToReceiveMode != NULL) {
		serialCom->switchDriverToReceiveMode(serialCom);
	}

	serialCom->ongoingParameterSetting = true;
	serialCom->rxBufferPtr = rxDataPtr;
	serialCom->rxLength = rxDataLength;
	serialCom->rxTestCritertiaCount = 0;
	if (resetBuffer)
		serialCom->rxCounter = 0;
	serialCom->rxBufferSize = rxBufferSize;
	//Setez lungimea maxima a bufferului de receptie. Este utila in special in cazul
	//in care fac receptie cu terminator, dar si in cazul de fata este utila mai
	//ales pentru receptia in buffere multiple cu dimensiuni diferite
	//caracterul terminator nu este receptionat eliminandu-se riscul suprascrierii
	//unei zone de memorire nealocata.
	serialCom->rxComplete = false;
	serialCom->unprocessedState = false;
	serialCom->terminalCharBasedReception = false;
	//indic faptul ca receptia NU se face  pana la intalnirea unui caracter
	//terminator ci pana cand e receptionat un anumit numar de caractere
	serialCom->hwError = false;
	serialCom->rxBufferLimitError = false;
	serialCom->lastTestedIndex = 0;
	serialCom->rxCharTimerCounting = false;
	serialCom->rxTransferTimeout = false;
	serialCom->rxTrasnferTimeoutTickCounter = 0;
	serialCom->rxTransferTimerCounting = true;
	serialCom->rxOngoing = true;
	serialCom->blockReceptionOnFinish = blockReceptionOnFinish;
	serialCom->ongoingParameterSetting = false;
}

/******************************************************************************/

/*
 sReceptionat - receive buffer
 cTermRX - terminal character. Reception ends when the terminal characher is received or the buffer length is reached.
 nLungMaxBuferRX - max size of the input buffer
 bReseteazaBuffIntrare
 true = resets the internal counter to 0 when a receive command is issued, typical approach when an input message is expected to be received in one shot
 false = does not reset the counter and the buffer is written from the last recorder position + 1. This is useful when receiving messages in multi-stages (e.g. header and body) and when the header requires its own processing.
 bBlocheazaReceptiaLaFinal = blocks the method call until the a message is received. Usually not the case, since this would block the main loop. Typically set to false and poling is performed to check the if a message has been received using Receptie_OK()...
 */

void abtSerTerminalCharReception(SerialCom_t* serialCom, uint8_t *rxDataPtr,
		uint8_t terminalChar, uint16_t rxBufferSize,
		bool resetBuffer,
		bool blockReceptionOnFinish) {
	abtSerWatchdogReset();
	serialCom->ongoingParameterSetting = true;
	serialCom->rxBufferPtr = rxDataPtr;
	serialCom->rxLength = 0;
	serialCom->rxTerminalChar = terminalChar;
	serialCom->rxTestCritertiaCount = 0;
	if (resetBuffer)
		serialCom->rxCounter = 0;
	serialCom->rxBufferSize = rxBufferSize;
	//Setez lungimea maxima a bufferului de receptie. Este utila in cazul in care
	//caracterul terminator nu este receptionat eliminandu-se riscul suprascrierii
	//unei zone de memorire nealocata.
	serialCom->rxComplete = false;
	serialCom->unprocessedState = false;
	serialCom->terminalCharBasedReception = true;
	//indic faptul ca receptia NU se face  pana la intalnirea unui caracter
	//terminator ci pana cand e receptionat un anumit numar de caractere
	serialCom->hwError = false;
	serialCom->rxBufferLimitError = false;
	serialCom->lastTestedIndex = 0;
	serialCom->rxCharTimerCounting = false;
	serialCom->rxTransferTimeout = false;
	serialCom->rxTrasnferTimeoutTickCounter = 0;
	serialCom->rxTransferTimerCounting = true;
	serialCom->rxOngoing = true;
	serialCom->blockReceptionOnFinish = blockReceptionOnFinish;
	serialCom->ongoingParameterSetting = false;
}


/******************************************************************************/
void abtSerMultiCriteriaReception(SerialCom_t* serialCom, uint8_t *rxDataPtr,
		uint8_t *sMultiTermRX, uint16_t *sMultiLungimeRX, uint8_t rxTestCriteriaCount,
		uint16_t rxBufferSize,
		bool resetBuffer,
		bool blockReceptionOnFinish) {

	//Se receptioneaza caractere pana cand unul se gaseste in sirul de caractere
	//terminatoare posibile (stabilite in *sTermRX)iar sirul receptionat are exact
	//lungimea stabilita in *sLungimi la acelasi index ca si caracterul terminator
	//pana cand CReceptionat == sTerm[i] && LungimeSir == sLungimi[i]
	//i=:1,nCriteriiTest  nCriteriiTest(1...255) cand nCriteriiTest=0 receptia
	//se face cu un singur test fie de caracter terminator fie de lungime (rutinele
	//uart#_ReceptieTermTX si uart#_ReceptieLunigme)
	//(de uart_nCriteriiTestRX ori se testeaza daca caracterul recptionat se
	//gaseste in sirul de caractere terminatoare si daca acest caracter
	//se gaseste la pozitia dorita.
	//Functia este utila atunci cand avem de receptionat mai multe siruri
	//de caractere cu lungimi fixe avand caractere terminatoare diferite
	//si mai ales atunci cand aceste caractere terminatoare se pot gasi
	//si in partea utila a sirului receptionat fara a avea insa semnificatia
	//de terminator
	//OBSERVATIE!!!
	//Daca in tabelul cu lungimi avem pozitii in care aceasta valoare este
	//0 atunci pentru caracterul corespunzator (de pe aceeasi pozitie) din
	//tabelul caracterelor terminatoare receptia se va face pana la prima
	//aparitie a caracterului fara a fi testata si pozitia acestuia - Se
	//implementeaza practic aceeasi functionalitate ca si in cazul rutinei
	//uart#_ReceptieTermTX

	/*
	 Exemplu:

	 char sTermRX[32]="%&/";
	 char sPoz[32]={1,0,7};

	 uart1_ReceptieLungSiTermTX(sSirA,sTermRX, sPoz, 3, 255);

	 stserialCom->uart0_1.bRxOK va fi setat atunci cand fie primul caracter introdus
	 este '%' fie cand a fost receptionat '&' indiferent de pozitie fie cand
	 al saptelea caracter receptionat a fost '/'
	 */

	abtSerWatchdogReset();

	if (serialCom->usingDriverDirectionControl && serialCom->switchDriverToReceiveMode != NULL) {
		serialCom->switchDriverToReceiveMode(serialCom);
	}

	serialCom->ongoingParameterSetting = true;
	serialCom->rxBufferPtr = rxDataPtr;
	serialCom->rxLength = 0;
	serialCom->rxTestCritertiaCount = rxTestCriteriaCount;
	serialCom->multiTerminalCharPtr = sMultiTermRX;
	serialCom->multiLengthRxPtr = sMultiLungimeRX;
	if (resetBuffer)
		serialCom->rxCounter = 0;
	serialCom->rxBufferSize = rxBufferSize;
	//Setez lungimea maxima a bufferului de receptie. Este utila in cazul in care
	//caracterul terminator nu este receptionat eliminandu-se riscul suprascrierii
	//unei zone de memorire nealocata.
	serialCom->rxComplete = false;
	serialCom->unprocessedState = false;
	serialCom->terminalCharBasedReception = true;
	//indic faptul ca receptia NU se face  pana la intalnirea unui caracter
	//terminator ci pana cand e receptionat un anumit numar de caractere
	serialCom->hwError = false;
	serialCom->rxBufferLimitError = false;
	serialCom->lastTestedIndex = 0;
	serialCom->rxCharTimerCounting = false;
	serialCom->rxTransferTimeout = false;
	serialCom->rxTrasnferTimeoutTickCounter = 0;
	serialCom->rxTransferTimerCounting = true;
	serialCom->rxOngoing = true;
	serialCom->blockReceptionOnFinish = blockReceptionOnFinish;
	serialCom->ongoingParameterSetting = false;
}
