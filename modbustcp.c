#include "modbusap.h"
#include "modbustcp.h"

#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>


uint16_t TI=0;

int cConnect(client_add,port);
int cDisconnect(fd);

int Send_modbus_request(char server_add[256], int port, uint8_t* APDU, uint8_t *APDU_R, uint8_t *APDU_Rlen, uint16_t APDU_len)
{
	uint8_t MBAP_PDUR[7]
	int MBAPDUlen=APDU_len+7, MBAPDU_Rlen, sock, PDU_send;

    //len mensagem + 7 (7 bytes do MBAP - para preencher depois)
	uint8_t MBAPDU[MBAPDUlen];
	uint8_t MBAPDU_R[MBAPDU_Rlen];
	uint16_t len=(uint16_t)(APDU_len+1),transaction_rcv,protocol_rcv,len_rcv;
	uint8_t unit_rcv; //tamanho mensagem de cima + 1 byte para a posicao 6 (stores the unit identifier of the current pdu)
	
	MBAPDU[0]=(uint8_t)(TI>>8); 		//transaction ID, começa em 0 e vai incrementando, para o caso de haver varias mensagens, responder pela ordem correta e à mensagem correta
	//TI HIGH
    MBAPDU[1]=(uint8_t)(TI&0x00FF); 	//TI 16 bits, é preciso dividir em 8+8   //TI LOW
	MBAPDU[2]=0; 						//mostra tipo de protocolo, tem de ser 0 
	MBAPDU[3]=0; 						//o mesmo do bit anterior 
	MBAPDU[4]=(uint8_t)(len>>8);  		//len 16 bits, dividir 8+8, len HIGH
	MBAPDU[5]=(uint8_t)(len&0x00FF);  	//len LOW
	MBAPDU[6]=1; 						//stores the unit identifier of the current pdu

    for(int i=0; i<APDU_len; i++)
	{
		MBAPDU[i+7]=APDU[i]; 			//guarda a partir do bit 7 o apdu que veio da mensagem da camada de cima
	}

    TI++;

	
/********opens TCP client socket and connects to server  (*)***********/

    sock=cConnect("127.0.0.1",port);

	// write (fd, PDU, PDUlen) // sends Modbus TCP PDU
	send(sock,MBAPDU,MBAPDUlen,0);

	// read (fd, PDU_R, PDU_Rlen) // response o timeout
	recv(sock,MBAP_PDUR,7,0); //guarda o header da resposta 
	
	transaction_rcv=(MBAP_PDUR[0] << 8) + MBAP_PDUR[1]; //junta 2 variaveis de 8 bits numa so variavel de 16 bits (TI) MBAP_PDUR[0] é o [HIGH] e o MBAP_PDUR[1] é o [LOW] 
	protocol_rcv=(MBAP_PDUR[2] << 8) + MBAP_PDUR[3]; //junta 2 variaveis de 8 bits numa so variavel de 16 bits (00)
	len_rcv=(MBAP_PDUR[4] << 8) + MBAP_PDUR[5]; //junta 2 variaveis de 8 bits numa so variavel de 16 bits (tamanho)
	unit_rcv=MBAP_PDUR[6];
    
	len_rcv = len_rcv -1; 	//The length field is a byte count of the following fields, including the Unit Identifier and data fields									
  
	recv(sock,APDU_R,len_rcv,0); 

	cDisconnect(sock);

	return 0;                                                               
}


int cConnect(client_add,port)
{	
	int so = socket(PF_INET, SOCK_STREAM, 0);

	serv.sin_family = AF_INET;
	serv.sin_port = htons(port);
	inet_aton(client_add,&serv.sin_addr);
	
	connect(so, (struct sockaddr *) &serv, addlen);

	return so;
}

int cDisconnect(fd)
{	
	int ok=close(fd);
    //falta error checks 
	return ok;
}
