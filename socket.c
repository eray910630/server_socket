#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <netdb.h>
#include <strings.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>





#define MaxLine 1024


int coneccion_cliente(char*hostname,int puerto)
{
   int clientefd;
   
   struct hostent *hp;
   struct sockaddr_in serveraddr;
   bzero(&serveraddr, sizeof(serveraddr));
   serveraddr.sin_family=AF_INET;
   serveraddr.sin_port=htons((unsigned short)puerto); 
   inet_aton(hostname,&serveraddr.sin_addr);
   clientefd=socket(AF_INET,SOCK_STREAM,0);
   if (clientefd < 0)
   {
   	 return -1;
   }
   int fd=connect(clientefd,(struct sockaddr*)&serveraddr, sizeof(serveraddr));
   return clientefd;
}
int is_operation(char*p)
 {
    if (p[0]=='a' && p[1]=='d' && p[2]=='d' && strlen(p)==4)
    {
    	return 1;
    }
    else if (p[0]=='s' && p[1]=='u' && p[2]=='b' && strlen(p)==4 )
    {
    	return 2;
    }
    else if (p[0]=='m' && p[1]=='u' && p[2]=='l' && strlen(p)==4)
     {
    	return 3;
     }
    else if (p[0]=='d' && p[1]=='i' && p[2]=='v' && strlen(p)==4)
    {
    	return 4;
    }
   
    return -1;
}

int is_clear(char*p)
 {
   if (p[0]=='c' && p[1]=='l' && p[2]=='e' && p[3]=='a' && p[4]=='r' && strlen(p)==6)
   {
   	return 1;
   }
  return 0;
 }


int Imprime(char*p)
{
    if (p[0]=='i' && p[1]=='g' && p[2]=='u' && p[3]=='a' && p[4]=='l' )
    {
    	return 1;
    }
    return 0;
}
int main(int argc, char  *argv[])
{
	char buf[MaxLine];
	int cliente,puerto;
	char*hostname;
	if (argc!=3)
	{
		fprintf(stderr, " usage %s <host> <port>\n",argv[0]);
		exit(0);
	}
	hostname=argv[1];
	puerto=atoi(argv[2]);
	cliente =coneccion_cliente(hostname,puerto);//127.0.1.1 8080
	int op=1;
	int contador =0;
   int numero=0;
	while(fgets(buf,MaxLine,stdin)!=NULL)
	{  
		
		printf("el cliente escribe : ");
	    write(cliente,buf,strlen(buf));
		fputs(buf,stdout);
		 if (is_clear(buf))
 	     {
 	  	   contador=0;
 	  	   op=1;
 	     }
 	  else if (Imprime(buf))
 	  {
 	     printf("El calculo del cliente es  : %d\n",contador);
 	     
 	  }

		else
 	    {
 	  		if (op>0)
 	  		{
 	  			if (op==1)
 	  			{
 	  				numero=atoi(buf);
 	  				//printf("la operacion es %d\n",op);
 	  			    contador+=numero;
 	  			    op=0;
 	  			    //printf("%d\n",contador);
 	  			}
 	  			else if (op==2)
 	  			{
 	  				numero=atoi(buf);
 	  				//printf("la operacion es %d\n",op);
 	  			    contador-=numero;
 	  			    op=0;
 	  			   // printf("%d\n",contador);
 	  			}
 	  			else if (op==3)
 	  			{
 	  				numero=atoi(buf);
 	  				op=0;
 	  				//printf("la operacion es %d\n",op);
 	  				//printf("%d\n",numero);
 	  				contador*=numero;
 	  				//printf("%d\n",contador);
 	  			}
 	  			else if (op==4)
 	  			{
 	  				numero=atoi(buf);
 	  				op=0;
 	  				//printf("la operacion es %d\n",op);
 	  				contador/=numero;
 	  				//printf("%d\n",contador);
 	  			}
 	  			
 	  		}
 	  		int operacion=is_operation(buf);

 	  		if (operacion > 0)
 	  		{
 	  			op=operacion;
 	  		}
 	  	
 	  }
		
		
	} 
	close(cliente);
	exit(0);
}