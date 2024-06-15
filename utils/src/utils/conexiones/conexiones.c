#include <utils/conexiones/conexiones.h>

int iniciar_servidor(char* IP, char* PUERTO)
{
	int socket_servidor;

	struct addrinfo hints, *servinfo;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	int error;	

	error = getaddrinfo(IP, PUERTO, &hints, &servinfo);
	// chequeo valor de retorno
	if(error){
		fprintf(stderr,"Error en getaddrinfo: %s\n",gai_strerror(error));
		return -1;
	}

	// Creamos el socket de escucha del servidor
	socket_servidor = socket(servinfo->ai_family,
                    		servinfo->ai_socktype,
                    		servinfo->ai_protocol);
	// chequeo valor de retorno
	if(socket_servidor == -1){
		fprintf(stderr, "Error al crear el socket: \n%s", strerror(errno));
        return -1;
	}

	if (setsockopt(socket_servidor, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0) 
    perror("setsockopt(SO_REUSEADDR) failed");

	// Asociamos el socket a un puerto
	error = bind(socket_servidor, servinfo->ai_addr, servinfo->ai_addrlen);
	// chequeo valor de retorno
	if (error == -1) {
        fprintf(stderr, "Error al hacer bind: \n%s", strerror(errno));
        return -1;
    }

	// Escuchamos las conexiones entrantes
	error = listen(socket_servidor, SOMAXCONN);
	// chequeo valor de retorno
	if(error == -1){
		fprintf(stderr, "Error el servidor no esta escuchando: \n%s", strerror(errno));
        return -1;
	}

	freeaddrinfo(servinfo);

	return socket_servidor;
} 


int intentos_de_conexion = 0;

int crear_conexion(char* IP, char* PUERTO)
{
	struct addrinfo hints;
	struct addrinfo *server_info;


	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	int error;
	error = getaddrinfo(IP, PUERTO, &hints, &server_info);
	// chequeo valor de retorno
	if(error){
		fprintf(stderr,"Error en getaddrinfo: %s\n",gai_strerror(error));
		return -1;
	}

	// Ahora vamos a crear el socket.
	int socket_cliente = socket(server_info->ai_family, 
								server_info->ai_socktype, 
								server_info->ai_protocol);

	// chequeo valor de retorno
	if(socket_cliente == -1){
		fprintf(stderr, "Error al crear el socket: \n%s", strerror(errno));
		return -1;
	}

	error = connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen);
	
	// Manejo de error de conexión con servidor
	if(error == -1){
		fprintf(stderr, "\x1B[31mError al conectarse con servidor (%s:%s): %s \n",IP, PUERTO, strerror(errno));
		intentos_de_conexion++; // contamos este intento
		while(intentos_de_conexion <= MAX_INTENTOS_CONEXION-1) { 
			printf("Intento: %d. ", intentos_de_conexion);
			printf("Quedan %d intento(s) antes de Timeout!\n", MAX_INTENTOS_CONEXION-intentos_de_conexion);
			printf("Reintentando conexion en %d segundos...\n\n\x1B[37m", TIEMPO_ESPERA_RECONEXION);
			sleep(TIEMPO_ESPERA_RECONEXION); // Esperamos 5 segundos antes de reintentar conexión, por si alguno de los otros módulos no se inicio todavía.
			return crear_conexion(IP, PUERTO);
		}
		puts("\x1B[31mConnection timeout!\n\n\x1B[37m");
		intentos_de_conexion = 0; 
		return -1; // cuando el conteo de intentos haya superado los 4, se retorna -1 para indicar Timeout.
	}

	freeaddrinfo(server_info);

	return socket_cliente;
}


int esperar_cliente(int fd_servidor)
{
	// Aceptamos un nuevo cliente
	int fd_cliente = accept(fd_servidor, NULL, NULL);

	if(fd_cliente == -1){
		fprintf(stderr, "Error al aceptar cliente: \n%s", strerror(errno));
    	return EXIT_FAILURE;
	}

	return fd_cliente;
} 

void liberar_conexion(int fd_cliente)
{
	close(fd_cliente);
}