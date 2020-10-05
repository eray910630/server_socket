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

#define MAXLINE 1024
// void sigchld_handler(int sig)
// {
// while (waitpid(-1, 0, WNOHANG) > 0)
// ;
// return;
// }
// Signal(SIGCHLD, sigchld_handler);

int server(int puerto)
{

	int server_fd;
	int optval=1;
	struct sockaddr_in serveraddr;
	server_fd=socket(AF_INET,SOCK_STREAM,0);
	if (server_fd < 0)
	{
       return -1;
	}
	if (setsockopt(server_fd,SOL_SOCKET,SO_REUSEADDR,(const void *)&optval, sizeof(int)) < 0) //elimina alguna direccion en uso
	{
		return -1;
	}
	bzero((char*)&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family=AF_INET;
	serveraddr.sin_addr.s_addr=htonl(INADDR_ANY);
	serveraddr.sin_port=htons((unsigned short)puerto);
	if (bind(server_fd,(struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0)
	{
		return -1;
	}
	if (listen(server_fd,5) < 0)
	{
		return -1;
	}
	return server_fd;
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

void calculadora(int conect)
{
   int n;
  char buf[MAXLINE];
  int contador =0;
  int numero=0;
  int op=1;
 while((n = read(conect, buf, MAXLINE)) != 0) 
 {
 	  buf[n]='\0';
 	  if (is_clear(buf))
 	  {
 	  	contador=0;
 	  	op=1;
 	  }
 	  else if (Imprime(buf))
 	  {
 	     printf("El servidor responde : %d\n",contador);
 	     
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
}
int main(int argc, char  *argv[])
{
	int server_fd,puerto,connfd,clientlen;
	struct sockaddr_in clientaddr;
	struct hostent *hp;
	char*tras;

	if (argc!=2)
	{
			fprintf(stderr, "usage <host> <port> %s\n",argv[0] );
			exit(0);
	}
	puerto=atoi(argv[1]);
	server_fd=server(puerto);


	while(1)
	{
		clientlen=sizeof(clientaddr);
		connfd=accept(server_fd,(struct sockaddr *)&clientaddr,&clientlen);
		tras=inet_ntoa(clientaddr.sin_addr);
		printf("estableciendo conexion con %s\n",tras);
        calculadora(connfd);
        close(connfd); 
	}
	
	exit(0);
}