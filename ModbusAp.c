#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


int Write_multiple_regs(char server_add[256], int port, uint16_t st_r, uint16_t n_r, uint16_t *val)
{
	int i=0, contador=0;
	if( (port > 65536) || (port < 0) ) return -1;  // a porta tem que ser valida
	if( (st_r < 1) || (st_r >10) ) return -1;  // nao posso começar num registo inferior a 1 nem posso ultrapassar o meu limite (limite definido por nos)
	if( (n_r > 10) || (n_r < 1) ) return -1;  // nao posso escrever menos que 1 registo nem mais que 10
	if( (st_r + n_r >10) return -1;  // o inicio dos registos + o numero de registos que vou escrever nao pode ser superior ao meu limite superior
	
	int tamanho = 6+(n_r*2);
	uint8_t *APDU = malloc(tamanho);  //6 bytes fixos (apdu desta funçao nesta camada tem sempre 6 fixos) + 2 bytes por cada registo que eu queira escrever
	
	
	//	(registos ocupam 2 bytes)
	
	
	APDU[0] = (uint8_t) 16;  //numero da função a ser executada pelo servidor
	APDU[1] = (uint8_t) (st_r >> 8); //start address [HIGH]
	APDU[2] = (uint8_t) (st_r & 0x00ff); //start address [LOW]
	APDU[3] = (uint8_t) (n_r >> 8); //numero de registos a escrever [HIGH]
	APDU[4] = (uint8_t) (n_r & 0x00ff); //numero de registos a escrever [LOW]
	APDU[5] = (uint8_t) (n_r*2); //numero de bytes restantes neste APDU que falta ler (numero de registos*2 pois cada registo sao 2 bytes)	//serve para camada seguinte saber que tem de ler 6bytes + n_r*2
	contador = 6; //posicao do vetor APDU inicial
	for(i=0;n_r;i++) //ciclo que percorre todas as posiçoes de VAL
	{
		APDU[contador] = (uint8_t) (val[i] >> 8);  //guarda o HIGH do val[contador]
		//val sao 16 bits, guardo 8 bits do val numa pos do apdu e os restantes 8 bits noutra posicao
 		
		APDU[contador+1] = (uint8_t) (val[i] & 0x00ff); //guarda o LOW do val[contador]
		
		//cada pos do val, ocupa 2 posiçoes do apdu
		contador = contador+2; //ja coloquei informaçao no contador e contador+1, agora tenho que passar para o contador+2
	}

 	
	Read_h_regs(char server_add[256] ,int port, uint16_t st_r , unint16_t n_r , uint16_t *val)
	{
		if(  server_add==NULL) return -1;
		if( (port > 65536) || (port < 0) ) return -1;  // a porta tem que ser valida
		if( (st_r < 1) || (st_r >10) ) return -1;  // nao posso começar num registo inferior a 1 nem posso ultrapassar o meu limite
		if( (n_r > 10) || (n_r < 1) ) return -1;  // nao posso escrever menos que 1 registo nem mais que 10
		if( (st_r + n_r >10) return -1;  // o inicio dos registos + o numero de registos que vou escrever nao pode ser superior ao meu limite superior

		int tamanho=5;//tamanho APDU
		uint8_t *APDU = malloc(sizeof(uint8_t)*tamanho);  //6 bytes fixos + 2 bytes por cada registo que eu queira escrever
		
		st_r--;
		
		ADPU[0]=(unit8_t) 0x03; //Função para ler os registos
		ADPU[1]=(unit8_t)(st_r >> 8); //start adress ->High
		ADPU[2]=(unit8_t)(st_r & 0x00ff); //start adress ->Low
		ADPU[3]=(unit8_t)(n_r >> 8); //nº restistos a ler ->High
		ADPU[4]=(unit8_t)(n_r & 0x00ff);//nº registos a ler ->Low 

		


	}
	
}